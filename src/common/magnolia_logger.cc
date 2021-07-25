/*
 * =====================================================================================
 *
 *       Filename:  magnolia_logger.cc
 *       
 *
 *    Description:  
 *      This file is a magnolia logger source file 
 *
 * =====================================================================================
 */
#include "magnolia_logger.h"
#include <cstring>
#include <cstdio>
#include <cstdarg>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

using namespace std;
MagnoliaLogger* MagnoliaLogger::instance_= nullptr;
const char *log_message_level_string[LOG_LEVEL_MAX] = {"ERROR", "WARN", "INFO", "DEBUG", "TRACE"};

MagnoliaLogger::MagnoliaLogger() 
{
	log_level_ = LOG_LEVEL_ERROR;
}

MagnoliaLogger::~MagnoliaLogger()
{
}

MagnoliaLogger* MagnoliaLogger::get_instance()
{
	if (instance_ == nullptr) {
		instance_ = new MagnoliaLogger();
	}

	return instance_;
}

void MagnoliaLogger::destroy_instance()
{
	if (!instance_) {
		return;
	}
	delete instance_;
	instance_ = nullptr;
}

void MagnoliaLogger::set_log_level(log_message_level log_level)
{
	if (log_level >= LOG_LEVEL_MAX) {
		return;
	}
	this->log_level_ = log_level;
}

void MagnoliaLogger::print_log(const char*function_name, int line, const char *tag_name, log_message_level log_level, const char *str, ...)
{
	if (this->log_level_ < log_level) {
		return;
	}

	char log_string[512] = {0};

    // get current time
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    // convert to std::time_t in order to convert to std::tm (broken time)
    auto timer = std::chrono::system_clock::to_time_t(now);

    // convert to broken time
	std::tm buf;
    //std::tm bt = *std::localtime_r(&timer, &buf);
    localtime_r(&timer, &buf);

    std::ostringstream oss;

    oss << std::put_time(&buf, "%H:%M:%S"); // HH:MM:SS
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

	snprintf(log_string, 256, "%s [%s] [%s:%d] : ", log_message_level_string[log_level], oss.str().c_str(), function_name, line);
	strncat(log_string, str, 512);

	va_list args;
	va_start(args, str);
	vprintf(log_string, args);
	va_end(args);
}
