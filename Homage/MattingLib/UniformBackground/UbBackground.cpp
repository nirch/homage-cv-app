
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"
#include	"ImageLabel/ImageLabel.h"
#include	"BnImage/BnImage.h"

#include "UniformBackground.h"

#include "ImageMorphology/ImageMorphology.h"

static image_type *	image1_sampleN( image_type *sim, int N, image_type *im );

#include "plnTracker/PlnHeadTracker/PlnHeadTracker.h"

#include "../UnBackground/bImage.h"

#include "../UnBackground/UnBackground.h"



//#define EXCEPTION


typedef struct seg_type {
	int	i0;
	int i1;
	int	y;
	int n;
} seg_type ;


typedef struct segC_type {
	int	j0;
	int j1;

	int y;

	int type;
	int dx;		// max dx;
	int	j;		// index of max dx
	int	av;

	int	n;
} segC_type;





int	bImage_complete( image_type *bim );

image_type *	bImage_grad_x( image_type *bim, image_type *im );

image_type *	bImage_grad_x_c( image_type *bim, image_type *im );


void	bImage_grad_xTT( image_type *bim );


int	bImage_fill( image_type *bim, int a0[], int a1[] );

int	bImage_row( image_type *bim, int a0[], int a1[] );



int	bImage_seg( image_type *bim, image_type *eim, int a[] );

static int	bImage_dilate_L0( image_type *bim, image_type *eim, int a[], int n, segC_type ac[] );

 int	bImage_dilate_L( image_type *bim, image_type *eim, int a[], int n, segC_type ac[100] );

 int	bImage_dilate_R( image_type *bim, image_type *eim, int a[], int n, segC_type ac[100] );


 static  int	bImage_state( segC_type ac0[], segC_type *ac1, int nC, int Dx, int Av, int nc[2] );

 int	bImage_erode( image_type *bim, image_type *eim, int a0[], int a1[], segC_type ac0[], segC_type ac1[], int Dx, int Av );

 int	bImage_erode_up( image_type *bim, image_type *eim, int a0[], int a1[], segC_type ac0[], segC_type ac1[], int Dx, int Av );

 static int	bImage_erodeG( image_type *bim, image_type *eim, image_type *cim, int a0[], int a1[], segC_type ac0[], segC_type ac1[], int Dx, int Av );




 static int	bImage_fill_gap( image_type *bim, image_type *eim, int a[], segC_type ac[], float dx, float av, int side );


 static void	segC_log( segC_type ac[], int nC );


 static int	bImage_smooth_L( image_type *bim, image_type *eim, int a[], segC_type ac[] );
 static int	bImage_smooth_R( image_type *bim, image_type *eim, int a[], segC_type ac[] );

 static int	bImage_segC( image_type *bim, image_type *eim, int i, int j0, int n, segC_type *s );

#ifndef  __ANDROID__
#define _HEAD
#endif


 int CUniformBackground::ProcessBackground( image_type *sim, int iFrame )
 {
	 if( m_mim == NULL )
		 return( -10 );


#ifdef EXCEPTION
	 try {
#endif

	if( m_flip == 1 )
		m_sim = image3_rotate180( sim, m_sim );
	else m_sim = image_make_copy( sim, m_sim );



	// ProcessInitBackground( m_sim, m_mim[0], 1 );
    ProcessInitBackground( m_sim );
    
        

#ifdef EXCEPTION
	 }

	 catch (...) {
		 return( -9 );
	 }
#endif

	return( m_state );

}


int	CUniformBackground::ProcessInitBackground( image_type *sim )
{
int	i,	ret;

	if( m_unBackground != NULL ){
		ret = m_unBackground->Process( sim, 1 );

		if( ret > 0 ){
			m_iHead = 2*m_unBackground->GetHead() + 1;
			m_bim = m_unBackground->GetBim( m_bim );
            
            m_state = m_unBackground->GetState();

			GPLOG( ("Background: %d", m_unBackground->GetState() ) );
			return( 1 );
		}
	}

	ProcessInitBackground( sim, m_mim[0], 0 );
	if( m_nM == 1 ){
		m_iHead = 1;
		 GPLOG( ("Old Background: %d", m_state ) );
		return( 1 );
	}




	image_type *m_bim0 = bImage_copy( m_bim, NULL );
	int	nc[2];
	nc[0] = m_nc[0];
	nc[1] = m_nc[1];
	int iMax = 0;


	
	for( i = 1 ; i < m_nM ; i++ ){
		ProcessInitBackground( sim, m_mim[i], 0 );
		if( m_nc[0] < nc[0]+6 &&  m_nc[1] < nc[1]+6 )
			continue;

		m_bim0 = bImage_copy( m_bim, NULL );
		nc[0] = m_nc[0];
		nc[1] = m_nc[1];
		iMax = i;
	}

	 m_bim = bImage_copy( m_bim0, m_bim );

	 m_iHead = 1 + 2*iMax;


	 GPLOG( ("Old Background: %d", m_state ) );

	 BIMAGE_DUMP( m_bim, m_N, "bg", 1, "FS" );

	 return( 1 );
}

