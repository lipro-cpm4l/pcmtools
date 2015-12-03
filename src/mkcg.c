/*
 * Core functions to handle different PC/M character generator PROMs.
 *
 * Copyright (C) 2002-21015  Stephan Linz <linz@li-pro.net>
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

#if HAVE_UNISTD_H && HAVE_GETOPT_H
# include <unistd.h>
# include <getopt.h>
#else
# error missing unistd.h or getopt.h
#endif

static const char *_mkcg_optstring_noneg = ":bho::qxvV";
static const struct option _mkcg_options_noneg[] = {
	{"banner",	no_argument,		0, 'b'},
	{"help",	no_argument,		0, 'h'},
	{"hexdump",	no_argument,		0, 'x'},
	{"overview",	optional_argument,	0, 'o'},
	{"quiet",	no_argument,		0, 'q'},
	{"verbose",	no_argument,		0, 'v'},
	{"version",	no_argument,		0, 'V'},
	{0, 0, 0, 0},
};

static const char *_mkcg_optstring_neg = ":bhno::qxvV";
static const struct option _mkcg_options_neg[] = {
	{"banner",	no_argument,		0, 'b'},
	{"help",	no_argument,		0, 'h'},
	{"hexdump",	no_argument,		0, 'x'},
	{"neg",		no_argument,		0, 'n'},
	{"overview",	optional_argument,	0, 'o'},
	{"quiet",	no_argument,		0, 'q'},
	{"verbose",	no_argument,		0, 'v'},
	{"version",	no_argument,		0, 'V'},
	{0, 0, 0, 0},
};

/* Print help info.  This long message is split into
 * several pieces to help translators be able to align different
 * blocks and identify the various pieces. */
static void _mkcg_print_help(mkcg_cg *cg)
{
	/* TRANSLATORS: --help output 1 (synopsis)
	 * no-wrap */
	OUT("\
Usage: %s [OPTION]... XPMFILE...\n",
		cg->progname ? cg->progname : "no programm");

	/* TRANSLATORS: --help output 2 (brief description)
	 * no-wrap */
	OUT("\n%s", cg->description ? cg->description : "no description");

	/* TRANSLATORS: --help output 3 (options 1/4)
	 * no-wrap */
	OUT("%s", "\n\
Options:\n\
  -h, --help     display this help and exit\n\
  -V, --version  display version information and exit\n");

	/* TRANSLATORS: --help output 4 (options 2/4)
	 * no-wrap */
	OUT("%s", "\n\
  -q, --quiet    don't report any errors\n\
  -v, --verbose  show more progress information on stderr\n");

	/* TRANSLATORS: --help output 5 (options 3/4)
	 * no-wrap */
	OUT("%s", "\n\
  -x, --hexdump  make xxd conform hexdump (default)\n\
  -b, --banner   make banner dump\n\
  -o[COLS], --overview[=COLS]\n\
                 make a new pixmap with all XPMFILEs merged together\n");

	/* TRANSLATORS: --help output 6 (options 4/4)
	 * no-wrap */
	if (!(cg->options & OPT_MKCG_NEGATED)) {
		OUT("%s", "\n\
  -n, --neg      negated (inverse) output (banner and hexdump)\n");
	}

	/* TRANSLATORS: --help output 6 (end)
	 * TRANSLATORS: the placeholder indicates the bug-reporting address
	 * for this application.  Please add _another line_ with the
	 * address for translation bugs.
	 * no-wrap */
	OUT("\n\
Report bugs to <%s>\n", PACKAGE_BUGREPORT);
	OUT("\
%s home page: <%s>\n", PACKAGE_NAME, PACKAGE_URL);
}

/* Print version and copyright information.  */
static void _mkcg_print_version(mkcg_cg *cg)
{
	OUT("%s (%s) %s\n", cg->progname, PACKAGE, VERSION);
	/* xgettext: no-wrap */
	OUT("%s", "\n");

	/* It is important to separate the year from the rest of the message,
	 * as done here, to avoid having to retranslate the message when a new
	 * year comes around.  */
	OUT("\
Copyright (C) %s %s\n\
License: GNU GPL v2+ <http://www.gnu.org/licenses/gpl.html>\n\
This is free software.  There is NO WARRANTY, to the extent permitted by law.\n",
              "2002-2015", "Li-Pro.Net");
	OUT("\n\
Written by: %s\n", "Stephan Linz <linz@li-pro.net>");
}

