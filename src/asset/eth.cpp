#include <eth.h>
#include <source.h>

ethernet_state eth;
NTPConfig ntpCfg;

static const char ROLE_E = 'E';
static const char ROLE_C = 'C';
static const char ROLE_P = 'P';

static char role_char(uint8_t r)
{
    switch (r)
    {
    case Emergency:
        return ROLE_E;
    case Codeblue:
        return ROLE_C;
    case patient:
        return ROLE_P;
    default:
        return '-';
    }
}

void event_log_t::push(uint8_t pin, uint8_t kamar, uint8_t bed, uint8_t role)
{
    entries[head].t_ms = millis();
    entries[head].pin = pin;
    entries[head].kamar = kamar;
    entries[head].bed = bed;
    entries[head].role = role;
    head = (head + 1) % EVENT_LOG_SIZE;
    if (count < EVENT_LOG_SIZE)
        count++;
}

void buzzer_config_t::defaults()
{
    mode = BZ_FAST;
    on_ms[BZ_FAST] = 100;
    off_ms[BZ_FAST] = 100;
    on_ms[BZ_RHYTHM] = 250;
    off_ms[BZ_RHYTHM] = 600;
    on_ms[BZ_SLOW] = 1000;
    off_ms[BZ_SLOW] = 1000;
}

const char *buzzer_config_t::mode_name(uint8_t m) const
{
    switch (m)
    {
    case BZ_FAST:
        return "fast";
    case BZ_RHYTHM:
        return "rhythm";
    case BZ_SLOW:
        return "slow";
    default:
        return "?";
    }
}

void mqtt_config_t::defaults()
{
    strncpy(host, "192.168.0.10", MQTT_HOST_LEN);
    host[MQTT_HOST_LEN - 1] = '\0';
    port = 1883;
    strncpy(topic, "nurse/call", MQTT_TOPIC_LEN);
    topic[MQTT_TOPIC_LEN - 1] = '\0';
    user[0] = '\0';
    enabled = false;
}

static bool ethernetCableConnected()
{
    auto link = Ethernet.linkStatus();
    if (link == LinkON)
        return true;
    if (link == LinkOFF)
        return false;
    return true;
}

void ethernet_state::begin(buzzer_t *bz)
{
    bz_ref = bz;
    buzzer_cfg.defaults();
    mqtt.defaults();
    event_log.head = 0;
    event_log.count = 0;
    apply_buzzer();

    Ethernet.init(10);
    delay(250);
    if (Ethernet.begin(eth_mac) == 0)
    {
        IPAddress ip(192, 168, 0, 8);
        Ethernet.begin(eth_mac, ip);
        eth_connected = false;
        first_initialize = false;
    }
    else
    {
        eth_connected = true;
        first_initialize = true;
    }

#ifdef DEBUG_ETH
    Serial.print(F("web @ "));
    Serial.println(Ethernet.localIP());
#endif

    server.begin();
    reset_parser();

    if (eth_connected)
        ntpCfg.Udp.begin(ntpCfg.localPort);
}

void ethernet_state::apply_buzzer()
{
    if (!bz_ref)
        return;
    uint8_t m = buzzer_cfg.mode;
    if (m >= BZ_MODE_COUNT)
        m = BZ_FAST;
    bz_ref->intervalOn = buzzer_cfg.on_ms[m];
    bz_ref->intervalOff = buzzer_cfg.off_ms[m];
}

void ethernet_state::push_event(uint8_t pin, uint8_t kamar, uint8_t bed, uint8_t role)
{
    event_log.push(pin, kamar, bed, role);
}

void ethernet_state::loop()
{
    unsigned long now = millis();
    if (!ethernetCableConnected())
        return;

    if (!eth_connected)
    {
        if (now - last_reconnect >= RECONNECT_INTERVAL)
        {
            last_reconnect = now;
            if (Ethernet.begin(eth_mac) != 0)
            {
                eth_connected = true;
                first_initialize = true;
                server.begin();
            }
        }
        return;
    }

    if (now - last_maintain >= MAINTAIN_INTERVAL)
    {
        last_maintain = now;
        byte r = Ethernet.maintain();
        if (r == 1 || r == 3)
        {
            eth_connected = false;
            return;
        }
    }

    EthernetClient client = server.available();
    if (client)
    {
        handle_client(client);
        delay(1);
        client.stop();
    }
}

