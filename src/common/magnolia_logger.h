/*
 * =====================================================================================
 *
 *       Filename:  magnolia_logger.h
 *       
 *
 *    Description:  
 *      This file is a magnolia logger header file 
 *
 * =====================================================================================
 */
#ifndef _MAGNOLIA_LOGGER_H
#define _MAGNOLIA_LOGGER_H

#include <dlfcn.h>
#include "image_processing_main.h"

enum log_message_level
{
	LOG_LEVEL_ERROR = 0,
	LOG_LEVEL_WARN,
	LOG_LEVEL_INFO,
	LOG_LEVEL_DEBUG,
	LOG_LEVEL_TRACE,
	LOG_LEVEL_MAX,
};

class MagnoliaLogger {
	public:
		void set_log_level(log_message_level log_level);
		void print_log(const char*function_name, int line, const char *tag_name, log_message_level log_level, const char *str,...);
		static MagnoliaLogger* get_instance();
		static void destroy_instance();

	private:
		MagnoliaLogger(); 
		~MagnoliaLogger();
		int log_level_;
		static MagnoliaLogger *instance_;
};

#define MGNL_PRINTF(tag, level, fmt, ...) \
	do { \
		MagnoliaLogger *logger = MagnoliaLogger::get_instance(); \
		logger->print_log(__FUNCTION__, __LINE__, tag, level, fmt, ##__VA_ARGS__); \
	} while(0);

#define MGNL_SET_LOG_LEVEL(level) \
	do { \
		MagnoliaLogger *logger = MagnoliaLogger::get_instance(); \
		logger->set_log_level((log_message_level)level); \
		\
		void *handle; \
		handle = dlopen("image_processing_lib.so", RTLD_LAZY); \
		if (!handle) { \
			MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Open Library failed. %s\n", dlerror()); \
			return; \
		} \
		image_processing_log_level_handler_t *fnImageProcessing = (image_processing_log_level_handler_t*)dlsym(handle, "ImageProcessingSetLogLevel"); \
		if (dlerror() != NULL) { \
			MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "Open Library function ImageProcessingDispatcher failed\n"); \
			dlclose(handle); \
			return; \
		} \
		fnImageProcessing(level); \
		dlclose(handle); \
	} while(0);

#endif /* _MAGNOLIA_LOGGER_H */