int	CUniformBackground::ProcessInitBackground( image_type *sim, image_type *mim, int fState )
{
	
	IMAGE_DUMP( sim, "bg", 1, "SRC" );	

	gpTime_start( &m_rTime );

	m_yim = image1_from( sim, m_yim );



	m_bim = bImage_set( sim, &m_roi, m_N, m_bim );
	bImage_mask( m_bim, mim, &m_roi, m_N );


	BIMAGE_DUMP( m_bim, m_N, "bg", 1, "A" );	
	

	m_bimDx = bImage_grad_x_c( m_bim, m_bimDx );

#ifdef _DUMP
	image_type *im = image1S_axb(  m_bimDx, 1, 128, NULL );
	IMAGE_DUMP_DUP( im, 8, 1, "bg", 1, "D" );

	im = image1_threshold_in( im, 128-18, 128+12, im );
	IMAGE_DUMP_DUP( im, 8, 1, "bg", 1, "DM" );


	image1S_abs_128(  m_bimDx, im );
	IMAGE_DUMP( im, "bg", 1, "D1" );
	image_destroy( im, 1 );


	im = bImage_grad_x_c( m_bim, NULL );
	image1S_abs_128(  im, im );

	IMAGE_DUMP( im, "bg", 1, "D2" );
	image_destroy( im, 1 );



#endif


	int	a0[128],	a1[128];
	segC_type ac0[128],	ac1[128]; 

	bImage_row( m_bim, a0, a1 );



	int	N = 3;
	bImage_dilate_L0( m_bim, m_bimDx, a0, -N, ac0 );


	bImage_dilate_L( m_bim, m_bimDx, a0, -N, ac0 );
	bImage_dilate_R( m_bim, m_bimDx, a1, N, ac1 );

	int	nC = m_bim->height;



//	segC_log( ac0, m_bim->height );
	segC_log( ac1, m_bim->height );

	bImage_erode( m_bim, m_bimDx, a0, a1, ac0, ac1, m_prm->dx, m_prm->av );
//	segC_log( ac0, m_bim->height );
//	segC_log( ac1, m_bim->height );

	bImage_erode_up( m_bim, m_bimDx, a0, a1, ac0, ac1, m_prm->dx, m_prm->av );

//	segC_log( ac0, m_bim->height );
//	segC_log( ac1, m_bim->height );


	bImage_smooth_L( m_bim, m_bimDx, a0, ac0 );
	bImage_smooth_R( m_bim, m_bimDx, a1, ac1 );

	segC_log( ac0, m_bim->height );
	segC_log( ac1, m_bim->height );

	BIMAGE_DUMP( m_bim, m_N, "bg", 1, "E" );




	m_state = bImage_state( ac0, ac1, nC, m_prm->dx, m_prm->av, m_nc );

	if( TestDrakness( m_yim ) >0 )
		m_state = -10;


	GPTRACE( (3, "background: %d\n", m_state ) );


	if( fState == 1){
		return( m_state );
	}




	bImage_fill_gap(  m_bim, m_bimDx, a0, ac0, m_prm->dx, m_prm->av, F_LEFT );
	bImage_fill_gap(  m_bim, m_bimDx, a1, ac1, m_prm->dx, m_prm->av, F_RIGHT );




	bImage_fill( m_bim, a0, a1 );

	BIMAGE_DUMP( m_bim, m_N, "bg", 1, "F" );


	gpTime_stop( &m_rTime );




	ProcessBn( sim, 2*m_bnT );
	IMAGE_DUMP_DUP( m_bnImD, 8, 1, "bg", 1, "bn" );


	image_type *m_bim0 = bImage_copy( m_bim, NULL );

	// update the background by lerning process
	if( ProcessInitBackground_lern( sim, m_bim0, a0, a1, ac0,  ac1, 1 ) > 0 )
		ProcessInitBackground_lern( sim, m_bim0, a0, a1, ac0,  ac1, 2 );


	image_destroy( m_bim0, 1 );


	return( 1 );
}








int	CUniformBackground::ProcessInitBackground_lern( image_type *sim, image_type *m_bim0, int a0[], int a1[], segC_type ac0[], segC_type ac1[], int step )
{

	ProcessCompare( sim );
	IMAGE_DUMP( m_cim, "bg", step, "cm" );

	image_type *cim8 = image1_sampleN( m_cim, 8,  NULL );
	IMAGE_DUMP_DUP( cim8, 8, 4, "bg", step, "cm8" );


	if( bImage_erodeG( m_bim, m_bimDx, cim8, a0, a1, ac0, ac1, m_prm->dx+4, m_prm->av+4 ) <= 0 ){
		image_destroy( cim8, 1 );
		return( -1 );
	}


	m_bim = bImage_copy( m_bim0, m_bim );

	bImage_clearN( m_bim );
	bImage_fill( m_bim, a0, a1 );

	BIMAGE_DUMP( m_bim, m_N, "bg", step, "EE" );
	BIMAGE_DUMP( m_bim, m_N, "bg", 1, "FE" );
	

	image_destroy( cim8, 1 );

	return( 1 );
}	





