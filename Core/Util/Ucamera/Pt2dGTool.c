/***********************
 ***	Pt2dTool.c   ***
 ***********************/
#include	<math.h>
#include	"Uigp/igp.h"
#include	"Umath/Matrix2Type.h"
#include	"Ubox/Box2d.h"
#include	"Pt2dType.h"



#define	GPT_PT( gpt, i, j )	( (gpt)->p[i * (gpt)->Nj + j] )
#define GPT_I( gpt, p )		( ( (p).y - (gpt)->box.y0 )*(gpt)->inv_d )
#define GPT_J( gpt, p )		( ( (p).x - (gpt)->box.x0 )*(gpt)->inv_d )

#define GPT_Y2I( gpt, y )		( ( (y) - (gpt)->box.y0 )*(gpt)->inv_d )
#define GPT_X2J( gpt, x )		( ( (x) - (gpt)->box.x0 )*(gpt)->inv_d )



pt2dG_type *
pt2dG_alloc( box2f_type *b, float d )
{
pt2dG_type	*gpt;
int	i;

	gpt = (pt2dG_type *)malloc( sizeof( pt2dG_type) );

	gpt->d = d;
	gpt->inv_d = 1.0 /d;

	gpt->box = *b;


	gpt->Ni = ( gpt->box.y1 - gpt->box.y0) * gpt->inv_d;
	if( gpt->Ni * gpt->d <  gpt->box.y1 - gpt->box.y0 )
		gpt->Ni++;

	gpt->Nj = ( gpt->box.x1 - gpt->box.x0) * gpt->inv_d;
	if( gpt->Nj * gpt->d <  gpt->box.x1 - gpt->box.x0 )
		gpt->Nj++;


	gpt->p = ( pt2d_type **)malloc( gpt->Ni * gpt->Nj*sizeof(pt2d_type *) );

	for( i = 0 ; i < gpt->Ni*gpt->Nj ; i++ )
		gpt->p[i] = NULL;

	pt2dG_clear( gpt );

	return( gpt );
}




void
pt2dG_destroy( pt2dG_type *gpt )
{
	free( gpt->p );

	free( gpt );
}


void
pt2dG_clear( pt2dG_type *gpt )
{
int	i;

	for( i = 0 ; i < gpt->Ni*gpt->Nj ; i++ )
		gpt->p[i] = NULL;

}



int
pt2dG_create( pt2dA_type *apt, float d )
{

	if( apt->box.x1 - apt->box.x0 <= 0 )
		pt2dA_box( apt );


	apt->gpt = pt2dG_alloc( &apt->box, d );


	pt2dG_reset( apt );

	return( 1 );
}


int	
pt2dG_reset( pt2dA_type *apt )
{
pt2dG_type *gpt;
int	i,	i0,	j0,	k;


	gpt = apt->gpt;

	pt2dG_clear( gpt );


	for( i = 0 ; i < apt->nP ; i++ ){
		pt2d_type *pt = &apt->p[i];

		i0 = GPT_I( gpt, pt->p );
		j0 = GPT_J( gpt, pt->p );

		k = i0 * gpt->Nj + j0;

		pt->id = i;

		pt->np = gpt->p[k];
		gpt->p[k] = pt;

	}

	return( 1 );

}


pt2dA_type * 
pt2dG_copy_neighbor( pt2dA_type *apt, vec2d *p, float D, pt2dA_type *capt )
{
pt2dG_type *gpt;
pt2d_type *pt;
vec2f_type	v;
int	i,	j,	i0,	j0,	i1,	j1;
float	D2,	d;


	capt = pt2dA_realloc( capt, apt->nP );

	capt->type = apt->type;
	capt->nP = 0;

	gpt = apt->gpt;


	i0 = GPT_Y2I( gpt, p->y - D );
	j0 = GPT_X2J( gpt, p->x - D );
	i1 = GPT_Y2I( gpt, p->y + D ) + 1;
	j1 = GPT_X2J( gpt, p->x + D ) + 1;




	if( i0 < 0 )	i0 = 0;
	if( j0 < 0 )	j0 = 0;
	if( i1 > gpt->Ni )	i1 = gpt->Ni;
	if( j1 > gpt->Nj )	j1 = gpt->Nj;


	D2 = D*D;

	for( i = i0 ; i < i1 ; i++ ){
		for( j = j0 ; j < j1 ; j++ ){
			pt = GPT_PT( gpt, i, j );

			for( ; pt != NULL ; pt = pt->np ){
//				pt2d_type *pt = &apt->p[i];

				v.x = pt->p.x - p->x;
				v.y = pt->p.y - p->y;

				d = v.x*v.x + v.y *v.y;

				if( d == 0 )	continue;
				if( d > D2 )	continue;

				capt->p[capt->nP] =  *pt;
//				capt->p[capt->nP].id = i;
				capt->nP++;

			}
		}
	}



	return( capt );
}



