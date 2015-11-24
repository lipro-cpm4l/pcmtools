#include "pcmtools.h"

bool mkcg_isvalidch(XpmImage *image, mkcg_cg *cg, mkcg_ch *ch)
{
	int cnt;

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
		else {
			ch->dot_color_id = image->ncolors + 1; /* FIXME: overflow condition 255 -> 0 */
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
	int	cnt_w, cnt_h, bit, bits;

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
	int	cnt, cnt_w, cnt_h, bit, bits, bytes;

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
		//strncpy(buf, string, strlen(string));

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
	char		*buf;
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
