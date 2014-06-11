/*********************************
 ***   DynamicBackground.cpp   ***
 *********************************/

#ifdef _DEBUG
//#define _DUMP
#endif


#include	<string.h>
#include	<math.h>
#include <stdlib.h>


#include	"Ulog/log.h"
#include	"ImageType/ImageType.h"
#include	"ImageDump/ImageDump.h"

#include	"Imagelabel/BwLabel.h"

#include	"BnImage.h"

#include	"ImageMorphology/ImageMorphology.h"


#include	"Background.h"



static int	bnImage_update( image_type *sim,
						   image_type *var, image_type *gvar,
						   float tVal0, int tNo );


static int	bnImage_update5( image_type *sim, image_type *var, image_type *gvar, float tVal0 );


static int	bnImage_compare( image_type *sim, image_type *var, float tVal0 );


static void	bnImage_filter_0x01( image_type *gIm, int tNo );


static void	bnImage_remove_blobe_0x01( image_type *sim, int tNo );





static int	bnImage_test_bim_0x04( image_type *gvar );

static int	bnImage_update_bim_0x04( image_type *rgbBim, image_type *bim, image_type *gbim,
						image_type *rgbSim, image_type *sim, image_type *gvar,
						int ds );

static int	bnImage_update_bim_0x04_8( image_type *rgbBim, image_type *bim, image_type *gbim,
									image_type *rgbSim, image_type *sim, image_type *gvar,
									int ds );


static void	bnImage_fill_0x02( image_type *sim );

static void	bnImage_remove_blobe_0x02( image_type *sim );



static void	bnImage_close_0x0( image_type *sim );




static void	bnImage_mask_face( image_type *sim, facePose_type *pose, int ds, int bodyWidth  );



static int	bnImage_test_bim_0x02( image_type *sim, image_type *bim,
									image_type *gvar, image_type *gbim,
									int ds, float tVal0 );


static void	bnImage_clear_0x02( image_type *sim, image_type *bim, int tVal, image_type *gvar );



#define DS	8

//int iFrame = -1;
static int tS;





CDynamicBackground::CDynamicBackground()
{
	m_iFrame = 0;


	m_rgbBim = NULL;
	m_bim = NULL;
	m_bim4 = NULL;


	m_bgP = NULL;



	m_gbim = NULL;
	m_gvar = NULL;


	m_nH = 0;

	m_poseFrame = -10;


	m_tNo = 5;

	m_tVal0 = 4;


	m_tS = 1;
	tS = m_tS;



	m_nSataticBg = 100;


	m_ex = 4;



	m_bodyWidth = 75;

	m_pose.width = 0;



	for( int i = 0 ; i < MAX_N ; i++ )
		m_aIm4[i] = NULL;


	for( int i = 0 ; i < MAX_N ; i++ )
		m_eIm[i] = NULL;


	gpTime_init( &m_tBg );
	gpTime_init( &m_tSh );


	m_inBoxIm = NULL;

	m_update = 1;

	m_state = 0;


	m_bodyHistogram = 1;
	m_useBodyHistogram = 1;

	m_closeBody = 0;

	m_rgb = 0;


	m_silhouetteIm = NULL;

	m_silhouetteUpdate = 0;

	m_nccIm = NULL;


	m_eBim = NULL;
	m_gim = NULL;


	m_dbEdge = 0;

	m_dbRgb = 0;

	m_processBox = 0;

}



CDynamicBackground::~CDynamicBackground()
{
	DeleteContents();
#ifdef _AA_
	if( m_rgbBim != NULL ){
		image_destroy( m_rgbBim, 1 );
		m_rgbBim = NULL;
	}


	if( m_bim != NULL ){
		image_destroy( m_bim, 1 );
		m_bim = NULL;
	}



	if( m_bim4 != NULL ){
		image_destroy( m_bim4, 1 );
		m_bim4 = NULL;
	}



	if( m_bgP != NULL ){
		bgStage_destroy( m_bgP );
		m_bgP = NULL;
	}


	for( int i = 0 ; i < MAX_N ; i++ )
		if( m_aIm4[i] != NULL ){
			image_destroy( m_aIm4[i], 1 );
			m_aIm4[i] = NULL;
		}



	for( int i = 0 ; i < MAX_N ; i++ )
		if( m_eIm[i] != NULL ){
			image_destroy( m_eIm[i], 1 );
			m_eIm[i] = NULL;
		}



	if( m_gvar != NULL ){
		image_destroy( m_gvar, 1 );
		m_gvar = NULL;
	}


	if( m_gbim != NULL ){
		image_destroy( m_gbim, 1 );
		m_gbim = NULL;
	}



	if( m_inBoxIm != NULL ){
		image_destroy( m_inBoxIm, 1 );
		m_inBoxIm = NULL;
	}


	if( m_nccIm != NULL ){
		image_destroy( m_nccIm, 1 );
		m_nccIm = NULL;
	}


	if( m_eBim != NULL ){
		image_destroy( m_eBim, 1 );
		m_eBim = NULL;
	}


	if( m_gim != NULL ){
		image_destroy( m_gim, 1 );
		m_gim = NULL;
	}

	if( m_silhouetteIm != NULL ){
		image_destroy( m_silhouetteIm, 1 );
		m_silhouetteIm = NULL;
	}
#endif
}