static void _mkcg_exit_func(int status, void *arg)
{
	mkcg_cg	*cg = (mkcg_cg *)arg;

	switch (status) {

		case PCMT_EXSTAT_WRONGOPT:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "invalid option");
			break;

		case PCMT_EXSTAT_NOFILES:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "missing file list");
			break;

		case PCMT_EXSTAT_CRITERR:
			ERR("%s\n", "critical internal error");
			break;

		case PCMT_EXSTAT_NOMEM:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "no memory");
			break;

		case PCMT_EXSTAT_CONVERR:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "internal conversion error");
			break;

		case PCMT_EXSTAT_XPM_CE:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "XPM: color error");
			break;

		case PCMT_EXSTAT_XPM_OF:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "XPM: can not open file");
			break;

		case PCMT_EXSTAT_XPM_FI:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "XPM: no pixmap file");
			break;

		case PCMT_EXSTAT_XPM_NM:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "XPM: no memory");
			break;

		case PCMT_EXSTAT_XPM_CF:
			if (!(cg->options & OPT_MKCG_QUIET))
				ERR("%s\n", "internal conversion error");
			break;

		default:
			break;
	}

	if (cg->ch) free(cg->ch);
}

void mkcg_getopt(mkcg_cg *cg, int argc, char **argv)
{
	on_exit(_mkcg_exit_func, (void *)cg);

	while (true) {
		static const char *optstring;
		static const struct option *long_options;
		int option_index = 0;
		int c;

		if (cg->options & OPT_MKCG_NEGATED) {
			optstring = _mkcg_optstring_noneg;
			long_options = _mkcg_options_noneg;
		} else {
			optstring = _mkcg_optstring_neg;
			long_options = _mkcg_options_neg;
		}

		c = getopt_long (argc, argv, optstring,
				long_options, &option_index);
		if (c == -1) break;

		switch (c) {
			case 'b':
				cg->options |= (cg->options & OPT_MKCG_ACTIONMASK) ?
					cg->options : OPT_MKCG_BANNER;
				break;

			case 'x':
				cg->options |= (cg->options & OPT_MKCG_ACTIONMASK) ?
					cg->options : OPT_MKCG_HEXDUMP;
				break;

			case 'o':
				cg->options |= (cg->options & OPT_MKCG_ACTIONMASK) ?
					cg->options : OPT_MKCG_OVERVIEW;
				if ((optarg != NULL) && isdigit(optarg[0]))
					cg->opt_overview_cols = atol(optarg);
				else
					cg->opt_overview_cols = 16;
				break;

			case 'n':
				cg->options |= OPT_MKCG_NEGATED;
				cg->options |= OPT_MKCG_INVERSE;
				break;

			case 'q':
				cg->options |= OPT_MKCG_QUIET;
				break;

			case 'v':
				cg->options |= OPT_MKCG_VERBOSE;
				break;

			case 'V':
				_mkcg_print_version(cg);
				exit(EXIT_SUCCESS);

			case 'h':
				_mkcg_print_help(cg);
				exit(EXIT_SUCCESS);

			default:
				exit(PCMT_EXSTAT_WRONGOPT);
		}
	}
}

