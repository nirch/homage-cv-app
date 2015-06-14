
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

//#ifdef _DEBUG
#define _DUMP 
//#endif

#include "Ucamera/Pt2dType.h"

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"
#include	"ImageMark/ImageMark.h"

#include	"Lk/LkRegistration.h"

#include "DynamicMask.h"










CDynamicMask::CDynamicMask()
{
	m_at = NULL;
	m_nT = 0;

	m_apt = NULL;

	m_mim = NULL;

	m_state = 0;
}


CDynamicMask::~CDynamicMask()
{
	DeleteContents();

}



void CDynamicMask::DeleteContents()
{

int	i;

	for( i = 0 ; i < m_nT ; i++ ){
		delete m_at[i];
	}
	m_nT = 0;

	if( m_mim != NULL ){
		image_destroy( m_mim, 1 );
		m_mim = NULL;
	}
	
	if( m_yim != NULL ){
		image_destroy( m_yim, 1 );
		m_yim = NULL;
	}

	if( m_apt != NULL ){
		pt2dA_destroy( m_apt );
		m_apt = NULL;
	}

}







int	CDynamicMask::Init( image_type *sim, image_type *mim, pt2dA_type *apt, int dd )
{
int	i;
box2i	b;

	m_mim = image1_dup( mim, 8, m_mim );
	//m_mim = image_make_copy( mim, m_mim );


	IMAGE_DUMP_SCALE( m_mim, 255, "Db", 1, "MASK");

//	apt->a[0] = apt->a[4];
//	apt->nA = 1;

	m_apt = pt2dA_copy( apt, m_apt );

	m_yim = image1_from( sim, NULL );
	if( m_yim == sim )
		m_yim = image_make_copy( sim, NULL );


	m_at = ( CLkRegistration **)malloc( apt->nA * sizeof( CLkRegistration*) );

	for( i = 0 ; i < apt->nA ; i++ ){
		CLkRegistration *lk = new CLkRegistration();

		lk->SetRegistrationType(LK_SCALE_ROTATE);

		pt2d_type *pt = &m_apt->a[i];


		int i0 = pt->p.y;
		int j0 = pt->p.x;

		b.y0 =i0 - dd;
		b.y1 =i0 + dd;
		b.x0 = j0 - dd;
		b.x1 = j0 + dd;


		if( lk->SetRefernceImage( m_yim, &b ) < 0 ){
			delete lk;
			//fprintf( stdout, "A" );
			continue;
		}


		m_apt->a[m_nT] = *pt;
		m_at[m_nT++] = lk;
	}

	m_apt->nA = m_nT;




	m_as = vec2fA_alloc( m_nT );
	m_ap = vec2fA_alloc( m_nT );


	PT2DA_DUMP( m_apt, "db", 1, NULL );
	


	return( 1 );
}


int	CDynamicMask::Process( image_type *sim, int iFrame )
{

	int	i,	n;


	if( m_nT == 0 ){
		m_state = 0;
		return( -1 );
	}





	m_iFrame = iFrame;

	m_p.x = 0;
	m_p.y = 0;
	m_angle = 0;
	m_scale = 0;

	image_type *yim = image1_from( sim, NULL );

//	fprintf( stdout, "track: %d\n", m_iFrame );

	for( i = 0, n = 0 ; i < m_nT ; i++ ){
		CLkRegistration *lk = m_at[i];

		pt2d_type *pt = &m_apt->a[i];

		//lk->InitilizeState( pt->p.x, pt->p.y, 0, 1.0 );

		lk->Registration( yim );

		lkState_type	state;
		lk->GetStat( &state );

		vec2f_type p;
		lt2_type lt;
		lk->GetLocallLt( &p, &lt );

		float	a,	b,	angle, scale;
		lt2_similarity_get( &lt, &a, &b, &angle, &scale );
		//fprintf( stdout, "%.2f %.2f %.2f %.2f  %.2f \n", a, b, angle, scale,  state.ssd );

		if( scale < 0.85 || scale > 1.2 )
			continue;

		m_p.x += a;
		m_p.y += b;
		m_angle += angle;
		m_scale += scale;

		m_as->a[n] = pt->p;
		m_ap->a[n].x = pt->p.x + a;
		m_ap->a[n].y = pt->p.y + b;
		n++;

		//vec2f_type	p;
		lk->GetGlobalPoint( &p );


		pt->p = p;

	}


	m_p.x /= n;
	m_p.y /= n;
	m_angle /= n;
	m_scale /= n;
	m_state = 1;

	m_as->nA = m_ap->nA = n;
	lt2_similarity_approximate( &m_lt, m_as, m_ap );


	PT2DA_DUMP( m_apt, "db", iFrame, NULL );


	//MoveIm( yim, iFrame );

	if( yim != sim )
		image_destroy( yim, 1 );


	return( 1 );
	//	image_dump( im, "mask", 1, "mm"  );
}





