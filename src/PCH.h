#pragma once

#include "F4SE/F4SE.h"
#include "RE/Fallout.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <mutex>
#include <numbers>
#include <rttidata.h>
#include <string.h>
#include <sstream>
#include <vector>

#pragma warning(push)
#include <fmt/chrono.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <toml++/toml.h>
#pragma warning(pop)

namespace logger = F4SE::log;


#define DLLEXPORT __declspec(dllexport)

#ifndef _WIN32
auto& _stricmp = strcmp;
void* __RTDynamicCast(void*, std::ptrdiff_t, void*, void*, bool);
#endif
