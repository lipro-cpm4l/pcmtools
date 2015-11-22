#include "pcmtools.h"

#define BOUND_BITS	7	/* !!! never less than EXP_WIDTH !!! */
#define BOUND_BYTES	8

#define EXP_WIDTH	7	/* !!! never more than 8 !!! */
#define EXP_HIGHT	8	/* !!! never more than BOUND_BYTES !!! */
#define EXP_MAX_COLOR	2	/* !!! more than 2 colors are stupid, because
				       only one dot color will be used !!! */
#define EXP_DOT_COLOR	"#000000"


mkzg_zg ZG;


static void usage(char *progname)
{
	INF("Usage: %s [OPTION]... XPMFILE...", progname);
	INF("%s", "Convert one or more pixmaps into a special hexdump needed by the");
	INF("%s", "original PC/M 64x16 character generator PROM. You can translate");
	INF("%s", "this hexdump into a plain binary file with the powerful Vim tool");
	INF("%s", "'xxd' and further into srecord.");
	INF("%s", "");
	INF("%s", "-b,        --banner    make banner dump");
	INF("%s", "-h,        --help      show this quick help message");
	INF("%s", "-n,        --neg       negated output (banner and hexdump)");
	INF("%s", "-o [COLS], --overview  build an new pixmap with all XPMFILEs together");
	INF("%s", "-q,        --quiet     don't report any errors");
	INF("%s", "-v,        --verbose   show more progress information at stderr");
	INF("%s", "-x,        --hexdump   make xxd conform hexdump (default)");
	INF("%s", "");
	INF("Package: %s", PACKAGE);
	INF("Version: %s", VERSION);
	INF("%s", "Please, report any errors to <linz@li-pro.net>");
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
	mkzg_zg	*zgp = (mkzg_zg *)arg;

	switch (status) {

		case EXSTAT_WRONGOPT:
#if 0
			if (!(zgp->options & OPT_MKZG_QUIET))
#endif
			ERR("%s\n", "invalid option");
		case EXSTAT_HELP:
			usage(zgp->progname);
			break;

		case EXSTAT_NOFILES:
			if (!(zgp->options & OPT_MKZG_QUIET))
				ERR("%s\n", "missing file list");
			break;

		case EXSTAT_CRITERR:
			ERR("%s\n", "critical internal error");
			break;

		case EXSTAT_NOMEM:
			if (!(zgp->options & OPT_MKZG_QUIET))
				ERR("%s\n", "no memory");
			break;

		case EXSTAT_CONVERR:
			if (!(zgp->options & OPT_MKZG_QUIET))
				ERR("%s\n", "internal conversion error");
			break;

		case XpmColorError:
			if (!(zgp->options & OPT_MKZG_QUIET))
				ERR("%s\n", "XPM: color error");
			break;

		case XpmOpenFailed:
			if (!(zgp->options & OPT_MKZG_QUIET))
				ERR("%s\n", "XPM: can not open file");
			break;

		case XpmFileInvalid:
			if (!(zgp->options & OPT_MKZG_QUIET))
				ERR("%s\n", "XPM: no pixmap file");
			break;

		case XpmNoMemory:
			if (!(zgp->options & OPT_MKZG_QUIET))
				ERR("%s\n", "XPM: no memory");
			break;

		case XpmColorFailed:
			if (!(zgp->options & OPT_MKZG_QUIET))
				ERR("%s\n", "internal conversion error");
			break;

		case EXSTAT_OK:
		default:
			break;
	}

	if (zgp->z) free(zgp->z);
}

