/************************
 *	PlnApproximate.cpp	*
 ************************/
#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"
#include	"Ucamera/Pt2dType.h"
#include	"PlnType.h"
#include	"Cln/ClnType.h"

gapp_type *	pt2dA_to_gapp( pt2dA_type *apt, gapp_type *gapp );


int 
pln_approximateA(pt2dA_type *apt, int fClose, pln_type **pl )
{
	gapp_type *gapp = pt2dA_to_gapp( apt, NULL );

	int ret = pln_approximate( gapp, fClose, pl );

	gapp_destroy( gapp );

	return( ret );
}


gapp_type *
pt2dA_to_gapp( pt2dA_type *apt, gapp_type *gapp )
{
	if( gapp != NULL )
		gapp_destroy( gapp );


	gapp = gapp_alloc( apt->nA );
	//gapp->no = 0;
	int i ;
	for( i = 0 ; i < apt->nA ; i++ ){
		pt2d_type *pt = &apt->a[i];
	
		gapp->v[gapp->no].y = pt->p.x;
		gapp->v[gapp->no].x = pt->p.y;

		gapp->tang[gapp->no].y = pt->n.x;
		gapp->tang[gapp->no].x = pt->n.y;

		gapp->flag[gapp->no] = 0;

		gapp->no++;
	}

	return( gapp );
}


gapp_type *	gapp_breakpoint( gapp_type *gapp, int d );


int
pln_approximate( gapp_type *gapp, int fClose, pln_type **pl )
{
gapp_prm	Qgprm;
gapp_type	*ga;

	Qgprm.dwight = 0;//0.10;
	Qgprm.err = 0.01;

	Qgprm.min_len = 30;//3.0;
	Qgprm.Min_pnumber = 4;
	Qgprm.max_len = 60.0;

	Qgprm.Max_dev = 2;
	Qgprm.Min_dev = 0.5;



	ga = gapp_breakpoint( gapp,  4 );



	if( fClose == 1 ){
		ga->v[ga->no] = ga->v[0];
		ga->tang[ga->no] = ga->tang[0];
		ga->flag[ga->no] = ga->flag[0];

		ga->no++;
	}


	geo_approximate( ga, &Qgprm );


	*pl = pln_from_gapp( ga );
	(*pl)->state = ( fClose == 1 )? PLN_CLOSE : PLN_OPEN;


	gapp_destroy( ga );
	return( 1 );
}


static void	gapp_breakpoint_x( gapp_type *gapp, int d );
static void	gapp_breakpoint_y( gapp_type *gapp, int d );
static int	gapp_get_breakpoint( gapp_type *gapp, int i0 );


gapp_type *
gapp_breakpoint( gapp_type *gapp, int d )
{
gapp_type	*ga;
int	i,	j,	i0;

	gapp_breakpoint_x( gapp, d );
		
	gapp_breakpoint_y( gapp, d );

	ga = gapp_alloc( gapp->no+1 );


	i0 = gapp_get_breakpoint( gapp, 0 );
	if( i0 < 0 )	i0 = 0;


	for( j = 0, i = i0 ; j < gapp->no ; j++, i++ ){
		if( i >= gapp->no )	i = 0;

		ga->v[j] = gapp->v[i];
		ga->tang[j] = gapp->tang[i];
		ga->flag[j] = gapp->flag[i];
	}
	ga->no = gapp->no;

	return( ga );
}


static void
gapp_breakpoint_x( gapp_type *gapp, int d )
{
int	i,	j,	n,	k;

	for( i = 0 ; i < gapp->no ; ){
		for( j = i+1, n = 0 ; j < gapp->no ; j++, n++ ){
			if( gapp->v[i].x != gapp->v[j].x )	break;
		}

		if( j == gapp->no ){
			for( j = 0  ; j < i ; j++, n++ ){
				if( gapp->v[i].x != gapp->v[j].x )	break;
			}		
		}

		if( n > 4 ){
			gapp->flag[i] = 1;
			if( (k = j-1) < 0 )	k = gapp->no-1;
			gapp->flag[k] = 1;
		}

		if( j < i )	break;

		i = j;
	}
}


static void
gapp_breakpoint_y( gapp_type *gapp, int d )
{
	int	i,	j,	n,	k;

	for( i = 0 ; i < gapp->no ; ){
		for( j = i+1, n = 0 ; j < gapp->no ; j++, n++ ){
			if( gapp->v[i].y != gapp->v[j].y )	break;
		}

		if( j == gapp->no ){
			for( j = 0  ; j < i ; j++, n++ ){
				if( gapp->v[i].y != gapp->v[j].y )	break;
			}		
		}

		if( n > 4 ){
			gapp->flag[i] = 1;

			if( (k = j-1) < 0 )	k = gapp->no-1;
			gapp->flag[k] = 1;
		}

		if( j < i )	break;

		i = j;
	}
}


static int
gapp_get_breakpoint( gapp_type *gapp, int i0 )
{
int	i;

	for( i = i0 ; i < gapp->no ; i++ )
		if( gapp->flag[i] == 1 )	return( i );

	return( -1 );
}
