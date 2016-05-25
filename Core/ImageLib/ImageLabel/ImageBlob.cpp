/**************************
 ***   Image1Blob.cpp   ***
 **************************/
#include	<math.h>

#ifdef _DEBUG
#define _DUMP
#endif



#include "ImageType/ImageType.h"
#include "ImageLabel.h"



image_type *
image1_connect_component( image_type *sim, int T, bwLabel_type **abw, int *nBw, image_type *im )
{
	int	i,	j;
	u_char *sp;
	short *bp;

	im = image_create( sim->height, sim->width, 2, 1, NULL );

	sp = sim->data;
	bp = im->data_s;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = ( *sp < T )? 0 :1;


	image2_bwLabel( im, abw, nBw );
	if( *nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return( NULL );
	}

//	image2_bwLabel_set_id( im, *abw );

	image2_bwLabel_set_aux( im, *abw, *nBw );

	return( im );
}




void 
image1_remove_blob( image_type *sim, int nP, int nT, int value )
{
image_type *im;
bwLabel_type *abw;
int nBw; 
int	i,	j;
u_char *sp;
short *bp;

	im = image_create( sim->height, sim->width, 2, 1, NULL );

	sp = sim->data;
	bp = im->data_s;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = ( *sp < nP )? 0 :1;


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}

//	image2_bwLabel_set_id( im, abw );



	for( i = 0 ; i < nBw ; i++ ){
		if( abw[i].no < nT )
			abw[i].id = -1;
	}



	sp = sim->data;
	bp = im->data_s;

	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( abw[*bp].id < 0 )
				*sp = value;
		}

		free( abw );
		image_destroy( im, 1 );
}


int	bwLabel_filter( bwLabel_type *bw, int nBw, int pT, int bT );



void 
image1_remove_blobM( image_type *sim, int nP, int pT, int bT, int value )
{
	image_type *im;
	bwLabel_type *abw;
	int nBw; 
	int	i,	j;
	u_char *sp;
	short *bp;


	im = image_create( sim->height, sim->width, 2, 1, NULL );

	sp = sim->data;
	bp = im->data_s;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = ( *sp < nP )? 0 :1;


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}

//	image2_bwLabel_set_id( im, abw );



	//for( i = 0 ; i < nBw ; i++ ){
	//	if( abw[i].no < nT )
	//		abw[i].id = -1;
	//}


//	imageLabel2_set_boundary( im, abw, nBw );

	for( i = 0 ; i < nBw ; i++ ){
		bwLabel_type *bw = &abw[i];
		bw->existence = 0;

	//	if( bw->boundary == 1 )	bw->existence = 1;
	}
	
	bwLabel_filter( abw, nBw, pT, bT );



	sp = sim->data;
	bp = im->data_s;

	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( abw[*bp].id < 0 )
				*sp = value;
		}

		free( abw );
		image_destroy( im, 1 );
}



int
bwLabel_filter( bwLabel_type *bw, int nBw, int pT, int bT )
{
int	i,	i0,	j;
int	ab[64],	 nB;

	nB = 0;
	for( i = 0 ; i < nBw ; i++ ){
//		if( bw[i].id != i )	continue;

		if( bw[i].id != i ){
			bw[i].id = -1;
			continue;
		}

		if(bw[i].no < pT ){
			if( bw[i].existence != 1 )
				bw[i].id = -1;
			continue;
		}

		if( bT < 0 || nB < bT ){
			ab[nB++] = i;
			continue;
		}

		i0 = ab[nB-1];

		if( bw[i0].no > bw[i].no ){
			if( bw[i].existence != 1 )
				bw[i].id = -1;
			continue;
		}

		if( bw[i0].existence != 1 )
			bw[i0].id = -1;

		for( j = nB ; j > 0 ; j-- ){
			if(  bw[ab[j-1]].no > bw[i].no  )
				break;
			ab[j] = ab[j-1];
		}
		ab[j] = i;
	}

	return( nB  );
}


#ifdef _AA_
void 
image1_fill_blob( image_type *sim, int nP, int pT, int bT, int value )
{
image_type *im;
bwLabel_type *abw;
int nBw; 
int	i,	j;
u_char *sp;
u_short *bp;

	im = image_create( sim->height, sim->width, 2, 1, NULL );

	sp = sim->data;
	bp = im->data_us;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = ( *sp < nP )? 1 :0;


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}





	imageLabel2_set_boundary( im, abw, nBw );


	for( i = 0 ; i < nBw ; i++ ){
		bwLabel_type *bw = &abw[i];
		bw->existence = 0;

		if( bw->boundary == 1 )	bw->existence = 1;
	}


	bwLabel_filter( abw, nBw, pT, bT );



	sp = sim->data;
	bp = im->data_us;

	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( *bp == 0 )	continue;
			if( abw[*bp].id < 0 )
				*sp = value;
		}

		free( abw );
		image_destroy( im, 1 );
}
#endif




void 
image1_fill_blob( image_type *sim, int nP, int pT, int bT, int value )
{

	int	i,	j;
	u_char *sp;
	u_short *bp;



	imageLabel_type *abw = imageLabelUS_N( sim, nP, 0, 0, NULL );


	imageLabelUS_set_boundary( abw );


	for( i = 0 ; i < abw->nA ; i++ ){
		bwLabel_type *bw = &abw->a[i];
		bw->existence = 0;

		if( bw->boundary == 1 )	bw->existence = 1;
	}


	bwLabel_filter( abw->a, abw->nA, pT, bT );



	sp = sim->data;
	bp = abw->im->data_us;

	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( *bp == 0 )	continue;
			if( abw->a[*bp].id < 0 )
				*sp = value;
		}


	imageLabel_destroy( abw );
}

void 
image1_fill_blobV( image_type *vim, image_type *sim, int nP, int pT, int bT, int vT, int value )
{
	image_type *im;
	bwLabel_type *abw;
	int nBw; 
	int	i,	j;
	u_char *sp;
	short *bp;

	im = image_create( sim->height, sim->width, 2, 1, NULL );

	sp = sim->data;
	bp = im->data_s;
	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ )
			*bp = ( *sp < nP )? 1 :0;


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}



	imageLabel2_set_boundary( im, abw, nBw );
	imageLabel2_set_value( vim, im, abw, nBw );


	for( i = 0 ; i < nBw ; i++ ){
		bwLabel_type *bw = &abw[i];
		bw->existence = 0;
	
		if( bw->boundary == 1 )
			bw->existence = 1;

		if( bw->av < vT &&  bw->no > 100 )
			bw->existence = 1;
	}




	bwLabel_filter( abw, nBw, pT, bT );



	sp = sim->data;
	bp = im->data_s;

	for( i = 0 ; i < sim->height ; i++ )
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( *bp == 0 )	continue;
			if( abw[*bp].id < 0 )
				*sp = value;
		}

		free( abw );
		image_destroy( im, 1 );
}


int
imageLabelUS_set_pixel( image_type *sim, imageLabel_type *abw, int value )
{
int	i,	j;

	u_char *sp = sim->data;
	short *bp = abw->im->data_s;

	for( i = 0 ; i < sim->height ; i++ ){
		for( j = 0 ; j < sim->width ; j++, sp++, bp++ ){
			if( *bp == 0 )	continue;
			if( abw->a[*bp].id < 0 )
				*sp = value;
		}
	}

	return( 1 );
}