void CDynamicBackground::DeleteContents()
{
	if( m_rgbBim != NULL ){
		image_destroy( m_rgbBim, 1 );
		m_rgbBim = NULL;
	}


	if( m_bim != NULL ){
		image_destroy( m_bim, 1 );
		m_bim = NULL;
	}



	if( m_bim4 != NULL ){
		image_destroy( m_bim4, 1 );
		m_bim4 = NULL;
	}



	if( m_bgP != NULL ){
		bgStage_destroy( m_bgP );
		m_bgP = NULL;
	}


	for( int i = 0 ; i < MAX_N ; i++ )
		if( m_aIm4[i] != NULL ){
			image_destroy( m_aIm4[i], 1 );
			m_aIm4[i] = NULL;
		}



		for( int i = 0 ; i < MAX_N ; i++ )
			if( m_eIm[i] != NULL ){
				image_destroy( m_eIm[i], 1 );
				m_eIm[i] = NULL;
			}



			if( m_gvar != NULL ){
				image_destroy( m_gvar, 1 );
				m_gvar = NULL;
			}


			if( m_gbim != NULL ){
				image_destroy( m_gbim, 1 );
				m_gbim = NULL;
			}



			if( m_inBoxIm != NULL ){
				image_destroy( m_inBoxIm, 1 );
				m_inBoxIm = NULL;
			}


			if( m_nccIm != NULL ){
				image_destroy( m_nccIm, 1 );
				m_nccIm = NULL;
			}


			if( m_eBim != NULL ){
				image_destroy( m_eBim, 1 );
				m_eBim = NULL;
			}


			if( m_gim != NULL ){
				image_destroy( m_gim, 1 );
				m_gim = NULL;
			}

			if( m_silhouetteIm != NULL ){
				image_destroy( m_silhouetteIm, 1 );
				m_silhouetteIm = NULL;
			}
}



int CDynamicBackground::ProcessFrame( image_type *rgbIm, image_type *sim,
									 facePose_type *pose, int iFrame, image_type **im )
{
int	ret;

	gpTime_start( &m_tBg );
	ret = ProcessFrame( rgbIm, sim, pose, iFrame );
	gpTime_stop( &m_tBg );


	gpTime_start( &m_tSh );

	if( m_closeBody == 0 ){
		*im = produceSilhouette( rgbIm, sim, m_aIm4[0], *im );
	}
	else	{

		*im = produceSilhouetteClose( rgbIm, sim, m_aIm4[0], *im );

		m_silhouetteIm = produceSilhouetteClose( rgbIm, sim, m_aIm4[0], m_silhouetteIm, 0 );
	}


	gpTime_stop( &m_tSh );


	return( ret );
}