int	CDynamicMask::ProcessMask( image_type *sim, int iFrame )
{
int	i,	j;
	if( m_state == 0 )
		return( -1 );

	//image1_mask( sim, m_mim, sim );

	IMAGE_DUMP_SCALE( m_mim, 255, "Db", iFrame, "MASK");
	IMAGE_DUMP( sim, "Db", iFrame, "IN");


	int	di = (m_p.y > 0 )? m_p.y + 0.5 : m_p.y - 0.5;
	int dj = (m_p.x > 0 )? m_p.x + 0.5 : m_p.x - 0.5;

	u_char *sp = sim->data;
	u_char *mp = m_mim->data;
	

	for( i = 0 ; i < sim->row ; i++ ){
		if( i + di < 0 || i + di >= sim->height )
			continue;

		for( j = 0 ; j < sim->column ; j++, sp++, mp++ ){

			if( *mp == 0 )	continue;

			if( j + dj < 0 || j + dj >= sim->width )
				continue;

			sp = IMAGE_PIXEL( sim, i+ di, j+dj );

			*sp =  0;
		}
	}

	IMAGE_DUMP( sim, "Db", iFrame, "OUT");


	return( 1 );
}



int	CDynamicMask::ProcessMaskN( image_type *sim, image_type *cim, int iFrame )
{
	int	i,	j;
	if( m_state == 0 )
		return( -1 );

	lt2_type	lt;
	lt2_inverse( &m_lt, &lt );


	image_type *im = image1_interpolation_lt2( m_yim, &lt, 0, 0, sim->width, sim->height, NULL );
	image_type *mim = image1_interpolation_lt2( m_mim, &lt, 0, 0, sim->width, sim->height, NULL );

	IMAGE_DUMP( sim, "Db", iFrame, "SRC" );
	IMAGE_DUMP( im, "Db", iFrame, "LT" );

	image_type *dim = image1_abs_diff( sim, im, NULL );

	image1_threshold( dim, 24 );

	IMAGE_DUMP( dim, "Db", iFrame, "DIF" );


	IMAGE_DUMP( cim, "Db", iFrame, "cim-in" );

	int	algin = cim->width - 8;;
	for( i = 0 ; i < cim->height ; i += 8 ){
		for( j = 0 ; j < cim->width ; j += 8 ){

			u_char *mp = IMAGE_PIXEL( mim, i, j );
			u_char *dp = IMAGE_PIXEL( dim, i, j );

			int	n,	k,	m,	d;
			m = d = 0;
			for( k = 0 ; k < 8 ; k++, mp += algin , dp += algin ){
				for( n = 0 ; n < 8 ; n++, mp++, dp++ ){
					if( *mp != 0 )	m++;
					if( *dp == 0 )	d++;

				}
			}

			if( m < 48 )	continue;
			if( d < 48 )	continue;

			u_char *tp = IMAGE_PIXEL( cim, i, j );

			for( k = 0 ; k < 8 ; k++, tp += algin){
				for( n = 0 ; n < 8 ; n++, tp++ ){
					*tp = 0;
				}
			}

		}
	}




	IMAGE_DUMP( cim, "Db", iFrame, "cim-out" );

	image_destroy( im, 1 );
	image_destroy( mim, 1 );
	image_destroy( dim, 1 );

	return( 1 );
}


int	CDynamicMask::MoveIm( image_type *sim, int iFrame )
{

	if( m_state == 0 )
		return( -1 );




	lt2_type lt;

	lt.a0 = 1.0;
	lt.b0 = 0;
	lt.c0 = m_p.x;

	lt.a1 = 0;
	lt.b1 = 1.0;
	lt.c1 = m_p.y;

	lt = m_lt;


	lt2_inverse( &m_lt, &lt );


	image_type *im = image1_interpolation_lt2( m_yim, &lt, 0, 0, sim->width, sim->height, NULL );

	IMAGE_DUMP( sim, "Db", iFrame, "SRC" );
	IMAGE_DUMP( im, "Db", iFrame, "LT" );

	image_destroy( im, 1 );


	return( 1 );
}

