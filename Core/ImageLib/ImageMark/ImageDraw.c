/*************************
 ***   ImageDraw.cpp   ***
 *************************/
#include	<math.h>
#include	"Uigp/igp.h"

#include	"ImageType/ImageType.h"

#include	"ImageMark.h"




void
image4_draw_circle( image_type *sim, float x, float y, float r, int color )
{
u_int	*tp;
int	i0,	j0;

	j0 = x + 0.5;
	i0 = y + 0.5;

	j0 = x;// + 0.5;
	i0 = y;// + 0.5;


	if( i0 < 0 || i0 > sim->row )	return;
	if( j0 < 0 || j0 > sim->column )	return;

	tp = (u_int * )IMAGE_PIXEL( sim, i0, j0 );

	*tp = color;
}



static void	image1_mask_circle( image_type *im, float x, float y, float r, int color );

void
image_mask_circle( image_type *im, float x, float y, float r, int color )
{
	if( im->depth == 1 ){
		image1_mask_circle( im, x, y, r, color );
		return;
	}
}


static void
image1_mask_circle( image_type *im, float x, float y, float r, int color )
{
u_char	*tp;
int	i0,	j0,	i,	j;
float	dx,	dy,	d;

	j0 = x -r -1;
	i0 = y -r -1;


	for( i = i0 ; i < i0 + 2*r+1 ; i++ ){
		if( i < 0 || i >= im->row )	continue;
		tp = IMAGE_PIXEL( im, i, j0 );

		dy = i + 0.5 - y;
		for( j = j0 ; j < j0 + 2*r+1 ; j++, tp++ ){
			if( j < 0 || j >= im->column )	continue;

			dx = j + 0.5 - x;

			d = dx *dx + dy * dy;

			if( d > r*r )	continue;

			*tp = color;
		}
	}
}


void
image_draw_rectangle( image_type *im, int x0, int y0, int dx, int dy, int color )
{
int	i,	j;
int	x1,	y1;

	if( (x1 = x0 + dx) > im->width )	x1 = im->width;
	if( x0 < 0 )	x0 = 0;
	if( (y1 = y0 + dy) > im->height )	y1 = im->height;
	if( y0 < 0 )	y0 = 0;


	if( im->depth == 1 ){
		for( i = y0 ; i < y1 ; i++ ){
			u_char *tp = IMAGE_PIXEL( im, i, x0 );
			for( j = x0 ; j < x1 ; j++, tp++ )
				*tp = color;
		}
		return;
	}

	if( im->depth == 4 ){
		for( i = y0 ; i < y1 ; i++ ){
			u_int *tp = (u_int *)IMAGE_PIXEL( im, i, x0 );
			for( j = x0 ; j < x1 ; j++, tp++ )
				*tp = color;
		}
		return;
	}
}


void
image4_draw_rectangle( image_type *im, int col0, int row0, int col, int row, int color )
{
int	i,	j;



//	i = ( row0 >= 0 )? 0 : -row0;
	for( i = 0 ; i < row && row0 + i < im->row ; i++ ){
		if( row0 + i >= im->row || row0 + i < 0 )	continue;
		if( col0 >= 0 )
			*IMAGE4_PIXEL( im, row0 + i, col0 ) = color;
		if( col0+col < im->column )
			*IMAGE4_PIXEL( im, row0 + i, col0+col ) = color;
	}

//	i = ( col0 >= 0 )? 0 : -col0;
	for( j = 0 ; j < col && col0 + j < im->column ; j++ ){
		if( col0 + j >= im->column || col0 + j < 0 )	continue;
		if( row0 >=0 && row0 < im->height )
			*IMAGE4_PIXEL( im, row0, col0 + j ) = color;
		if( row0 + row < im->row && row0 + row >= 0 )
			*IMAGE4_PIXEL( im, row0 + row, col0 + j ) = color;
	}
}

void
image4_draw_point( image_type *im, int x, int y, int color )
{
	if( x < 1 || x >= im->column-2 || y < 1 || y >= im->row-2 )
		return;

	if( y > 0 && x >= 0  )
		*IMAGE4_PIXEL( im, y-1, x ) = color;
	if( x > 0 )
		*IMAGE4_PIXEL( im, y, x-1 ) = color;
	*IMAGE4_PIXEL( im, y, x ) = color;

	if( x < im->column-2 )
		*IMAGE4_PIXEL( im, y, x+1 ) = color;

	if( y < im->row-2 )
		*IMAGE4_PIXEL( im, y+1, x ) = color;
}





