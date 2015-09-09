
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
#define _DUMP 
#endif

#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"Ulog/Log.h"
#include	"Ubox/Box1i.h"

#include	"Umath/EigenType.h"




#include "Uln/PlnType.h"

#include "../PlnHeadTracker/HeadPose.h"

#include "PlnHeadDetect.h"


int	pln_head_detcet( pln_type *pl, plnHeadDetectPrm_type *prm, pt2dA_type *hapt );


int	p2dA_ny_seg( pt2dA_type *apt, int h, box1i_type ab[], int *nB );

static int	pln_head_detect( pt2dA_type *apt, box1i_type ab[], int cyclic, vec2f_type *p, float *scale, float *ds );



CPlnHeadDetect::CPlnHeadDetect()
{
	m_cln = NULL;

	m_prm = NULL;

	m_ah = NULL;

}


CPlnHeadDetect::~CPlnHeadDetect()
{
	DeleteContents();

}



void CPlnHeadDetect::DeleteContents()
{

	if( m_cln != NULL ){
		cln_destroy( m_cln );
		m_cln = NULL;
	}


}


int CPlnHeadDetect::Init( cln_type *cln, char *prmFile )
{
	DeleteContents();

	m_prm = plnHeadDetectPrm_alloc();

	if( prmFile != NULL && prmFile[0] != 0 )
		plnHeadDetectPrm_read( m_prm, prmFile );



	m_cln = cln;

	lt2_similarity_set( &m_lt, m_cln->ctr.x, m_cln->ctr.y, 0.0F, 1.0F );

	pln_translate( m_cln->a[0], -m_cln->ctr.x, -cln->ctr.y );


	pln_box( m_cln->a[0], &m_box );

	m_r = 0.5*(m_box.y1 - m_box.y0);

	m_p0.x = m_box.x0;
	m_p0.y = 0.5*(m_box.y1 + m_box.y0);

	m_size = 7.5;



	// compute template size
	pt2dA_type	*hapt = pt2dA_alloc( 100 );

	if( pln_head_detcet( m_cln->a[0], m_prm, hapt ) < 1 )
		return( -1 );

	m_size = hapt->a[0].r;

	pt2dA_destroy( hapt );



	return( 1 );
}



int CPlnHeadDetect::Process( plnA_type *apl )
{
	int	i,	n;

	pt2dA_type	*hapt = pt2dA_alloc( 256 );
	
	//n = 0;
	//n += pln_head_detcet( m_cln->a[0], m_prm, hapt );

	for( i = 0, n = 0 ; i < apl->nA ; i++ ){
		pln_type *bpl = apl->a[i];
		if( bpl == NULL )	continue;

		n += pln_head_detcet( bpl, m_prm, hapt );
	}


	PT2DA_DUMP( hapt, "AA", 2, "ALL" );


	m_ah = headPoseA_realloc( m_ah, hapt->nA );
	m_ah->nA = 0;

	for( i = 0 ; i < hapt->nA ; i++ ){
		pt2d_type *pt = &hapt->a[i];

		vec2f_type p;

		headPose_type *h = headPose_alloc();

		float scale = pt->r / m_size;


		p.x = pt->p.y;
		p.y = pt->p.x;
		headPose_set( h, i, 1, p.x - m_p0.x*scale, p.y - m_p0.y*scale, scale, 0 );

		SetFittingMeasure( apl, h );

		if( h->qulity < 0.5 && pt->f > 5 ){
			headPose_type h1 = *h;
			scale = (pt->r + pt->f) / m_size;
			headPose_set( &h1, i, 1, p.x - m_p0.x*scale, p.y - m_p0.y*scale, scale, 0 );

			SetFittingMeasure( apl, &h1 );
			if( h1.qulity > h->qulity )
				*h = h1;

		}

		m_ah->a[m_ah->nA++] = h;
	}


	HEADPOSEA_DUMP_PL( m_ah, m_cln->a[0], "AA", 1, NULL );


	Filter( m_ah );

	HEADPOSEA_DUMP_PL( m_ah, m_cln->a[0], "AA", 1, "FILTER" );

	return( 1 );
}


int CPlnHeadDetect::Filter( headPoseA_type *ah )
{
int	i,	j;

	for( i = 0, j = 0 ; i < ah->nA ; i++ ){
		headPose_type *h = ah->a[i];

		if( h->scale < m_prm->scale || h->qulity < m_prm->qulity ){
			headPose_destroy( h );
			continue;
		}

		h->iFrame = j;
		ah->a[j++] = h;
	}

	ah->nA = j;

	return( 1 );
}



