#include	<stdio.h>
#include	"Uigp/igp.h"
#include	"mbmap.h"

#define		GRAY_LEVEL( g )		( ( g < 0)? 0: (( g > 255)? 255 : g ) )


#define		CV_YQA_TO_RGB( y, q, a, r, g, b )	\
{ \
    int	t; \
	 t = (int)(q) + (int)(y) -128; \
	*r = (u_char)GRAY_LEVEL( t ); \
					\
	t = (int)(a) + (int)(y) -128; \
	*b = (u_char)GRAY_LEVEL( t ); \
					\
	t = (int)(((float) (y) -0.299F*(float)*(r) - \
	      	0.114F*(float)*(b))/0.587F ); \
	*g = (u_char)GRAY_LEVEL( t ); \
	}


#define		CV_RGB_TO_YQA( y, q, a, r, g, b )	\
{ \
    int	t; \
\
	t = 0.299 * (float)(r) + 0.587*(float)(g) + 0.114*(float) (b); \
	y = (u_char) GRAY_LEVEL(t); \
					\
	t = (float) (r) - (int) (y) + 128; \
	q = (u_char) GRAY_LEVEL(t); \
					\
	t = (float) (b) - (int) (y) + 128; \
	a = (u_char) GRAY_LEVEL(t); \
}




void
mbmap_cv_yqa_to_rgb(mbmap y, mbmap q, mbmap a, mbmap * r, mbmap * g, mbmap * b)
{
int     row, column, i, j;
u_char *py, *pq, *pa;
u_char *pr, *pg, *pb;

	row = MBMAP_ROW(y);
	column = MBMAP_COLUMN(y);

	MBMAP_CREATE(*r, row, column);
	MBMAP_CREATE(*g, row, column);
	MBMAP_CREATE(*b, row, column);

	pr = MBMAP_PIXEL_ADRESS(*r, 0, 0);
	pg = MBMAP_PIXEL_ADRESS(*g, 0, 0);
	pb = MBMAP_PIXEL_ADRESS(*b, 0, 0);

	for (i = 0; i < row; i++) {
		py = MBMAP_PIXEL_ADRESS(y, i, 0);
		pq = MBMAP_PIXEL_ADRESS(q, i / 2, 0);
		pa = MBMAP_PIXEL_ADRESS(a, i / 2, 0);

		for (j = 0; j < column; j += 2) {

			CV_YQA_TO_RGB(*py, *pq, *pa, pr, pg, pb);
			py++;
			pr++; pg++; pb++;

			CV_YQA_TO_RGB(*py, *pq, *pa, pr, pg, pb);
			py++; pq++; pa++;
			pr++; pg++; pb++;
		}
	}
}


void
mbmap_cv_yqa1_to_rgb(mbmap y, mbmap q, mbmap a, mbmap * r, mbmap * g, mbmap * b)
{
	int     row, column, i, j;
	u_char *py, *pq, *pa;
	u_char *pr, *pg, *pb;

	row = MBMAP_ROW(y);
	column = MBMAP_COLUMN(y);

	MBMAP_CREATE(*r, row, column);
	MBMAP_CREATE(*g, row, column);
	MBMAP_CREATE(*b, row, column);

	pr = MBMAP_PIXEL_ADRESS(*r, 0, 0);
	pg = MBMAP_PIXEL_ADRESS(*g, 0, 0);
	pb = MBMAP_PIXEL_ADRESS(*b, 0, 0);

	for (i = 0; i < row; i++) {
		py = MBMAP_PIXEL_ADRESS(y, i, 0);
		pq = MBMAP_PIXEL_ADRESS(q, i, 0);
		pa = MBMAP_PIXEL_ADRESS(a, i, 0);

		for (j = 0; j < column; j += 2) {

			CV_YQA_TO_RGB(*py, *pq, *pa, pr, pg, pb);
			py++;
			pr++; pg++; pb++;

			CV_YQA_TO_RGB(*py, *pq, *pa, pr, pg, pb);
			py++; pq++; pa++;
			pr++; pg++; pb++;
		}
	}
}