int CDynamicBackground::ProcessFrame( image_type *rgbSim, image_type *sim, facePose_type *pose, int iFrame )
{
int	no,	no0;

	m_iFrame = ( iFrame >= 0 )? iFrame : m_iFrame + 1;



	if( pose != NULL && pose->state != 0 ){

		if( m_pose.width == 0 )
			m_bodyWidth = 1.75 * pose->width;	// 2.25

		m_pose = *pose;
		m_poseFrame = m_iFrame;
	}




	imageA_icycle( m_aIm4, m_tNo+1 );
	if( m_rgb & 0x02 )
		m_aIm4[0] = bnImage_average4( rgbSim, m_aIm4[0] );
	else
		m_aIm4[0] = bnImage_average4( sim, m_aIm4[0] );


	
	if( m_bim == NULL ){
		InitBackground( rgbSim, sim, m_aIm4[0] );
		m_iFrame0 = iFrame;
		return( 1 );
	}


	RestoreBackground();



	// motion
	no0 = bnImage_update( m_aIm4[0], m_aIm4[1], m_gvar, m_tVal0, m_tNo );
	BNIMAGE_DUMP(  sim, m_gvar, DS, "im", m_iFrame, "gvar-in00", 1 );


	if( m_aIm4[m_tNo] != NULL )
		bnImage_update5( m_aIm4[0], m_aIm4[m_tNo], m_gvar, m_tVal0 );
		
	BNIMAGE_DUMP(  sim, m_gvar, DS, "im", m_iFrame, "gvar-in0", 1 );

	if( m_dbEdge == 1 ){
		DbEdgeMotion( sim );
		BNIMAGE_DUMP(  sim, m_gvar, DS, "im", m_iFrame, "gvar-in0-D", 1 );
	}



	if( m_processBox == 1 )
		ProcessBox( sim );




	if( m_update == 0 )
		return( 1 );



	if( m_iFrame % m_tS  ){
		return( 1 );
	}




	// remove 0x01 with less then 3 neighbor 
	bnImage_filter_0x01( m_gvar,  3 );



	// close motion hole
	bnImage_close_0x0( m_gvar );




	// remove 0x01 blob with less then 300 
	bnImage_remove_blobe_0x01( m_gvar, 300 );
	





	no = bnImage_no( m_gvar );

	if( no > 0.8 * m_gvar->width*m_gvar->height ){
		//GPTRACE( (3, "   NO MOTION: %d", m_iFrame) );
		m_state = DB_STATUS_NO_MOTION;
		return(1);
	}


	if(  no < 0.075 * m_gvar->width*m_gvar->height ){
		if( m_iFrame -  m_iFrame0 > m_nSataticBg  ){//|| no0 < 0.45 * m_gvar->width*m_gvar->height ){
			ReplaceBackground( rgbSim, sim, m_aIm4[0] );
		}

		
		return(1);
	}




	if( m_poseFrame == m_iFrame )
		bnImage_mask_face( m_gvar, pose, DS, m_bodyWidth );



	BNIMAGE_DUMP(  sim, m_gvar, DS, "im", m_iFrame, "gvar", 1 );





	bnImage_test_bim_0x02( m_aIm4[0], m_bim4, m_gvar, m_gbim, DS, m_tVal0 );


	if( m_dbRgb == 1)
		bnImage_clear_0x02( rgbSim, m_rgbBim, 6, m_gvar );

	bnImage_remove_blobe_0x02( m_gvar );



//	BNIMAGE_DUMP(  sim, m_gvar, "test", DS, m_iFrame, 3 );


	bnImage_fill_0x02( m_gvar );


	m_nUpdate = 0;
	for( int i = 0 ; i < m_ex ; i++ ){
		int n = bnImage_test_bim_0x04( m_gvar );
		if( n == 0 )	break;

		m_nUpdate += bnImage_update_bim_0x04_8(  m_rgbBim, m_bim, m_gbim, rgbSim, sim, m_gvar, DS );
	}

	m_pUpdate = m_nUpdate*100.0 / ( m_gvar->width * m_gvar->height );

#ifdef _DUMP
	image_dump( m_bim, "im", m_iFrame, "bim" );
#endif


	if( m_rgb & 0x02 )
		m_bim4 = bnImage_average4( m_rgbBim, m_bim4 );
	else
		m_bim4 = bnImage_average4( m_bim, m_bim4 );




	m_state = DB_UPDATE;

	return( 1 );
}




int CDynamicBackground::InitBackground( image_type *rgbSim, image_type *bim, image_type *bim4 )
{

	if( m_rgb != 0 && rgbSim != NULL )
		m_rgbBim = image_make_copy( rgbSim, m_rgbBim );

	m_bim = image_make_copy( bim, m_bim );
	m_bim4 = image_make_copy( bim4, m_bim4 );

	int height = ((bim->height+DS-1)/DS );
	int width = ((bim->width+DS-1)/DS );


	m_gbim = image_create( height, width, sizeof(bn_type), 1, NULL );
	bnImage_init( m_gbim );


	m_gvar = image_create( height, width, sizeof(bn_type), 1, NULL );

	bnImage_init( m_gvar );

	m_iFrame0 = m_iFrame;

	return(  1 );
}


int CDynamicBackground::ReplaceBackground( image_type *rgbBim, image_type *bim, image_type *bim4 )
{
	if( m_bgP == NULL || m_bgP->iFrame < 0 || m_iFrame - m_iFrame0 > 50 ){

		m_bgP = bgStage_set( m_bgP, m_rgbBim, m_bim, m_bim4, m_iFrame );
#ifdef _DUMP
		image_dump( m_bim, "bim", m_iFrame, "P" );
#endif

	}


	if( m_rgb != 0 && m_rgbBim != NULL )
		m_rgbBim = image_make_copy( rgbBim, m_rgbBim );
	m_bim = image_make_copy( bim, m_bim );
	m_bim4 = image_make_copy( bim4, m_bim4 );


	bnImage_init( m_gbim );


	bnImage_init( m_gvar );

	GPTRACE( (3, "   REPLACE: %d", m_iFrame) );

#ifdef _DUMP
	image_dump( m_bim, "bim", m_iFrame, "replace" );
#endif


	m_iFrame0 = m_iFrame;

	m_state = DB_STATUS_BACKGROUND_CHANGE;

	return(1);
}



