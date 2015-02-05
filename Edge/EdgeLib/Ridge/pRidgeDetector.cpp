/*****************************
***   pRidgeDetector.cpp   ***
******************************/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>

#ifdef _DEBUG
//#define _DUMP
#endif

#include	"Uigp/igp.h"
#include "Ulog/Log.h"
#include "Uvec/Vec3d.h"
#include "Umath/Matrix2Type.h"

#include "Ucamera/Pt2dType.h"
#include "ImageType/ImageType.h"
#include "ImageMark/ImageMark.h"
#include "ImageDump/ImageDump.h"


#include	"ImageApp/ImageApp.h"
#include	"Ujet2/Jet2Type.h"


#include "PRidgeType.h"



#ifdef _DEBUG
#define _DUMP 
#endif

static image_type *	pRidge_detector_1( image_type *sim, pRidgeDetectPrm_type *prm, image_type *im );
static image_type *	pRidgeM_detector_1( image_type *sim, image_type *mim, pRidgeDetectPrm_type *prm, image_type *im );


static void	pRidge_detector_2( image_type *sim, image_type *rim, pRidgeDetectPrm_type *prm );

void	pRidge_detector_3( image_type *rim );


static void	pRidge_filter( image_type *rim );



image_type *
pRidge_detector( image_type *sim, image_type *mim, image_type *rmim, pRidgeDetectPrm_type *prm, image_type *rim )
{

	IMAGE_DUMP( sim, "r", 1, "src" );

	if( mim == NULL )
		rim = pRidge_detector_1( sim, prm, rim );
	else
		rim = pRidgeM_detector_1( sim, mim, prm, rim );

	pRidge_detector_2( sim, rim, prm );

	PRIDGE_DUMP( rim, "r", 0, "2" );


	pRidge_detector_3( rim );

	PRIDGE_DUMP( rim, "r", 0, "3" );

	if( rmim != NULL )
		pRidge_mask( rim, 0, rmim );

	pRidge_linking( rim );

	pRidge_linking_union( rim );

	

	PRIDGE_DUMP( rim, "r", 0, NULL );

	pRidge_filter( rim );

	PRIDGE_DUMP( rim, "r", 0, "filter" );



//	pRidge_edge_mask( sim, rim );
	
//	image_type *wim = pRidge_weight( rim, NULL );


	return( rim );
}








static image_type *
pRidge_detector_1( image_type *sim, pRidgeDetectPrm_type *prm, image_type *im )
{
pRidge_type	*rp;
djet2_type	d;
jet2_type	f;
int	i,	j;


	im = image_recreate( im, sim->height, sim->width, sizeof(pRidge_type), 1 );

	rp = ( pRidge_type *)im->data;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, rp++ )
			rp->state = 0;



	jet2App_type *ja;

	ja = jet2App_init( prm->size, prm->weight );


	int D = ja->nh;

	for( i = D ; i < sim->height - D ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( im, i, D );

		for( j = D ; j < sim->width -D ; j++, rp++ ){
			rp->state = 0;

			image1_jet2App( sim, i - ja->nh, j - ja->nh, ja, &f );

			rescaling_jet2( &f, 2, ja->dn_inv, ja->dn_inv, &f );

			diagonalN_jet2( &f, 1.0, &d );


			if( d.lm1 < 0 && ( prm->sign & 0x01) == 0 || d.lm1 > 0 && ( prm->sign & 0x02) == 0 )
				continue;

			if( ABS(d.lm1) < prm->lm_min )	continue;
			if( ABS(d.dx)  > 0.55 )	continue;

			if( (d.lm1 > 0) != (d.lm2 > 0 ) ){
				if( ABS(d.lm1) < prm->saddleR * ABS(d.lm2) )
					continue;
			}

			rp->state = 1;
	

			rp->f = VALUE_JET2( f, d.n.x * d.dx, d.n.y * d.dx );

			rp->v = d.n;
			rp->lm = d.lm1;

			rp->d = d.dx;


			rp->p.x = d.n.x * d.dx + j + 0.5;
			rp->p.y = d.n.y * d.dx + i + 0.5;
		}
	}


	jet2App_destroy( ja );


	return( im ); 
}


