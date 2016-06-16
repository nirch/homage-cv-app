
#ifndef _UN_EDGE_H_
#define _UN_EDGE_H_


#include "Uvec/Vec3d.h"
#include "Utime/GpTime.h"

#include "Uln/Cln/ClnType.h"

#include "ImageType/ImageType.h"

#include "PlnLib/PlnCurvature/PlnCurvature.h"


typedef struct unEdgePrm_type {
	int thin;

	plnCurvaturePrm_type c;

} unEdgePrm_type;




class CUnEdge 
{
public:
	
	CUnEdge();

	~CUnEdge();

	void DeleteContents();


	int	Init( char *prmFile );


	int	Process(  image_type *sim, int iFrame );

	int	ProcessStraightLine( vl2fA_type **avl );

protected:


private:

	
	int	ReadPrm( char *inFile );



	unEdgePrm_type *m_prm;


	image_type *m_yim;
	image_type *m_eim;


	plnA_type *m_apl;

	vl2fA_type *m_avl;

public:
	class CRidgeDetector *m_dr;

	int	m_iFrame;


	gpTime_type m_gTime;

};



unEdgePrm_type *	unEdgePrm_alloc();

void	CUnEdgePrm_destroy( unEdgePrm_type *prm );


#endif


