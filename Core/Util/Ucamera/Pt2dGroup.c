/*************************
 ***   Pt2dGroup.cpp   ***
 *************************/
#include	<math.h>
#include	<stdlib.h>

#include "Uigp/igp.h"
#include "Ucamera/Pt2dType.h"
#include "Pt2dGroupType.h"




void
pt2dA_groupM( pt2dA_type *apt, float dis )
{
int	i,	j,	k;
int64_t	mask,	mGroup;
float	x,	y;
float	dis2;
int	nGroup;
	dis2 = dis*dis;

	nGroup = 0;
	mGroup = 0;
	apt->p[0].group = nGroup++;
	for( i = 1; i < apt->nP; i++ ){
		pt2d_type	*pt = &apt->p[i];
		pt->group = apt->nP;

		mask = 0;
		for( j = i-1 ; j >= 0 ; j-- ){
			pt2d_type	*cpt = &apt->p[j];
			if( mask & ( ((int64_t)1)<< cpt->group)  )	continue;

			x = pt->p.x - cpt->p.x;
			y = pt->p.y - cpt->p.y;

			if( x*x + y*y > dis2 )	continue;

			mask |= ( ((int64_t)1)<< cpt->group);
			if( cpt->group < pt->group )	pt->group = cpt->group;
		}

		if( mask == 0 ){

			for( k = 0 ; k < nGroup ; k++ )
				if( (mGroup & ( ((int64_t)1) <<k)) == 0 )
					break;
			pt->group = k;
			if( k >= nGroup )	nGroup++;
			mGroup |= ( ((int64_t)1)<<pt->group);
			continue;
		}


		mGroup = ( ((int64_t)1)<< pt->group);
		for( j = 0 ; j < i ; j++ ){
			pt2d_type	*cpt = &apt->p[j];
			if( mask & ( ((int64_t)1)<< cpt->group)  )
				cpt->group = pt->group;

			mGroup |= ( ((int64_t)1)<< cpt->group);
		}
	}
}



// the some as pt2dA_group - use the radius 
void
pt2dA_groupMM( pt2dA_type *apt, float dis )
{
int	i,	j,	k;
int64_t	mask,	mGroup;
float	x,	y;
float	td;
int	nGroup;


	nGroup = 0;
	mGroup = 0;
	apt->p[0].group = nGroup++;
	for( i = 1; i < apt->nP; i++ ){
		pt2d_type	*pt = &apt->p[i];
		pt->group = apt->nP;

		mask = 0;
		for( j = i-1 ; j >= 0 ; j-- ){
			pt2d_type	*cpt = &apt->p[j];
			if( mask & ( ((int64_t)1)<< cpt->group)  )	continue;


			td = pt->r + cpt->r + dis;
			

			x = pt->p.x - cpt->p.x;
			y = pt->p.y - cpt->p.y;

			if( x*x + y*y > td*td )	continue;

			mask |= ( ((int64_t)1)<< cpt->group);
			if( cpt->group < pt->group )	pt->group = cpt->group;
		}

		if( mask == 0 ){

			for( k = 0 ; k < nGroup ; k++ )
				if( (mGroup & ( ((int64_t)1) <<k)) == 0 )
					break;
			pt->group = k;
			if( k >= nGroup )	nGroup++;
			mGroup |= ( ((int64_t)1)<<pt->group);
			continue;
		}


		mGroup = ( ((int64_t)1)<< pt->group);
		for( j = 0 ; j < i ; j++ ){
			pt2d_type	*cpt = &apt->p[j];
			if( mask & ( ((int64_t)1)<< cpt->group)  )
				cpt->group = pt->group;

			mGroup |= ( ((int64_t)1)<< cpt->group);
		}
	}
}
		
void
pt2dA_group( pt2dA_type *apt, float dis )
{
int	i,	j;
float	x,	y;
float	td;
int	nGroup;
pt2d_type	*pt,	*cpt;

	apt->nGroup = 0;

	nGroup = 0;
	apt->p[0].group = nGroup++;
	apt->nGroup = 1;


	for( i = 1; i < apt->nP; i++ ){
		pt = &apt->p[i];
		pt->group = -1;

		for( j = 0 ; j < i ; j++ ){
			cpt = &apt->p[j];
			
			if( cpt->group == pt->group )
				continue;

//			td = pt->r + cpt->r + dis;
			td = dis* dis;

			x = pt->p.x - cpt->p.x;
			y = pt->p.y - cpt->p.y;

			if( x*x + y*y > td*td )	continue;

			if( pt->group == -1 ){
				pt->group = cpt->group;
				continue;
			}

			if( pt->group < cpt->group )
				pt2dA_group_set_id( apt, i+1, cpt->group, pt->group );
			else
				pt2dA_group_set_id( apt, i+1, pt->group, cpt->group );

			apt->nGroup--;
		}

		if( pt->group == -1 ){
			pt->group = nGroup++;
			apt->nGroup++;
		}
	}

	apt->mGroup = nGroup;

}

void
pt2dA_group_radius( pt2dA_type *apt, float dis )
{
	int	i,	j;
	float	x,	y;
	float	td;
	int	nGroup;
	pt2d_type	*pt,	*cpt;

	apt->nGroup = 0;

	nGroup = 0;
	apt->p[0].group = nGroup++;
	apt->nGroup = 1;


	for( i = 1; i < apt->nP; i++ ){
		pt = &apt->p[i];
		pt->group = -1;

		for( j = 0 ; j < i ; j++ ){
			cpt = &apt->p[j];

			if( cpt->group == pt->group )
				continue;

			td = pt->r + cpt->r + dis;
			//td = dis* dis;

			x = pt->p.x - cpt->p.x;
			y = pt->p.y - cpt->p.y;

			if( x*x + y*y > td*td )	continue;

			if( pt->group == -1 ){
				pt->group = cpt->group;
				continue;
			}

			if( pt->group < cpt->group )
				pt2dA_group_set_id( apt, i+1, cpt->group, pt->group );
			else
				pt2dA_group_set_id( apt, i+1, pt->group, cpt->group );

			apt->nGroup--;
		}

		if( pt->group == -1 ){
			pt->group = nGroup++;
			apt->nGroup++;
		}
	}

	apt->mGroup = nGroup;

}