bool ethernet_state::parse_request(EthernetClient &client)
{
    bool first_line = true;
    String line;
    line.reserve(64);
    start_time = millis();

    while (client.connected() || client.available())
    {
        if (client.available())
        {
            char c = client.read();
            if (c == '\n')
            {
                if (first_line)
                {
                    int s1 = line.indexOf(' ');
                    int s2 = line.indexOf(' ', s1 + 1);
                    if (s1 < 0 || s2 < 0)
                        return false;
                    line.substring(0, s1).toCharArray(method, sizeof(method));
                    line.substring(s1 + 1, s2).toCharArray(path, sizeof(path));
                    first_line = false;
                }
                else if (line.length() == 0 || (line.length() == 1 && line[0] == '\r'))
                {
                    body_len = 0;
                    unsigned long body_start = millis();
                    while (millis() - body_start < 400)
                    {
                        while (client.available() && body_len < BODY_SIZE - 1)
                        {
                            body[body_len++] = client.read();
                            body_start = millis();
                        }
                        if (!client.connected() && !client.available())
                            break;
                    }
                    body[body_len] = '\0';
                    return true;
                }
                line = "";
            }
            else if (c != '\r')
            {
                line += c;
            }
        }
        if (millis() - start_time > 3000)
            return false;
    }
    return false;
}

void ethernet_state::reset_parser()
{
    method[0] = '\0';
    path[0] = '\0';
    body[0] = '\0';
    body_len = 0;
    start_time = 0;
}

void ethernet_state::handle_client(EthernetClient &client)
{
    reset_parser();
    if (!parse_request(client))
    {
        send_status(client, 400, F("Bad Request"));
        return;
    }

    bool is_get = strcmp(method, "GET") == 0;
    bool is_post = strcmp(method, "POST") == 0;

    if (is_get && (strcmp(path, "/") == 0 || strcmp(path, "/index") == 0))
    {
        route_index(client);
    }
    else if (is_get && strcmp(path, "/event-log") == 0)
    {
        route_event_log(client);
    }
    else if (is_get && strcmp(path, "/buzzer") == 0)
    {
        route_buzzer_get(client);
    }
    else if (is_post && strcmp(path, "/buzzer") == 0)
    {
        route_buzzer_post(client);
    }
    else if (is_get && strcmp(path, "/mqtt") == 0)
    {
        route_mqtt_get(client);
    }
    else if (is_post && strcmp(path, "/mqtt") == 0)
    {
        route_mqtt_post(client);
    }
    else
    {
        send_status(client, 404, F("Not Found"));
    }
}

void ethernet_state::send_status(EthernetClient &client, int code, const __FlashStringHelper *msg)
{
    client.print(F("HTTP/1.1 "));
    client.print(code);
    client.print(' ');
    client.println(msg);
    client.println(F("Content-Type: text/plain"));
    client.println(F("Connection: close"));
    client.println();
    client.println(msg);
}

void ethernet_state::send_redirect(EthernetClient &client, const __FlashStringHelper *to)
{
    client.println(F("HTTP/1.1 303 See Other"));
    client.print(F("Location: "));
    client.println(to);
    client.println(F("Connection: close"));
    client.println();
}

void ethernet_state::send_html_head(EthernetClient &client, const __FlashStringHelper *title)
{
    client.println(F("HTTP/1.1 200 OK"));
    client.println(F("Content-Type: text/html; charset=utf-8"));
    client.println(F("Connection: close"));
    client.println();
    client.print(F("<!doctype html><html><head><meta name=viewport content='width=device-width,initial-scale=1'><title>"));
    client.print(title);
    client.print(F("</title><style>body{font-family:sans-serif;margin:1em;max-width:520px}"
                   "h1{font-size:1.2em}nav a{display:inline-block;margin-right:1em}"
                   "table{border-collapse:collapse;width:100%}td,th{border:1px solid #ccc;padding:4px;font-size:.9em}"
                   "label{display:block;margin-top:.6em}input,select{font-size:1em;padding:4px;width:100%;box-sizing:border-box}"
                   "button{margin-top:1em;padding:.5em 1em;font-size:1em}"
                   ".pill{display:inline-block;padding:2px 8px;border-radius:8px;background:#eee;margin-right:4px}"
                   "</style></head><body>"));
    client.print(F("<nav><a href='/'>home</a> <a href='/event-log'>event-log</a> <a href='/buzzer'>buzzer</a> <a href='/mqtt'>mqtt</a></nav><hr>"));
}

