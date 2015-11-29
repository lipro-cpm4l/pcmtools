/*
 * Tool for the original PC/M 64x16 character generator PROM.
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
 *
 * TODO:2000 centralized the CLI i/f in libmkcg
 */

#include "pcmtools.h"

#define BOUND_BITS	7	/* !!! never less than EXP_WIDTH !!! */
#define BOUND_BYTES	8

#define EXP_WIDTH	7	/* !!! never more than 8 !!! */
#define EXP_HIGHT	8	/* !!! never more than BOUND_BYTES !!! */
#define EXP_MAX_COLOR	2	/* !!! more than 2 colors are stupid, because
				       only one dot color will be used !!! */
#define EXP_DOT_COLOR	"#000000"


mkcg_cg CG;


static void usage(char *progname)
{
	INF("Usage: %s [OPTION]... XPMFILE...", progname);
	INF("%s", "Convert one or more pixmaps into a special hexdump needed by the");
	INF("%s", "original PC/M 64x16 character generator PROM. You can translate");
	INF("%s", "this hexdump into a plain binary file with the powerful Vim tool");
	INF("%s", "'xxd' and further into srecord.");
	INF("%s", "");
	INF("%s", "-b,       --banner           make banner dump");
	INF("%s", "-h,       --help             show this quick help message");
	INF("%s", "-n,       --neg              negated output (banner and hexdump)");
	INF("%s", "-o[COLS], --overview[=COLS]  build an new pixmap with all XPMFILEs together");
	INF("%s", "-q,       --quiet            don't report any errors");
	INF("%s", "-v,       --verbose          show more progress information at stderr");
	INF("%s", "-x,       --hexdump          make xxd conform hexdump (default)");
	INF("%s", "");
	INF("Package: %s", PACKAGE);
	INF("Version: %s", VERSION);
	INF("Please, report any errors to <%s>", PACKAGE_BUGREPORT);
}

#define EXSTAT_OK	0
#define EXSTAT_HELP	101
#define EXSTAT_WRONGOPT	102
#define EXSTAT_NOFILES	103
#define EXSTAT_CRITERR	104
#define EXSTAT_NOMEM	105
#define EXSTAT_CONVERR	106

static void exit_func(int status, void *arg)
{
	mkcg_cg	*cgp = (mkcg_cg *)arg;

	switch (status) {

		case EXSTAT_WRONGOPT:
#if 0
			if (!(cgp->options & OPT_MKCG_QUIET))
#endif
			ERR("%s\n", "invalid option");
		case EXSTAT_HELP:
			usage(cgp->progname);
			break;

		case EXSTAT_NOFILES:
			if (!(cgp->options & OPT_MKCG_QUIET))
				ERR("%s\n", "missing file list");
			break;

		case EXSTAT_CRITERR:
			ERR("%s\n", "critical internal error");
			break;

		case EXSTAT_NOMEM:
			if (!(cgp->options & OPT_MKCG_QUIET))
				ERR("%s\n", "no memory");
			break;

		case EXSTAT_CONVERR:
			if (!(cgp->options & OPT_MKCG_QUIET))
				ERR("%s\n", "internal conversion error");
			break;

		case XpmColorError:
			if (!(cgp->options & OPT_MKCG_QUIET))
				ERR("%s\n", "XPM: color error");
			break;

		case XpmOpenFailed:
			if (!(cgp->options & OPT_MKCG_QUIET))
				ERR("%s\n", "XPM: can not open file");
			break;

		case XpmFileInvalid:
			if (!(cgp->options & OPT_MKCG_QUIET))
				ERR("%s\n", "XPM: no pixmap file");
			break;

		case XpmNoMemory:
			if (!(cgp->options & OPT_MKCG_QUIET))
				ERR("%s\n", "XPM: no memory");
			break;

		case XpmColorFailed:
			if (!(cgp->options & OPT_MKCG_QUIET))
				ERR("%s\n", "internal conversion error");
			break;

		case EXSTAT_OK:
		default:
			break;
	}

	if (cgp->ch) free(cgp->ch);
}

