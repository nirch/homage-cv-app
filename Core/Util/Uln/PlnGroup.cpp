/*************************
 ***   Pt2dGroup.cpp   ***
 *************************/
#include	<math.h>
#include	<stdlib.h>

#include "Uigp/igp.h"



#include "Uln/PlnType.h"

void	pt2dA_group_set_id( pt2dA_type *apt, int no, int cgroup, int group );

void	plnA_group_set_id( plnA_type *apl, int no, int cgroup, int group );

void	plnA_group_set( plnA_type *apl, int group );



		
void
plnA_group_parallel( plnA_type *apl, float dt, float aT, float D0, float D1 )
{
int	i,	j;
int	nGroup;
pln_type	*pl,	*cpl;
float	u;

	apl->nGroup = 0;

	nGroup = 0;
	apl->a[0]->group = nGroup++;
	apl->nGroup = 1;


	for( i = 1; i < apl->nA; i++ ){
		pl = apl->a[i];
		pl->group = -1;

		for( j = 0 ; j < i ; j++ ){
			cpl = apl->a[j];
			
			if( cpl->group == pl->group )
				continue;

			if( pln_parallel_distance(  pl, cpl, dt, aT, D0, D1, &u ) <= 0 )
				continue;


			if( pl->group == -1 ){
				pl->group = cpl->group;
				continue;
			}

			if( pl->group < cpl->group )
				plnA_group_set_id( apl, i+1, cpl->group, pl->group );
			else
				plnA_group_set_id( apl, i+1, pl->group, cpl->group );

			apl->nGroup--;
		}

		if( pl->group == -1 ){
			pl->group = nGroup++;
			apl->nGroup++;
		}
	}

	apl->mGroup = nGroup;
}



void
plnA_group_set_id( plnA_type *apl, int no, int cgroup, int group )
{
int	i;
pln_type	*pl;

	for( i = 0; i < no ; i++ ){
		pl = apl->a[i];
		if( pl->group == cgroup )
			pl->group = group;
	}
}

void
plnA_group_set( plnA_type *apl, int group )
{
	int	i;
	pln_type	*pl;

	for( i = 0; i < apl->nA ; i++ ){
		pl = apl->a[i];

		pl->group = group;
	}
}




int
plnA_group_max( plnA_type *apl, int *iG, int *no )
{
	pln_type	*pl;
	int		*ag,	i,	iMax;

	ag = (int *)malloc( apl->mGroup *sizeof(int) );

	for( i = 0 ; i < apl->mGroup ; i++ )
		ag[i] = 0;

	for( i = 0 ; i < apl->nA ; i++ ){
		pl = apl->a[i];
		ag[ pl->group]++;
	}

	iMax = 0;
	for( i = 1 ;i < apl->mGroup ; i++ ){
		if( ag[i] > ag[iMax] ){
			iMax = i;
		}
	}

	*no = ag[iMax];
	*iG = iMax;

	free( ag );

	return( iMax );
}




pt2dGroupA_type *
pt2dGroupA_set_pln( pt2dGroupA_type *ag, plnA_type *apl )
{
	pt2dGroup_type	**aPg, *pg;	
	int	i;


	if( ag == NULL )
		ag = pt2dGroupA_alloc( apl->nGroup );


	aPg = (pt2dGroup_type **)malloc( (apl->nA+1)*sizeof(pt2dGroup_type *) );
	for( i = 0; i < apl->nA ; i++ ){
		aPg[i] = NULL;
	}


	ag->nG = 0;
	for( i = 0; i < apl->nA; i++ ){
		pln_type	*pt = apl->a[i];

		pg = aPg[pt->group];
		if( pg == NULL ){
			pg = aPg[pt->group] = &ag->g[ag->nG++];
			pg->group = pt->group;
			pg->no = 1;
			continue;
		}

		pg->no++;
	}



	free( aPg );

	return( ag );
}



#ifdef _AA_

int
pt2dGroupA_max( pt2dGroupA_type *ag )
{
	int	i,	imax,	no;

	imax = 0;
	no = ag->g[0].no;
	for( i = 0 ; i < ag->nG ; i++ ){
		if( no < ag->g[i].no ){
			no = ag->g[i].no;
			imax = i;
		}
	}

	return( imax );
}		




pt2dGroupA_type *
pt2dGroupA_alloc( int no )
{
pt2dGroupA_type	*apt;

	apt = (pt2dGroupA_type *)malloc( sizeof( pt2dGroupA_type) );

	apl->nG = 0;
	apl->g = ( pt2dGroup_type *)malloc( no*sizeof(pt2dGroup_type) );


	return( apt );
}


void
pt2dGroupA_destroy( pt2dGroupA_type *ag )
{
	free( ag->g );
	free( ag );
}



pt2dGroupA_type *
pt2dGroupA_set( pt2dGroupA_type *ag, pt2dA_type *apt )
{
pt2dGroup_type	**aPg, *pg;	
int	i;


	if( ag == NULL )
		ag = pt2dGroupA_alloc( apl->nGroup );

	
	aPg = (pt2dGroup_type **)malloc( (apl->nP+1)*sizeof(pt2dGroup_type *) );
	for( i = 0; i < apl->nP ; i++ ){
		aPg[i] = NULL;
	}


	ag->nG = 0;
	for( i = 0; i < apl->nP; i++ ){
		pln_type	*pt = &apl->a[i];

		pg = aPg[pt->group];
		if( pg == NULL ){
			pg = aPg[pt->group] = &ag->g[ag->nG++];
			pg->group = pt->group;
			pg->no = 1;
			pg->p = pt->p;
			continue;
		}

		pg->no++;
		pg->p.x += pt->p.x;
		pg->p.y += pt->p.y;
	}


	for( i = 0 ; i < ag->nG ; i++ ){
		ag->g[i].p.x /= ag->g[i].no;
		ag->g[i].p.y /= ag->g[i].no;
	}

	free( aPg );

	return( ag );
}




int
pt2dGroupA_filter( pt2dGroupA_type *ag, int noT )
{	
int	i,	j;


	for( i = 0,	j =0 ; i < ag->nG ; i++ ){

		if( ag->g[i].no < noT )
			continue;

		ag->g[j++] = ag->g[i];
	}

	ag->nG = j;

	return( ag->nG );
}
#endif