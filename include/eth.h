#ifndef ETH_H
#define ETH_H

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#define EVENT_LOG_SIZE 16
#define MQTT_HOST_LEN 32
#define MQTT_TOPIC_LEN 32
#define MQTT_USER_LEN 24

enum buzzer_mode_t : uint8_t
{
    BZ_FAST = 0,
    BZ_RHYTHM = 1,
    BZ_SLOW = 2,
    BZ_MODE_COUNT = 3
};

struct event_log_entry
{
    unsigned long t_ms;
    uint8_t kamar;
    uint8_t bed;
    uint8_t role;
    uint8_t pin;
};

struct event_log_t
{
    event_log_entry entries[EVENT_LOG_SIZE];
    uint8_t head;
    uint8_t count;
    void push(uint8_t pin, uint8_t kamar, uint8_t bed, uint8_t role);
};

struct buzzer_config_t
{
    buzzer_mode_t mode;
    int on_ms[BZ_MODE_COUNT];
    int off_ms[BZ_MODE_COUNT];
    void defaults();
    const char *mode_name(uint8_t m) const;
};

struct mqtt_config_t
{
    char host[MQTT_HOST_LEN];
    uint16_t port;
    char topic[MQTT_TOPIC_LEN];
    char user[MQTT_USER_LEN];
    bool enabled;
    void defaults();
};

const int NTP_PACKET_SIZE = 48;
struct NTPConfig
{
    IPAddress timeServer;
    byte packetBuffer[NTP_PACKET_SIZE];
    EthernetUDP Udp;
    unsigned int localPort;
    void sendNTPpacket();
    unsigned long getNTPTime();
    void update(int interval_sync = 60000);
    const long utcOffsetSeconds = 7 * 3600;
    NTPConfig() : timeServer(216, 239, 35, 0), localPort(8888)
    {
        memset(packetBuffer, 0, NTP_PACKET_SIZE);
    }
};

struct buzzer_t;

struct ethernet_state
{
private:
    bool eth_connected = false;
    bool first_initialize = false;
    unsigned long last_maintain = 0;
    unsigned long last_reconnect = 0;
    static const unsigned long MAINTAIN_INTERVAL = 500;
    static const unsigned long RECONNECT_INTERVAL = 5000;

    char method[8];
    char path[64];
    static const size_t BODY_SIZE = 256;
    char body[BODY_SIZE];
    size_t body_len;
    unsigned long start_time;

    byte eth_mac[6] = {0x02, 0xA1, 0x01, 0x16, 0x3D, 0x5C};
    EthernetServer server = EthernetServer(80);

    void apply_buzzer();

public:
    event_log_t event_log;
    buzzer_config_t buzzer_cfg;
    mqtt_config_t mqtt;
    buzzer_t *bz_ref = nullptr;

    void begin(buzzer_t *bz);
    void loop();
    void push_event(uint8_t pin, uint8_t kamar, uint8_t bed, uint8_t role);

    void handle_client(EthernetClient &client);
    bool parse_request(EthernetClient &client);
    void reset_parser();

    void route_index(EthernetClient &client);
    void route_event_log(EthernetClient &client);
    void route_buzzer_get(EthernetClient &client);
    void route_buzzer_post(EthernetClient &client);
    void route_mqtt_get(EthernetClient &client);
    void route_mqtt_post(EthernetClient &client);

    void send_status(EthernetClient &client, int code, const __FlashStringHelper *msg);
    void send_html_head(EthernetClient &client, const __FlashStringHelper *title);
    void send_html_tail(EthernetClient &client);
    void send_redirect(EthernetClient &client, const __FlashStringHelper *to);

    bool form_get(const char *key, char *out, size_t out_size);
};

extern ethernet_state eth;

#endif
