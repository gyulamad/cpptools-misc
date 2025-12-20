#pragma once

#include <stdint.h>

#define __DATE_TIME__ datetime::ms_to_datetime()

typedef long long time_ms;
typedef long long time_sec;

static constexpr time_ms second_ms = 1000;
static constexpr time_ms minute_ms = 60 * second_ms;
static constexpr time_ms hour_ms = 60 * minute_ms;
static constexpr time_ms day_ms = 24 * hour_ms;
static constexpr time_ms week_ms = 7 * day_ms;

static constexpr time_sec minute_sec = 60;
static constexpr time_sec hour_sec = 60 * minute_sec;
static constexpr time_sec day_sec = 24 * hour_sec;
static constexpr time_sec week_sec = 7 * day_sec;
