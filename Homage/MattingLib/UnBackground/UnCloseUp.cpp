//  Defines the entry point for the console application.
//
#include <stdio.h>
#include	<string.h>
#include	<math.h>
#include <stdlib.h>

#ifdef _DEBUG
#define _DUMP
#endif

#include "Ulog/Log.h"
#include "Ubox/Box1i.h"

#include "ImageType/ImageType.h"
#include "ImageDump/ImageDump.h"
#include "ImageLabel/ImageLabel.h"


#include	"UnBackground.h"


#include "ImageLabel/ImageLabel.h"


#include "bImage.h"


int	sbA_segment( sbA_type *as,  box1iA_type *ab );



int	CUnBackground::ProcessCloseUp()
{
	int		i,	k;

	box1iA_type  *ab0 = box1iA_alloc( 20 );
	sbA_segment( &m_as0,  ab0 );

	

	box1iA_type  *ab1 = box1iA_alloc( 20 );
	sbA_segment( &m_as1,  ab1 );




	
	int i0 = m_bim->height - 24;

	// Left
	if( ab0->nA > 1 ){
		for( i = ab0->nA-1 ; i > 0 ; i-- ){
			box1i_type *b = &ab0->a[i];
			if( b->i0 <  i0 )	break;
		}
		
		box1i_type *b = &ab0->a[i];
		for( k = b->i1+1 ; k < m_as0.nA ; k++ )
			m_as0.a[k].j0 = m_as0.a[k].j1 = -1;
	}



	// right
	if( ab1->nA > 1 ){
		for( i = ab1->nA-1 ; i > 0 ; i-- ){
			box1i_type *b = &ab1->a[i];
			if( b->i0 <  i0 )	break;
		}

		box1i_type *b = &ab1->a[i];
		for( k = b->i1+1 ; k < m_as0.nA ; k++ )
			m_as1.a[k].j0 = m_as1.a[k].j1 = -1;
	}




	box1iA_destroy( ab0 );

	box1iA_destroy( ab1 );


	return( 1 );
}







int
sbA_segment( sbA_type *as,  box1iA_type *ab )
{
	int	i,	k;



	for( i = 0 ; i < as->nA ;  ){
		sb_type *s = &as->a[i];
		if( s->j0 < 0 || s->j1 < 0 ){
			i++;
			continue;
		}

		box1i_type *b = &ab->a[ab->nA++];
		b->i0 = b->i1 = i;
		for( k = b->i0+1 ; k < as->nA ; k++ ){
			sb_type *s = &as->a[k];

			if( as->a[k].j0 < 0 || as->a[k].j1 < 0||   as->a[k].j1 < as->a[k-1].j0 || as->a[k].j0 > as->a[k-1].j1 )
				break;
			
			b->i1 = k;
		}

		i = k;

	}


#ifdef _DEBUG
	box1iA_write( "AA", ab, stdout );
#endif


	return( 1 );
}


