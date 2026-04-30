#include <sdlog.h>
#include <SdFat.h>
#include <ArduinoJson.h>
#include <source.h>

#define SD_MISO_PIN 6
#define SD_CS_PIN   7
#define SD_MOSI_PIN 8
#define SD_SCK_PIN  9
#define SD_SOFT_SPEED SD_SCK_MHZ(0)

static SoftSpiDriver<SD_MISO_PIN, SD_MOSI_PIN, SD_SCK_PIN> softSpi;
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SOFT_SPEED, &softSpi)

static SdFat32 sd;
sd_logger sd_log;

static unsigned long g_epoch_base = 0;
static unsigned long g_millis_base = 0;

void time_set_epoch(unsigned long epoch)
{
    if (epoch == 0)
        return;
    g_epoch_base = epoch;
    g_millis_base = millis();
}

unsigned long time_now_epoch()
{
    if (g_epoch_base == 0)
        return 0;
    return g_epoch_base + (millis() - g_millis_base) / 1000UL;
}

static bool is_leap(int y)
{
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

void time_format_now(char *buf, size_t len)
{
    unsigned long epoch = time_now_epoch();

    if (epoch == 0)
    {
        unsigned long sec = millis() / 1000UL;
        unsigned int h = (unsigned int)((sec / 3600UL) % 100UL);
        unsigned int m = (unsigned int)((sec % 3600UL) / 60UL);
        unsigned int s = (unsigned int)(sec % 60UL);
        snprintf(buf, len, "0000-00-00 %02u:%02u:%02u", h, m, s);
        return;
    }

    unsigned long days = epoch / 86400UL;
    unsigned long sod = epoch % 86400UL;
    unsigned int hour = (unsigned int)(sod / 3600UL);
    unsigned int mn = (unsigned int)((sod % 3600UL) / 60UL);
    unsigned int sc = (unsigned int)(sod % 60UL);

    int year = 1970;
    while (true)
    {
        unsigned long ydays = is_leap(year) ? 366UL : 365UL;
        if (days < ydays)
            break;
        days -= ydays;
        year++;
    }

    static const uint8_t dim[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int month = 0;
    while (month < 12)
    {
        uint8_t d = dim[month];
        if (month == 1 && is_leap(year))
            d = 29;
        if (days < d)
            break;
        days -= d;
        month++;
    }

    int day = (int)days + 1;
    snprintf(buf, len, "%04d-%02d-%02d %02u:%02u:%02u",
             year, month + 1, day, hour, mn, sc);
}

const char *sd_logger::event_name(uint8_t role)
{
    switch (role)
    {
    case Emergency:
        return "code red";
    case Codeblue:
        return "code blue";
    case patient:
        return "code green";
    default:
        return "unknown";
    }
}

void sd_logger::begin()
{
    ready = false;

    if (!sd.begin(SD_CONFIG))
    {
        Serial.println(F("SD: init failed"));
        return;
    }
    ready = true;
    Serial.println(F("SD: ready"));

    if (!sd.exists(SD_LOG_FILE))
    {
        File32 f = sd.open(SD_LOG_FILE, O_WRITE | O_CREAT);
        if (f)
        {
            f.print(F("[]"));
            f.close();
        }
    }
}

void sd_logger::log_event(uint8_t role, uint8_t kamar, uint8_t bed)
{
    if (!ready)
        return;

    char ts[24];
    time_format_now(ts, sizeof(ts));

    char src[24];
    snprintf(src, sizeof(src), "kamar%u_bed%u",
             (unsigned)kamar, (unsigned)bed);

    JsonDocument doc;
    doc["created_at"] = ts;
    doc["event"] = event_name(role);
    doc["source"] = src;

    File32 f = sd.open(SD_LOG_FILE, O_RDWR | O_CREAT);
    if (!f)
    {
        Serial.println(F("SD: open log failed"));
        return;
    }

    uint32_t sz = f.size();

    if (sz == 0)
    {
        f.print(F("[\n  "));
        serializeJson(doc, f);
        f.print(F("\n]"));
    }
    else if (sz <= 2)
    {
        f.truncate(1);
        f.seek(1);
        f.print(F("\n  "));
        serializeJson(doc, f);
        f.print(F("\n]"));
    }
    else
    {
        f.truncate(sz - 1);
        f.seek(f.size());
        f.print(F(",\n  "));
        serializeJson(doc, f);
        f.print(F("\n]"));
    }

    f.sync();
    f.close();
}