int CDynamicBackground::RestoreBackground()
{
	if( m_bgP == NULL || m_bgP->iFrame < 0  || m_iFrame - m_iFrame0 >= m_nSataticBg )
		return( 0 );


	int no = bnImage_compare( m_aIm4[0], m_bgP->bim4, m_tVal0 );

	if( no < 0.5 * m_gvar->width*m_gvar->height )
		return( 0 );


	bgStage_restore( m_bgP, &m_rgbBim, &m_bim, &m_bim4, &m_iFrame0 );
	m_bgP->iFrame = -1;


	bnImage_init( m_gbim );


	bnImage_init( m_gvar );

	

	GPTRACE( (3, "   Restore: %d", m_iFrame) );
#ifdef _DUMP
	image_dump( m_bim, "bim", m_iFrame, "restore" );
#endif

	m_state = DB_STATUS_BACKGROUND_CHANGE;
	for( int i = m_tNo-2 ; i > 0 ; i-- )
		bnImage_update( m_aIm4[i], m_aIm4[i+1], m_gvar, m_tVal0, m_tNo );

	return( 1 );
}







image_type * CDynamicBackground::GetNoiseImage( image_type *sim, image_type *im )
{
	im = bnImage_dup( sim, m_gvar, DS, 0x01, im );

	return( im );
}



static void
bnImage_filter_0x01( image_type *gIm, int tN )
{
bn_type	*sp,	*sp0,	*sp1;
int	i,	j;

	for( i = 1 ; i < gIm->height-1 ; i++ ){

		sp = (bn_type *)IMAGE_PIXEL( gIm, i, 1 );
		sp0 = sp - gIm->width;
		sp1 = sp + gIm->width;

		for( j = 1 ; j < gIm->width-1 ; j++, sp++, sp0++, sp1++ ){


			if( sp->flag == 0 )	continue;

			int n  = ((sp0-1)->flag&0x1) + ((sp0)->flag&0x01) + ((sp0+1)->flag&0x01) +
						((sp-1)->flag&0x01) + ((sp+1)->flag&0x01) +
						((sp1-1)->flag&0x01) + ((sp1)->flag&0x01) + ((sp1+1)->flag&0x01);




			if( n < tN )
				sp->flag |= 0x08;


//			if( n == 0 )
//				sp->flag = 0;
		}
	}


	for( i = 1 ; i < gIm->height-1 ; i++ ){
		sp = (bn_type *)IMAGE_PIXEL( gIm, i, 1 );

		for( j = 1 ; j < gIm->width-1 ; j++, sp++ ){


			if( sp->flag &0x08 )
				sp->flag = 0;
		}
	}
}







static void
bnImage_remove_blobe_0x01( image_type *sim, int tN )
{
image_type *im;
bwLabel_type *abw;
int nBw; 
int	i,	j;
short	*sp;
bn_type	*gv;



	im = image_create( sim->height, sim->width, 2, 1, NULL );

	gv = ( bn_type*)sim->data;
	sp = im->data_s;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++, sp++ ){
			*sp = gv->flag;
		}
	}


	image2_bwLabel( im, &abw, &nBw );
	if( nBw < 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}

	image2_bwLabel_set_id( im, abw );





	int m = 0;
	for( i = 0 ; i < nBw ; i++ ){
		if( abw[i].no < tN )
			abw[i].id = -1;
	}




	gv = ( bn_type*)sim->data;
	sp = im->data_s;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++, sp++ ){

			if( *sp < 0 )	continue;

			if( abw[*sp].id < 0 )
				gv->flag = 0;
		}
	}



	free( abw );
	image_destroy( im, 1 );
}



