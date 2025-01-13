#pragma once

#define FMT_UNICODE 0
#include <spdlog/spdlog.h>

#define LINFO(...)       spdlog::info(__VA_ARGS__)
#define LWARN(...)       spdlog::warn(__VA_ARGS__)
#define LERROR(...)      spdlog::error(__VA_ARGS__)
#define LCRITICAL(...)   spdlog::critical(__VA_ARGS__)
