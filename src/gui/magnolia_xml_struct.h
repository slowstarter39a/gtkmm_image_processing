/*
 * =====================================================================================
 *
 *       Filename:  magnolia_xml_struct.h
 *
 *    Description:
 *	This file is a magnolia xml struct header file
 *
 * =====================================================================================
 */
#ifndef _MAGNOLIA_XML_STRUCT_H
#define _MAGNOLIA_XML_STRUCT_H

#include <tinyxml2.h>
using namespace tinyxml2;

class MagnoliaXmlStruct
{
	public:
		MagnoliaXmlStruct();
		~MagnoliaXmlStruct();
		int read_xml();
		int write_xml(const char *name, char *val);
		int get_log_level();
		void set_log_level(int log_level);
		char* get_default_image_path();
		void set_default_image_path(char *img_path);
		int get_lib_type();
		void set_lib_type(int lib_type);
		void make_default_xml_if_not_exists();

	private:
		int log_level_;
		char default_image_path_[300];
		int lib_type_;
};

#endif //_MAGNOLIA_XML_STRUCT_H
