/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/********************
 ***   LkType.h   ***
 ********************/

#ifndef         _LN_TYPE_
#define         _LN_TYPE_
#ifdef __cplusplus
extern "C" {
#endif

#include "Uigp/igp.h"
#include	"Uvec/Vec2d.h"
#include "UGeoApp/GeoApp.h"
#include "Umath/LT2Type.h"
#include "Ubox/Box2d.h"


typedef struct ln_type {

	vec2f_type	ctr;

	vec2f_type	v;		// len* ( cos(angle) , sin(angle) )
	float	a;		// maximal height of parabola


	// AUX
	float	len;

	vec2f_type	u;			// right normal to v
	float   c_prb;		// 2-nd order coefficient of parabola



	struct ln_type	*p[2];		// F_BEGIN and F_END
} ln_type;





typedef struct dPln_type {
	float	sgt;

	float	u;
	float	t;
	float	gt;
	ln_type	*l;


} dPln_type;



#define		LN_ALLOC()	\
			( (ln_type *)malloc( sizeof( ln_type ) ) )


#define		LN_NEXT( link )	( link->p[F_END] )

#define		LN_PREV( link )	( link->p[F_BEGIN] )

ln_type *	ln_alloc();
void		ln_destroy( ln_type *ln );

ln_type *	lnL_alloc( int ino );

void		lnL_destroy( ln_type *ln );



void	ln_set_aux( ln_type *l ) ;

ln_type *	ln_create( vec2f_type *p0, vec2f_type *p1, float crbA );

float	lnL_length( ln_type *link );
float	lnL_lengthE( ln_type *link, ln_type *el );


int		lnL_no( ln_type *link );

void	ln_connect( ln_type *l, ln_type *nl );


void	ln_t2xy( vec2f_type *vm, ln_type *link, float t, vec2f_type *p );



void	ln_tanget( ln_type *l, float t, vec2f_type *tanget );

void	ln_tanget_beg( ln_type *l, vec2f_type *t );

void	ln_tanget_end( ln_type *l, vec2f_type *t );

void	ln_Ntanget( ln_type *l, float t, vec2f_type *nt );

void	ln_Ntanget_beg( ln_type *l, vec2f_type *t );

void	ln_Ntanget_end( ln_type *l, vec2f_type *t );

int		ln_tanget_point_10( ln_type *l, vec2d *vm, float *t, vec2d *p );

int		ln_tanget_point_01( ln_type *l, vec2d *vm, float *t, vec2d *p );

float	ln_distance( vec2d *ctr, ln_type *l, vec2d *v, float *u, float *t );

int		lnL_distance( vec2f_type *ctr, ln_type  *link, ln_type *elink, vec2f_type  *v,
				  ln_type **sl, float *u, float *t, float *gt );

int		ln_distanceG( vec2f_type *ctr, ln_type  *link, ln_type *elink, vec2f_type  *v, float *gt );

//void	ln_from_gapp( gapp_type *gapp, vec2f_type *ctr, ln_type **gelm );


	// LnBox.c
void	lnL_box( vec2d *ctr, ln_type *link, ln_type *last, box2f_type *box );

void	ln_box( vec2d *ctr, ln_type *link, box2f_type *box );







	// LnTool1.c
ln_type *	lnL_copy( ln_type *plink, ln_type *elink );

void	lnL_end_point( vec2d *ctr, ln_type *link, vec2d *p );

void	lnL_lastP( vec2d *ctr, ln_type *link, vec2d *p, ln_type **ll );


ln_type *	lnL_last( ln_type *link );

void	lnL_connect( ln_type *link, ln_type *nl );


void	lnL_gt2lt( ln_type *link, float gt, ln_type **l0, float *t0 );

void	lnL_gt2lt_p( vec2f_type *ctr, ln_type *link, float gt, vec2f_type *ctr0, ln_type **l0, float *t0 );


int		lnL_copy_sub( vec2d *pctr, ln_type *plink, float gt0, float gt1, vec2d *ctr, ln_type **link  );

void	lnL_connect_s( vec2f_type *ctr, ln_type *link, vec2f_type *ctr1, ln_type *link1, float T );


ln_type *	lnL_inverse( ln_type *link );

void	ln_inverse( ln_type *l );


void	lnL_lt2( vec2d *ctr, ln_type *link, lt2_type *lt );

void	lnL_at_b( vec2d *ctr, ln_type *link, float a, float b );


int	lnL_from_points( vec2d p[], int no, int Fclose, vec2f_type *ctr, ln_type **lnL );

int	lnL_from_circle( vec2f_type *ctr0, float R, float dr, vec2f_type *ctr, ln_type **lnL );

int	lnL_from_sub_circle( vec2f_type *ctr0, float R, vec2f_type *p0, vec2f_type *p1, float dr, vec2f_type *ctr, ln_type **lnL );



void	ln_to_vlf( vec2f_type *ctr, ln_type *l, struct vl2f_type *vl );

void	ln_from_vlf( vec2f_type *ctr, ln_type *l, struct vl2f_type *vl );

	// LnSplt.c
ln_type *	ln_split( ln_type *link, float t, float dt );

ln_type *	ln_split_t( ln_type *link, float t0 );



	// LnFromGapp.c
int		lnL_from_gapp( struct gapp_type *gapp, vec2f_type *ctr, ln_type **lnL );


	// LnWrite.c
int		lnL_write( vec2f_type *ctr, ln_type *lnL, char *file );

void	lnL_dump( vec2f_type *ctr, ln_type *lnL, char *prefix, int index, char *suffix );


// LnReda.c
int		lnL_read_file( char *file, vec2f_type *ctr, ln_type **link  );



#ifdef __cplusplus
}
#endif
#endif
