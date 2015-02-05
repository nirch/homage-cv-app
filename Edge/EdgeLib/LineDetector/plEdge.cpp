/**********************
 ***   plEdge.cpp   ***
 **********************/
#ifdef _DEBUG
#define	_DUMP
#endif

#include	<math.h>



//#include "Wutil/UWgpw/GpwType.h"



#include "plEdgeType.h"







plEdge_type * plEdge_alloc()
{
	plEdge_type *r =( plEdge_type *)malloc( sizeof(plEdge_type));

	r->pl = NULL;

	return( r );
}


void plEdge_destroy( plEdge_type *r )
{

	if( r->pl != NULL )
		pln_destroy( r->pl );

	free( r );
}


void plEdge_draw( gpw_type *gpw, plEdge_type *r )
{
//	int	color;

//	color = YELLOW;



//	if( r->pl != NULL )
//		gpw_pln( gpw, r->pl, color, color, 0x01 );
}


void
plEdge_translate( plEdge_type *c, vec2f_type *ctr )
{

	c->pl->ctr.x += ctr->y;
	c->pl->ctr.y += ctr->x;

}



plEdgeA_type * plEdgeA_alloc( int n )
{
	plEdgeA_type *ar =( plEdgeA_type *)malloc( sizeof(plEdgeA_type));

	ar->NA = n;

	ar->a =( plEdge_type **)malloc( ar->NA * sizeof(plEdge_type *));

	ar->nA = 0;

	return( ar );
}

plEdgeA_type * plEdgeA_realloc( plEdgeA_type *ar, int n )
{
	if( ar == NULL ){
		ar = plEdgeA_alloc( n );
		return( ar );
	}

	if( ar->NA >= n )
		return( ar );


	ar->NA = n;

	ar->a =( plEdge_type **)realloc( ar->a, ar->NA * sizeof(plEdge_type *));

	return( ar );
}


void plEdgeA_destroy( plEdgeA_type *ar )
{
int	i;

	for( i = 0 ; i < ar->nA ; i++ )
		plEdge_destroy( ar->a[i] );

	free( ar->a );

	free( ar );
}


void plEdgeA_clear( plEdgeA_type *ar )
{
	int	i;

	if( ar == NULL )
		return;

	for( i = 0 ; i < ar->nA ; i++ )
		plEdge_destroy( ar->a[i] );
	
	ar->nA = 0;
}



void
plEdgeA_translate( plEdgeA_type *ae, vec2f_type *ctr )
{
	int	i;

	for( i = 0 ; i < ae->nA ; i++ ){
		pln_type *pl = ae->a[i]->pl;
		pl->ctr.x += ctr->y;
		pl->ctr.y += ctr->x;
	}
}

void
plEdgeA_scale( plEdgeA_type *ae, float scale )
{
	int	i;

	for( i = 0 ; i < ae->nA ; i++ ){
		pln_type *pl = ae->a[i]->pl;
		pln_scale( pl, scale );
	}
}



plnA_type *
plEdgeA_to_plnA( plEdgeA_type *ae, float minLen, int fCopy )
{
plnA_type *apl;
int	i;

	apl = plnA_alloc( ae->nA );

	for( i = 0 ; i < ae->nA ; i++ ){
		pln_type *pl = ae->a[i]->pl;
		if( pl->len < minLen )
			continue;
		if( fCopy == 1)
			pl = pln_copy( pl );

		pl->id = i;

		apl->a[apl->nA++] = pl;
	}

	return( apl );
}


void		
plEdgeA_write( plEdgeA_type *ae, char *outFile )
{
	char	file[256];

	plnA_type *apl = plEdgeA_to_plnA( ae, 0, 0 );
	gpFilename_force_extension( outFile, ".pl", file );
	plnA_write( apl, file );

	apl->nA = 0;
	plnA_destroy( apl );
}

void		
plEdgeA_dump( plEdgeA_type *ae, char *prefix, int index, char *suffix )
{
	
	plnA_type *apl = plEdgeA_to_plnA( ae, 0, 0 );


	plnA_dump( apl, prefix, index, suffix );

	apl->nA = 0;
	plnA_destroy( apl );
}

int
plEdgeA_select( plEdgeA_type *ar, vec2f_type *p, float D )
{
int	i;
float	uMin;
int	iMin;
dPln_type d;

	iMin = -1;
	uMin = 2*D;
	for( i = 0 ; i < ar->nA ; i++ ){
		plEdge_type	*e = ar->a[i];

		pln_type *pl = ar->a[i]->pl;

		pln_distance( pl, p, &d );
		if( d.gt < 0 || d.gt > pl->len )
			continue;

		if( ABS(d.u) < ABS(uMin) ){
			uMin = d.u;
			iMin = i;
		}
	}

	return( iMin );
}