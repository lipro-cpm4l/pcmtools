/*
 * Core functions to handle different PC/M character generator PROMs.
 *
 * Copyright (C) 2002-2020  Stephan Linz <linz@li-pro.net>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA  02110-1301, USA.
 */

#include "pcmtools.h"

static void _mkcg_SetXpmColor(XpmColor *entry,
		const char *string, const char *c_color)
{
#define CONCAT_AT_POINTER(PTR,STR) { \
		char *buf; \
		buf = (char *)malloc(strlen(STR) + 1); \
		snprintf(buf, strlen(STR) + 1, "%s", STR); \
		PTR = buf; \
	}

	CONCAT_AT_POINTER(entry->string, string);
	CONCAT_AT_POINTER(entry->c_color, c_color);

	entry->symbolic	=
	entry->m_color	=
	entry->g4_color	=
	entry->g_color	= (char *)NULL;

#undef CONCAT_AT_POINTER
}

static void _mkcg_UnsetXpmColor(XpmColor *entry)
{
#define FREE_AT_POINTER(PNT)	if (PNT) free(PNT)
	if (entry) {
		FREE_AT_POINTER(entry->string);
		FREE_AT_POINTER(entry->symbolic);
		FREE_AT_POINTER(entry->m_color);
		FREE_AT_POINTER(entry->g4_color);
		FREE_AT_POINTER(entry->g_color);
		FREE_AT_POINTER(entry->c_color);
	}
#undef FREE_AT_POINTER
}

bool mkcg_out_xpm(mkcg_cg *cg)
{
#define COLNUM		4
#define COLID_NONE	0
#define COLID_DOT	1
#define COLID_BORDER	2
#define COLID_UNDEF	3
#define BORDER_WIDTH	1
	unsigned int	pixel_cnt, pixel_col_cnt, pixel_row_cnt,
			col_cnt, row_cnt, char_cnt, pixel_in_char;
	unsigned int	cols		= cg->opt_overview_cols;
	unsigned int	rows_full	= cg->number / cols;
	unsigned int	parts_last_row	= cg->number - (cols * rows_full);
	unsigned int	pixel_in_col	= cg->ch[0].image.width;
	unsigned int	pixel_in_cols	= cols * pixel_in_col;
	unsigned int	pixel_in_row	= cg->ch[0].image.height;
	unsigned int	pixel_in_rows	= (parts_last_row ?
						rows_full + 1 : rows_full)
						* pixel_in_row;
	unsigned int	pixel_num	= pixel_in_cols * pixel_in_rows;
	unsigned int	*pixeldata;
	XpmColor	*colortable;
	XpmImage	image;

	char filename[] = "/dev/stdout";

	colortable = (XpmColor *)malloc(COLNUM * sizeof(XpmColor));
	pixeldata  = (unsigned int *)malloc(pixel_num * sizeof(unsigned int));

	memset((void *)&image, 0, sizeof(image));

	if (cg->options & OPT_MKCG_INVERSE) {
		_mkcg_SetXpmColor(&colortable[COLID_DOT],	" ", "None");
		_mkcg_SetXpmColor(&colortable[COLID_NONE],	"#", "#000000");
	}
	else {
		_mkcg_SetXpmColor(&colortable[COLID_NONE],	" ", "None");
		_mkcg_SetXpmColor(&colortable[COLID_DOT],	"#", "#000000");
	}
	_mkcg_SetXpmColor(&colortable[COLID_BORDER],	"|", "#FF0000");
	_mkcg_SetXpmColor(&colortable[COLID_UNDEF],	"_", "#FFFF00");

	if (cg->options & OPT_MKCG_VERBOSE) {
		INF("cols:\t\t%d ", cols);
		INF("rows_full:\t%d ", rows_full);
		INF("parts_last_row:\t%d ", parts_last_row);
		INF("pixel_in_col:\t%d ", pixel_in_col);
		INF("pixel_in_cols:\t%d ", pixel_in_cols);
		INF("pixel_in_row:\t%d ", pixel_in_row);
		INF("pixel_in_rows:\t%d ", pixel_in_rows);
	}

	for (pixel_cnt = col_cnt = row_cnt = char_cnt = 0;
			pixel_cnt < pixel_num; pixel_cnt++) {

		/* position calc */
		pixel_col_cnt	= (pixel_cnt % pixel_in_cols);
		pixel_row_cnt	= pixel_cnt / pixel_in_cols;
		col_cnt		= pixel_col_cnt / pixel_in_col;
		row_cnt		= pixel_row_cnt / pixel_in_row;
		char_cnt	= col_cnt + (row_cnt * cols);
		pixel_in_char	= ((pixel_row_cnt - (row_cnt * pixel_in_row)) * pixel_in_col)
				+ (pixel_cnt % pixel_in_col);

		/* pixel in character */
		if (char_cnt < cg->number) {
			if (		(cg->ch[char_cnt].image.width  == pixel_in_col)
				&&	(cg->ch[char_cnt].image.height == pixel_in_row)
				&&	(cg->ch[char_cnt].image.data[pixel_in_char]
						== cg->ch[char_cnt].dot_color_id)		) {

				pixeldata[pixel_cnt] = COLID_DOT;

			} else {

				pixeldata[pixel_cnt] = COLID_NONE;

			}

		} else {

			pixeldata[pixel_cnt] = COLID_UNDEF;

		}
	}

	image.width		= pixel_in_cols;
	image.height		= pixel_in_rows;
	image.cpp		= 1;
	image.ncolors		= COLNUM;
	image.colorTable	= colortable;
	image.data		= pixeldata;

	XpmWriteFileFromXpmImage(filename, &image, NULL);

	_mkcg_UnsetXpmColor(&colortable[0]);
	_mkcg_UnsetXpmColor(&colortable[1]);
	_mkcg_UnsetXpmColor(&colortable[2]);

	if (colortable) free(colortable);

	return true;
}
