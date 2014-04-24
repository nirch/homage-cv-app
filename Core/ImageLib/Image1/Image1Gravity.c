/***************************
 ***   Image1Gravity.c   ***
 **************************/
#include	"Umath/Matrix2Type.h"
#include	"ImageType/ImageType.h"
#include	"Image1Tool.h"


int 
image1_gravity(image_type *im, vec2d *p, int dw, int dh, int nT, vec2d *cg )
{
u_char	*sp;
int	x0,	x1,	y0,	y1;
int	i,	j;
float	mass,	x,	y;
int	no;

	x0 = p->x - dw;
	if( x0 < 0 )	x0 = 0;
	x1 = p->x + dw;
	if( x1 > im->width )	x1 = im->width;

	y0 = p->y - dh;
	if( y0 < 0 )	y0 = 0;
	y1 = p->y + dh;
	if( y1 > im->width )	y1 = im->height;



	x = y = mass = 0;
	no = 0;
	for( i = y0 ; i < y1 ; i++ ){
		sp = IMAGE_PIXEL( im, i, x0 );

		for( j = x0 ; j < x1 ; j++, sp++ ){
			if( *sp == 0 )	continue;

			y += (*(sp))*i;
			x += (*(sp))*j;

			mass += (*(sp));
			no++;
		}
	}

	if( no < nT )	return( -1 );

	if( mass == 0 )	return( -1 );

	cg->x = (float)x / mass;
	cg->y = (float)y / mass;

	return( no );
}


int 
image1_gravityN(image_type *im, int gray, int nT, vec2d *cg )
{
u_char	*sp;
int	i,	j;
float	mass,	x,	y;
int	no;


	x = y = mass = 0;
	no = 0;
	for( i = 0 ; i < im->height ; i++ ){
		sp = IMAGE_PIXEL( im, i, 0 );

		for( j = 0 ; j < im->width ; j++, sp++ ){
			if( *sp != gray )	continue;

			y += (*(sp))*i;
			x += (*(sp))*j;

			mass += (*(sp));
			no++;
		}
	}


	if( no < nT )	return( -1 );


	if( mass == 0 )	return( -1 );

	cg->x = (float)x / mass;
	cg->y = (float)y / mass;

	return( no );
}



image_type *
image1_binary( image_type *sim, int T, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	tp = im->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++, tp++ ){

			*tp = ( *sp <= T )? 0 : 1;


		}
	}

	return( im );
}


image_type *
image1_binaryM( image_type *sim, int T, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;

	im = image_realloc( im, sim->width, sim->height, 1, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	tp = im->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++, tp++ ){

			//			*sp = ( *sp < T )? 1 : 0;
			*tp = ( *sp <= T )? 0 : 255;


		}
	}

	return( im );
}

void
image1_boundary_set( image_type *im, int val )
{
	u_char	*tp;
	int	i,	j;



	tp = im->data;
	for( j = 0 ; j < im->column ; j++ )
		*tp++ = val;


	tp = IMAGE_PIXEL(im, im->height-1, 0 );
	for( j = 0 ; j < im->column ; j++ )
		*tp++ = val;

	tp = IMAGE_PIXEL(im, 0, im->width-1 );
	for( i = 0 ; i < im->height-1 ; i++, tp += im->width )
		tp[0] = tp[1] = val;
}


void
image1_threshold( image_type *sim, int T )
{
u_char	*sp;
int	i,	j;


	sp = (u_char *)sim->data;
	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++ ){

			if( *sp < T )	*sp = 0;
		}
	}
}


//void
//image1_threshold2( image_type *sim, int T0, int T1 )
//{
//	u_char	*sp;
//	int	i,	j;
//
//
//	sp = (u_char *)sim->data;
//	for( i = 0 ; i < sim->row ; i++ ){
//		for( j = 0 ; j < sim->column ; j++, sp++ ){
//
//			if( *sp < T0 || *sp > T1 )	*sp = 0;
//		}
//	}
//}