int CUniformBackground::TestDrakness( image_type *sim )
{
	int	H[256];
	int	pixles;

	int	i;

	image1_histogram( sim, NULL, H, 1 );

	pixles = sim->width*sim->height;


	for( i = 1 ; i < 256 ; i++ )
		H[i] += H[i-1];


	int pixels = H[255];

	ubDarkPrm_type *d = &m_prm->d;

	if( H[d->T0] > d->F1* pixels  && H[d->T1] > d->F1 *pixels )
		return( 1 );


	return( -1 );
}







void
bImage_grad_xTT( image_type *bim )
{
	

	image_type *bimDx = bImage_grad_x( bim, NULL );


	image_type *im = image1S_axb(  bimDx, 1, 128, NULL );


	IMAGE_DUMP_DUP( im, 8, 1, "bg", 1, "DX" );

	image_destroy( bimDx, 1 );

	image_destroy( im, 1 );

}



int
bImage_row( image_type *bim, int a0[], int a1[] )
{
	int	i,	j,	no;
	bImage_type *bp;



	no = 0;

	a0[0] = a1[0] = -1;
	for( i = 1 ; i < bim->height-1 ; i++ ){
		bImage_type *bp0 = ( bImage_type *)IMAGE_PIXEL( bim, i, 0 );

		bp = bp0+1;
		for( j = 1 ; j < bim->width-1 ; j++, bp++ )
			if( bp->n <= 0 )		break;

		if( j >= bim->width-1 ){
			a0[i] = a1[i] = -1;
			continue;
		}

		if( j == 1 )
			a0[i] = j+5;//-1;
		else a0[i] = j-1;


		 bp = bp0 + bim->width-2;
		for( j = bim->width-2 ; j >= 0 ; j--, bp-- )
			if( bp->n <= 0 )		break;


		if( j ==  bim->width-2 )
			a1[i] = j -5;//-1;
		else a1[i] = j+1;
	}

	return( 1 );
}





int
bImage_fill( image_type *bim, int a0[], int a1[] )
{
	int	i,	k,	no;
	bImage_type *bp;



	no = 0;
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 0 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += bim->width ){

		if( a0[i] == -1 || a1[i] == -1 )
			continue;
		
		int	j0,	j1;
		bImage_type *bp0,	*bp1;

		if( a0[i] < 0 ){
			j0 = 0;
			bp0 = bp + a1[i];
		}
		else {
			j0 = a0[i];
			bp0 = bp + a0[i];
		}



		if( a1[i] < 0 ){
			j1 = bim->width-1;
			bp1 = bp0;
		}
		else {
			j1 = a1[i];
			bp1 = bp + a1[i];
		}



		bImage_type *cp;
		for( k = j0 + 1, cp = bp0+1 ; k < j1 ; k++, cp++ ){


			float f = (k - j0)/(float)( j1 - j0 ); 

			//if( f < 0.25 )	f *= 0.5;
			//else	if( f < 0.75 )	f = f *1.5 - 0.25;
			//else	f = 0.5 + 0.5*f;


			if( f < 0.25 )	f /= 3;
			else	if( f < 0.75 )	f = 0.5 + ( f - 0.5)*5.0/3 ;
			else	f = 1 - ( 1-f)/3;

			//if( f < 0.25 )	f *= 0.25;
			//else	if( f < 0.75 )	f = 0.375 + 0.25*f;//f *1.5 - 0.25;
			//else	f = 0.5 + 0.25*f;



			cp->r = (1-f) * bp0->r + f* bp1->r;
			cp->g = (1-f) * bp0->g + f* bp1->g;
			cp->b = (1-f) * bp0->b + f* bp1->b;
			cp->n = 1;
		}
	}


	return( 1 );
}














static int	bImage_seg( image_type *bim, image_type *eim, int a[], int n, segC_type as[] );

static int	bImage_segR( segC_type ac[], int nC, seg_type as[], int *nS, int Dx, int Av );


static int	bImage_dilate_L( image_type *bim, image_type *eim, segC_type *s0, int i, segC_type *c0, int dk, segC_type *c );


static int	bImage_dilate_R( image_type *bim, image_type *eim, segC_type *s0, int i, segC_type *c0, int dk, segC_type *c );



