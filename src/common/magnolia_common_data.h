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
	MAGNOLIA_CMD_COLOR_IMAGE_TO_GRAY_IMAGE    = 0,
	MAGNOLIA_CMD_INVERSION,
	MAGNOLIA_CMD_IMAGE_BINARIZATION,
	MAGNOLIA_CMD_HISTOGRAM,
	MAGNOLIA_CMD_MAX,
};

struct magnolia_image_binarization_type
{
	int value;
	void **dst_image;
};

struct magnolia_param_type
{
	magnolia_cmd_enum_type		mag_cmd;
	union {
		magnolia_image_binarization_type img_binarization;
	} param;
	int result_image_count;
};

struct pixbuf_label
{
	Gdk::Pixbuf *pixbuf;
	Glib::ustring *text;
};

struct gdk_pixbuf_image_info
{
	int row_stride;
	int n_channels;
	int width;
	int height;
	bool has_alpha;
	int colorspace;
	int bits_per_sample;
};

#endif /* _MAGNOLIA_COMMON_DATA_H  */


