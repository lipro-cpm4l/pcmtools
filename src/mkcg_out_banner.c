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

bool mkcg_out_banner(mkcg_cg *cg, mkcg_ch *ch)
{
	unsigned int	cnt_w, cnt_h, bit, bits;

	OUT("%s\n", "____________________");

	for (cnt_h = 0; cnt_h < ch->image.height; cnt_h++) {

		OUT("%s", "|");

		for (cnt_w = bits = 0; cnt_w < ch->image.width; cnt_w++) {

			bit = (ch->image.data[cnt_h * ch->image.width + cnt_w]
					== ch->dot_color_id) ? 1 : 0;

			OUT("%s", bit	? cg->options & OPT_MKCG_INVERSE ? " " : "#"
					: cg->options & OPT_MKCG_INVERSE ? "#" : " ");

			bits += bit;
			bits <<= 1;

		}

		bits >>= 1;
		bits = cg->options & OPT_MKCG_NEGATED ? ~bits : bits;
		bits &= (1 << cg->bound_bits) - 1;

		OUT("|  0x%02X\n", cg->options & OPT_MKCG_LEFTBOUND ? (bits << (8 - cg->bound_bits)) : bits);

	}

	return true;
}