static  int
bImage_state( segC_type ac0[], segC_type *ac1, int nC, int Dx, int Av, int nc[2] )
{
	int	i;

	seg_type as0[128],	as1[128];
	int nS0,	nS1;

	bImage_segR( ac0, nC, as0, &nS0, Dx, Av );
	bImage_segR( ac1, nC, as1, &nS1, Dx, Av );


	if( nS0 == 0 && nS0 == 0 ){
		return( -5 );
	}



	for( i = 0 ; i < nS0 ; i++ ){
		GPTRACE( (3, "%d %d   (%d)\n", as0[i].i0, as0[i].i1, as0[i].y) );
	}

	for( i = 0 ; i < nS1 ; i++ ){
		GPTRACE( (3, "%d %d   (%d)\n", as1[i].i0, as1[i].i1, as1[i].y) );
	}


	for( i = 1 ; i < nC-1 ; i++ )
		if( ac0[i].j0 > 0 )	break;

	int c00 = i;

	

	int nC0 = 0;
	for( i = 0 ; i < nS0 ; i++ )
		nC0 += as0[i].i1 - as0[i].i0 + 1;

	int nC1 = 0;
	for( i = 0 ; i < nS1 ; i++ )
		nC1 += as1[i].i1 - as1[i].i0 + 1;

	nc[0] = nC0;
	nc[1] = nC1;

	fprintf( stdout, "NC: %d ( %d %d )\n", nC0+nC1, nC0, nC1 );

	if( nC0 < 0.5 *(nC-c00) || nC1 < 0.5 *(nC-c00) )
		return( -11 );


	if( nC0 < 0.75 *(nC-c00) || nC1 < 0.75 *(nC-c00) )
		return( -4 );

	if( nS0 == 1 && nS1 == 1 ){
		int y = as0[0].y - as1[0].y;
		if( ABS(y) > 60 )
			return( -2 );

		return( 1 );
	}



	for( i = 0 ; i < nS0-1 ; i++){
		int y = as0[i+1].y - as0[i].y;
		if( ABS(y) > 60 )
			return( -3 );
	}

	for( i = 0 ; i < nS1-1 ; i++){
		int y = as1[i+1].y - as1[i].y;
		if( ABS(y) > 60 )
			return( -3 );
	}



	int y = as0[nS0-1].y - as1[nS1-1].y;
	if( ABS(y) > 60 )
		return( -2 );

	return( 1 );
}


static int
bImage_dilate_L0( image_type *bim, image_type *eim, int a[], int n, segC_type ac[] )
{
	int	i;


	bImage_seg( bim, eim, a,  n, ac);

	int Av = 4;
	int Dx = 8;

	int dj = ( n < 0 ) ? n+1 : 0;
	n = ABS(n);

	int	no = 0;
	for( i = 1; i < bim->height-1 ; i++  ){
		if( ac[i].j0 < 0 ){
			continue;
		}


		if( ABS( ac[i].dx) < Dx && ABS( ac[i].av) < Av ){
			continue;
		}

		int k;
		segC_type	c;
		for( k = 1 ; k < 5 ; k++ ){
	

			bImage_segC( bim, eim, i, a[i]-k+dj, n, &c );

			if( ABS( c.dx) > Dx || ABS(c.av) > Av )
				continue;

			a[i] -= k;

			no++;
			break;
		}

	}

	if( no > 0 )
		fprintf( stdout, "Dilate %d\n", no );

	return( 1 );
}



int
bImage_dilate_L( image_type *bim, image_type *eim, int a[], int n, segC_type ac[] )
{
	int	i;


	seg_type as[100];
	int nS = 0;



	bImage_seg( bim, eim, a,  n, ac);

	int Av = 4;
	int Dx = 8;
	bImage_segR( ac, bim->height, as, &nS, Dx, Av );


	if( nS > 0 ){
		int	k;
		int i0 = as[0].i0;

		int i1 = 0;

		segC_type	c;
		for( k = i0 - 1; k > i1 ; k-- ){
			if( ac[k].j0 == - 1)	break;
			if( bImage_dilate_L( bim, eim,  &ac[k+1], k,  &ac[k], 10, &c ) < 0 )
				break;
			ac[k] = c;
			as[0].i1 = k;
		}
	}



	int	k;
	for( i = 0 ; i < nS ; ){
		int i0 = as[i].i1;

		int i1 = (i < nS-1)? as[i+1].i0 : bim->height-1;
		segC_type	c;
		for( k = i0 + 1; k < i1 ; k++ ){
			if( bImage_dilate_L( bim, eim,  &ac[k-1], k,  &ac[k], 10, &c ) < 0 )
				break;
			ac[k] = c;
			as[i].i1 = k;
		}

		
		if( i < nS-1 && k == i1 ){
			int dy = ac[i1].y - ac[i1-1].y;
			if( ABS(dy) < 36 ){

				as[i].i1 = as[i+1].i1;
				nS--;
				for( k = i+1 ; k < nS ; k++ )
					as[k] = as[k+1];
				continue;
			}
		}
		
		i++;
	}






	for( i = 1 ; i < bim->height - 1 ; i++ ){
		if( a[i] < 0 )	continue;
		a[i] = ac[i].j1;
	}

#ifdef _DUMP
	for( i = 0 ; i < nS ; i++ ){
		fprintf( stdout, "L  %d %d   (%d)\n", as[i].i0, as[i].i1, as[i].y );
	}
#endif

	return( 1 );
}


