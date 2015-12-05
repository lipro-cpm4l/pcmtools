/*
 * Core functions to handle different PC/M character generator PROMs.
 *
 * Copyright (C) 2002-2015  Stephan Linz <linz@li-pro.net>
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

bool mkcg_isvalidch(XpmImage *image, mkcg_cg *cg, mkcg_ch *ch)
{
	unsigned int cnt;

	if ((image->width != cg->exp_ch_width) || (image->height != cg->exp_ch_hight)) {
		if (!(cg->options & OPT_MKCG_QUIET))
			ERR("dimension validation: %d x %d (%d x %d expected)",
					image->width, image->height,
					cg->exp_ch_width, cg->exp_ch_hight);
		return false;
	}

	if (image->ncolors > cg->exp_ch_max_color) {
		if (!(cg->options & OPT_MKCG_QUIET))
			ERR("too many colors: %d colors (%d colors expected)",
					image->ncolors, cg->exp_ch_max_color);
		return false;
	}

	for (cnt = 0; cnt < image->ncolors; cnt++) {
		if (strcmp(cg->exp_ch_dot_color, image->colorTable[cnt].c_color) == 0) {
			ch->dot_color_id = cnt;
			cnt = ~cg->exp_ch_max_color;
			break;
		}
		else {	/*
			 * FIXME:1000 overflow condition 255 -> 0
			 */
			ch->dot_color_id = image->ncolors + 1;
		}
	}

	if ((cnt != ~cg->exp_ch_max_color) && (image->ncolors != cg->exp_ch_max_color - 1)) {
		if (!(cg->options & OPT_MKCG_QUIET))
			ERR("missing dot color: %s expected", cg->exp_ch_dot_color);
		return false;
	}

	return true;
}