void
pt2dA_group_n( pt2dA_type *apt, float angle )
{
int	i,	j;
float	T,	t;
int	nGroup;
pt2d_type	*pt,	*cpt;

	T = cos( ANGLE_D2R(angle) );

	apt->nGroup = 0;

	nGroup = 0;
	apt->p[0].group = nGroup++;
	apt->nGroup = 1;


	for( i = 1; i < apt->nP; i++ ){
		pt = &apt->p[i];
		pt->group = -1;

		for( j = 0 ; j < i ; j++ ){
			cpt = &apt->p[j];

			if( cpt->group == pt->group )
				continue;

			t = cpt->n.x * pt->n.x + cpt->n.y * pt->n.y;
			if( t < 0 )	t = -t;

			if( t < T )	continue;
			if( pt->group == -1 ){
				pt->group = cpt->group;
				continue;
			}

			if( pt->group < cpt->group )
				pt2dA_group_set_id( apt, i+1, cpt->group, pt->group );
			else
				pt2dA_group_set_id( apt, i+1, pt->group, cpt->group );

			apt->nGroup--;
		}

		if( pt->group == -1 ){
			pt->group = nGroup++;
			apt->nGroup++;
		}
	}

	apt->mGroup = nGroup;

}

void
pt2dA_group_set_id( pt2dA_type *apt, int no, int cgroup, int group )
{
int	i;
pt2d_type	*pt;

	for( i = 0; i < no ; i++ ){
		pt = &apt->p[i];
		if( pt->group == cgroup )
			pt->group = group;
	}
}

int
pt2dA_group_max( pt2dA_type *apt, int *iG, int *no )
{
pt2d_type	*pt;
int		*ag,	i,	iMax;

	ag = (int *)malloc( apt->mGroup *sizeof(int) );

	for( i = 0 ; i < apt->mGroup ; i++ )
		ag[i] = 0;

	for( i = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];
		ag[ pt->group]++;
	}

	iMax = 0;
	for( i = 1 ;i < apt->mGroup ; i++ ){
		if( ag[i] > ag[iMax] ){
			iMax = i;
		}
	}
	
	*no = ag[iMax];
	*iG = iMax;

	free( ag );

	return( iMax );
}


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

	apt->nG = 0;
	apt->g = ( pt2dGroup_type *)malloc( no*sizeof(pt2dGroup_type) );


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
		ag = pt2dGroupA_alloc( apt->nGroup );

	
	aPg = (pt2dGroup_type **)malloc( (apt->nP+1)*sizeof(pt2dGroup_type *) );
	for( i = 0; i < apt->nP ; i++ ){
		aPg[i] = NULL;
	}


	ag->nG = 0;
	for( i = 0; i < apt->nP; i++ ){
		pt2d_type	*pt = &apt->p[i];

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


int
pt2dGroupA_reorder( pt2dGroupA_type *ag )
{	
	int	i,	j;


	for( i = 1,	j = 0 ; i < ag->nG ; i++ ){
		pt2dGroup_type g = ag->g[i];
		for( j = i ; j > 0 ; j-- ){
			if( g.no < ag->g[j-1].no )
				break;

			ag->g[j] = ag->g[j-1];
		}

		ag->g[j] = g;
	}


	return(1 );
}



void
pt2dA_group_nv( pt2dA_type *apt, int iGroup, vec2f_type *nv )
{
	pt2d_type	*pt;
	vec2f_type	v,	n;
	float	t;
	int	i;

	int nV = 0;
	v.x = v.y = 0;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];
		if( pt->group != iGroup )
			continue;

		if( nV == 0 ){
			v = pt->n;
			nV = 1;

			n = pt->n;
			continue;
		}

		if( n.x * pt->n.x +  n.y * pt->n.y > 0 ){
			v.x += pt->n.x;
			v.y += pt->n.y;
		}
		else	{
			v.x -= pt->n.x;
			v.y -= pt->n.y;
		}
		nV++;
	}

	t = hypot( v.x, v.y );

	v.x /= t;
	v.y /= t;

	*nv = v;
}


void
pt2dA_group_nv3( pt2dA_type *apt, int iGroup, int iGroup0, int iGroup1, vec2f_type *nv )
{
	pt2d_type	*pt;
	vec2f_type	v,	n;
	float	t;
	int	i;

	int nV = 0;
	v.x = v.y = 0;
	for( i = 0 ; i < apt->nP ; i++ ){
		pt = &apt->p[i];
		if( pt->group != iGroup && pt->group != iGroup0 && pt->group != iGroup1 )
			continue;

		if( nV == 0 ){
			v = pt->n;
			nV = 1;

			n = pt->n;
			continue;
		}

		if( n.x * pt->n.x +  n.y * pt->n.y > 0 ){
			v.x += pt->n.x;
			v.y += pt->n.y;
		}
		else	{
			v.x -= pt->n.x;
			v.y -= pt->n.y;
		}
		nV++;
	}

	t = hypot( v.x, v.y );

	v.x /= t;
	v.y /= t;

	*nv = v;
}