int
bImage_dilate_R( image_type *bim, image_type *eim, int a[], int n, segC_type ac[] )
{
	int	i;

//	segC_type ac[100]; 
	seg_type as[100];
	int nS = 0;



	bImage_seg( bim, eim, a,  n, ac);


	int Av = 4;
	int Dx = 8;
	bImage_segR( ac, bim->height, as, &nS, Dx, Av );





/// new

	if( nS > 0 ){
		int	k;
		int i0 = as[0].i0;

		int i1 = 0;

		segC_type	c;
		for( k = i0 - 1; k > i1 ; k-- ){
			if( ac[k].j0 == - 1)	break;
			if( bImage_dilate_R( bim, eim,  &ac[k+1], k,  &ac[k], 10, &c ) < 0 )
				break;
			ac[k] = c;
			as[0].i0 = k;
		}
	}


	// end new






	int	k;
	for( i = 0 ; i < nS ; ){
		int i0 = as[i].i1;
		

		int i1 = (i < nS-1)? as[i+1].i0 : bim->height-1;

		segC_type	c;
		for( k = i0 + 1; k < i1 ; k++ ){
			if( bImage_dilate_R( bim, eim,  &ac[k-1], k,  &ac[k], 10, &c ) < 0 )
				break;
			ac[k] = c;
			as[i].i1 = k;
		}

		if( i < nS-1 && k == i1 ){
			as[i].i1 = as[i+1].i1;
			nS--;
			for( k = i+1 ; k < nS ; k++ )
				as[k] = as[k+1];
			continue;
		}

		i++;
	}

	for( i = 1 ; i < bim->height - 1 ; i++ ){
		if( a[i] < 0 )	continue;
		a[i] = ac[i].j0;
	}


#ifdef _DUMP
	for( i = 0 ; i < nS ; i++ ){
		fprintf( stdout, "R  %d %d   (%d)\n", as[i].i0, as[i].i1, as[i].y );
	}
#endif


	return( 1 );
}




static int
	bImage_segR( segC_type ac[], int nC, seg_type as[], int *nS, int Dx, int Av )
{
	int	i;



	*nS = 0;



	seg_type *s;
	for( i = 1; i < nC-1 ;  ){
		if( ac[i].j0 < 0 ){
			i++;
			continue;
		}


		if( ABS( ac[i].dx) > Dx || ABS( ac[i].av) > Av ){
			i++;
			continue;
		}

		s = &as[(*nS)++];
		s->i0 = s->i1 = i;
		s->y = ac[i].y;
		s->n = 1;
		for( i = s->i1+1 ; i < nC-1 ; i++ ){
			float dy = ac[i].y - ac[i-1].y;
			if( ABS( ac[i].dx) > Dx+1 || ABS( ac[i].av) > Av+2 ||  ABS( ac[i].av) > Av && ABS(dy) > Dx )
				break;

	
	



		
			int y = ac[i].y - ac[i-1].y;
			if( ABS(y) > 10 )
				break;
			

			s->y += ac[i].y;
			s->n ++;
			s->i1 = i;
		}			

		s->y /= s->n;

		if( s->n <= 3 )
			(*nS)--;


	}

	return( 1 );
}




static int
bImage_seg( image_type *bim, image_type *eim, int a[], int n, segC_type as[] )
{
int	i;

	int dj = ( n < 0 ) ? n+1 : 0;
	n = ABS(n);

	for( i = 1 ; i < bim->height-1 ; i++ ){
		segC_type *s  = &as[i];
		if( a[i] < 0  ){
			s->j0  = s->j1 = -1;
			s->type = 0;
			continue;
		}


		bImage_segC( bim, eim, i, a[i]+dj, n, s );
	}

	return( 1 );
}


static int
bImage_dilate_L( image_type *bim, image_type *eim, segC_type *s0, int i, segC_type *c0, int dk, segC_type *c )
{
int	k,	j0;


	int j00 = MIN( s0->j0, c0->j0 );
	j00 -= dk;
	if( j00 + s0->n < s0->j0 )	j00 = s0->j0 - s0->n;
	if( j00 < 0 )	j00 = 0;
	
	for( k = 0 ; ; k++ ){
		j0 = c0->j0 - k;
		if( j0 < j00 )	return( -1 );
		

		bImage_segC( bim, eim, i, c0->j0-k, c0->n, c );

		if( ABS( c->dx) > 8 || ABS( c->av) > 4 )
			continue;

		int y = c->y - s0->y;
		if( ABS(y) > 10 )
			continue;

		return( k );
	}

	return( -1 );
}