int CPlnHeadDetect::Get( headPose_type *h0, headPose_type *h )
{
	int	i;

	float	tMax = 0;

	int	i0 = -1;
	
	for( i = 0 ;  i < m_ah->nA ; i++ ){
		headPose_type *h = m_ah->a[i];

		if( h0 == NULL ){
			if( i0 <= 0 || h->qulity > m_ah->a[i0]->qulity )
				i0 = i;
			continue;
		}

		vec2f_type	p;
		p.x = h->p.x - h0->p.x;
		p.y = h->p.y - h0->p.y;
		float t = p.x*p.x + p.y*p.y;

		if( i0 < 0 || t < tMax ){
			tMax = t;
			i0 = i;
		}
	}

	if( i0 < 0 )
		return( -1 );

	*h = *m_ah->a[i0];

	return( 1 );
}


int CPlnHeadDetect::SetFittingMeasure( plnA_type *apl, headPose_type *h )
{
	float gt0 = 0;
	float gt1 =  m_cln->a[0]->len;
	float T = 4;

	pln_type *bpl = GetPln( h );

	PLN_DUMP( bpl, "AA",1, "Measure" );

	T = m_prm->du * h->scale;
	T = PUSH_TO_RANGE( T, 2, 8 );

	//float dg = 0.25 * bpl->len;
	//gt0 = dg;
	//gt1 = bpl->len - dg;


	lnFit_type f;
	lt2_similarity_set( &f.lt, h->p.x, h->p.y, h->angle, h->scale );
	plnA_fitT( apl, m_cln->a[0], gt0, gt1, 6, 4, &f );
	lt2_similarity_get( &f.lt, &h->p.x, &h->p.y, &h->angle, &h->scale );


	bpl = GetPln( h );

	PLN_DUMP( bpl, "AA",1, "Measure-2" );

	float	cover0,	dis0,	cover1,	dis1;
	plnA_fit_compareW( apl, bpl, gt0*h->scale, 0.5*gt1*h->scale, T,  m_prm->du, &cover0, &dis0 );
	plnA_fit_compareW( apl, bpl, 0.5*gt0*h->scale, gt1*h->scale, T,  m_prm->du, &cover1, &dis1 );


	h->qulity = MIN( cover0, cover1);

	pln_destroy( bpl );

	return( 1 );
}



pln_type *	CPlnHeadDetect::GetPln( headPose_type *h )
{
	pln_type *pl;
	lt2_type	lt;

	lt2_similarity_set( &lt, h->p.x, h->p.y, h->angle, h->scale );

	pl =  pln_affine_lt( m_cln->a[0], &lt, NULL );

	return( pl );
}




int	CPlnHeadDetect::Write( char *outFile )
{
char file[256];
int	i;

	gpFilename_force_extension(outFile, ".txt", file );
	headPoseA_write( m_ah, file );



	plnA_type *apl = plnA_alloc( m_ah->nA );
	for( i = 0 ; i < m_ah->nA ; i++ ){
		apl->a[apl->nA++] = GetPln( m_ah->a[i] );
	}


	gpFilename_force_extension(outFile, ".pl", file );
	plnA_write( apl, file );


	return( 1 );
}

void CPlnHeadDetect::Trace( FILE *fp )
{
	fprintf( stdout, "Head: %d\n", m_ah->nA );

}

int
pln_head_detcet( pln_type *pl, plnHeadDetectPrm_type *prm, pt2dA_type *hapt )
{
int	i;

	pt2dA_type *apt = pln_sampleP( pl, 0, pl->len, 1.0, NULL );

	int	cyclic = ( pl->state & PLN_CLOSE )? 1 : 0;

	int h = 5;
	//for( i = h ; i < apt->nA-h ; i++ ){
	for( i = 0 ; i < apt->nA ; i++ ){

		pt2d_type *pt = &apt->a[i];
		pt->r = 0;
//		if( i < h || i > apt->nA + h )
//			continue;

		eigen2d_type e;
		pt2A_eigen(apt, i, h, cyclic, &e );


		pt->n = e.v1;
		pt->r = e.e1;
		pt->f = e.e2;
	}

	apt->type = PT2D_4V;
	PT2DA_DUMP( apt, "AA", 1, "EIGEN" );



	box1i_type	ab[256];
	int nB;
	p2dA_ny_seg( apt, h, ab, &nB );


//	box1i_print(  "AA", ab, nB, stdout );


	for( i = 0 ; i < nB -1 ; i++ ){
		vec2f_type	p;
		float scale;
		float ds;

		if( pln_head_detect( apt, &ab[i], cyclic, &p, &scale, &ds ) < 0 )
			continue;

		pt2d_type *pt = &hapt->a[hapt->nA++];

		pt->p.x = p.x;
		pt->p.y = p.y;
		pt->r = scale;
		pt->f = ds;
	}



	pt2dA_destroy( apt );

	return( 1 );
}