static int
bnImage_test_bim_0x04( image_type *gvar )
{
bn_type	*gv,	*gv0,	*gv1;
int	i,	j,	n,	flag;


	gv = ( bn_type*)gvar->data;


	n = 0;


	//firat row
	i = 0;
	gv = (bn_type *)IMAGE_PIXEL( gvar, i, 1 );
	gv0 = gv - gvar->width;
	gv1 = gv+ gvar->width;

	for( j = 1 ; j < gvar->width-1 ; j++, gv++, gv0++, gv1++ ){

		if( gv->flag != 1 )	continue;


		flag = 
			((gv-1)->flag&0x2) +   ((gv+1)->flag&0x2) +
			((gv1-1)->flag&0x2) +  ((gv1)->flag&0x2) +   ((gv1+1)->flag&0x2);


		if( flag >= 4 ){
			gv->flag |= 0x04;
			n++;
		}
	}





	for( i = 1 ; i < gvar->height-1 ; i++ ){

		gv = (bn_type *)IMAGE_PIXEL( gvar, i, 0 );
		gv0 = gv - gvar->width;
		gv1 = gv+ gvar->width;

		flag =  ((gv0)->flag&0x2) +   ((gv0+1)->flag&0x2) +
			((gv+1)->flag&0x2) +
			((gv1)->flag&0x2) +   ((gv1+1)->flag&0x2);

		if( flag >= 4 ){
			gv->flag |= 0x04;
			n++;
		}


		gv++;
		gv0++;
		gv1++;
	
		
		for( j = 1 ; j < gvar->width-1 ; j++, gv++, gv0++, gv1++ ){


			if( (gv->flag & 0X1) == 0 )	continue;

			flag =  ((gv0-1)->flag&0x2) +  ((gv0)->flag&0x2) +   ((gv0+1)->flag&0x2) +
					((gv-1)->flag&0x2) +   ((gv+1)->flag&0x2) +
					((gv1-1)->flag&0x2) +  ((gv1)->flag&0x2) +   ((gv1+1)->flag&0x2);

			int no =  ((gv0-1)->flag&0x1) +  ((gv0)->flag&0x1) +   ((gv0+1)->flag&0x1) +
				((gv-1)->flag&0x1) +   ((gv+1)->flag&0x1) +
				((gv1-1)->flag&0x1) +  ((gv1)->flag&0x1) +   ((gv1+1)->flag&0x1);

	// check baskets to decide is't noise ~~
			if( flag >= 6 && no > 5){ //5
				gv->flag |= 0x04;
				n++;
			}
		}


		flag =  ((gv0-1)->flag&0x2) +  ((gv0)->flag&0x2) +  
			((gv-1)->flag&0x2) +  
			((gv1-1)->flag&0x2) +  ((gv1)->flag&0x2);

		if( flag >= 4 ){
			gv->flag |= 0x04;
			n++;
		}

	}



	//last row
	i = gvar->height-1;
	gv = (bn_type *)IMAGE_PIXEL( gvar, i, 1 );
	gv0 = gv - gvar->width;
	gv1 = gv+ gvar->width;

	for( j = 1 ; j < gvar->width-1 ; j++, gv++, gv0++, gv1++ ){

		if( gv->flag != 1 )	continue;


		int flag =  ((gv0-1)->flag&0x2) +  ((gv0)->flag&0x2) +   ((gv0+1)->flag&0x2) +
			((gv-1)->flag&0x2) +   ((gv+1)->flag&0x2);



		if( flag >= 4 ){
			gv->flag |= 0x04;
			n++;
		}
	}


	return( n );
}



static int
bnImage_update_bim_0x04_8( image_type *rgbBim, image_type *bim, image_type *gbim,
						image_type *rgbSim, image_type *sim, image_type *gvar,
						int ds )
{
	u_int	*vp;
	u_int	*bp;
	bn_type	*gv,	*gb;
	int	i,	j;
	int	k,	k1,	align,	align4,	align12;
	int	nUpdate;


	//__restrict bn_type *gv = ( bn_type*)gvar->data;
	//__restrict bn_type *gb = ( bn_type*)gbim->data;


	gv = ( bn_type*)gvar->data;

	gb = ( bn_type*)gbim->data;


	align = (sim->width - 8 ) >> 2;
	align4 = sim->width >> 2;
	align12 = 3*align4;

	nUpdate = 0;
	for( i = 0 ; i < sim->row ; i += ds ){
		k1 = ( i+ds < sim->height)? ds : sim->height-i;
		for( j = 0 ; j < sim->column ; j += ds, gb++, gv++ ){


			if( (gv->flag & 0x4) == 0 )	continue;

			if( gb->no > 50 && (gb->no*2) > (3*gv->no) && gv->no < 50  || gb->no > 20 && gv->no < 10 ){
			//if( gb->no > 50 && gb->no > 1.5*gv->no && gv->no < 50  || gb->no > 20 && gv->no < 10 ){ //50

				if( (gv->flag &0x02)== 0 ){
					continue;
				}
			}

//		align = sim->width-n1;

			vp = (u_int *)IMAGE_PIXEL( sim, i, j );
			bp = (u_int *)IMAGE_PIXEL( bim, i, j );


			//for( k = 0 ; k < k1 ; k++, bp += align, vp += align ){

			//	*bp++ = *vp++;
			//	*bp++ = *vp++;
			//}
			for( k = 0 ; k < k1 ; k++, bp += align4, vp += align4 ){

				bp[0] = vp[0];
				bp[1] = vp[1];
			}




			if( rgbBim != NULL ){
				u_int *sp = (u_int *)IMAGE_PIXEL( rgbSim, i, j );
				bp = (u_int *)IMAGE_PIXEL( rgbBim, i, j );
				for( k = 0 ; k < k1 ; k++, bp += align12, sp += align12 ){
					bp[0] = sp[0];
					bp[1] = sp[1];
					bp[2] = sp[2];
					bp[3] = sp[3];
					bp[4] = sp[4];
					bp[5] = sp[5];
				}
			}



			if( (gv->flag &0x02)== 0 )
				gb->no = 1;
			gv->flag = 0x3;

			nUpdate++;
		}
	}

	return( nUpdate );
}