static int
bImage_dilate_R( image_type *bim, image_type *eim, segC_type *s0, int i, segC_type *c0, int dk, segC_type *c )
{
	int	k,	j0;


	int j00 = MAX( s0->j0, c0->j0 );
	j00 += dk;
	if( j00 > s0->j0 +s0->n)	j00 = s0->j0 + s0->n;
	if( j00 +s0->n > bim->width-1 )	j00 = bim->width-1 - s0->n;

	for( k = 0 ; ; k++ ){
		j0 = c0->j0 + k;
		if( j0 > j00 )	return( -1 );

		bImage_segC( bim, eim, i, j0, c0->n, c );

		if( ABS( c->dx) > 8 || ABS( c->av) > 4 )
			continue;

		int y = c->y - s0->y;
		if( ABS(y) > 10 )
			continue;

		return( k );
	}

	return( -1 );
}


static int
bImage_segC( image_type *bim, image_type *eim, int i, int j0, int n, segC_type *s )
{
	int	j;
	s_char *tp;
	int a;


	s->j0 = j0;
	s->j1 = j0 + n-1;

	s->dx = 0;
	s->y = 0;
	a = 0;
	
	int	k = 0;


			
	tp = (s_char *)IMAGE_PIXEL( eim, i-1, s->j0-1 );
	bImage_type *bp = (bImage_type *)IMAGE_PIXEL( bim, i, s->j0 );
	for( j = s->j0 ; j <= s->j1 ; j++, tp++, bp++ ){
		if( j <= 0 || j >= bim->width -1 )	continue;
		a += *tp;
		if( ABS(*tp) > ABS(s->dx) ){
			s->dx = *tp;
			s->j = j;
		}

		int y = IMAGE_RGB2Y( bp->r, bp->g, bp->b );
		s->y += y;
		k++;
	}

	s->av = a / k;
	s->y /= k;

	s->n = n;
	if( ABS(s->dx ) < 5 || ABS(s->av) < 4 )
		s->type = 1;

	return( 1 );
}
		






int
bImage_erode( image_type *bim, image_type *eim, int a0[], int a1[], segC_type ac0[], segC_type ac1[], int Dx, int Av )
{
	int	i,	j,	no;
	bImage_type *bp;


	no = 0;
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 0 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += bim->width ){

		if( ac0[i].j1 < 0 )	continue;


		int	j0,	j1;
		bImage_type *bp0,	*bp1;


		j0 = ac0[i].j1;
		j1 = ac1[i].j0;
		int n = ac0[i].n;




		segC_type c;
		bImage_type *cp;


		bp0 = bp + j0;

		int j11 = 0.5*(j1 + j0 );
		
		for( j = j0+1, cp = bp0+1 ; j < j11 ; j++, cp++ ){
			bImage_segC( bim, eim, i, j-n, n, &c );
			if( ABS( c.dx) > Dx || ABS( c.av) > Av )
				break;

			ac0[i] = c;
			a0[i] = c.j1;
			cp->n = 1;
		}


		if( j >=  j1 ){
			a0[i] = a1[i] = -1;
			ac0[i].j0 = ac0[i].j1 = ac1[i].j0 = ac1[i].j1 = -1;
			continue;
		}




		if( j1 < bim->width-1 - 0 ){
			bp1 = bp +j1;
			for( j = j1-1, cp = bp1-1 ; j > j0 ; j--, cp-- ){
				bImage_segC( bim, eim, i, j, n, &c );
				if( ABS( c.dx) > Dx || ABS( c.av) > Av )
					break;


				ac1[i] = c;
				a1[i] = c.j0;
				cp->n = 1;
			}


			if( j <=  j0 ){
				a0[i] = a1[i] = -1;
				ac0[i].j0 = ac0[i].j1 = ac1[i].j0 = ac1[i].j1 = -1;
				continue;
			}
		}






	}


	return( 1 );
}

static void
segC_log( segC_type ac[], int nC )
{
	int	i;



	for( i = 1 ; i < nC-1 ; i++ ){

		if( ac[i].j1 < 0 )	continue;

		segC_type *c = &ac[i];
		GPLOG(( "%d %d %d  %d %d\n", i, c->j0, c->j1, c->dx, c->av ));
	}

	GPLOG_FLUSH();
}






static int	segC_filter( seg_type as[], int *nS, segC_type ac[], float dt );


