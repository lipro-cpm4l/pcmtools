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

#if HAVE_GETOPT_H
# include <getopt.h>
#else
# error missing GNU extension to parse command-line options
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
Copyright (C) 2002-%d %s\n\
License: GNU GPL v2+ <http://www.gnu.org/licenses/gpl.html>\n\
This is free software.  There is NO WARRANTY, to the extent permitted by law.\n",
              COPYRIGHT_YEAR, "Li-Pro.Net");
}

/* Exit status codes:
 * null     if full success
 * positive if partial success
 * negative if failure
 */
#define PCMT_EXSTAT_XPM_CE	(XpmColorError)	/* XpmColorError = 1 */
#define PCMT_EXSTAT_OK		(XpmSuccess)	/* XpmSuccess = 0 */
#define PCMT_EXSTAT_XPM_OF	(XpmOpenFailed)	/* XpmOpenFailed = -1 */
#define PCMT_EXSTAT_XPM_FI	(XpmFileInvalid)/* XpmFileInvalid = -2 */
#define PCMT_EXSTAT_XPM_NM	(XpmNoMemory)	/* XpmNoMemory = -3 */
#define PCMT_EXSTAT_XPM_CF	(XpmColorFailed)/* XpmColorFailed = -4 */
#define PCMT_EXSTAT_WRONGOPT	(-10)
#define PCMT_EXSTAT_NOFILES	(-11)
#define PCMT_EXSTAT_NOMEM	(-12)
#define PCMT_EXSTAT_CONVERR	(-13)
#define PCMT_EXSTAT_CRITERR	(-20)

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
