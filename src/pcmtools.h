/*
 * Interface definition of the PC/M Tool Chain core functions.
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

#include "config.h"
#include <stdio.h>

#if STDC_HEADERS
# include <ctype.h>
# include <stdlib.h>
# include <string.h>
#else
# error missing standard C headers
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#else
# error missing POSIX operating system API
#endif

#if HAVE_STDBOOL_H
# include <stdbool.h>
#else
# if !HAVE__BOOL
#  ifdef __cplusplus
typedef bool _Bool;
#  else
typedef unsigned char _Bool;
#  endif
# endif
# define bool	_Bool
# define false	0
# define true	1
# define __bool_true_false_are_defined 1
#endif

#if defined(HAVE_X11_XPM_H) && defined(HAVE_LIBXPM)
# include <X11/xpm.h>
#else
# error missing Xpm library interface
#endif


#define OUT(FORMAT,...) fprintf(stdout, FORMAT, __VA_ARGS__)
#define INF(FORMAT,...) fprintf(stderr, FORMAT "\n", __VA_ARGS__)
#define ERR(FORMAT,...) fprintf(stderr, "%s:%d::%s(): " FORMAT "\n", \
			__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)


typedef struct {

	XpmImage	image;
	XpmInfo		info;

	unsigned int	dot_color_id;

	char		*filename;

} mkcg_ch;

typedef struct {

	char			*progname;
	char			*description;

	unsigned int		bound_bits;
	unsigned int		bound_bytes;
	unsigned int		exp_ch_width;
	unsigned int		exp_ch_hight;
	unsigned int		exp_ch_max_color;
	const char		*exp_ch_dot_color;

	size_t			number;
	mkcg_ch			*ch;

	unsigned int		options;
				/* actions */
				#define OPT_MKCG_BANNER		0x00000001
				#define OPT_MKCG_HEXDUMP	0x00000002
				#define OPT_MKCG_OVERVIEW	0x00000004
				#define OPT_MKCG_ACTIONMASK	( OPT_MKCG_BANNER \
								| OPT_MKCG_HEXDUMP \
								| OPT_MKCG_OVERVIEW )
				/* memory manipulation */
				#define OPT_MKCG_NEGATED	0x00010000	/* CG content */
				#define OPT_MKCG_LEFTBOUND	0x00020000
				/* output manipulation */
				#define OPT_MKCG_INVERSE	0x20000000
				#define OPT_MKCG_QUIET		0x40000000
				#define OPT_MKCG_VERBOSE	0x80000000
	unsigned int		opt_overview_cols;

} mkcg_cg;


void mkcg_getopt(mkcg_cg *cg, int argc, char **argv);
void mkcg_execopt(mkcg_cg *cg, int argc, char **argv);

bool mkcg_isvalidch(XpmImage *image, mkcg_cg *cg, mkcg_ch *ch);
bool mkcg_out_banner(mkcg_cg *cg, mkcg_ch *ch);
bool mkcg_out_xxd(mkcg_cg *cg, mkcg_ch *ch, unsigned int addr);
bool mkcg_out_xpm(mkcg_cg *cg);

