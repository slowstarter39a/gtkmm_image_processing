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
		static int get_aaa();

	private:
		MagnoliaLogger(); 
		~MagnoliaLogger();
		int log_level;
		static MagnoliaLogger *instance_;
};

#define MGNL_PRINTF(tag, level, fmt, ...) \
	do { \
		MagnoliaLogger *logger = MagnoliaLogger::get_instance(); \
		logger->print_log(__FUNCTION__, __LINE__, tag, level, fmt, ##__VA_ARGS__); \
	} while(0);

#endif /* _MAGNOLIA_LOGGER_H */
