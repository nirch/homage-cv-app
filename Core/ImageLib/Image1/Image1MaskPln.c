/****************************
 ***    Image1MaskPln.c   ***
 ****************************/
#ifdef _DEBUG
#define _DUMP
#endif

#include	<math.h>
#include	<string.h>


#include  "Uln/LnType.h"
#include  "Uln/PlnType.h"
#include  "Uln/Cln/ClnType.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"


static void	contour_image_mask_normal( image_type *sim );
static void	image1_mask_append( image_type *sim, image_type *im );

static void	image1_mask_normal( image_type *sim );


#ifdef _AA_
image_type *
contourA_image_mask( contourA_type *ac, int width, int height, image_type *im )
{
image_type	*tim;
int	i;

	im = image_realloc( im, width, height, 1, IMAGE_TYPE_U8, 1 );
	image1_const( im, 0 );

	tim = NULL;
	for( i = 0 ; i < ac->nC ; i++ ){
		tim = contour_image_mask( ac->c[i], width, height, tim );
		image1_mask_append( tim, im );
	}

	if( tim != NULL )
		image_destroy( tim, 1 );

	return( im );

}
#endif



static void	linkL_expand( vec2d *ctr, ln_type *link, int Fbr, image_type *image );

static void	link_expand( vec2d *ctr, ln_type *l, int Fbr, image_type *image );

static void  image1_mask_pln_fill( image_type *image );


image_type *
image1_mask_plnA( plnA_type *apl, int width, int height, image_type *im )
{
	pln_type	*pl;
	int	i,	Fbr;


	im = image_realloc( im, width, height, 1, IMAGE_TYPE_U8, 1 );
	image1_const( im, 0 );


//	contour_set_dirrection( c );


	Fbr = 2;	// right contour
	for( i = 0 ; i < apl->nA ; i++ ){
		pl = apl->a[i];
		linkL_expand( &pl->ctr, pl->link, Fbr, im );
	}


//	IMAGE_DUMP( im, "BB", 2, NULL );

	image1_mask_pln_fill( im );


//	contour_image_mask_normal( im );


	return( im );
}


image_type *
image1_mask_cln( cln_type *cln, int width, int height, int fNormal, image_type *im )
{
	pln_type	*pl;
	int	i,	Fbr;


	im = image_realloc( im, width, height, 1, IMAGE_TYPE_U8, 1 );
	image1_const( im, 0 );


	cln_set_dirrection( cln );


	Fbr = 2;	// right contour
	for( i = 0 ; i < cln->nA ; i++ ){
		pl = cln->a[i];
		linkL_expand( &pl->ctr, pl->link, Fbr, im );
	}


	image1_mask_pln_fill( im );

	IMAGE_DUMP( im, "im", 1, "fill" );

	if( fNormal )
		image1_mask_normal( im );

	IMAGE_DUMP( im, "im", 2, "fill" );

	return( im );
}




image_type *
image1_mask_pln( pln_type *pl, int width, int height, image_type *im )
{
	int	Fbr;


	im = image_realloc( im, width, height, 1, IMAGE_TYPE_U8, 1 );
	image1_const( im, 0 );


	pln_interior_force_right( pl );


	Fbr = 2;	// right contour
	linkL_expand( &pl->ctr, pl->link, Fbr, im );



	//	IMAGE_DUMP( im, "BB", 2, NULL );

	image1_mask_pln_fill( im );


	//	contour_image_mask_normal( im );


	return( im );
}






static int	link_bes( ln_type *link );
static void	link_bes_begin( ln_type *l, int *Fbes );
static void	link_bes_end( ln_type *l, int *Fbes );



void 
linkL_expand( vec2d *ctr, ln_type *link, int Fbr, image_type *image )
{
	vec2d	  actr;
	ln_type *l;

	for( l = link ; l != NULL ; l = LN_NEXT( l ) )
		ln_set_aux( l );


	for ( l = link, actr = *ctr; l; l = LN_NEXT( l ) ) {
		link_expand( &actr, l, Fbr, image );

		actr.x += l->v.x;
		actr.y += l->v.y;
	}
}