void ethernet_state::send_html_tail(EthernetClient &client)
{
    client.print(F("</body></html>"));
}

void ethernet_state::route_index(EthernetClient &client)
{
    send_html_head(client, F("Nurse Call"));
    client.print(F("<h1>Nurse Call - Menu</h1>"));
    client.print(F("<ol>"));
    client.print(F("<li><a href='/event-log'>Event Log</a></li>"));
    client.print(F("<li><a href='/buzzer'>Buzzer Setting</a> "
                   "<span class=pill>mode: "));
    client.print(buzzer_cfg.mode_name(buzzer_cfg.mode));
    client.print(F("</span></li>"));
    client.print(F("<li><a href='/mqtt'>MQTT</a> "
                   "<span class=pill>"));
    client.print(mqtt.enabled ? F("enabled") : F("disabled"));
    client.print(F("</span></li>"));
    client.print(F("</ol>"));

    client.print(F("<p><small>uptime "));
    client.print(millis() / 1000);
    client.print(F("s &middot; ip "));
    IPAddress ip = Ethernet.localIP();
    for (int i = 0; i < 4; i++)
    {
        client.print(ip[i]);
        if (i < 3)
            client.print('.');
    }
    client.print(F("</small></p>"));
    send_html_tail(client);
}

void ethernet_state::route_event_log(EthernetClient &client)
{
    send_html_head(client, F("Event Log"));
    client.print(F("<h1>Event Log</h1>"));
    if (event_log.count == 0)
    {
        client.print(F("<p>(no events yet)</p>"));
        send_html_tail(client);
        return;
    }

    client.print(F("<table><tr><th>#</th><th>age</th><th>pin</th><th>kamar</th><th>bed</th><th>role</th></tr>"));

    unsigned long now = millis();
    int8_t idx = (int8_t)event_log.head - 1;
    for (uint8_t i = 0; i < event_log.count; i++)
    {
        if (idx < 0)
            idx = EVENT_LOG_SIZE - 1;
        const event_log_entry &e = event_log.entries[idx];
        unsigned long age = (now - e.t_ms) / 1000;
        client.print(F("<tr><td>"));
        client.print(i + 1);
        client.print(F("</td><td>"));
        if (age < 60)
        {
            client.print(age);
            client.print(F("s"));
        }
        else if (age < 3600)
        {
            client.print(age / 60);
            client.print(F("m"));
        }
        else
        {
            client.print(age / 3600);
            client.print(F("h"));
        }
        client.print(F("</td><td>"));
        client.print(e.pin);
        client.print(F("</td><td>"));
        client.print(e.kamar);
        client.print(F("</td><td>"));
        client.print(e.bed);
        client.print(F("</td><td>"));
        client.print(role_char(e.role));
        client.print(F("</td></tr>"));
        idx--;
    }
    client.print(F("</table>"));
    client.print(F("<p><small>"));
    client.print(event_log.count);
    client.print(F(" / "));
    client.print((int)EVENT_LOG_SIZE);
    client.print(F(" entries</small></p>"));
    send_html_tail(client);
}

void ethernet_state::route_buzzer_get(EthernetClient &client)
{
    send_html_head(client, F("Buzzer"));
    client.print(F("<h1>Buzzer Setting</h1>"
                   "<p>Flip-flop with up to 3 mode presets.</p>"
                   "<form method=post action='/buzzer'>"
                   "<label>Mode<select name=mode>"));
    for (uint8_t m = 0; m < BZ_MODE_COUNT; m++)
    {
        client.print(F("<option value="));
        client.print(m);
        if (m == buzzer_cfg.mode)
            client.print(F(" selected"));
        client.print('>');
        client.print(buzzer_cfg.mode_name(m));
        client.print(F("</option>"));
    }
    client.print(F("</select></label>"));

    for (uint8_t m = 0; m < BZ_MODE_COUNT; m++)
    {
        client.print(F("<fieldset><legend>"));
        client.print(buzzer_cfg.mode_name(m));
        client.print(F("</legend>"));
        client.print(F("<label>on(ms)<input type=number min=10 max=5000 name=on"));
        client.print(m);
        client.print(F(" value="));
        client.print(buzzer_cfg.on_ms[m]);
        client.print(F("></label>"));
        client.print(F("<label>off(ms)<input type=number min=10 max=5000 name=off"));
        client.print(m);
        client.print(F(" value="));
        client.print(buzzer_cfg.off_ms[m]);
        client.print(F("></label></fieldset>"));
    }

    client.print(F("<button type=submit>Save</button></form>"));
    client.print(F("<p><small>active mode applied to buzzer immediately.</small></p>"));
    send_html_tail(client);
}

