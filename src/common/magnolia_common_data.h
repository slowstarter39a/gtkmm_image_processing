/*
 * =====================================================================================
 *
 *       Filename:  magnolia_common_data.h
 *
 *    Description:  
 *      This file is a magnolia common data header file 
 *
 * =====================================================================================
 */
#ifndef _MAGNOLIA_COMMON_DATA_H
#define _MAGNOLIA_COMMON_DATA_H

enum magnolia_error_type
{
	MAGNOLIA_SUCCESS           = 0,
	MAGNOLIA_FAILURE,
	MAGNOLIA_NOT_IMPLEMENTED,
	MAGNOLIA_ERROR_MAX,
};

enum magnolia_cmd_enum_type
{
	MAGNOLIA_CMD_INVERSION 		= 0,
	MAGNOLIA_CMD_HISTOGRAM , 
	MAGNOLIA_CMD_MAX,
};

struct magnolia_cmd_param_type
{
	magnolia_cmd_enum_type		mag_cmd;
};

struct pixbuf_label
{
	Gdk::Pixbuf *pixbuf;
	Glib::ustring *text;
};

#endif /* _MAGNOLIA_COMMON_DATA_H  */


