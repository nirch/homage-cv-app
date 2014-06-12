/*******************
 ***  PlnTool.c  ***
 *******************/
#include <string.h>
#include <math.h>
#include "Uigp/igp.h"

#ifdef _WIN32
#define _GPMEMORY_LEAK 
#endif
#include "Uigp/GpMemoryLeak.h"


#include "Pt2dType.h"



pt2dF_type *	
pt2dF_alloc( int n )
{
pt2dF_type	*vpl;
int	i;

	vpl = ( pt2dF_type *)malloc( sizeof( pt2dF_type) );
	
	vpl->NA = n;
	
	vpl->a = ( pt2dA_type **)malloc( vpl->NA * sizeof( pt2dA_type*) );

	vpl->nA = 0;

	for( i = 0 ; i <  vpl->NA ; i++ )
		vpl->a[i] = NULL;
	
	return( vpl );
}


void 
pt2dF_destroy( pt2dF_type *vpl )
{
int	i;


	for( i = 0 ; i < vpl->nA ; i++ ){
		if( vpl->a[i] != NULL ){
			pt2dA_destroy( vpl->a[i] );
			vpl->a[i] = NULL;
		}
	}


	free( vpl->a );

	free( vpl );
}


void
pt2dF_add( pt2dF_type *vpl, pt2dA_type *apl, int iFrame )
{
int	i;

	if( iFrame >= vpl->NA ){
		
		int NA = vpl->NA + iFrame+32;
		vpl->a = ( pt2dA_type **)realloc( vpl->a, NA*sizeof(pt2dA_type *) );

		for( i = NA ; i <  vpl->NA ; i++ )
			vpl->a[i] = NULL;

		vpl->NA = NA;
	}

	if( vpl->nA > iFrame && vpl->a[iFrame] != NULL )
		pt2dA_destroy( vpl->a[iFrame] );

	vpl->a[iFrame] = apl;
	apl->iFrame = iFrame;


	if( vpl->nA <= iFrame )
		vpl->nA = iFrame+1;

}






void
pt2dF_clear( pt2dF_type *vpl, int iFrame )
{
	if( vpl->a[iFrame] != NULL ){
		pt2dA_destroy( vpl->a[iFrame] );

		vpl->a[iFrame] = NULL;
	}
}

FILE *
pt2dF_write_open( char *file )
{
	FILE *fp;

	if( (fp = fopen( file, "wb")) == NULL )
		return( NULL );

	pt2dF_write_header( fp );

	return( fp );
}


int
pt2dF_write_header( FILE *fp )
{ 

	fprintf(fp, "%s  %d  %d\n", "PTF", PT2D_VERSION, 0 );

	return( 1 );
}


int
pt2dF_read( pt2dF_type **fpt, char *file )
{
FILE	*fp;
int	version,	n;
char	signature[64];




	if( (fp = fopen( file, "rb")) == NULL )
		return( -1 );

	fscanf(fp, "%s  %d  %d", &signature, &version, &n );
	if( strcmp( signature, "PTF") != 0 ){
		fseek( fp, 0, SEEK_SET );
		version = 0;
	}



	*fpt = pt2dF_alloc( 1000 );
	int iFrame;

	pt2d_type pt;
	pt2dA_type	*apt = NULL;

	while( 1 ){

		if( version == 0 ){
			if( fscanf( fp, "%d    %d     %f %f   %f %f   %f %f",
					&iFrame, &pt.id, &pt.p.x, &pt.p.y, &pt.n.x, &pt.n.y, &pt.r, &pt.f ) != 8 )
				break;

			pt.group = ( pt.id < 0 )? 3 : 0;
		}
		else {
			if( fscanf( fp, "%d   %d  %d     %f %f   %f %f   %f %f",
				&iFrame, &pt.group, &pt.id, &pt.p.x, &pt.p.y, &pt.n.x, &pt.n.y, &pt.r, &pt.f ) != 9 )
				break;
		}


		if( apt != NULL && apt->iFrame != iFrame ){
			pt2dF_add( *fpt, apt, apt->iFrame );
			apt = NULL;
		}
		if( apt == NULL ){
			apt = pt2dA_alloc( 16 );
			apt->iFrame = iFrame;
			apt->axis = PT2D_AXIS_XY;
		}


		apt->a[apt->nA++] = pt;
	}
	

	if( apt != NULL )
		pt2dF_add( *fpt, apt, apt->iFrame );


	fclose( fp );

	return( 1 );
}