image_type *
image1_threshold2( image_type *sim, int T0, int T1, image_type *im )
{
u_char	*sp,	*tp;
int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data;
	tp = im->data;
	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++, tp++ ){

			if( *sp < T0 || *sp > T1 )
				*tp = 0;
			else *tp = *sp;
		}
	}

	return( im );
}



image_type *
image1_maskT( image_type *sim, image_type *mim, int T0, int T1, image_type *im )
{
	u_char	*sp,	*tp,	*mp;
	int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = sim->data;
	mp = mim->data;

	tp = im->data;
	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, mp++ ){
			if( *mp < T0 || *mp > T1 ){
				*tp++ = 0;
				sp++;
			}
			else {
				*tp++ = *sp++;
			}

		}
	}

	return( im );
}


image_type *
imageM_inverse( image_type *sim, image_type *im )
{
u_char	*sp,	*mp;
int	i,	j;

	im = image_recreate( im, sim->height, sim->width, 1, 1 );

	sp = (u_char *)sim->data;
	mp = im->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++, mp++ ){

			*mp = ( *sp == 0 )? 1 : 0;
		}
	}

	return( im );
}


//void
//image1_mask( image_type *sim, image_type *mim )
//{
//u_char	*sp,	*mp;
//int	i,	j;
//
//
//	sp = (u_char *)sim->data;
//	mp = mim->data;
//
//	for( i = 0 ; i < sim->row ; i++ ){
//		for( j = 0 ; j < sim->column ; j++, sp++, mp++ ){
//
//			if( *mp == 0  )	*sp = 0;
//		}
//	}
//}


int
image1_mask_no( image_type *sim )
{
u_char	*sp;
int	i,	j,	no;


	sp = (u_char *)sim->data;

	no = 0;
	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++ ){

			if( *sp != 0  )	no++;
		}
	}

	return( no );
}


void
image1_set( image_type *sim, box2i *b, int val )
{
u_char	*sp;
int	i,	j,	x0,	y0,	x1,	y1;



	imageA_box( sim, b, &x0, &y0, &x1, &y1 );

	sp = (u_char *)sim->data;

	for( i = y0 ; i < y1 ; i++ ){
		sp = IMAGE_PIXEL( sim, i, x0 );
		for( j = x0 ; j < x1 ; j++, sp++ ){
			*sp = val;
		}
	}
}




int 
image1_moment(image_type *im, box2i *b, vec2f_type *cg, vec2d *cv, float *ce )
{
u_char	*sp;
int	x0,	x1,	y0,	y1;
int	i,	j;
float	mass,	x,	y,	xx,	xy,	yy;
float	e1,	e2;
matrix2_type	m;
int	no;



	if( (x0 = b->x0) < 0 )	x0 = 0;
	if( (x1 = b->x1) > im->width )	x1 = im->width;

	if( (y0 = b->y0) < 0 )	y0 = 0;
	if( (y1 = b->y1) > im->height )	y1 = im->height;




	x = y = xx = xy = yy = mass = 0;
	no = 0;
	for( i = y0 ; i < y1 ; i++ ){
		sp = IMAGE_PIXEL( im, i, x0 );

		for( j = x0 ; j < x1 ; j++, sp++ ){
			if( *sp == 0 )	continue;

			y += (*(sp))*i;
			x += (*(sp))*j;

			xx += (*(sp))*j*j;
			xy += (*(sp))*j*i;
			yy += (*(sp))*i*i;


			mass += (*(sp));
			no++;
		}
	}

	if( no < 10 )	return( 0 );

	cg->x = x / mass;
	cg->y = y / mass;

	
	m.a00 = xx / mass - cg->x*cg->x;
	m.a01 = m.a10 = xy / mass - cg->x*cg->y;
	m.a11 = yy / mass - cg->y*cg->y;

	
	matrix2S_eigen( &m, &e1, cv, &e2 );
	*ce = e2/e1;

	return( no );
}