#define G_SFP		10		// shift for fixed point    
#define G_FP		1024		// 1<<G_SFP : 1 for fixed point 
#define G_FPF		1024.0F	// 1 for fixed point FLOAT 

static void 
link_expand( vec2d *ctr, ln_type *l, int Fbr, image_type *image )
{
	box2d	box;
	u_char	*tp, *tp_row ;

	vec2d	V0;
	int	i,	j,	k,  Fbes;
	int	i0,	i1,	j0,	j1;

	int	  flag0[2],	flag1[2];
	int   u0, uu;
	int   t0, tt;
	int   u, t, dt;
	int   ux, uy, len, c_prb;

	int	k0,	k1;

	ln_box( ctr, l, &box );


//	i0 = ctr->x + box.x0 -2;
//	i1 = ctr->x + box.x1 +2.5;
	i0 = box.x0 -2;
	i1 = box.x1 +2.5;

	if( i1 <= 0 )	return;
	if( i0 >= image->row )	return;
	if( i0 < 0 )	i0 = 0;
	if( i1 >= image->row )	i1 = image->row;


//	j0 = ctr->y + box.y0 -2;
//	j1 = ctr->y + box.y1 +2.5;
	j0 = box.y0 -2;
	j1 = box.y1 +2.5;

	if( j1 <= 0 )	return;
	if( j0 >= image->column )	return;
	if( j0 < 0 )	j0 = 0;
	if( j1 > image->column )	j1 = image->column;


	Fbes = link_bes( l );
	flag0[F_RIGHT] = ( Fbes & 0x01 )? 1 : 0;
	flag0[F_LEFT] = ( Fbes & 0x02 )? 1 : 0;

	flag1[F_RIGHT] = ( Fbes & 0x04 )? 1 : 0;
	flag1[F_LEFT] = ( Fbes & 0x08 )? 1 : 0;


	V0.x = i0 + 0.5 - ctr->x;
	V0.y = j0 + 0.5 - ctr->y;


	ux = l->u.x * G_FPF;
	uy = l->u.y * G_FPF;
	len = l->len * G_FPF;
	c_prb = l->c_prb * G_FPF;

	u0 = ( l->u.x * V0.x + l->u.y * V0.y) * G_FPF;
	t0 = (-l->u.y * V0.x + l->u.x * V0.y) * G_FPF;

	tp_row = image->data + image->column*i0 + j0;

	for( i = i0 ; i < i1 ; i++, u0 += ux, t0 += -uy, tp_row += image->column ){

		uu = u0;
		tt = t0;
		for( j = j0, tp = tp_row; j < j1; j++, uu += uy, tt += ux, tp++ ){

			u = uu;
			t = tt;

			if( t < 0 ){
				if( u < 0 ) {
					if( flag0[F_LEFT] == 0 ) continue;
					u += t;
				}
				else {
					if( flag0[F_RIGHT] == 0 ) continue;
					u -= t;
				}

				t = 0;
			}
			else	if( (dt = t - len) > 0  ){
				if( u < 0 ) {
					if( flag1[F_LEFT] == 0 ) continue;
					u -= dt;
				}
				else {
					if( flag1[F_RIGHT] == 0 ) continue;
					u += dt;
				}

				t = len;
			}
			else	{
				dt = (t *dt ) >> G_SFP;
				u -= (c_prb*dt ) >> G_SFP;

				//u -= ((t *dt ) >> (G_SFP+G_SFP));
			}




			k = u>>(G_SFP-6);
	

			if( Fbr == 1 )      k = -k;

	
			k = 128 - k;
			if( k <= 0 || k >= 256 )	continue;

			k = 255 - k;

	
			if( *tp == 0 ){
				*tp = k;
				continue;
			}


			if( (k0 = ( 128 - *tp) ) < 0 )	k0 = -k0;
			if( (k1 = ( 128 - k) ) < 0 )	k1 = -k1;
			
			if( k1 < k0 )
				*tp = k;

		}
	}
}