static image_type *
pRidgeM_detector_1( image_type *sim, image_type *mim, pRidgeDetectPrm_type *prm, image_type *im )
{
	pRidge_type	*rp;
	djet2_type	d;
	jet2_type	f;
	int	i,	j;


	im = image_recreate( im, sim->height, sim->width, sizeof(pRidge_type), 1 );

	rp = ( pRidge_type *)im->data;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, rp++ )
			rp->state = 0;



	jet2App_type *ja;

	ja = jet2App_init( prm->size, prm->weight );


	int D = ja->nh;

	for( i = D ; i < sim->height - D ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( im, i, D );
		u_char *mp = IMAGE_PIXEL( mim, i, D );

		for( j = D ; j < sim->width -D ; j++, rp++, mp++ ){
			rp->state = 0;

			if( *mp == 0 )	continue;

			image1_jet2App( sim, i - ja->nh, j - ja->nh, ja, &f );

			rescaling_jet2( &f, 2, ja->dn_inv, ja->dn_inv, &f );

			diagonalN_jet2( &f, 1.0, &d );


			if( d.lm1 < 0 && ( prm->sign & 0x01) == 0 || d.lm1 > 0 && ( prm->sign & 0x02) == 0 )
				continue;

			if( ABS(d.lm1) < prm->lm_min )	continue;
			if( ABS(d.dx)  > 0.55 )	continue;

			if( (d.lm1 > 0) != (d.lm2 > 0 ) ){
				if( ABS(d.lm1) < prm->saddleR * ABS(d.lm2) )
					continue;
			}

			rp->state = 1;


			rp->f = VALUE_JET2( f, d.n.x * d.dx, d.n.y * d.dx );

			rp->v = d.n;
			rp->lm = d.lm1;

			rp->d = d.dx;


			rp->p.x = d.n.x * d.dx + j + 0.5;
			rp->p.y = d.n.y * d.dx + i + 0.5;
		}
	}


	jet2App_destroy( ja );


	return( im ); 
}


static void
pRidge_detector_2( image_type *sim, image_type *rim, pRidgeDetectPrm_type *prm )
{
	pRidge_type	*rp;
	djet2_type	d;
	jet2_type	f;
//	float	t;
	int	i,	j;



	jet2App_type *ja;

	ja = jet2App_init( prm->size+2, prm->weight );



	int D = ja->nh;
	for( i = D ; i < sim->height - D ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( rim, i, D );

		for( j = D ; j < sim->width -D; j++, rp++ ){
			if( rp->state == 0 )	continue;

			image1_jet2App( sim, i - ja->nh, j - ja->nh, ja, &f );

			rescaling_jet2( &f, 2, ja->dn_inv, ja->dn_inv, &f );

			diagonalN_jet2( &f, 1.0, &d );


			if( d.lm1 < 0 && ( prm->sign & 0x01) == 0 || d.lm1 > 0 && ( prm->sign & 0x02) == 0 ){
				rp->state = 0;
				continue;
			}

			if( ABS(d.lm1) < 0.75*prm->lm_min ){
				rp->state = 0;
				continue;
			}

			if( ABS(d.dx)  > 0.55 ){
				rp->state = 0;
				continue;
			}

			if( (d.lm1 > 0) != (d.lm2 > 0 ) ){
				if( ABS(d.lm1) < prm->saddleR * ABS(d.lm2) ){
					rp->state = 0;
					continue;
				}
			}

//			t = d.dx - rp->d;
//			if( ABS(t) > 0.1 )
			if( ABS(d.dx) > ABS(rp->d) + 0.1 &&  ABS(d.dx) > 0.4 )
				rp->state = 0x02;
		}
	}

	jet2App_destroy( ja );
}




int
pRidge_detector( image_type *sim, int i0, int j0, pRidge_type *rp, jet2App_type *ja, pRidgeDetectPrm_type *prm )
{
djet2_type	d;
jet2_type	f;


//	jet2App_type *ja;
//	ja = jet2App_init( prm->size, prm->weight );


//	rp->state = 0;


	image1_jet2App( sim, i0 - ja->nh, j0 - ja->nh, ja, &f );

	rescaling_jet2( &f, 2, ja->dn_inv, ja->dn_inv, &f );

	diagonalN_jet2( &f, 1.0, &d );


	if( d.lm1 < 0 && ( prm->sign & 0x01) == 0 || d.lm1 > 0 && ( prm->sign & 0x02) == 0 )
		return( -1 );

	if( ABS(d.lm1) < prm->lm_min )	return( -1 );
	if( ABS(d.dx)  > 0.55 )	return( -1 );

	if( (d.lm1 > 0) != (d.lm2 > 0 ) ){
		if( ABS(d.lm1) < prm->saddleR * ABS(d.lm2) )
			return( -1 );
	}

	rp->state = 1;


	rp->f = VALUE_JET2( f, d.n.x * d.dx, d.n.y * d.dx );

	rp->v = d.n;
	rp->lm = d.lm1;

	rp->d = d.dx;


	rp->p.x = d.n.x * d.dx + j0 + 0.5;
	rp->p.y = d.n.y * d.dx + i0 + 0.5;

	return( 1 );
}