int 
image1_momentV(image_type *im, box2i *b, vec2f_type *cg, vec2d *cv, float *ce, int du )
{
u_char	*sp;
int	x0,	x1,	y0,	y1;
int	i,	j;
float	mass,	x,	y,	xx,	xy,	yy;
float	e1,	e2;
matrix2_type	m;
vec2d	u,	dv;
float	t;
int	no;



	if( (x0 = b->x0) < 0 )	x0 = 0;
	if( (x1 = b->x1) > im->width )	x1 = im->width;

	if( (y0 = b->y0) < 0 )	y0 = 0;
	if( (y1 = b->y1) > im->height )	y1 = im->height;



	u.x = -cv->y;
	u.y =  cv->x;

	x = y = xx = xy = yy = mass = 0;
	no = 0;
	for( i = y0 ; i < y1 ; i++ ){
		sp = IMAGE_PIXEL( im, i, x0 );

		for( j = x0 ; j < x1 ; j++, sp++ ){
			if( *sp == 0 )	continue;

			dv.x = j - cg->x;
			dv.y = i - cg->y;

			t = u.x * dv.x + u.y * dv.y;
			if( ABS(t) > du )
				continue;



			y += (*(sp))*i;
			x += (*(sp))*j;

			xx += (*(sp))*j*j;
			xy += (*(sp))*j*i;
			yy += (*(sp))*i*i;


			mass += (*(sp));
			no++;
		}
	}

	if( no < 10 )	return( 0 );

	cg->x = x / mass;
	cg->y = y / mass;


	m.a00 = xx / mass - cg->x*cg->x;
	m.a01 = m.a10 = xy / mass - cg->x*cg->y;
	m.a11 = yy / mass - cg->y*cg->y;


	matrix2S_eigen( &m, &e1, cv, &e2 );
	*ce = e2/e1;

	return( no );
}





int 
image1_momentG(image_type *im, box2i *b, int T0, int T1, vec2f_type *cg, vec2d *cv, float *ce )
{
	u_char	*sp;
	int	x0,	x1,	y0,	y1;
	int	i,	j;
	float	mass,	x,	y,	xx,	xy,	yy;
	float	e1,	e2;
	matrix2_type	m;
	int	no;



	if( (x0 = b->x0) < 0 )	x0 = 0;
	if( (x1 = b->x1) > im->width )	x1 = im->width;

	if( (y0 = b->y0) < 0 )	y0 = 0;
	if( (y1 = b->y1) > im->height )	y1 = im->height;




	x = y = xx = xy = yy = mass = 0;
	no = 0;
	for( i = y0 ; i < y1 ; i++ ){
		sp = IMAGE_PIXEL( im, i, x0 );

		for( j = x0 ; j < x1 ; j++, sp++ ){
			if( *sp < T0 || *sp > T1 )	continue;

			y += (*(sp))*i;
			x += (*(sp))*j;

			xx += (*(sp))*j*j;
			xy += (*(sp))*j*i;
			yy += (*(sp))*i*i;


			mass += (*(sp));
			no++;
		}
	}

	if( no < 10 )	return( 0 );

	cg->x = x / mass;
	cg->y = y / mass;


	m.a00 = xx / mass - cg->x*cg->x;
	m.a01 = m.a10 = xy / mass - cg->x*cg->y;
	m.a11 = yy / mass - cg->y*cg->y;


	matrix2S_eigen( &m, &e1, cv, &e2 );
	*ce = e2/e1;

	return( no );
}



void
image1_mask_point( image_type *im, float x0, float y0, float r )
{
u_char	*tp;
int	i,	j;
box2i	b;
float	x,	y,	r2;

	r2 = r*r;

	if( (b.x0 = x0 - r) < 0 )	b.x0 = 0;
	if( (b.x1 = x0 + r + 1) > im->width )	b.x1 = im->width;

	if( (b.y0 = y0 - r) < 0 )	b.x0 = 0;
	if( (b.y1 = y0 + r + 1) > im->height )	b.x1 = im->height;



	for( i = b.y0 ; i < b.y1 ; i++ ){
		tp = IMAGE_PIXEL( im, i, b.x0 );
		y = i+0.5 - y0;

		for( j = b.x0 ; j < b.x1 ; j++, tp++ ){
			x = j+0.5 - x0;

			if( x*x + y*y < r2 )
				*tp = 0;
		}
	}

}