static int
pln_head_detect( pt2dA_type *apt, box1i_type ab[], int cyclic, vec2f_type *p, float *scale, float *ds )
{
int	i;
	int	k,	K;

	if( ab[0].n == 1 && ab[1].n == 1 )
		return( -1 );

	int i0 = ab[0].i1;
	int i1 = ab[1].i0;

	float	dT = 10;

	p->x = 0.5*(apt->a[i0].p.x + apt->a[i1].p.x );
	p->y = 0.5*(apt->a[i0].p.y + apt->a[i1].p.y );

	float mx1, my1,	mx0,	my0,	dx,	dy,	d1,	d0,	d;


	K = ( cyclic == 1)? 0.5*apt->nA : apt->nA - i1;

	int	j1,	j0;
	//for( i = i1+1, j1 = i1, my1 = mx1 = d1 = 0 ; i < apt->nA ; i++ ){
	for( i = i1+1, j1 = i1, my1 = mx1 = d1 = 0, k = 0 ; k < K ; k++, i++ ){

		if( i >= apt->nA )	i = 0;

		dy = apt->a[i].p.y - p->y;
		dx = apt->a[i].p.x - p->x;
		if( dx < 0 )	dx = -dx;

		if( dx < 2.0 )	continue;

		d = dy / dx;
		if( d > d1 ){
			j1 = i;
			d1 = d;
			my1 = dy;
			mx1 = dx;
		}


		if( d > 0.55 )
			break;
	}



	K = ( cyclic == 1)? 0.5*apt->nA : i0;
	///for( i = i0-1, j0 = i0, d0 = my0 = mx0 = 0, k = 0 ; i > 0 ; i-- ){
	for( i = i0-1, j0 = i0, d0 = my0 = mx0 = 0, k = 0 ; k < K  ; i--, k++ ){
		if( i < 0 )	i = apt->nA-1;

		dy = apt->a[i].p.y - p->y;
		dx = -(apt->a[i].p.x - p->x);
		if( dx < 0 )	dx = -dx;

		if( dx < 2.0 )	continue;


		d = dy/dx;
		if( d > d0 ){
			j0 = i;
			d0 = d;
			my0 = dy;
			mx0 = dx;
		}

		if( d > 0.55 )
			break;
	}

	//if( j1 - i1 < 0.75*dT && i0 - j0  < 0.5*dT )
	//	return( -1 );


	if( d0 < 0.5 || d1 < 0.5 )
		return( -1 );

	float s0 = d0*my0;
	float s1 = d1*my1;

//	*scale = (d0*my0  + d1*my1)*0.5;
	*scale = (s0  + s1)*0.5;

	*ds = s1 - *scale;
	if( *ds < 0 )	*ds = - *ds;

	return( 1 );
}



int
p2dA_ny_seg( pt2dA_type *apt, int h, box1i_type ab[], int *nB ) 
{
int	i;

	ab[0].i0 = ab[0].i1 = -1;
	int	s = -1;

	*nB = 0;
	//for( i = h ; i < apt->nA-h ; i++ ){
	for( i = 0 ; i < apt->nA ; i++ ){
		pt2d_type *pt = &apt->a[i];

		int s0 = pt->n.y < 0 ;

		if( s < 0 ){
			s = s0;
			ab[*nB].i0 = ab[*nB].i1 = i;
			ab[*nB].n = 1;
		}

		if( s == s0 ){
			ab[*nB].i1 = i;
			ab[*nB].n += 1;
			continue;
		}


		(*nB)++;
		s = s0;
		ab[*nB].i0 = ab[*nB].i1 = i;
		ab[*nB].n = 1;
	}

	(*nB)++;

	return( *nB );

}








		
		