static int
bnImage_update_bim_0x04( image_type *rgbBim, image_type *bim, image_type *gbim,
						image_type *rgbSim, image_type *sim, image_type *gvar,
						int ds )
{
	u_char	*vp;
	u_char	*bp;
	bn_type	*gv,	*gb;
	int	i,	j;
	int	k,	k1,	n,	n1,	align;
	int	nUpdate;

	//__restrict bn_type *gv = ( bn_type*)gvar->data;
	//__restrict bn_type *gb = ( bn_type*)gbim->data;


	gv = ( bn_type*)gvar->data;

	gb = ( bn_type*)gbim->data;


	nUpdate = 0;
	for( i = 0 ; i < sim->row ; i += ds ){
		k1 = ( i+ds < sim->height)? ds : sim->height-i;
		for( j = 0 ; j < sim->column ; j += ds, gb++, gv++ ){


			if( (gv->flag & 0x4) == 0 )	continue;

			if( gb->no > 50 && (gb->no*2) > (3*gv->no) && gv->no < 50  || gb->no > 20 && gv->no < 10 ){
				//if( gb->no > 50 && gb->no > 1.5*gv->no && gv->no < 50  || gb->no > 20 && gv->no < 10 ){ //50

				if( (gv->flag &0x02)== 0 ){
					continue;
				}
			}


			n1 = ( j+ds < sim->width)? ds : sim->width-j;
			align = sim->width-n1;

			vp = (u_char *)IMAGE_PIXEL( sim, i, j );
			bp = (u_char *)IMAGE_PIXEL( bim, i, j );


			for( k = 0 ; k < k1 ; k++, bp += align, vp += align ){
				for( n = 0 ; n < n1 ; n++, bp++, vp++ ){
					*bp = (*vp);
				}
			}

			//if( rgbBim != NULL ){
			//	__restrict u_char *sp = (u_char *)IMAGE_PIXEL( rgbSim, i, j );
			//	__restrict bp = (u_char *)IMAGE_PIXEL( rgbBim, i, j );


			if( rgbBim != NULL ){
				u_char *sp = (u_char *)IMAGE_PIXEL( rgbSim, i, j );
				bp = (u_char *)IMAGE_PIXEL( rgbBim, i, j );


				for( k = 0 ; k < k1 ; k++, bp += 3*align, sp += 3*align ){
					for( n = 0 ; n < n1 ; n++ ){
						*bp++ = *sp++;
						*bp++ = *sp++;
						*bp++ = *sp++;
					}
				}

			}



			if( (gv->flag &0x02)== 0 )
				gb->no = 1;
			gv->flag = 0x3;

			nUpdate++;
		}
	}

	return( nUpdate );
}






static void
bnImage_fill_0x02( image_type *sim )
{
image_type *im;
bwLabel_type *abw;
int nBw; 
int	i,	j;
short	*sp;
bn_type	*gv;




	im = image_create( sim->height, sim->width, 2, 1, NULL );

	gv = ( bn_type*)sim->data;
	sp = im->data_s;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++, sp++ ){

			*sp = (gv->flag & (0x2|0x08) ) == 0;
		}
	}


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}

	image2_bwLabel_set_id( im, abw );





	int m = 0;
	for( i = 0 ; i < nBw ; i++ ){

		//		if( dval[i] < 35 && abw[i].no < 300 )
		if( abw[i].no < 30 )
			abw[i].id = -1;
	}




	gv = ( bn_type*)sim->data;
	sp = im->data_s;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++, sp++ ){

			if( *sp < 0 )	continue;

			if( abw[*sp].id < 0 )
				gv->flag |= 0x4;
		}
	}



	free( abw );
	image_destroy( im, 1 );
}



static void
bnImage_remove_blobe_0x02( image_type *sim )
{
	image_type *im;
	bwLabel_type *abw;
	int nBw; 
	int	i,	j;
	short	*sp;
	bn_type	*gv;




	im = image_create( sim->height, sim->width, 2, 1, NULL );

	gv = ( bn_type*)sim->data;
	sp = im->data_s;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++, sp++ ){

			*sp = (gv->flag & (0x2) );
		}
	}


	image2_bwLabel( im, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( im, 1);
		return;
	}

	image2_bwLabel_set_id( im, abw );





	int m = 0;
	for( i = 0 ; i < nBw ; i++ ){
		if( abw[i].no < 125 )
			abw[i].id = -1;
	}




	gv = ( bn_type*)sim->data;
	sp = im->data_s;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++, sp++ ){

			if( *sp < 0 )	continue;

			if( abw[*sp].id < 0 )
				gv->flag &= 0x1;
		}
	}



	free( abw );
	image_destroy( im, 1 );
}




