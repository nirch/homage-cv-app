/**********************
 ***	ViewSub.c   ***
 *********************/
#include	<stdio.h>
#include	<math.h>


#include	"ViewType.h"



void
view_rescale( view_type *vw, float fk )
{
	vw->uv.x0 *= fk;
	vw->uv.x1 *= fk;
	vw->uv.y0 *= fk;
	vw->uv.y1 *= fk;
 
	view_set_pixel( vw, vw->Vpixel, vw->Upixel );
} 



void
view_sub_upixel( view_type *bvw, int pixel, view_type *vw )
{
float	t;

	*vw = *bvw;

	if( pixel > bvw->Upixel )	return;

	t = 0.5*(bvw->uv.x1 - bvw->uv.x0)*pixel / bvw->Upixel;

	vw->uv.x0 = -t;
	vw->uv.x1 = t;
	vw->uv.y0 = -t;
	vw->uv.y1 = t;

	view_set_pixel( vw, pixel, pixel );
}



void
view_sub_pixel( view_type *bvw, int row, int column, view_type *vw )
{
float	t;

	*vw = *bvw;

	t = 0.5*(bvw->uv.x1 - bvw->uv.x0)*column / bvw->Upixel;

	vw->uv.x0 = -t;
	vw->uv.x1 = t;


	t = 0.5*(bvw->uv.y1 - bvw->uv.y0)*row / bvw->Vpixel;
	vw->uv.y0 = -t;
	vw->uv.y1 = t;

	view_set_pixel( vw, row, column );
}


void
view_crop_pixel( view_type *vw, int row, int column )
{
float	t;


	t = 0.5*(vw->uv.x1 - vw->uv.x0)*column / vw->Upixel;

	vw->uv.x0 = -t;
	vw->uv.x1 = t;


	t = 0.5*(vw->uv.y1 - vw->uv.y0)*row / vw->Vpixel;
	vw->uv.y0 = -t;
	vw->uv.y1 = t;

	view_set_pixel( vw, row, column );
}



void
view_extend_pixel( view_type *vw, int row, int column )
{
float	t,	t0;

	t = 0.5*(vw->uv.x1 - vw->uv.x0)*column / vw->Upixel;

	t0 = 0.5*(vw->uv.x1 + vw->uv.x0);
	vw->uv.x0 = t0 - t;
	vw->uv.x1 = t0 + t;


	t = 0.5*(vw->uv.y1 - vw->uv.y0)*row / vw->Vpixel;

	t0 = 0.5*(vw->uv.y1 + vw->uv.y0);
	vw->uv.y0 = t0 - t;
	vw->uv.y1 = t0 + t;

	view_set_pixel( vw, row, column );
}




void
view_sub_box( view_type *bvw, box2i *box,
			view_type *vw, int *row0, int *col0 )
{
int	tmp,	row,	col;
vec2d	p,	vp;

	if( box->x0 > box->x1 ){
		tmp = box->x1;
		box->x1 = box->x0;
		box->x0 = tmp;
	}

	if( box->y0 > box->y1 ){
		tmp = box->y1;
		box->y1 = box->y0;
		box->y0 = tmp;
	}


	if( box->x0 < 0 )	box->x0 = 0;
	if( box->x1 > bvw->Upixel )	box->x1 = bvw->Upixel;

	if( box->y0 < 0 )	box->y0 = 0;
	if( box->y1 > bvw->Vpixel )	box->y1 = bvw->Vpixel;


	*row0 = box->y0;
	*col0 = box->x0;

	row = box->y1-box->y0;
	col = box->x1-box->x0;

        *vw = *bvw;

	VEC2D_SET( p, box->y0, box->x0 )
	view_pixel2vp( bvw, &p, &vp );
	vw->uv.x0 = vp.x;
	vw->uv.y1 = vp.y;

	VEC2D_SET( p, box->y1, box->x1 )
	view_pixel2vp( bvw, &p, &vp );
	vw->uv.x1 = vp.x;
	vw->uv.y0 = vp.y;

	view_set_pixel( vw, row, col );
}
