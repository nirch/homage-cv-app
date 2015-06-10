/**************************
 ***	ImageFields.c	***
 *************************/

#include "ImageType/ImageType.h"


static void	image4_to_fields( image_type *sim, image_type **oddField, image_type **evenField );
static void	image3_to_fields( image_type *sim, image_type **oddField, image_type **evenField );


void
image_to_fields( image_type *sim, image_type **oddField, image_type **evenField )
{
	if( sim->depth == 4 ){
		image4_to_fields( sim, oddField, evenField );
		return;
	}

	if( sim->depth == 3 ){
		image3_to_fields( sim, oddField, evenField );
		return;
	}

}


static void
image3_to_fields( image_type *sim, image_type **oddField, image_type **evenField )
{
u_char	*sp,	*p1,	*p2;
int	evenRow,	oddRow;
int	i,	j;


	if( sim->row % 2 == 0 )
		evenRow = oddRow = sim->row / 2;
	else {
		evenRow = sim->row / 2;
		oddRow = evenRow + 1;
	}

	*oddField = image_realloc( *oddField, sim->width, oddRow, 3, IMAGE_TYPE_U8, 1 );
	*evenField = image_realloc( *evenField, sim->width, oddRow, 3, IMAGE_TYPE_U8, 1 );

	sp = sim->data;
	p1 = (*oddField)->data;
	p2 = (*evenField)->data;


	for( i = 0 ; i < sim->row ; i += 2 ) {

		for( j = 0 ; j < 3*sim->column ; j++ )
			*p1++ = *sp++;


		for( j = 0 ; j < 3*sim->column ; j++ )
			*p2++ = *sp++;
	}
}



static void
image4_to_fields( image_type *sim, image_type **oddField, image_type **evenField )
{
u_int	*sp,	*p1,	*p2;
int	evenRow,	oddRow;
int	i,	j;


	if( sim->row % 2 == 0 )
		evenRow = oddRow = sim->row / 2;
	else {
		evenRow = sim->row / 2;
		oddRow = evenRow + 1;
	}

	*oddField = image_realloc( *oddField, sim->width, oddRow, 4, IMAGE_TYPE_U8, 1 );
	*evenField = image_realloc( *evenField, sim->width, oddRow, 4, IMAGE_TYPE_U8, 1 );

	sp = (u_int *)sim->data;
	p1 = (u_int *)(*oddField)->data;
	p2 = (u_int *)(*evenField)->data;
	

	for( i = 0 ; i < sim->row ; i += 2 ) {

		for( j = 0 ; j < sim->column ; j++ )
			*p1++ = *sp++;


		for( j = 0 ; j < sim->column ; j++ )
			*p2++ = *sp++;
	}
}







static image_type *	image3_from_fields( image_type *oddField, image_type *evenField, image_type *im );
static image_type *	image4_from_fields( image_type *oddField, image_type *evenField, image_type *im );
static image_type *	image6_from_fields( image_type *oddField, image_type *evenField, image_type *im );



image_type *
image_from_fields(image_type *oddField, image_type *evenField, image_type *im )
{
	if( oddField->depth == 4 ){
		im = image4_from_fields( oddField, evenField, im );
		return( im );
	}

	if( oddField->depth == 3 ){
		im = image3_from_fields( oddField, evenField, im );
		return( im );
	}

	if( oddField->channel == 3 && oddField->depth == 6 ){
		im = image6_from_fields( oddField, evenField, im );
		return( im );
	}

	return( NULL );

}


static image_type *
image3_from_fields( image_type *oddField, image_type *evenField, image_type *im )
{
	u_char	*tp,	*p1,	*p2;
	int	row;
	int	i,	j;


	row = evenField->row + oddField->row;

	im = image_realloc( im, oddField->width, oddField->height*2, 3, IMAGE_TYPE_U8, 1 );

	tp = im->data;
	p1 = oddField->data;
	p2 = evenField->data;

	for( i = 0 ; i < im->row/2 ; i++ ) {
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *p1++;
			*tp++ = *p1++;
			*tp++ = *p1++;
		}

		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *p2++;
			*tp++ = *p2++;
			*tp++ = *p2++;
		}

	}
	return( im );
}