static void
bnImage_close_0x0( image_type *sim )
{
image_type *im;
int	i,	j;
u_char	*sp;
bn_type	*gv;



	image_type *bim;
	bwLabel_type *abw;
	int nBw; 
	short	*bp;



	bim = image_create( sim->height, sim->width, 2, 1, NULL );

	gv = ( bn_type*)sim->data;
	bp = bim->data_s;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++, bp++ ){

			*bp = (gv->flag == 0 );
		}
	}


	image2_bwLabel( bim, &abw, &nBw );
	if( nBw <= 1 ){
		free( abw );
		image_destroy( bim, 1);
		return;
	}

	image2_bwLabel_set_id( bim, abw );





	int m = 0;
	for( i = 0 ; i < nBw ; i++ ){
		if( abw[i].no < 10 )
			abw[i].id = -1;
	}



	im = image_create( sim->height, sim->width, 1, 1, NULL );


	sp = im->data;
	bp = bim->data_s;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++,  bp++, sp++ ){

			if( *bp == 0 || abw[*bp].id < 0 )
				*sp = 0;
			else
				*sp = 1;
		}
	}


	free( abw );
	image_destroy( bim, 1 );








	image1_close( im, 1 );
//	image1_open( im, 3 );


	gv = ( bn_type*)sim->data;
	sp = im->data;
	for( i = 0 ; i < sim->row ; i ++ ){
		for( j = 0 ; j < sim->column ; j ++, gv++, sp++ ){
			if( *sp != 0 )
				gv->flag = 0;
		}
	}

	image_destroy( im, 1 );
}







static void
bnImage_mask_face( image_type *sim, facePose_type *pose, int ds, int bodyWidth )
{
int	i,	j,	i0,	j0,	i1,	j1;
bn_type	*gv;


	if( pose->state == 0 )
		return;


	float dh = 0.375  * pose->height;
	float dw = 0.75 * pose->width;


	i0 = (pose->p.y - dh)/ds;
	i1 = (pose->p.y + dh)/ds + 1;

	j0 = (pose->p.x - dw)/ds;
	j1 = (pose->p.x + dw)/ds + 1;

	if( i0 < 0 )	i0 = 0;
	if( i1 >= sim->height )	i1 = sim->height-1;

	if( j0 < 0 )	j0 = 0;
	if( j1 >= sim->width )	j1 = sim->width-1;



	for( i = i0 ; i <= i1 ; i ++ ){
		gv = (bn_type *)IMAGE_PIXEL( sim, i, j0 );
		for( j = j0 ; j <= j1 ; j ++,   gv++ )
			gv->flag = 0;
	}




	i0 = (pose->p.y+2*dh )/ds;
	i1 = sim->height-1;
	if( i0 < 0 )	i0 = 0;
	i1 = sim->height-1;


	j0 = (pose->p.x - bodyWidth)/ds;
	j1 = (pose->p.x + bodyWidth)/ds + 1;
	if( j0 < 0 )	j0 = 0;
	if( j1 >= sim->width )	j1 = sim->width-1;





	for( i = i0 ; i <= i1 ; i ++ ){
		gv = (bn_type *)IMAGE_PIXEL( sim, i, j0 );
		for( j = j0 ; j <= j1 ; j ++,   gv++ )
			gv->flag = 0;
	}

}




static int
bnImage_update( image_type *sim, image_type *var, image_type *gvar,
			   float tVal0, int tNo )
{
bn_type	*gv;
short	*tp;
int	i,	j;
int	sum,	no;

	image_type *im = NULL;



	im = bnImage_abs_diff( sim, var, im );
	image2_binary_down( im, tVal0 * 64 );


	gv = ( bn_type*)gvar->data;

	for( i = 0,	no = 0 ; i < gvar->height ; i++ ){
		tp = (short*)IMAGE_PIXEL( im, 2*i+1, 1 );
		short *tp0 = tp - im->width;
		short *tp1 = tp + im->width;
		for( j = 0 ; j < gvar->width ; j++, gv++, tp += 2, tp1 += 2, tp0 += 2 ){

			sum = *(tp0-1) + *(tp0) + *(tp0+1) +
					*(tp-1) + *(tp) + *(tp+1) +
					*(tp1-1) + *(tp1) + *(tp1+1);


			if( sum == 9 ){
				gv->no++;
				no++;
				gv->flag = ( gv->no < tNo )? 0 : 1;

			}
			else	{
				gv->no = 1;
				gv->flag = 0;
			}	
		}
	}

	image_destroy( im, 1 );

	return( no );
}



