/*
 * =====================================================================================
 *
 *       Filename:  magnolia_xml_struct.cc
 *
 *    Description:
 *      This file is a magnolia xml struct source file
 *
 * =====================================================================================
 */

#include <cstring>
#include <cstdio>
#include <unistd.h>
#include "magnolia_xml_struct.h"
#include "magnolia_logger.h"

static const char *tag = __FILE__;
const char *xml_file_name = "magnolia_config.xml";

MagnoliaXmlStruct::MagnoliaXmlStruct()
{
	log_level = 0;
	lib_type = 0;
	memset(default_image_path, 0, sizeof(default_image_path));
	make_default_xml_if_not_exists();
}

MagnoliaXmlStruct::~MagnoliaXmlStruct()
{
}
void MagnoliaXmlStruct::make_default_xml_if_not_exists()
{
	if (!access(xml_file_name, F_OK)) {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "%s already exists! Skip creating a default file\n", xml_file_name);
		return;
	}

    XMLDocument doc;
 
    doc.LinkEndChild(doc.NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\""));
    doc.LinkEndChild(doc.NewComment("this is a comment"));
 
    auto config_element = doc.NewElement("config");

    auto log_level_element = doc.NewElement("log_level");
    log_level_element->SetText("10");
    config_element->LinkEndChild(log_level_element);

    auto default_image_path_element  = doc.NewElement("default_image_path");
	char cur_path[256] = {0,};
	getcwd(cur_path, 256);
    default_image_path_element->SetText(cur_path);
    config_element->LinkEndChild(default_image_path_element);
 
    auto lib_type_element = doc.NewElement("lib_type");
    lib_type_element->SetText("8");
    config_element->LinkEndChild(lib_type_element);

	doc.LinkEndChild(config_element);
 
	doc.SaveFile(xml_file_name);
}

int MagnoliaXmlStruct::read_xml()
{
	XMLDocument doc;
	XMLElement *root_element = NULL;
	XMLElement *ptr_element = NULL;
	XMLError xml_error = XML_SUCCESS;

	xml_error = doc.LoadFile(xml_file_name);

	if (xml_error != XML_SUCCESS) {
		return -1;
	}
	else {
		root_element = doc.RootElement();
	}

	if (root_element != NULL) {
		ptr_element = root_element->FirstChildElement("log_level");
		if (ptr_element) {
			log_level = atoi(ptr_element->GetText());
		}
		else {
			MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "ptr_element about log_level is NULL\n");
			return -1;
		}

		ptr_element = ptr_element->NextSiblingElement("default_image_path");
		if (ptr_element) {
			memset(default_image_path, 0, sizeof(default_image_path));
			std::snprintf(default_image_path, sizeof(default_image_path), "%s", ptr_element->GetText());
		}
		else {
			MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "ptr_element about default_image_path is NULL\n");
			return -1;
		}

		ptr_element = ptr_element->NextSiblingElement("lib_type");
		if (ptr_element) {
			lib_type = atoi(ptr_element->GetText());
		}
		else {
			MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "ptr_element about lib_type is NULL\n");
			return -1;
		}
	}
	else {
		MGNL_PRINTF(tag, LOG_LEVEL_ERROR, "root_element is NULL\n");
	}

	return 0;
}

int MagnoliaXmlStruct::write_xml(const char *name, char *val)
{
	XMLDocument doc;
	XMLElement *root_element = NULL;
	XMLElement *ptr_element = NULL;
	XMLError xml_error = XML_SUCCESS;

	xml_error = doc.LoadFile(xml_file_name);

	if (xml_error != XML_SUCCESS) {
		return -1;
	}
	else {
		root_element = doc.RootElement();
	}

	if (root_element != NULL) {
		for (ptr_element = root_element->FirstChildElement(); ptr_element != NULL; ptr_element = ptr_element->NextSiblingElement()) {
			if (!strcmp(ptr_element->Name(), name)) {
				ptr_element->SetText(val);
				break;
			}
		}
	}
	doc.SaveFile(xml_file_name);

	return 0;
}

int MagnoliaXmlStruct::get_log_level()
{
	return log_level;
}

void MagnoliaXmlStruct::set_log_level(int log_level)
{
	this->log_level = log_level;

	char buf[10] = {0,};
	std::snprintf(buf, sizeof(buf), "%d", log_level);
	write_xml((const char*)"log_level", buf);
}

char* MagnoliaXmlStruct::get_default_image_path()
{
	return default_image_path;
}

void MagnoliaXmlStruct::set_default_image_path(char *img_path)
{
	strncpy(default_image_path, img_path, sizeof(default_image_path));

	write_xml((const char*)"default_image_path", default_image_path);
}

int MagnoliaXmlStruct::get_lib_type()
{
	return lib_type;
}

void MagnoliaXmlStruct::set_lib_type(int lib_type)
{
	this->lib_type = lib_type;
	
	char buf[10] = {0,};
	std::snprintf(buf, sizeof(buf), "%d", lib_type);
	write_xml((const char*)"lib_type", buf);
}
