
#ifndef _LINE_DECTECTOR_
#define _LINE_DECTECTOR_

#if defined _DEBUG || defined DEBUG
#pragma comment( lib, "EdgeLibD.lib" )
#else
#pragma comment( lib, "EdgeLib.lib" )
#endif


#include "Uln/PlnType.h"
#include "plEdgeType.h"
#include "ImageType/ImageType.h"


class CSlineDetector 
{
public:
	
	CSlineDetector();

	~CSlineDetector();

	void DeleteContents();

	int		Init( char *prmFile );


	int		Process( image_type *sim, box2i *box, int iFrame = 0 );
	int		Process( image_type *sim, int iFrame = 0 );



	plnA_type *	Get( int fCopy );

	plnA_type *	Get( int minLen, int fCopy );




	void Draw( gpw_type *gpw, int fLine, int fCircle );

	void Draw( gpw_type *gpw );

	void	Write( char *outFile );

private:


	box2i m_box;


	image_type	*m_gim;
	image_type	*m_gimS;

	image_type	*m_yim;

	image_type	*m_yim2;

public:
	struct pEdgePrm_type	*m_prm;

//	plnA_type	*m_apl;

	plEdgeA_type *m_ae;
	
	// aux;
	int	m_scale;
};


#endif


