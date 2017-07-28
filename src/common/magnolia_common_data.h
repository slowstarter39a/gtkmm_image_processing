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

#define SUCCESS				0
#define FAILURE				1

enum magnolia_cmd_enum_type
{
	MAGNOLIA_CMD_INVERSE 		= 0,
	MAGNOLIA_CMD_HISTOGRAM , 
	MAGNOLIA_CMD_MAX,
};

struct magnolia_cmd_type
{
	magnolia_cmd_enum_type		mag_cmd;
};

#endif /* _MAGNOLIA_COMMON_DATA_H  */