static int
bImage_fill_gap( image_type *bim, image_type *eim, int a[], segC_type ac[], float dx, float av, int side )
{
	int	i,	j,	k;


	seg_type as[100];
	int nS = 0;


	bImage_segR( ac, bim->height, as, &nS, dx, av );



	segC_filter( as, &nS, ac, 24 );

	if( nS <= 0 )
		return( -1 );


	for( i = 0 ; i < nS-1 ; i++  ){
		seg_type * s = &as[i];
		seg_type *s1 = &as[i+1];

		bImage_type *bp0 = ( bImage_type *)IMAGE_PIXEL( bim, s->i1, a[s->i1] );
		bImage_type *bp1 = ( bImage_type *)IMAGE_PIXEL( bim, s1->i0, a[s1->i0] );


	
		for( k = s->i1+1 ; k < s1->i0 ; k++ ){
			float f = ( k - s->i1)/(float)(s1->i0 - s->i1);
			int r = ( bp1->r - bp0->r)*f + bp0->r;
			int g = ( bp1->g - bp0->g)*f + bp0->g;
			int b = ( bp1->b - bp0->b)*f + bp0->b;

			
			int k0 = ( a[s1->i0] - a[s->i1])*f + a[s->i1];
			if( side == F_LEFT ){
				if( k0 < a[k] )	a[k] = k0;
			}
			else if( k0 > a[k] )	a[k] = k0;


			int	j0,	j1;
			if( (j0 = a[k]-2) < 1 )	j0 = 1;
			if( (j1 = a[k]+2) > bim->width-2 )	j1 = bim->width-2;

			bImage_type *bp = ( bImage_type *)IMAGE_PIXEL( bim, k, j0);
			for( j = j0; j <= j1 ; j++, bp++ ){
		
				bp->r = r;
				bp->g = g;
				bp->b = b;
			}
		}
	}

	

	seg_type * s = &as[nS-1];


	bImage_type *bp0,	*bp;
	bp0 = ( bImage_type *)IMAGE_PIXEL( bim, s->i1, a[s->i1] );

	for( i = s->i1+1 ; i < bim->height-1 ; i++ ){

	//	a[i] = a[s->i1];
		int k0 =  a[s->i1];
		if( side == F_LEFT ){
			if( k0 < a[i] )	a[i] = k0;
		}
		else if( k0 > a[i] )	a[i] = k0;


		int	j0,	j1;
		if( (j0 = a[i]-6) < 1 )	j0 = 1;
		if( (j1 = a[i]+6) > bim->width-2 )	j1 = bim->width-2;

		bp = ( bImage_type *)IMAGE_PIXEL( bim, i, j0);
		for( j = j0; j <= j1 ; j++, bp++ ){

			bp->r = bp0->r;
			bp->g = bp0->g;
			bp->b = bp0->b;
		}
	}






	return( 1 );
}




static int
segC_filter( seg_type as[], int *nS, segC_type ac[], float dt )
{

int	i,	j,	n;


	if( *nS <= 1 )	return( -1 );



	int	i0 = 0;
	for( i = 1 ; i < *nS ; i++ ){
		seg_type *s = &as[i];

		if( s->i1 - s->i0 > as[i0].i1 - as[i0].i0 )
			i0 = i;
	}


	int y0 = as[i0].y;

	for( i = 0, n = 0 ; i < *nS ; i++ ){
		seg_type *s = &as[i];
		int dy = s->y - y0;
		if( ABS(dy) < 20 )
			n += s->i1 - s->i0;
	}


	for( i = 0, j = 0 ; i < *nS ; i++ ){
		seg_type *s = &as[i];
		int dy = s->y - y0;

		if( ABS(dy) >  96  && s->i1 - s->i0 < 0.5 * n )
			continue;

		as[j++] = as[i];
	}
	*nS = j;


#ifdef _DUMP
	for( i = 0 ; i < *nS ; i++ ){
		fprintf( stdout, "F  %d %d   (%d)\n", as[i].i0, as[i].i1, as[i].y );
	}
#endif

	return( 1 );
}







int 
bImage_erode_up( image_type *bim, image_type *eim, int a0[], int a1[], segC_type ac0[], segC_type ac1[], int Dx, int Av )
{
	int	i,	j,	no;
	bImage_type *bp;


	for( i = 1 ; i < bim->height-1 ; i++ ){

		if( ac0[i].j0 > -1 )	break;
	}

	if( i >= bim->height-1 )
		return( -1 );

	int i0 =i;

	int n = 5;

	no = 0;
	
	for( i = i0-1 ; i > 0 ; i-- ){
		bp = ( bImage_type *)IMAGE_PIXEL( bim, i, 0 );

		a0[i] = ac0[i+1].j1;
		a1[i] = ac1[i+1].j0;

		bImage_segC( bim, eim, i, ac0[i+1].j0, n, &ac0[i] );
		bImage_segC( bim, eim, i, ac1[i+1].j0, n, &ac1[i] );



		int	j0,	j1;
		bImage_type *bp0,	*bp1;


		j0 = ac0[i].j1;
		j1 = ac1[i].j0;
		int n = ac0[i].n;



		segC_type c;
		bImage_type *cp;


		bp0 = bp + j0;
		s_char *ep = (s_char *)IMAGE_PIXEL(eim, i-1, j0-1 );

		for( j = j0+1, cp = bp0+1 ; j < j1 ; j++, ep++, cp++ ){
			bImage_segC( bim, eim, i, j-n, n, &c );
			if( ABS( c.dx) > Dx || ABS( c.av) > Av )
				break;

			ac0[i] = c;
			a0[i] = c.j1;
			cp->n = 1;
		}


		if( j >=  j1 ){
			a0[i] = a1[i] = -1;
			ac0[i].j0 = ac0[i].j1 = ac1[i].j0 = ac1[i].j1 = -1;
			break;
		}




		if( j1 < bim->width-1 - 0 ){
			bp1 = bp +j1;
			for( j = j1-1, cp = bp1-1 ; j > j0 ; j--, ep--, cp-- ){
				bImage_segC( bim, eim, i, j, n, &c );
				if( ABS( c.dx) > Dx || ABS( c.av) > Av )
					break;

				//				if( ABS(*ep) > 4)	break;
				ac1[i] = c;
				a1[i] = c.j0;
				cp->n = 1;
			}
		}


	}


	return( 1 );
}