void
image_draw_line( image_type *im, float x0, float y0, float x1, float y1, int color )
{
	int	i0,	j0,	i1,	j1,	i,	j;
	float	dx,	dy,	d,	t;
	float	len;
	vec2d	v,	u;


	v.x = x1-x0;
	v.y = y1-y0;

	len =  hypot( v.x, v.y );
	v.x /= len;
	v.y /= len;

	u.x = v.y;
	u.y = -v.x;


	if( y0 < y1 ){
		i0 = y0 - 2.5;
		i1 = y1 + 2.5;
	}
	else	{
		i0 = y1 - 2.5;
		i1 = y0 + 2.5;
	}

	if( x0 < x1 ){
		j0 = x0 - 2.5;
		j1 = x1 + 2.5;
	}
	else	{
		j0 = x1 - 2.5;
		j1 = x0 + 2.5;
	}

	if( i0 < 0 )	i0 = 0;
	if( i1 > im->height )	i1 = im->height;

	if( j0 < 0 )	j0 = 0;
	if( j1 > im->width )	j1 = im->width;



	if( im->depth == 1 ){
		u_char	c = color;
		for( i = i0 ; i < i1 ; i++ ){
			u_char	*tp = (u_char *)IMAGE_PIXEL( im, i, j0 );

			dy = i + 0.5 - y0;
			for( j = j0 ; j < j1 ; j++, tp++ ){

				dx = j + 0.5 - x0;

				t = v.x * dx + v.y * dy;
				if( t < -0.5 || t > len+0.5 )	continue;

				d = u.x * dx + u.y * dy;
				if( d < -0.5 || d > 0.5 )	continue;

				*tp = c;
			}
		}
		return;
	}



	if( im->depth == 4 ){
		for( i = i0 ; i < i1 ; i++ ){
			u_int	*tp = (u_int *)IMAGE_PIXEL( im, i, j0 );

			dy = i + 0.5 - y0;
			for( j = j0 ; j < j1 ; j++, tp++ ){

				dx = j + 0.5 - x0;

				t = v.x * dx + v.y * dy;
				if( t < -0.5 || t > len+0.5 )	continue;
				d = u.x * dx + u.y * dy;
				if( d < -0.5 || d > 0.5 )	continue;

				*tp = color;
			}
		}
		return;
	}


	if( im->depth == 3 ){
		int R = IMAGE4_RED( color );
		int G = IMAGE4_GREEN( color );
		int B = IMAGE4_BLUE( color );


		for( i = i0 ; i < i1 ; i++ ){
			u_char *tp = IMAGE_PIXEL( im, i, j0 );

			dy = i + 0.5 - y0;
			for( j = j0 ; j < j1 ; j++, tp += 3 ){

				dx = j + 0.5 - x0;

				t = v.x * dx + v.y * dy;
				if( t < -0.5 || t > len+0.5 )	continue;
				d = u.x * dx + u.y * dy;
				if( d < -0.5 || d > 0.5 )	continue;

				*tp     = R;
				*(tp+1) = G;
				*(tp+2) = B;
			}
		}
		return;
	}

}

void
image4_draw_line( image_type *im, float x0, float y0, float x1, float y1, int color )
{
u_int	*tp;
int	i0,	j0,	i1,	j1,	i,	j;
float	dx,	dy,	d,	t;
float	len;
vec2d	v,	u;



	v.x = x1-x0;
	v.y = y1-y0;
	
	len =  hypot( v.x, v.y );
	v.x /= len;
	v.y /= len;

	u.x = v.y;
	u.y = -v.x;


	if( y0 < y1 ){
		i0 = y0 - 2.5;
		i1 = y1 + 2.5;
	}
	else	{
		i0 = y1 - 2.5;
		i1 = y0 + 2.5;
	}

	if( x0 < x1 ){
		j0 = x0 - 2.5;
		j1 = x1 + 2.5;
	}
	else	{
		j0 = x1 - 2.5;
		j1 = x0 + 2.5;
	}




	for( i = i0 ; i < i1 ; i++ ){
		if( i < 0 || i >= im->row )	continue;
		tp = (u_int *)IMAGE_PIXEL( im, i, j0 );

		dy = i + 0.5 - y0;
		for( j = j0 ; j < j1 ; j++, tp++ ){
			if( j < 0 || j >= im->column )	continue;

			dx = j + 0.5 - x0;

			t = v.x * dx + v.y * dy;
			if( t < -0.5 || t > len+0.5 )	continue;
			d = u.x * dx + u.y * dy;
			if( d < -0.5 || d > 0.5 )	continue;

			*tp = color;
		}
	}
}



static void	image1_draw_strip( image_type *im, vec2d *p0, vec2d *p1, float tm, float um, int color );