int main(int argc, char **argv)
{
	unsigned int	cnt;
	int		status;

	CG.options	= OPT_MKCG_LEFTBOUND /* default not: OPT_MKCG_NEGATED */;
	CG.progname	= argv[0];
	CG.ch		= (mkcg_ch *)NULL;

	CG.bound_bits		= BOUND_BITS;
	CG.bound_bytes		= BOUND_BYTES;
	CG.exp_ch_width		= EXP_WIDTH;
	CG.exp_ch_hight		= EXP_HIGHT;
	CG.exp_ch_max_color	= EXP_MAX_COLOR;
	CG.exp_ch_dot_color	= EXP_DOT_COLOR;

	on_exit(exit_func, (void *)&CG);

	while (1) {
		int option_index = 0;
		int c;
		static struct option long_options[] = {
			{"banner",	no_argument,		0, 'b'},
			{"help",	no_argument,		0, 'h'},
			{"hexdump",	no_argument,		0, 'x'},
			{"neg",		no_argument,		0, 'n'},
			{"overview",	optional_argument,	0, 'o'},
			{"quiet",	no_argument,		0, 'q'},
			{"verbose",	no_argument,		0, 'v'},
			{0, 0, 0, 0},
		};

		c = getopt_long (argc, argv, ":bhno::qxv",
				long_options, &option_index);
		if (c == -1) break;

		switch (c) {
			case 'b':
				CG.options |= (CG.options & OPT_MKCG_ACTIONMASK) ?
					CG.options : OPT_MKCG_BANNER;
				break;

			case 'x':
				CG.options |= (CG.options & OPT_MKCG_ACTIONMASK) ?
					CG.options : OPT_MKCG_HEXDUMP;
				break;

			case 'o':
				CG.options |= (CG.options & OPT_MKCG_ACTIONMASK) ?
					CG.options : OPT_MKCG_OVERVIEW;
				if ((optarg != NULL) && isdigit(optarg[0]))
					CG.opt_overview_cols = atol(optarg);
				else
					CG.opt_overview_cols = 16;
				break;

			case 'n':
				CG.options |= OPT_MKCG_NEGATED;
				CG.options |= OPT_MKCG_INVERSE;
				break;

			case 'q':
				CG.options |= OPT_MKCG_QUIET;
				break;

			case 'v':
				CG.options |= OPT_MKCG_VERBOSE;
				break;

			case 'h':
				exit(EXSTAT_HELP);

			default:
				exit(EXSTAT_WRONGOPT);
		}
	}

	CG.options |= (CG.options & OPT_MKCG_ACTIONMASK) ? CG.options : OPT_MKCG_HEXDUMP;

	if (optind < argc) {

		CG.number = 0;
		while (optind < argc) {
			CG.number++;
			optind++;
		}

		optind -= CG.number;

		if (!(CG.ch = (mkcg_ch *)malloc(CG.number * sizeof(mkcg_ch))))
			exit(EXSTAT_NOMEM);

		for (cnt = 0; cnt < CG.number; cnt++) {

			CG.ch[cnt].filename		= argv[optind++];
			CG.ch[cnt].info.valuemask	= XpmReturnComments
							| XpmReturnExtensions;
			status = XpmReadFileToXpmImage(CG.ch[cnt].filename,
					&(CG.ch[cnt].image),
					&(CG.ch[cnt].info));
			if (status != XpmSuccess)
				exit(status);

			if (!mkcg_isvalidch(&(CG.ch[cnt].image), &CG, &(CG.ch[cnt])))
				exit(EXSTAT_CONVERR);

		}

		for (cnt = 0; cnt < CG.number; cnt++) {

			if (CG.options & OPT_MKCG_VERBOSE)
				INF("process: %s ... ", CG.ch[cnt].filename);

			switch (CG.options & OPT_MKCG_ACTIONMASK) {

				case OPT_MKCG_BANNER:
					mkcg_out_banner(&CG, &(CG.ch[cnt]));
					break;

				case OPT_MKCG_HEXDUMP:
					mkcg_out_xxd(&CG, &(CG.ch[cnt]), cnt * CG.bound_bytes);
					break;

				case OPT_MKCG_OVERVIEW:
					if (!cnt) mkcg_out_xpm(&CG);
					break;

				default:
					exit(EXSTAT_CRITERR);
			}
		}

		for (cnt = 0; cnt < CG.number; cnt++) {

			XpmFreeXpmImage(&(CG.ch[cnt].image));
			XpmFreeXpmInfo(&(CG.ch[cnt].info));

		}

		exit(EXSTAT_OK);

	} else {

		usage(argv[0]);
		exit(EXSTAT_NOFILES);
	}
}