void mkcg_execopt(mkcg_cg *cg, int argc, char **argv)
{
	unsigned int	cnt;
	int		status;

	cg->options |= (cg->options & OPT_MKCG_ACTIONMASK) ? cg->options : OPT_MKCG_HEXDUMP;

	if (optind < argc) {

		cg->number = 0;
		while (optind < argc) {
			cg->number++;
			optind++;
		}

		optind -= cg->number;

		if (!(cg->ch = (mkcg_ch *)malloc(cg->number * sizeof(mkcg_ch))))
			exit(PCMT_EXSTAT_NOMEM);

		for (cnt = 0; cnt < cg->number; cnt++) {

			cg->ch[cnt].filename		= argv[optind++];
			cg->ch[cnt].info.valuemask	= XpmReturnComments
							| XpmReturnExtensions;
			status = XpmReadFileToXpmImage(cg->ch[cnt].filename,
					&(cg->ch[cnt].image),
					&(cg->ch[cnt].info));
			if (status != XpmSuccess)
				exit(status);

			if (!mkcg_isvalidch(&(cg->ch[cnt].image), cg, &(cg->ch[cnt])))
				exit(PCMT_EXSTAT_CONVERR);

		}

		for (cnt = 0; cnt < cg->number; cnt++) {

			if (cg->options & OPT_MKCG_VERBOSE)
				INF("process: %s ... ", cg->ch[cnt].filename);

			switch (cg->options & OPT_MKCG_ACTIONMASK) {

				case OPT_MKCG_HEXDUMP:
					mkcg_out_xxd(cg, &(cg->ch[cnt]), cnt * cg->bound_bytes);
					break;

				case OPT_MKCG_BANNER:
					mkcg_out_banner(cg, &(cg->ch[cnt]));
					break;

				case OPT_MKCG_OVERVIEW:
					if (!cnt) mkcg_out_xpm(cg);
					break;

				default:
					exit(PCMT_EXSTAT_CRITERR);
			}
		}

		for (cnt = 0; cnt < cg->number; cnt++) {

			XpmFreeXpmImage(&(cg->ch[cnt].image));
			XpmFreeXpmInfo(&(cg->ch[cnt].info));

		}

		exit(EXIT_SUCCESS);

	} else {
		exit(PCMT_EXSTAT_NOFILES);
	}
}

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

bool mkcg_out_xxd(mkcg_cg *cg, mkcg_ch *ch, unsigned int addr)
{
	unsigned int	cnt, cnt_w, cnt_h, bit, bits, bytes;

	OUT("%07X: ", addr);

	bytes = 0;

	for (cnt_h = 0; cnt_h < ch->image.height; cnt_h++) {

		for (cnt_w = bits = 0; cnt_w < ch->image.width; cnt_w++) {

			bit = (ch->image.data[cnt_h * ch->image.width + cnt_w]
					== ch->dot_color_id) ? 1 : 0;
			bits += bit;
			bits <<= 1;

		}

		bytes++;

		bits >>= 1;
		bits = cg->options & OPT_MKCG_NEGATED ? ~bits : bits;
		bits &= (1 << cg->bound_bits) - 1;

		OUT("%02X ", cg->options & OPT_MKCG_LEFTBOUND ? (bits << (8 - cg->bound_bits)) : bits);

	}

	for (cnt = 0; cnt < cg->bound_bytes - bytes; cnt++) {

		bits = cg->options & OPT_MKCG_NEGATED ? ~0 : 0;
		bits &= (1 << cg->bound_bits) - 1;

		OUT("%02X ", cg->options & OPT_MKCG_LEFTBOUND ? (bits << (8 - cg->bound_bits)) : bits);

	}

	OUT("%s", "\n");

	return true;
}

static void set_XpmColor(XpmColor *entry,
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

static void unset_XpmColor(XpmColor *entry)
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
		set_XpmColor(&colortable[COLID_DOT],	" ", "None");
		set_XpmColor(&colortable[COLID_NONE],	"#", "#000000");
	}
	else {
		set_XpmColor(&colortable[COLID_NONE],	" ", "None");
		set_XpmColor(&colortable[COLID_DOT],	"#", "#000000");
	}
	set_XpmColor(&colortable[COLID_BORDER],	"|", "#FF0000");
	set_XpmColor(&colortable[COLID_UNDEF],	"_", "#FFFF00");

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

	unset_XpmColor(&colortable[0]);
	unset_XpmColor(&colortable[1]);
	unset_XpmColor(&colortable[2]);

	if (colortable) free(colortable);

	return true;
}