static int
bnImage_update5( image_type *sim, image_type *var, image_type *gvar, float tVal0)
{
bn_type	*gv;
short	*tp;
int	i,	j;
int	sum,	no;



	image_type *im = NULL;


	im = bnImage_abs_diff( sim, var, im );
	image2_binary_down( im, tVal0*64 );



	gv = ( bn_type*)gvar->data;

	for( i = 0, no = 0 ; i < gvar->height ; i++ ){
		tp = (short *)IMAGE_PIXEL( im, 2*i+1, 1 );
		short *tp0 = tp - im->width;
		short *tp1 = tp + im->width;
		for( j = 0 ; j < gvar->width ; j++, gv++, tp += 2, tp1 += 2, tp0 += 2 ){

			if( gv->flag == 0 ){
				no++;
				continue;
			}

			sum = *(tp0-1) + *(tp0) + *(tp0+1) +
				*(tp-1) + *(tp) + *(tp+1) +
				*(tp1-1) + *(tp1) + *(tp1+1);


			if( sum < 9 ){
				gv->flag = 0;
				no++;
			}
		}
	}

	image_destroy( im, 1 );

	return( no );
}



static int
bnImage_compare( image_type *sim, image_type *var, float tVal0 )
{
short	*tp;
int	i,	j;
int	sum,	no;



	image_type *im = NULL;



	im = bnImage_abs_diff( sim, var, im );
	image2_binary_down( im, tVal0 * 64 );


	for( i = 1,	no = 0 ; i < im->height-1 ; i += 2 ){
		tp = (short*)IMAGE_PIXEL( im, i, 1 );
		short *tp0 = tp - im->width;
		short *tp1 = tp + im->width;
		for( j = 1 ; j < im->width-1 ; j += 2, tp += 2, tp1 += 2, tp0 += 2 ){


			sum = *(tp0-1) + *(tp0) + *(tp0+1) +
				*(tp-1) + *(tp) + *(tp+1) +
				*(tp1-1) + *(tp1) + *(tp1+1);


			if( sum == 9 ){
				no++;
			}
		}
	}


	image_destroy( im, 1 );

	return( no );
}





static int
bnImage_test_bim_0x02( image_type *sim, image_type *bim, image_type *gvar, image_type *gbim,
				int ds, float tVal0 )
{
bn_type	*gv,	*gb;
short	*tp;
int	i,	j;
int	sum;



	image_type *im = NULL;

	im = bnImage_abs_diff( sim, bim, im );
	image2_binary_down( im, tVal0*64 );


	gv = ( bn_type*)gvar->data;
	gb = ( bn_type*)gbim->data;

	for( i = 0 ; i < gvar->height ; i++ ){
		tp = (short *)IMAGE_PIXEL( im, 2*i+1, 1 );
		short *tp0 = tp - im->width;
		short *tp1 = tp + im->width;
		for( j = 0 ; j < gvar->width ; j++, gv++, gb++, tp += 2, tp1 += 2, tp0 += 2 ){

			if( gv->flag == 0 )	continue;
			if( *tp != 1 )	continue;

			sum = *(tp0-1) + *(tp0) + *(tp0+1) +
				*(tp-1) + *(tp) + *(tp+1) +
				*(tp1-1) + *(tp1) + *(tp1+1);


//			if( sum == 9 ){
			if( sum >= 4 ){
				gv->flag |= 0x02;
				gb->no += tS;

			}

		}
	}

	image_destroy( im, 1 );

	return( 1 );
}






void CDynamicBackground::Trace( FILE *fp )
{

	gpTime_print( fp, "Background-Update", &m_tBg );

	gpTime_print( fp, "Background-Silhouette", &m_tSh );
}


static void
bnImage_clear_0x02( image_type *sim, image_type *bim, int tVal, image_type *gvar )
{
	u_char	*sp,	*bp;
	int	i,	j,	n,	k,	align;

	int	av[3];

	align = 3*(sim->width - 8);


	bn_type *gb = (bn_type*)gvar->data;
	for( i = 0 ; i < gvar->height ; i++ ){


		for( j = 0 ; j < gvar->width ; j++, gb++ ){

			if( (gb->flag & 0x02) == 0 )	continue;

			sp = IMAGE_PIXEL( sim, i*8, j*8 );
			bp = IMAGE_PIXEL( bim, i*8, j*8 );

			av[0] = av[1] = av[2] = 0;
			for( k = 0 ; k < 8 ; k++, sp += align, bp += align ){
				for( n = 0 ; n < 8 ; n++ ){
					av[0] += *sp++ - *bp++;
					av[1] += *sp++ - *bp++;
					av[2] += *sp++ - *bp++;
				}
			}

			if( av[0] < 0 )	av[0] = -av[0];
			if( av[1] < 0 )	av[1] = -av[1];
			if( av[2] < 0 )	av[2] = -av[2];


			av[0] = av[0] >> 6;
			av[1] = av[1] >> 6;
			av[2] = av[2] >> 6;

			if( av[0] >tVal || av[1] > tVal || av[2]> tVal ){
				gb->flag &= ~0x02;
//				gb->no -= 1;
			}

		}
	}

}