
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<string.h>


#ifdef _DEBUG
//#define _DUMP 
#endif

#include	"Uigp/igp.h"
#include	"Utime/GpTime.h"
#include	"Ulog/Log.h"

#include "Uln/PlnType.h"
#include "Uln//CrPlnType.h"

#include "PlnMerging.h"



//int	plnA_merging( plnA_type *apl, pln_type *bpl );

int	pln_merge_end_0(  pln_type *bpl,  plnA_type *apl, crPln_type *cr );

int	pln_merge_end1_0(  pln_type *bpl,  plnA_type *apl, crPln_type *cr );



void	crPlnA_inverse( crPlnA_type *cr, pln_type *pl, int iPl );

void	crPln_inverse( crPln_type *cr, pln_type *pl = NULL );




int	pln_merge_cr(  pln_type *bpl,  plnA_type *apl, crPln_type *cr, plnMergingPrm_type *prm, float *dlen );

int	pln_merge_cr2(  pln_type *bpl,  plnA_type *apl, crPlnA_type *ac );


plnA_type *
plnA_merging( plnA_type *apl, plnA_type *tapl, plnMergingPrm_type *prm )
{
	int i;

	if( tapl == NULL )
		return( apl );


	int n = tapl->nA + apl->nA;

	for( i = 0 ; i < apl->nA ; i++ )
		plnA_merging( tapl, apl->a[i], prm );


	plnA_decrease( tapl );

	apl->nA = 0;
	plnA_destroy( apl );


	return( tapl );
}

int
plnA_merging( plnA_type *apl, pln_type *bpl, plnMergingPrm_type *prm ) //float dU )
{
crPlnA_type *ac;

float	dR1,	dR2;
int	n,	i;

	dR1 = dR2 = prm->dU;//4.0;

	PLNA_DUMP( apl, "PLN", 1, "BASE" );
	PLN_DUMP( bpl, "PLN", 1, "M" );

	ac = crPlnA_alloc( 64 );
	crPln_createA( bpl, apl, -1, dR1, dR2, 0,  ac );




	if( ac->nA == 0 ){
		plnA_add( apl, bpl );
		crPlnA_destroy( ac );
		PLNA_DUMP( apl, "PLN", 1, "OUT" );
		return( 0 );
	}



	crPlnA_union( ac->a, &ac->nA );

	if( ac->nA == 1 ){
		crPln_type *cr = &ac->a[0];
		if( apl->a[cr->iPl]->state == PLN_CLOSE ){
			pln_destroy( bpl );
			return( 1 );
		}
	}


	for( i = 0 ; i < ac->nA ; i++ ){
		crPln_type *cr = &ac->a[i];

		if( cr->d[1].gt < cr->d[0].gt )
			crPlnA_inverse( ac, apl->a[cr->iPl], cr->iPl );

//			PLNA_DUMP( apl, "PLN", 1, "IN" );

			//crPln_inverse( apl->a[cr->iPl], cr );
	}


	crPlnA_direction( ac );


	crPlnA_order( ac );

//



	n = 0;

	int	j;
	for( i = 0, j = 0 ; i < ac->nA ; i++ ){
		crPln_type *cr = &ac->a[i];

		pln_type *pl = apl->a[cr->iPl];


		if( pl->len < bpl->len ){
			if( cr->d[0].sgt < prm->dT && cr->d[1].sgt > pl->len - prm->dT ){
				pln_destroy( apl->a[cr->iPl] );
				apl->a[cr->iPl] = NULL;
				n++;
				continue;
			}
		}

		ac->a[j++] = ac->a[i];
	}

	ac->nA = j;


	if( ac->nA == 0 ){
		plnA_add( apl, bpl );
		crPlnA_destroy( ac );
		return( n );
	}



	for( i = 0 ; i < ac->nA ; i++ ){
		crPln_type *cr = &ac->a[i];

		pln_type *pl = apl->a[cr->iPl];


		if( bpl->len < pl->len ){		
			if( cr->gt0 < prm->dT && cr->gt1 > bpl->len - prm->dT ){
				pln_destroy( bpl );
				crPlnA_destroy( ac );
				n++;
				return( n );
			}
		}
	}



	if( ac->nA == 2 ){
		if( pln_merge_cr2(  bpl,  apl, ac ) > 0 ){
			crPlnA_destroy( ac );
			return( 1 );
		}
		//return( 1 );
	}



	float dlen = 0;
	for( i = ac->nA -1, n = 0 ; i >=  0 ; i-- ){
		crPln_type *cr = &ac->a[i];

		cr->gt0 += dlen;
		cr->gt1 += dlen;
		cr->d[0].gt += dlen;
		cr->d[1].gt += dlen;

		n += pln_merge_cr(  bpl,  apl, cr, prm, &dlen );

		if( n == 1 )
			break;
	

	}

	if( n == 0 )
		plnA_add( apl, bpl );





	crPlnA_destroy( ac );




	

	return( n );
}




