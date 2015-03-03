/************************
 *	ImageLabelContour.c	*
 ************************/
#ifdef _DEBUG
#define _DUMP
#endif

#include	"Uigp/igp.h"
#include	"Ucamera/Pt2dType.h"
#include	"Uln/PlnType.h"
#include	"Uln/Cln/ClnType.h"

#include	"ImageType/ImageType.h"

#include	"ImageLabel.h"


#define US_MS	0x8000
#define US_MN	0x7FFF


#define SIDE_NEXT( side )	( (side < 3 )? side+1 : 0 )
#define SIDE_PREV( side )	( (side > 0 )? side-1 : 3 )




static vec2f_type	DV[] = { -0.5, 0,
					  0,  -0.5,
					  0.5, 0,
					  0,   0.5 };

static vec2f_type	DT[] = { -1,   0,
					  0,  -1,
					  1,   0,
					  0,   1 };


static vec2f_type	DVC[] = {	-0.5,  -0.5,
						 0.5,  -0.5,
						 0.5,   0.5,
						-0.5,   0.5 };

static vec2f_type	DTC[] = {	-1,  -1,
						 1,  -1,
						 1,   1,
						-1,   1 };


static vec2f_type	DP[] = { 0, -1,
					 1,  0,
					 0,  1,
					 -1, 0 };

static vec2f_type	DPC[] = {  -1, -1,
						1,  -1,
						1,  1,
						-1, 1 };



static int	image_find_border( image_type *im, int val, int i0, int j0, int *ir, int *jr, int *side );

static void	agpLevel_contour_1( image_type *im, int i0, int j0, int side0, gapp_type *gapp );

static void	add_point( gapp_type *gapp, vec2d *v, int side );


static void	add_corner_point( gapp_type *gapp, vec2d *v, int side );

static gapp_type *	gapp_breakpoint( gapp_type *gapp, int d );


//static int	pln_approximate( gapp_type *gapp, int fClose, pln_type **pl );



cln_type *
imageLabelUS_contour( image_type *im, int id )
{
int	i,	j,	i0,	j0,	side;
int	no;
gapp_type *gapp;
cln_type *cln;

	gapp = gapp_alloc( 200000 );



	i0 = 1;
	j0 = 1;

	no = 0;

	cln = cln_alloc();

	while( image_find_border( im, id, i0, j0, &i,  &j, &side ) > 0 ){

		agpLevel_contour_1( im, i, j, side, gapp );
//		gapp_dump( gapp, "gapp", no, NULL );

		if( gapp->no < 64 )	
			continue;

		
		
		pln_type *pl;

		if( cln != NULL && cln->nA > 0 ){
			gapp_inverse( gapp );
//			gapp_approximate_link( gapp, 1, &ctr, &link );


			pln_approximate( gapp, 1, &pl );

			pln_inverse( pl );


//			c = contour_create( &ctr, link, PL_CLOSE, 0 );
//			contour_inverse( c );
		}
		else {
			pln_approximate( gapp, 1, &pl );

//			gapp_approximate_link( gapp, 1, &ctr, &link );
//			c = contour_create( &ctr, link, PL_CLOSE, 0 );
		}

		pln_set_length( pl );

		cln->a[cln->nA ++ ] = pl;

	

//		CONTOUR_DUMP( c, "c", no, NULL );
//		ac = contour_union( ac, c );

	
		no++;

		i0 = i;
		j0 = j;

	}

	gapp_destroy( gapp );

//	CONTOUR_DUMP( ac, "c", no, "gl" );

	return( cln );

}

#define VP( p )	((p&US_MN))

static void
agpLevel_contour_1( image_type *im, int i0, int j0, int side0, gapp_type *gapp )
{
u_short	*sp,	*np;
u_short	*sp0;
u_short	*sp1,	*np1;
vec2f_type	v;
int	DN[4],	DN1[4];
int	side;


	DN[0] = -1;
	DN[1] = -im->width;
	DN[2] = 1;
	DN[3] = im->width;

	DN1[0] = -im->width;
	DN1[1] = 1;
	DN1[2] = im->width;
	DN1[3] = -1;


	v.x = j0 + 0.5;
	v.y = i0 + 0.5;

	sp0 = (u_short*)IMAGE_PIXEL( im, i0, j0 );

	sp = sp0;
	side = side0;



	gapp->no = 0;
	while( gapp->no == 0 || sp != sp0 || side != side0 ){
		*sp |= US_MS;
		add_point( gapp, &v, side );

		np = sp + DN[side];

		sp1 = sp + DN1[side];
		np1 = sp1 + DN[side];



		// is corner
		if( VP(*sp1) != VP(*sp) && VP(*sp1) != VP(*np) || VP(*np1) != VP(*sp) && VP(*np1) != VP(*np) )
			add_corner_point( gapp, &v, side );







	
		if( (*sp&US_MN) == (*np1&US_MN) ){
			sp = np1;
			v.x += DPC[side].x;
			v.y += DPC[side].y;
			side = SIDE_PREV( side );

			continue;
		}


		if( (*sp&US_MN) != (*sp1&US_MN) ){
			side = SIDE_NEXT( side );

			continue;
		}

		sp = sp1;
		v.x += DP[side].x;
		v.y += DP[side].y;

	}
}


static void
add_point( gapp_type *gapp, vec2d *v, int side )
{
	gapp->v[gapp->no].y = v->x + DV[side].x;
	gapp->v[gapp->no].x = v->y + DV[side].y;

	gapp->tang[gapp->no].y = DT[side].x;
	gapp->tang[gapp->no].x = DT[side].y;

	gapp->flag[gapp->no] = 0;

	gapp->no++;
}

static void
add_corner_point( gapp_type *gapp, vec2d *v, int side )
{
	gapp->v[gapp->no].y = v->x + DVC[side].x;
	gapp->v[gapp->no].x = v->y + DVC[side].y;

	gapp->tang[gapp->no].y = DTC[side].x;
	gapp->tang[gapp->no].x = DTC[side].y;

	gapp->flag[gapp->no] = 1;

	gapp->no++;
}




static int
image_find_border( image_type *im, int val, int i0, int j0, int *ir, int *jr, int *side )
{
u_short	*sp, *sp0,	*sp1;
int		i, j;


	for( i = i0; i < im->row-1 ; i++ ){
		sp = (u_short *)IMAGE_PIXEL( im, i, 1 );
		sp0 = sp - im->width;
		sp1 = sp - im->width;


		for( j = 1 ; j < im->width ; j++, sp++, sp0++, sp1++ ){
			if( *sp & US_MS )	continue;
			if( *sp != val )	continue;

			if( *sp != VP(*(sp - 1)) ){
				*ir = i;
				*jr = j;
				*side = 0;
				return( 1 );
			}

			if( *sp != VP(*(sp0)) ){
				*ir = i;
				*jr = j;
				*side = 1;
				return( 1 );
			}

			if( *sp != VP(*(sp+1)) ){
				*ir = i;
				*jr = j;
				*side = 2;
				return( 1 );
			}

			if( *sp != VP(*(sp1)) ){
				*ir = i;
				*jr = j;
				*side = 3;
				return( 1 );
			}
	
		}
	}

	return( -1 );
}



#ifdef _AA_

static int
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

#endif