main(int argc, char **argv)
{
	unsigned int	cnt;
	unsigned int	black;
	int		status;
	Pixmap		pixmap;
	XpmImage	image;
	XpmInfo		info;

	ZG.options	= OPT_MKZG_LEFTBOUND /* default not: OPT_MKZG_NEGATED */;
	ZG.progname	= argv[0];
	ZG.z		= (mkzg_z *)NULL;

	ZG.bound_bits		= BOUND_BITS;
	ZG.bound_bytes		= BOUND_BYTES;
	ZG.exp_z_width		= EXP_WIDTH;
	ZG.exp_z_hight		= EXP_HIGHT;
	ZG.exp_z_max_color	= EXP_MAX_COLOR;
	ZG.exp_z_dot_color	= EXP_DOT_COLOR;

	on_exit(exit_func, (void *)&ZG);

	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		int c;
		static struct option long_options[] = {
			{"banner", 0, 0, 'b'},
			{"help", 0, 0, 'h'},
			{"hexdump", 0, 0, 'x'},
			{"neg", 0, 0, 'n'},
			{"overview", 0, 0, 'o'},
			{"quiet", 0, 0, 'q'},
			{"verbose", 0, 0, 'v'},
			{0, 0, 0, 0},
		};

		c = getopt_long (argc, argv, "bhno:qxv",
				long_options, &option_index);
		if (c == -1) break;

		switch (c) {
			case 'b':
				ZG.options |= (ZG.options & OPT_MKZG_ACTIONMASK) ?
					ZG.options : OPT_MKZG_BANNER;
				break;

			case 'x':
				ZG.options |= (ZG.options & OPT_MKZG_ACTIONMASK) ?
					ZG.options : OPT_MKZG_HEXDUMP;
				break;

			case 'o':
				ZG.options |= (ZG.options & OPT_MKZG_ACTIONMASK) ?
					ZG.options : OPT_MKZG_OVERVIEW;
				if (isdigit(optarg[0]))
					ZG.opt_overview_cols = atol(optarg);
				else {
					optind--;
					ZG.opt_overview_cols = 16;
				}
				break;

			case 'n':
				ZG.options |= OPT_MKZG_NEGATED;
				ZG.options |= OPT_MKZG_INVERSE;
				break;

			case 'q':
				ZG.options |= OPT_MKZG_QUIET;
				break;

			case 'v':
				ZG.options |= OPT_MKZG_VERBOSE;
				break;

			case 'h':
				exit(EXSTAT_HELP);

			default:
				exit(EXSTAT_WRONGOPT);
		}
	}

	ZG.options |= (ZG.options & OPT_MKZG_ACTIONMASK) ? ZG.options : OPT_MKZG_HEXDUMP;

	if (optind < argc) {

		ZG.number = 0;
		while (optind < argc) {
			ZG.number++;
			optind++;
		}

		optind -= ZG.number;

		if (!(ZG.z = (mkzg_z *)malloc(ZG.number * sizeof(mkzg_z))))
			exit(EXSTAT_NOMEM);

		for (cnt = 0; cnt < ZG.number; cnt++) {

			ZG.z[cnt].filename		= argv[optind++];
			ZG.z[cnt].info.valuemask	= XpmReturnComments
							| XpmReturnExtensions;
			status = XpmReadFileToXpmImage(ZG.z[cnt].filename,
					&(ZG.z[cnt].image),
					&(ZG.z[cnt].info));
			if (status != XpmSuccess)
				exit(status);

			if (!mkzg_isvalidz(&(ZG.z[cnt].image), &ZG, &(ZG.z[cnt])))
				exit(EXSTAT_CONVERR);

		}

		for (cnt = 0; cnt < ZG.number; cnt++) {

			if (ZG.options & OPT_MKZG_VERBOSE)
				INF("process: %s ... ", ZG.z[cnt].filename);

			switch (ZG.options & OPT_MKZG_ACTIONMASK) {

				case OPT_MKZG_BANNER:
					mkzg_out_banner(&ZG, &(ZG.z[cnt]));
					break;

				case OPT_MKZG_HEXDUMP:
					mkzg_out_xxd(&ZG, &(ZG.z[cnt]), cnt * ZG.bound_bytes);
					break;

				case OPT_MKZG_OVERVIEW:
					if (!cnt) mkzg_out_xpm(&ZG);
					break;

				default:
					exit(EXSTAT_CRITERR);
			}
		}

		for (cnt = 0; cnt < ZG.number; cnt++) {

			XpmFreeXpmImage(&(ZG.z[cnt].image));
			XpmFreeXpmInfo(&(ZG.z[cnt].info));

		}

		exit(EXSTAT_OK);

	} else {

		usage(argv[0]);
		exit(EXSTAT_NOFILES);
	}
}