int
pln_merge_cr2(  pln_type *bpl,  plnA_type *apl, crPlnA_type *ac )
{
	if( ac->nA != 2 )
		return( -1 );

	if( ac->a[0].iPl != ac->a[1].iPl )
		return( -1 );


	crPln_type *cr0 = &ac->a[0];

	crPln_type *cr1 = &ac->a[1];

	
	pln_type *pl = apl->a[cr0->iPl];

	if( pl->len > bpl->len ){

		if( bpl->len - cr1->gt1 < 10 ){
			pln_trim( bpl, F_END, bpl->len - cr1->gt0 +2 );
		}
		else {
			pln_type *spl = pln_split( bpl, cr1->gt1, 1.0 );
			plnA_add( apl, spl );
		}



		pln_type *spl = pln_split( bpl, cr0->gt1, 1.0 );
		plnA_add( apl, spl );


		if( cr0->gt0 < 10 ){
			pln_destroy( bpl );
		}
		else {
			pln_trim( bpl, F_END, bpl->len - cr0->gt0 - 2 );

			plnA_add( apl, bpl );	
		}


	}

	return( 1 );
}




int
pln_merge_cr(  pln_type *bpl0,  plnA_type *apl, crPln_type *cr, plnMergingPrm_type *prm, float *dlen )
{
pln_type	*spl;

pln_type *bpl;

	bpl = bpl0;
	pln_type *pl = apl->a[cr->iPl];
	if( pl == NULL )
		return( 0 );


	// end
	if( pl->len - cr->d[1].sgt <   bpl->len - cr->gt1 ){
		if( pl->len - cr->d[1].sgt > prm->dT ){
			spl = pln_split( pl, cr->d[1].sgt, 1 );

			plnA_add( apl, spl );
		}

	}
	else {
		if( bpl->len - cr->gt1 > prm->dT ){
			spl = pln_split( bpl,cr->gt1, 1.0 );
			plnA_add( apl, spl );
		}
	}



	// start
	if( cr->d[0].sgt < cr->gt0 ){
		if( cr->d[0].sgt > prm->dT ){

			spl = pln_split( pl, cr->d[0].sgt, 1 );
			int iPl = plnA_add( apl, spl );
			cr->iPl = iPl;
			cr->pl = spl;
			cr->d[0].sgt -= pl->len;
			cr->d[1].sgt -= pl->len;
			pl = spl;
		}
	}
	else	{
		if( cr->gt0 > prm->dT ){
			float len = bpl->len;
			pln_trim( bpl, F_BEGIN, cr->gt0 );

			len -= bpl->len;
			cr->d[0].gt -= len;
			cr->d[1].gt -= len;
			cr->gt0 -= len;
			cr->gt1 -= len;
		}
	}



	// bpl in 
	if( cr->gt0 < prm->dT && bpl->len - cr->gt1 < prm->dT ){
		
		//Yoram
		//if( bpl == bpl0 )	return( 0 );
		pln_destroy( bpl );
		return ( 1 );
	}


	// pl in
	if( cr->d[0].sgt < prm->dT && pl->len - cr->d[1].sgt < prm->dT ){
		plnA_destroy_pl( apl, cr->iPl );
		return( 0 );
	}


	if( cr->d[0].sgt > prm->dT ){		// cr->gt0 == 0 

		//if( cr->gt1 >  bpl->len - 10 ){
		//	if( bpl != bpl0 )
		//		return( 0 );
		//	pln_destroy( bpl );
		//	return ( 1 );
		//}

		if( bpl->len > pl->len ){
			pln_trim( pl, F_END, pl->len - cr->d[0].sgt + 2 );
			pln_trim( bpl, F_BEGIN, cr->gt0 + 2 );

			pln_append( pl, bpl );
			PLN_DUMP( pl, "AA", 2, NULL );
			return( (bpl != bpl0)? 0 :1  );
		}

		pln_trim( bpl, F_BEGIN, cr->gt1 + 2 );
		pln_append( pl, bpl );
		return( (bpl != bpl0)? 0 :1  );
	}


	//if( cr->d[1].sgt > pl->len - 10 ){
	//	plnA_destroy_pl( apl, cr->iPl );
	//	return( 0 );
	//}

	if( pl->len > bpl->len ){
		pln_trim( bpl, F_END, bpl->len - cr->gt0 + 4 );
		if( bpl->len < 2 ){
			pln_destroy( bpl );
			return( 1 );
		}

		pln_trim( pl, F_BEGIN, cr->d[0].sgt + 2 );

		pln_append( bpl, pl );
		apl->a[cr->iPl] = NULL;

		PLN_DUMP( bpl, "AA", 1, NULL );
		return( 0 );
	}
	else {
		pln_trim( pl, F_BEGIN, cr->d[1].sgt + 2 );
		
		pln_trim( bpl, F_END, bpl->len -cr->gt1 + 2 );

		pln_append( bpl, pl );
		PLN_DUMP( bpl, "AA", 1, NULL );
		apl->a[cr->iPl] = NULL;
		return( 0 );
	}

}


void
crPlnA_inverse( crPlnA_type *ac, pln_type *pl, int iPl )
{
int	i;

	for( i = 0 ; i < ac->nA ; i++ ){
		if( ac->a[i].iPl != iPl )	continue;

		crPln_inverse( &ac->a[i], pl );
	}

	pln_inverse( pl );
}


void
crPln_inverse( crPln_type *cr, pln_type *pl )
{
float tmp;

	tmp = cr->d[0].sgt;
	cr->d[0].sgt = pl->len - cr->d[1].sgt;
	cr->d[1].sgt = pl->len - tmp;

	tmp = cr->d[0].gt;
	cr->d[0].gt = cr->d[1].gt;
	cr->d[1].gt = tmp;

//	if( pl != NULL )
//		pln_inverse( pl );
}