void
pRidge_detector_3( image_type *rim )
{
	pRidge_type	*rp,	*nrp;
	int	arp[9];
	vec2f_type	dv;

	int	i,	j;



	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;


	for( i = 2 ; i < rim->height - 2 ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( rim, i, 2 );

		for( j = 2 ; j < rim->width -2 ; j++, rp++ ){
			if( rp->state == 0 )	continue;


			nrp = rp + arp[5];
			if( nrp->state > 0 ){
				dv.x = rp->p.x - nrp->p.x;
				dv.y = rp->p.y - nrp->p.y;
				if( dv.x *dv.x + dv.y*dv.y < 0.2*0.2 ){
					if( ABS(rp->lm) < ABS(nrp->lm))
						rp->state = 0;
					else	nrp->state = 0;
					continue;
				}
			}

			nrp = rp + arp[7];
			if( nrp->state > 0 ){
				dv.x = rp->p.x - nrp->p.x;
				dv.y = rp->p.y - nrp->p.y;
				if( dv.x *dv.x + dv.y*dv.y < 0.2*0.2 ){
					if( ABS(rp->lm) < ABS(nrp->lm))
						rp->state = 0;
					else	nrp->state = 0;
					continue;
				}
			}
		}
	}
}




static void
pRidge_filter( image_type *rim )
{
	pRidge_type	*rp;
	int	arp[9];

	int	i,	j;


	arp[0] = -rim->width - 1;
	arp[1] = -rim->width;
	arp[2] = -rim->width + 1;

	arp[3] = - 1;
	arp[4] = 0;
	arp[5] = + 1;

	arp[6] = rim->width - 1;
	arp[7] = rim->width;
	arp[8] = rim->width + 1;


	for( i = 2 ; i < rim->height - 2 ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( rim, i, 2 );

		for( j = 2 ; j < rim->width -2 ; j++, rp++ ){
			if( rp->state == 0 )	continue;

			if( rp->ng[0] >= 0 && rp->ng[1] >= 0 )
				continue;


			if( rp->ng[0] < 0 && rp->ng[1] < 0 ){
				rp->state = 0;
				continue;
			}

			//if( rp->ng[1] >= 0 )	continue;

			//nrp = rp+ arp[ rp->ng[0] ];
			//if( nrp->ng[1] <= 0 ){
			//	rp->state = 0;
			//}
		}
	}

}


static void
pRidge_filter_edge( image_type *rim, image_type *eim )
{
	pRidge_type	*rp;
	u_char	*ep;


	int	i,	j;


	for( i = 1 ; i < rim->height - 1 ; i++ ){

		rp = ( pRidge_type *)IMAGE_PIXEL( rim, i, 1 );
		ep = IMAGE_PIXEL( eim, i, 1 );

		for( j = 1 ; j < rim->width -1 ; j++, rp++, ep++ ){
			if( rp->state == 0 )	continue;
			if( *ep != 0 )
				rp->state = 0x02;
		}
	}
}



void
image_pRidge_dump( image_type *rim, char *prefix, int index, char *suffix )
{
	pt2dA_type *apt;

	apt = pt2dA_from_image_pRidge( rim );

	pt2dA_dump( apt, prefix, index, suffix );

	pt2dA_destroy( apt );
}

void
image_pRidge_write( image_type *rim, char *file )
{
	pt2dA_type *apt;


	apt = pt2dA_from_image_pRidge( rim );

	pt2dA_write_to_file( apt, file );

	pt2dA_destroy( apt );
}



pt2dA_type *
pt2dA_from_image_pRidge( image_type *dim )
{
pRidge_type	*rp;
int	i,	j;
pt2dA_type	*apt;

	apt = pt2dA_alloc( dim->width * dim->height + 10);
	apt->type = PT2D_4V;
	apt->nP = 0;



	rp = (pRidge_type *)dim->data;
	for( i = 2 ; i < dim->height-2 ; i++ ){
		rp = (pRidge_type *)IMAGE_PIXEL(dim, i, 2 );
		for( j = 2 ; j < dim->width-2 ; j++, rp++ ){
			if( rp->state == 0 )	continue;

			pt2d_type	*pt = &apt->p[apt->nP++];

			pt->p = rp->p;
			pt->n = rp->v;


			pt->group = 0;

			if( rp->ng[0] >= 0 && rp->ng[1] >= 0 )
				pt->group = 2;

			if( rp->state == 0x02 )
				pt->group = 3;
		}
	}

	return( apt );
}