static void  
image1_mask_pln_fill( image_type *image )
{
	u_char	*tp;
	int		i, j;
	char	fBg;


	tp = image->data;
	for ( i = 0; i < image->row; i++ )
		for ( j = 0, fBg = 1; j < image->column; j++, tp++ ) {

			if( *tp == 0 ){
				if( fBg == 0 )
					*tp = 255;
				continue;
			}


			if( *tp < 128 )
				fBg = 1;
			else fBg = 0;

			//dist = *tp;

			//if ( dist >= 0xC0 ){
			//	fBg = 1;
			//	*tp = 0xC;
			//	continue;
			//}

			//if ( dist  >= 0x80 ) {
			//	fBg = 1;
			//	*tp = ( ( dist - 0x40 ) >> 1 ) & 0x7F;
			//	continue;
			//}

			//if ( dist  >= 0x60 ) {
			//	fBg = 0;
			//	*tp = ( ( dist - 0x40 ) >> 1 ) & 0x7F;
			//	continue;
			//}

			//if( dist  != 0  ){
			//	fBg = 0;
			//	*tp = 0;
			//	continue;
			//}
		}	

}


#ifdef _AA_
static void  
image1_mask_pln_fill( image_type *image )
{
	u_char	*tp, dist;
	int		i, j;
	char	flagBg;


	tp = image->data;
	for ( i = 0; i < image->row; i++ )
		for ( j = 0, flagBg = 1; j < image->column; j++, tp++ ) {

			if( *tp == 0 ){
				if( flagBg != 0 )
					*tp = 0xC0;
				continue;
			}


			dist = *tp;

			if ( dist >= 0xC0 ){
				flagBg = 1;
				*tp = 0xC;
				continue;
			}

			if ( dist  >= 0x80 ) {
				flagBg = 1;
				*tp = ( ( dist - 0x40 ) >> 1 ) & 0x7F;
				continue;
			}

			if ( dist  >= 0x60 ) {
				flagBg = 0;
				*tp = ( ( dist - 0x40 ) >> 1 ) & 0x7F;
				continue;
			}

			if( dist  != 0  ){
				flagBg = 0;
				*tp = 0;
				continue;
			}
		}	

}
#endif



static int
link_bes( ln_type *link )
{
	int	Fbes;

	Fbes = 0x0f;

	link_bes_begin( link, &Fbes );
	link_bes_end( link, &Fbes );

	return( Fbes );
}


static void
link_bes_begin( ln_type *l, int *Fbes )
{
	ln_type	*nl, *l2;
	vec2d	v;


	if( (nl = LN_PREV(l) ) == NULL )
	{
		for ( l2 = l; l2; l2 = LN_NEXT(l2) )
			nl = l2;
	}

	{
		v.x = -nl->u.y;
		v.y =  nl->u.x;

		if( l->u.x * v.x + l->u.y * v.y > 0 )
			*Fbes &= ~0x02;
		else	*Fbes &= ~0x01;
	}


}




static void
link_bes_end( ln_type *l, int *Fbes )
{
	ln_type	*nl, *l2;
	vec2d		v;


	if( (nl = LN_NEXT(l) ) == NULL )
	{
		for ( l2 = l; l2; l2 = LN_PREV(l2) )
			nl = l2;
	}

	{
		v.x = -nl->u.y;
		v.y =  nl->u.x;

		if( l->u.x * v.x + l->u.y * v.y > 0 )
			*Fbes &= ~0x04;
		else	*Fbes &= ~0x08;
	}

}



static void
image1_mask_normal( image_type *sim )
{
u_char	*sp;
int	i,	j;

	IMAGE_DUMP( sim, "AA", 1, NULL );

	sp = (u_char *)sim->data;

	for( i = 0 ; i < sim->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++, sp++ ){
//			*sp = ( *sp == 0 )? 255 : 0;
//			*sp = ( *sp == 0 )? 255 : 0;

			if( *sp < 128 )
				*sp = 0;
			else 
				*sp = 255;



		}
	}
}


static void
image1_mask_append( image_type *sim, image_type *im )
{
	u_char	*sp,	*tp;
	int	i,	j;


	sp = sim->data;
	tp = im->data;

	for( i = 0 ; i < sim->width ; i++ ){
		for( j = 0 ; j < sim->height ; j++, sp++, tp++ ){
			if( *sp > 0 )
				*tp = *sp;
		}
	}
}