static int clamp_int(int v, int lo, int hi)
{
    if (v < lo)
        return lo;
    if (v > hi)
        return hi;
    return v;
}

void ethernet_state::route_buzzer_post(EthernetClient &client)
{
    char buf[8];
    if (form_get("mode", buf, sizeof(buf)))
    {
        int m = atoi(buf);
        if (m < 0 || m >= BZ_MODE_COUNT)
            m = 0;
        buzzer_cfg.mode = (buzzer_mode_t)m;
    }
    char key[6];
    for (uint8_t m = 0; m < BZ_MODE_COUNT; m++)
    {
        snprintf(key, sizeof(key), "on%u", m);
        if (form_get(key, buf, sizeof(buf)))
            buzzer_cfg.on_ms[m] = clamp_int(atoi(buf), 10, 5000);
        snprintf(key, sizeof(key), "off%u", m);
        if (form_get(key, buf, sizeof(buf)))
            buzzer_cfg.off_ms[m] = clamp_int(atoi(buf), 10, 5000);
    }
    apply_buzzer();
    send_redirect(client, F("/buzzer"));
}

void ethernet_state::route_mqtt_get(EthernetClient &client)
{
    send_html_head(client, F("MQTT"));
    client.print(F("<h1>MQTT</h1><form method=post action='/mqtt'>"));
    client.print(F("<label><input type=checkbox name=enabled value=1"));
    if (mqtt.enabled)
        client.print(F(" checked"));
    client.print(F("> enabled</label>"));
    client.print(F("<label>host<input name=host maxlength=31 value='"));
    client.print(mqtt.host);
    client.print(F("'></label>"));
    client.print(F("<label>port<input type=number min=1 max=65535 name=port value="));
    client.print(mqtt.port);
    client.print(F("></label>"));
    client.print(F("<label>topic<input name=topic maxlength=31 value='"));
    client.print(mqtt.topic);
    client.print(F("'></label>"));
    client.print(F("<label>user<input name=user maxlength=23 value='"));
    client.print(mqtt.user);
    client.print(F("'></label>"));
    client.print(F("<button type=submit>Save</button></form>"));
    client.print(F("<p><small>configuration only - publishing not yet wired.</small></p>"));
    send_html_tail(client);
}

void ethernet_state::route_mqtt_post(EthernetClient &client)
{
    char buf[16];
    mqtt.enabled = form_get("enabled", buf, sizeof(buf));
    form_get("host", mqtt.host, MQTT_HOST_LEN);
    form_get("topic", mqtt.topic, MQTT_TOPIC_LEN);
    form_get("user", mqtt.user, MQTT_USER_LEN);
    if (form_get("port", buf, sizeof(buf)))
    {
        long p = atol(buf);
        if (p < 1)
            p = 1;
        if (p > 65535)
            p = 65535;
        mqtt.port = (uint16_t)p;
    }
    send_redirect(client, F("/mqtt"));
}

static int hexv(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    return -1;
}

bool ethernet_state::form_get(const char *key, char *out, size_t out_size)
{
    if (out_size == 0)
        return false;
    out[0] = '\0';
    size_t klen = strlen(key);
    const char *p = body;
    const char *end = body + body_len;

    while (p < end)
    {
        const char *eq = (const char *)memchr(p, '=', end - p);
        if (!eq)
            break;
        const char *amp = (const char *)memchr(eq, '&', end - eq);
        if (!amp)
            amp = end;

        if ((size_t)(eq - p) == klen && memcmp(p, key, klen) == 0)
        {
            size_t out_i = 0;
            for (const char *q = eq + 1; q < amp && out_i + 1 < out_size; q++)
            {
                char c = *q;
                if (c == '+')
                {
                    out[out_i++] = ' ';
                }
                else if (c == '%' && q + 2 < amp)
                {
                    int h = hexv(q[1]);
                    int l = hexv(q[2]);
                    if (h >= 0 && l >= 0)
                    {
                        out[out_i++] = (char)((h << 4) | l);
                        q += 2;
                    }
                }
                else
                {
                    out[out_i++] = c;
                }
            }
            out[out_i] = '\0';
            return true;
        }
        p = amp + 1;
    }
    return false;
}