void
image_draw_strip( image_type *im, vec2d *p0, vec2d *p1, float tm, float um, int color )
{
	if( im->depth == 1 ){
		image1_draw_strip( im, p0, p1, tm, um, color );
		return;
	}
}


static void
image1_draw_strip( image_type *im, vec2d *p0, vec2d *p1, float tm, float um, int color )
{
	u_char	*tp;
	int	i0,	j0,	i1,	j1,	i,	j;
	float	dx,	dy,	d;
	vec2d	v,	u;
	float	t,len;


	d = MAX( tm, um )+2;
	if( p0->x < p1->x ){
		j0 = p0->x - d;
		j1 = p1->x + d;
	}
	else {
		j1 = p0->x + d;
		j0 = p1->x - d;
	}

	if( p0->y < p1->y ){
		i0 = p0->y - d;
		i1 = p1->y + d;
	}
	else {
		i1 = p0->y + d;
		i0 = p1->y - d;
	}


	v.x  = p1->x - p0->x;
	v.y  = p1->y - p0->y;

	len = hypot( v.x, v.y );
	v.x /= len;
	v.y /= len;

	u.x = v.y;
	u.y = -v.x;

	for( i = i0 ; i < i1 ; i++ ){
		if( i < 0 || i >= im->row )	continue;
		tp = IMAGE_PIXEL( im, i, j0 );

		dy = i + 0.5 - p0->y;
		for( j = j0 ; j < j1  ; j++, tp++ ){
			if( j < 0 || j >= im->column )	continue;

			dx = j + 0.5 - p0->x;

			t = dx *v.x + dy * v.y;
			if( t < -tm || t > len +tm )
				continue;

			t = dx *u.x + dy * u.y;
			if( t < -um || t > um )
				continue;

			*tp = color;
		}
	}
}


void
image4_draw_skeleton( image_type *im, vec2d p[], int color, int color1 )
{
	int	k;

	image4_draw_line( im, p[0].x, p[0].y, p[1].x, p[1].y, color  );
	image4_draw_line( im, p[1].x, p[1].y, p[2].x, p[2].y, color  );
	image4_draw_line( im, p[2].x, p[2].y, p[3].x, p[3].y, color  );

	image4_draw_line( im, p[2].x, p[2].y, p[4].x, p[4].y, color  );
	image4_draw_line( im, p[4].x, p[4].y, p[5].x, p[5].y, color  );
	image4_draw_line( im, p[5].x, p[5].y, p[6].x, p[6].y, color  );

	image4_draw_line( im, p[2].x, p[2].y, p[7].x, p[7].y, color  );
	image4_draw_line( im, p[7].x, p[7].y, p[8].x, p[8].y, color  );
	image4_draw_line( im, p[8].x, p[8].y, p[9].x, p[9].y, color  );


	image4_draw_line( im, p[0].x, p[0].y, p[10].x, p[10].y, color  );
	image4_draw_line( im, p[10].x, p[10].y, p[11].x, p[11].y, color  );
	image4_draw_line( im, p[11].x, p[11].y, p[12].x, p[12].y, color  );

	image4_draw_line( im, p[0].x, p[0].y, p[13].x, p[13].y, color  );
	image4_draw_line( im, p[13].x, p[13].y, p[14].x, p[14].y, color  );
	image4_draw_line( im, p[14].x, p[14].y, p[15].x, p[15].y, color  );


	for(  k = 0 ; k < 16 ; k++ ){
		//		int	color = 0xFF0000;
		image4_draw_point( im, p[k].x, p[k].y, color1 );
	}


}


void
image_draw_ellipse( image_type *im, float x, float y,
				   float r1, float r2, float angle, int color )
{
	float	cosa,	sina,	t,	scale,	pi_2;
	float	rxt,	ryt,	xt,	yt;
	int	wx,	wy;

	pi_2 = (float)(2.0 * M_PI);
	scale = pi_2 / (6 * r2 );

	cosa = (float)cos(angle);
	sina = (float)sin(angle);

	for( t = 0 ; t <= pi_2; t += scale ) {
		xt = (float)(r1 * cos(t));
		yt = (float)(r2 * sin(t));


		rxt = cosa * xt + sina * yt;
		ryt = -sina * xt + cosa * yt;


		wx = x+rxt;
		wy = y+ryt;

		if( wx < 0 || wx >= im->width )	continue;
		if( wy < 0 || wy >= im->height )	continue;

		if( im->depth == 1 ){
			u_char *tp = (u_char *)IMAGE_PIXEL( im, wy, wx );
			*tp = color;
		}
		else	{
			u_int *tp = (u_int *)IMAGE_PIXEL( im, wy, wx );
			*tp = color;
		}


	}
}