void
mbmap_cv_rgb_to_yqa(mbmap *r, mbmap *g, mbmap *b, mbmap *y, mbmap *q, mbmap *a )
{
int     row, column, i, j;
u_char *py, *pq, *pa;
u_char *pr, *pg, *pb;

	row = MBMAP_ROW(*r);
	column = MBMAP_COLUMN(*r);

	MBMAP_CREATE(*y, row, column);
	MBMAP_CREATE(*q, row, column);
	MBMAP_CREATE(*a, row, column);

	pr = MBMAP_PIXEL_ADRESS(*r, 0, 0);
	pg = MBMAP_PIXEL_ADRESS(*g, 0, 0);
	pb = MBMAP_PIXEL_ADRESS(*b, 0, 0);

	for (i = 0; i < row; i++) {
		py = MBMAP_PIXEL_ADRESS(*y, i, 0);
		pq = MBMAP_PIXEL_ADRESS(*q, i, 0);
		pa = MBMAP_PIXEL_ADRESS(*a, i, 0);

		for (j = 0; j < column; j += 1) {

			CV_RGB_TO_YQA(*py, *pq, *pa, *pr, *pg, *pb);
			py++; pq++; pa++;
			pr++; pg++; pb++;
		}
	}
}


void
mbmap_print_ascii_bmap(mbmap bmap, FILE * fp, int n)
{
	u_char *p;
	int     i, j, k;

	p = MBMAP_PIXEL_ADRESS(bmap, 0, 0);

	for (i = 0; i < MBMAP_ROW(bmap); i++) {
		for (j = 0, k = 0; j < MBMAP_COLUMN(bmap); j++) {
			fprintf(fp, "%4d", (int) (*p++));
			k++;
			if (k >= n) {
				fprintf(fp, "\n");
				k = 0;
			}
		}

		if (k > 0)
			fprintf(fp, "\n");

		fprintf(fp, "\n\n");
	}
}


void
mbmap_frame_to_fileds(mbmap bmap, mbmap * f1, mbmap * f2)
{
	int     row, column;
	int     i, j;
	u_char *sp, *tp1, *tp2;

	row = MBMAP_ROW(bmap) / 2;
	column = MBMAP_COLUMN(bmap);

	MBMAP_CREATE(*f1, row, column);
	MBMAP_CREATE(*f2, row, column);

	sp = MBMAP_PIXEL_ADRESS(bmap, 0, 0);
	tp1 = MBMAP_PIXEL_ADRESS(*f1, 0, 0);
	tp2 = MBMAP_PIXEL_ADRESS(*f2, 0, 0);

	for (i = 0; i < row; i++) {

		for (j = 0; j < column; j++)
			*tp1++ = *sp++;

		for (j = 0; j < column; j++)
			*tp2++ = *sp++;
	}
}


void
mbmap_fileds_to_frame(mbmap f1, mbmap f2, mbmap * frame)
{
	int     row, column;
	int     i;
	u_char *tp, *sp1, *sp2;

	row = MBMAP_ROW(f1) * 2;
	column = MBMAP_COLUMN(f2);

	MBMAP_CREATE(*frame, row, column);

	tp = MBMAP_PIXEL_ADRESS(*frame, 0, 0);
	sp1 = MBMAP_PIXEL_ADRESS(f1, 0, 0);
	sp2 = MBMAP_PIXEL_ADRESS(f2, 0, 0);

	for (i = 0; i < row; i += 2) {
		memcpy((char *) tp, (char *) sp1, column);
		tp += column;
		sp1 += column;

		memcpy((char *) tp, (char *) sp2, column);
		tp += column;
		sp2 += column;
	}
}


