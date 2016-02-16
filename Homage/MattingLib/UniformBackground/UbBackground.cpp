
#include	<string.h>
#include	<math.h>

#include	"Ulog/Log.h"

#ifdef _DEBUG
#define _DUMP 
#endif

#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"



#include "UniformBackground.h"



#include "UnBackground/bImage.h"

#include "UnBackground/UnBackground.h"







 int	bImage_fill( image_type *bim, int a0[], int a1[] );




 int CUniformBackground::ProcessBackgroundState( image_type *sim, int iFrame )
 {

	 if( m_flip == 1 )
		 m_sim = image3_rotate180( sim, m_sim );
	 else m_sim = image_make_copy( sim, m_sim );


	 m_unBackground->Process( sim, 1 );
		


	 m_state = m_unBackground->GetState();

	 GPLOG(("ProcessBackgroundState: %d ", iFrame, m_state ) );


	return( 1 );
}



 int CUniformBackground::ProcessBackground( image_type *sim, int iFrame )
 {
	 if( m_mim == NULL )
		 return( -10 );




	if( m_flip == 1 )
		m_sim = image3_rotate180( sim, m_sim );
	else m_sim = image_make_copy( sim, m_sim );




	ProcessInitBackground( m_sim );



	return( m_state );

}


int	CUniformBackground::ProcessInitBackground( image_type *sim )
{
int	ret;


	ret = m_unBackground->Process( sim, 1 );

	m_iHead = 2*m_unBackground->GetHead() + 1;
	m_bim = m_unBackground->GetBim( m_bim );

	m_state = m_unBackground->GetState();

	return( 1 );

}





int
bImage_fill( image_type *bim, int a0[], int a1[] )
{
	int	i,	k,	no;
	bImage_type *bp;



	no = 0;
	bp = ( bImage_type *)IMAGE_PIXEL( bim, 1, 0 );
	for( i = 1 ; i < bim->height-1 ; i++, bp += bim->width ){

		if( a0[i] == -1 || a1[i] == -1 )
			continue;


		if( a0[i] < 0 || a1[i] < 0 )
			continue;

//		fprintf( stdout, "%d  %d %d", i,  a0[59], a1[59] );
		
		int	j0,	j1;
		bImage_type *bp0,	*bp1;

		if( a0[i] < 0 ){
			j0 = 0;
			bp0 = bp + a1[i];
		}
		else {
			j0 = a0[i];
			bp0 = bp + a0[i];
		}



		if( a1[i] < 0 ){
			j1 = bim->width-1;
			bp1 = bp0;
		}
		else {
			j1 = a1[i];
			bp1 = bp + a1[i];
		}




		bImage_type *cp;
		for( k = j0 + 1, cp = bp0+1 ; k < j1 ; k++, cp++ ){


			float f = (k - j0)/(float)( j1 - j0 ); 

			//if( f < 0.25 )	f *= 0.5;
			//else	if( f < 0.75 )	f = f *1.5 - 0.25;
			//else	f = 0.5 + 0.5*f;


			if( f < 0.25 )	f /= 3;
			else	if( f < 0.75 )	f = 0.5 + ( f - 0.5)*5.0/3 ;
			else	f = 1 - ( 1-f)/3;

			//if( f < 0.25 )	f *= 0.25;
			//else	if( f < 0.75 )	f = 0.375 + 0.25*f;//f *1.5 - 0.25;
			//else	f = 0.5 + 0.25*f;



			cp->r = (1-f) * bp0->r + f* bp1->r;
			cp->g = (1-f) * bp0->g + f* bp1->g;
			cp->b = (1-f) * bp0->b + f* bp1->b;
			cp->n = 1;
		}
	}


	return( 1 );
}





