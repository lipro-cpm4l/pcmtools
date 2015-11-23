#include "config.h"
#include <stdio.h>

#if STDC_HEADERS
# include <ctype.h>
# include <stdlib.h>
# include <string.h>
#else
# error missing standard C headers
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

#if HAVE_UNISTD_H && HAVE_GETOPT_H
# include <unistd.h>
# include <getopt.h>
#else
# error missing unistd.h or getopt.h
#endif

#if defined(HAVE_X11_XPM_H) && defined(HAVE_LIBXPM)
# include <X11/xpm.h>
#else
# error missing Xpm library
#endif


#define ERR(FORMAT,...) fprintf(stderr, "%s(): " FORMAT "\n", __FUNCTION__, __VA_ARGS__)
#define INF(FORMAT,...) fprintf(stderr, FORMAT "\n", __VA_ARGS__)
#define OUT(FORMAT,...) fprintf(stdout, FORMAT, __VA_ARGS__)


typedef struct {

	XpmImage	image;
	XpmInfo		info;

	unsigned int	dot_color_id;

	char		*filename;

} mkzg_z;

typedef struct {

	char			*progname;

	unsigned int		bound_bits;
	unsigned int		bound_bytes;
	unsigned int		exp_z_width;
	unsigned int		exp_z_hight;
	unsigned int		exp_z_max_color;
	const unsigned char	*exp_z_dot_color;

	size_t			number;
	mkzg_z			*z;

	unsigned int		options;
				/* actions */
				#define OPT_MKZG_BANNER		0x00000001
				#define OPT_MKZG_HEXDUMP	0x00000002
				#define OPT_MKZG_OVERVIEW	0x00000004
				#define OPT_MKZG_ACTIONMASK	( OPT_MKZG_BANNER \
								| OPT_MKZG_HEXDUMP \
								| OPT_MKZG_OVERVIEW )
				/* memory manipulation */
				#define OPT_MKZG_NEGATED	0x00010000	/* ZG content */
				#define OPT_MKZG_LEFTBOUND	0x00020000
				/* output manipulation */
				#define OPT_MKZG_INVERSE	0x20000000
				#define OPT_MKZG_QUIET		0x40000000
				#define OPT_MKZG_VERBOSE	0x80000000
	unsigned int		opt_overview_cols;

} mkzg_zg;


bool mkzg_isvalidz(XpmImage *image, mkzg_zg *zg, mkzg_z *z);
bool mkzg_out_banner(mkzg_zg *zg, mkzg_z *z);
bool mkzg_out_xxd(mkzg_zg *zg, mkzg_z *z, unsigned int addr);
bool mkzg_out_xpm(mkzg_zg *zg);