void
mbmap_copy_to_exist_mbmap(mbmap s, mbmap t)
{
	int     i, j;
	int     row, column;
	u_char *sp, *tp, tmp;


	sp = MBMAP_DATA_ADRESS(s);
	tp = MBMAP_DATA_ADRESS(t);

	column = (MBMAP_COLUMN(s) < MBMAP_COLUMN(t)) ?
		MBMAP_COLUMN(s) : MBMAP_COLUMN(t);
	row = (MBMAP_ROW(s) < MBMAP_ROW(t)) ?
		MBMAP_ROW(s) : MBMAP_ROW(t);

	for (i = 0; i < row; i++) {

		memcpy((char *) tp, (char *) sp, column);
		tp += column;

		tmp = *(sp + column - 1);
		for (j = MBMAP_COLUMN(s); j < MBMAP_COLUMN(t); j++)
			*tp++ = tmp;

		sp += MBMAP_COLUMN(s);
	}


	sp = MBMAP_PIXEL_ADRESS(t, row - 1, 0);

	for (i = MBMAP_ROW(s); i < MBMAP_ROW(t); i++) {

		memcpy((char *) tp, (char *) sp, MBMAP_COLUMN(t));
		tp += MBMAP_COLUMN(t);
	}
}



void
mbmap_copy_sub_mbmap_to_farray(mbmap bmap, int x0, int y0, int row, int column,
			       float *arry, int acolumn)
{
	u_char *sp;
	float  *tp;
	int     i, j;
	int     algin, a_algin;

	tp = arry;
	a_algin = acolumn - column;

	sp = MBMAP_PIXEL_ADRESS(bmap, x0, y0);
	algin = MBMAP_COLUMN(bmap) - column;

	for (i = 0; i < row; i++, sp += algin, tp += a_algin)
		for (j = 0; j < column; j++)
			*tp++ = (float) *(sp++) - 128.0;
}


void
mbmap_copy_farry_to_sub_mbmap(float *arry, int acolumn,
			    mbmap bmap, int x0, int y0, int row, int column)
{
	float  *sp;
	u_char *tp;
	int     i, j;
	int     algin, a_algin;
	float   temp;

	sp = arry;
	a_algin = acolumn - column;

	tp = MBMAP_PIXEL_ADRESS(bmap, x0, y0);
	algin = MBMAP_COLUMN(bmap) - column;

	for (i = 0; i < row; i++, tp += algin, sp += a_algin)
		for (j = 0; j < column; j++) {
			temp = (*sp++) + 128.0F;
			*tp++ = (u_char) GRAY_LEVEL(temp);
		}
}




static void
mbmap_fill_pixel_line(u_char * sp, int len,
		      int fg_gval, int bg_gval,
		      int bar_type_no, int *bar_width, int *bar_quant)
{
	int     i, j;
	u_char *tp;

	tp = sp;
	memset((void *) tp, bg_gval, len);

	while (1 /* always */ )
		for (i = 0; i < bar_type_no; i++)
			for (j = 0; j < bar_quant[i]; j++) {
				if (tp >= sp + len)
					return;
				memset((void *) tp, fg_gval, bar_width[i]);
				tp += 2 * bar_width[i];
				if (tp >= sp + len)
					return;
			}

}


mbmap
mbmap_create_horizontal_barred_bmap(int rows, int cols,
				    int fg_gval, int bg_gval,
			    int bar_type_no, int *bar_width, int *bar_quant)
{
	mbmap   b_bmap;
	int     r;
	u_char *sp;

	MBMAP_CREATE(b_bmap, rows, cols);

	sp = MBMAP_DATA_ADRESS(b_bmap);

	mbmap_fill_pixel_line(sp, cols, fg_gval, bg_gval,
			      bar_type_no, bar_width, bar_quant);

	for (r = 1; r < rows; r++)
		memcpy((char *) (sp + cols * r), (char *) sp, cols);

	return (b_bmap);

}

mbmap
mbmap_create_vertical_barred_bmap(int rows, int cols,
				  int fg_gval, int bg_gval,
			    int bar_type_no, int *bar_width, int *bar_quant)
{
	mbmap   T_bmap;

	T_bmap = mbmap_create_horizontal_barred_bmap(cols, rows, fg_gval, bg_gval,
					 bar_type_no, bar_width, bar_quant);

	return (mbmap_transpose(T_bmap));




}