static image_type *
image6_from_fields( image_type *oddField, image_type *evenField, image_type *im )
{
	short 	*tp,	*p1,	*p2;
	int	row;
	int	i,	j;


	row = evenField->row + oddField->row;

	im = image_realloc( im, oddField->width, oddField->height*2, 3, IMAGE_TYPE(oddField), 1 );

	tp = im->data_s;
	p1 = oddField->data_s;
	p2 = evenField->data_s;

	for( i = 0 ; i < im->row/2 ; i++ ) {
		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *p1++;
			*tp++ = *p1++;
			*tp++ = *p1++;
		}

		for( j = 0 ; j < im->column ; j++ ){
			*tp++ = *p2++;
			*tp++ = *p2++;
			*tp++ = *p2++;
		}

	}
	return( im );
}



static image_type *
image4_from_fields( image_type *oddField, image_type *evenField, image_type *im )
{
u_int	*tp,	*p1,	*p2;
int	row;
int	i,	j;

	
	row = evenField->row + oddField->row;

	im = image_realloc( im, oddField->width, oddField->height*2, 4, IMAGE_TYPE_U8, 1 );

	tp = (u_int *)im->data;
	p1 = (u_int *)oddField->data;
	p2 = (u_int *)evenField->data;

	for( i = 0 ; i < im->row ; i++ ) {
		if( i % 2 == 0 ) {
			for( j = 0 ; j < im->column ; j++ )
				*tp++ = *p1++;
		}
		else {
			for( j = 0 ; j < im->column ; j++ )
				*tp++ = *p2++;
		}		
	}
	return( im );
}




image_type *
image_field( image_type *sim, int field, int half )
{
image_type	*im;
u_int	*sp,	*p;
int	row,	col;
int	i,	j;

	if( half == 1 )
		return( image_field_half( sim, field, NULL ) );



	row = sim->row/2;
	col = sim->column;

	im = image_create( row, col, 4, 1, NULL );


	sp = (u_int *)sim->data;
	p = (u_int *)(im)->data;

	
	if( field == 2 )
		sp += sim->column;
	
	for( i = 0 ; i < sim->row ; i += 2, sp += sim->column ) {

		for( j = 0 ; j < sim->column ; j++ )
			*p++ = *sp++;
	}

	return( im );
}


static image_type *image3_field_half( image_type *sim, int field, image_type *im );
static image_type *image4_field_half( image_type *sim, int field, image_type *im );

image_type *
image_field_half(image_type *sim, int field, image_type *im )
{
	if( sim->depth == 4 ){
		im = image4_field_half( sim, field, im );
		return( im );
	}

	if( sim->depth == 3 ){
		im = image3_field_half( sim, field, im );
		return( im );
	}


	return( NULL );
}


static image_type *
image4_field_half( image_type *sim, int field, image_type *im )
{

u_int	*sp,	*p;
int	height,	width;
int	i,	j;
int	red,	green,	blue;


	height = sim->height/2;
	width = sim->width/2;

	im = image_realloc( im, width, height, 4, IMAGE_TYPE_U8, 1 );


	sp = (u_int *)sim->data;
	p = (u_int *)(im)->data;

	
	if( field == 2 )
		sp += sim->column;
	
	for( i = 0 ; i < sim->height ; i += 2, sp += sim->column ) {

		for( j = 0 ; j < sim->width ; j += 2 ){
			red = (IMAGE4_RED(*sp) + IMAGE4_RED(*(sp+1)) )/2;
			green = (IMAGE4_GREEN(*sp) + IMAGE4_GREEN(*(sp+1)) )/2;
			blue = (IMAGE4_BLUE(*sp) + IMAGE4_BLUE(*(sp+1)) )/2;
			*p++ = IMAGE4_RGB( red, green, blue );

			sp += 2;
		}
	}

	return( im );
}



static image_type *
image3_field_half( image_type *sim, int field, image_type *im )
{
	u_char	*sp,	*tp;
	int	height,	width;
	int	i,	j;



	height = sim->height/2;
	width = sim->width/2;


	im = image_realloc( im, width, height, 3, IMAGE_TYPE_U8, 1 );


	sp = sim->data;
	tp = im->data;


	if( field == 2 )
		sp += 3*sim->width;

	for( i = 0 ; i < sim->row ; i += 2, sp += 3*sim->width ) {

		for( j = 0 ; j < sim->column ; j += 2 ){
			*tp++ = ( *sp + *(sp+3) )/2;
			*tp++ = ( *(sp+1) + *(sp+4) )/2;
			*tp++ = ( *(sp+2) + *(sp+5) )/2;

			sp += 6;
		}
	}

	return( im );
}