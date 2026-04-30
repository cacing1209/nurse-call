#ifndef SDLOG_H
#define SDLOG_H

#include <Arduino.h>

#define SD_LOG_FILE "events.json"

struct sd_logger
{
    bool ready;

    void begin();
    void log_event(uint8_t role, uint8_t kamar, uint8_t bed);
    static const char *event_name(uint8_t role);
};

extern sd_logger sd_log;

void time_set_epoch(unsigned long epoch);
unsigned long time_now_epoch();
void time_format_now(char *buf, size_t len);

#endif