static int
bImage_smooth_L( image_type *bim, image_type *eim, int a[], segC_type ac[] )
{
	int	i;



	for( i = 1 ; i < bim->height - 1 ; i++ )
		if( ac[i].j0 >=  0 )	break;

	i++;

	for(  ; i < bim->height - 2 ; i++ ){
		if( ac[i].j1 <= ac[i-1].j1 ||  ac[i].j1 <= ac[i+1].j1 )
			continue;

		int j = 0.5*(  ac[i-1].j0 + ac[i+1].j0 );
		bImage_segC( bim, eim, i, j, ac[i].n, &ac[i] );
	}



	for( i = 1 ; i < bim->height - 1 ; i++ ){
		if( a[i] < 0 )	continue;
		a[i] = ac[i].j1;
	}



	return( 1 );
}


static int
	bImage_smooth_R( image_type *bim, image_type *eim, int a[], segC_type ac[] )
{
	int	i;



	for( i = 1 ; i < bim->height - 1 ; i++ )
		if( ac[i].j0 >=  0 )	break;

	i++;

	for(  ; i < bim->height - 2 ; i++ ){
		if( ac[i].j0 >= ac[i-1].j0 ||  ac[i].j0 >= ac[i+1].j0 )
			continue;

		int j = 0.5*(  ac[i-1].j0 + ac[i+1].j0 );
		bImage_segC( bim, eim, i, j, ac[i].n, &ac[i] );
	}



	for( i = 1 ; i < bim->height - 1 ; i++ ){
		if( a[i] < 0 )	continue;
		a[i] = ac[i].j1;
	}



	return( 1 );
}





static image_type *
image1_sampleN( image_type *sim, int N, image_type *im )
{
	u_char	*sp0, *sp, *tp;
	int	i,	j,	k,	n;


	im = image_recreate( im, sim->row/N, sim->column/N, 1, 1 );

	int align = sim->width - N;

	tp = im->data;
	for( i = 0 ; i < sim->row ; i += N ){
		sp0 = IMAGE_PIXEL( sim, i, 0 );
		

		for( j = 0 ; j < sim->column ; j += N, sp0 += 8 ){
				int no = 0;
			for( k = 0, sp = sp0 ; k < N ; k++, sp += align ){
				for( n = 0 ;  n < N ; n++, sp++ ){
					if( *sp != 0 )
						no++;
				}
			}
			*tp++ = no;	
		}


	}

	return( im );
}


static int
bImage_erodeG( image_type *bim, image_type *eim, image_type *cim, int a0[], int a1[], segC_type ac0[], segC_type ac1[], int Dx, int Av )
{
	int	i,	j,	no;
	bImage_type *bp;


	no = 0;
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 0 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += bim->width ){

		if( ac0[i].j1 < 0 )	continue;


		int	j0,	j1;
		bImage_type *bp0,	*bp1;


		j0 = ac0[i].j1;
		j1 = ac1[i].j0;
		int n = ac0[i].n;



		segC_type c;
		bImage_type *cp;


		bp0 = bp + j0;
		s_char *gp = (s_char *)IMAGE_PIXEL(cim, i-1, j0-1+1 );

		for( j = j0+1, cp = bp0+1 ; j < j1 ; j++, cp++, gp++ ){
			if( *gp != 0 )	break;
			bImage_segC( bim, eim, i, j-n, n, &c );
			if( ABS( c.dx) > Dx || ABS( c.av) > Av )
				break;

			ac0[i] = c;
			a0[i] = c.j1;
			cp->n = 1;
			*gp = 16;

			no ++;
		}


		if( j >=  j1 ){
			a0[i] = a1[i] = -1;
			ac0[i].j0 = ac0[i].j1 = ac1[i].j0 = ac1[i].j1 = -1;
			continue;
		}




		if( j1 < bim->width-1 - 0 ){
			bp1 = bp +j1;
			gp = (s_char *)IMAGE_PIXEL(cim, i-1, j1-2 );
			for( j = j1-1, cp = bp1-1 ; j > j0 ; j--, cp--, gp-- ){
				if( *gp != 0 )	break;
				bImage_segC( bim, eim, i, j, n, &c );
				if( ABS( c.dx) > Dx || ABS( c.av) > Av )
					break;

//				if( ABS(*ep) > 4)	break;
				ac1[i] = c;
				a1[i] = c.j0;
				cp->n = 1;

				*gp = 16;
				no++;
			}
		}


	}


	fprintf( stdout, "erode: %d\n", no );

	return( no );
}