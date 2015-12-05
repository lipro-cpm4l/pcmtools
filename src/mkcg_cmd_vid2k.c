/*
 * Tool for the PC/M VID2k character generator PROM.
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

#define DESCRIPTION	"\
Convert one or more pixmaps into a special hexdump as needed for the\n\
PC/M VID2k character generator PROM. You can translate this hexdump\n\
into a plain binary file with the powerful Vim tool 'xxd' and\n\
further into srecord.\n"

#define BOUND_BITS	8	/* !!! never less than EXP_WIDTH !!! */
#define BOUND_BYTES	16

#define EXP_WIDTH	8	/* !!! never more than 8 !!! */
#define EXP_HIGHT	10	/* !!! never more than BOUND_BYTES !!! */
#define EXP_MAX_COLOR	2	/* !!! more than 2 colors are stupid, because
				       only one dot color will be used !!! */
#define EXP_DOT_COLOR	"#000000"


mkcg_cg CG;


int main(int argc, char **argv)
{
	CG.progname	= argv[0];
	CG.description	= DESCRIPTION;

	CG.options	= OPT_MKCG_NEGATED;
	CG.ch		= (mkcg_ch *)NULL;

	CG.bound_bits		= BOUND_BITS;
	CG.bound_bytes		= BOUND_BYTES;
	CG.exp_ch_width		= EXP_WIDTH;
	CG.exp_ch_hight		= EXP_HIGHT;
	CG.exp_ch_max_color	= EXP_MAX_COLOR;
	CG.exp_ch_dot_color	= EXP_DOT_COLOR;

	mkcg_getopt(&CG, argc, argv);
	mkcg_execopt(&CG, argc, argv);
}
