/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
/**********************
 ***   ViewType.h   ***
 **********************/

#ifndef		_VEIW_TYPE_
#define		_VEIW_TYPE_

#ifdef __cplusplus
extern "C" {
#endif

#include	"Uigp/igp.h"
#include	"Uprm/prm.h"



#include	"Ujet2/Jet2Type.h"
#include	"Ujet2/Jet23Type.h"
#include	"Uvec/Vec3d.h"
#include	 "Umath/Plane/Plane3d.h"
#include	"Ubox/Box2d.h"
#include	"Uconfig3d/Config3d.h"



/* for view_type.type */
#define		VIEW_PARALLEL		1
#define		VIEW_PERSPECTIVE	2

#define		VIEW_MINUS_N			// direction of N


typedef struct view_type {
	int	type;

	vec3d	N;		/* normal vector to the view plane */

	vec3d	cp;		/* Center of projection */

	float	dis;		/* distance of cp from the View Plane */
	float	angle;

	vec3d	U,	V;	/* View Plane Axis */
	box2d	uv;


	/* rescaling factor win2pixel */
	int	Upixel,	Vpixel;
	float	Ua,	Ub,	Ua_inv;
	float	Va,	Vb,	Va_inv;



	/* AUX */
	float	INcp,	IUcp,	IVcp;
} view_type ;



#define		VIEW_ALLOC()	( (view_type*)malloc( sizeof(view_type) ) )


	/* ViewLoad.c */
void	view_loads( char *dir, char *fname, int min, int max,
				view_type *vw[], int *vw_no );

int		view_load( char *file, view_type *vw );


	/* ViewDump.c */
void	view_dumps( char *dir, char *fname, view_type *vw[], int vw_no );

void	view_dump( char *dir, char *fname, view_type *vw );


	/* ViewPerPrm.c */
void	view_per_get_prm( GP_prm **prm, int *prm_no, void (**check)() );

void	view_per_init( char *file );

void	view_per_set_lprm();

//void	view_per_set_in_prm( view_type *vw );


	/* ViewBasePrm.c */
void	view_base_get_prm( GP_prm **prm, int *prm_no, void (**check)() );

void	view_base_init( char *file );

void	view_base_set_lprm( );




	/* ViewPer.c */
void	view_per_set( view_type *vw, vec3d *cp, int Fpn, vec3d *pn,
					int Fv, vec3d *v, int Fw );

void 	view_set_pixel( view_type *vw, int row, int col );



	/* ViewJet.c */
void	view_per_jmap22( view_type *vw,
				float x0, float y0,  jet2 *f, jmap22 *Q );

void	view_per_jmap23( view_type *vw,
				float x0, float y0, jet23 *f, jmap23 *Q );

void	view_per_z_jet2( view_type *vw, float x0, float y0, jet23 *f, jet2 *z);


void	view_per3_jmap23( view_type *vw,
			jet23 *X, jet23 *Y, jet23 *Z, jmap23 *Q );

void	view_per3_z_jet2( view_type *vw, jet23 *X, jet23 *Y, jet23 *Z, jet2 *z);



	/* ViewPerNormal.c */
void	view_per_normal_base( view_type *bv, view_type *nbv );

void	view_per_normal( view_type *bv, view_type *vw, view_type *nvw );



	/* ViewTool.c */

void	view_project_point( view_type *vw, vec3d *p, vec2d *v );

float	view_project_depth( view_type *vw, float x0, float y0, float z0 );

void	view_3d_vector( view_type *vw, float u, float v, vec3d *n );

void	view_3d_point( view_type *vw, float u, float v, vec3d *p );

void	view_3d_pointD( view_type *vw, float u, float v, float depth, vec3d *p );

void	view_3d_ray( view_type *vw, float u, float v, vec3d *p, vec3d *n );

void	view_cut_point( view_type *vw, vec3d *p, vec3d *rp, vec2d *v );

void	view_cut_point_n( view_type *vw, vec3d *p, vec3d *n, vec2d *v );

//int	view_cut_point_ray( view_type *vw, float depth,
//					vec3d *p, vec3d *n, vec2d *v );

void	view_project_2pixel( view_type *vw, vec3d *p, vec2d *v );


void	view_vp2pixel( view_type *vw, vec2d *vp, vec2d *p );

void	view_pixel2vp( view_type *vw, vec2d *p, vec2d *vp );

void	view_pixel2plane_xyz( view_type *vw, vec2d *p,
					vec3d *P, vec3d *N, vec3d *p3 );
void	view_pixel2planeD_xyz( view_type *vw, vec2d *p, vec3d *N, float D, vec3d *p3 );



void	view_actoal_box( view_type *base, view_type *vw, box2d *box );
void	view_actoal_pixel_box( view_type *base, view_type *vw, box2d *box );

//void	view_actoal_depth_box( view_type *base, float depth,
//				view_type *vw, box2d *box );

//void	view_actoal_depth_pixel_box( view_type *base,
//				float depth0, float depth1,
//				view_type *vw, box2d *box );


//void	view_set_XY( view_type *vw, float x, float y, float z );

void	view_transform_vector( view_type *vw, vec3d *v, view_type *nvw, vec3d *nv );

void	view_transform_point( view_type *vw, vec3d *P, view_type *nvw, vec3d *nP );


int		view_centering( view_type *vw );



	/* ViewSub.c */
void	view_rescale( view_type *vw, float fk );

void	view_sub_upixel( view_type *bvw, int pixel, view_type *vw );

void	view_crop_pixel( view_type *vw, int row, int column );

void	view_sub_pixel( view_type *bvw, int row, int column, view_type *vw );

void	view_extend_pixel( view_type *vw, int row, int column );

void	view_sub_box( view_type *bvw, box2i *box,
			view_type *vw, int *row0, int *col0 );



	/* ViewPlane.c */
void	view_plane_v2pixel( view_type *vw, plane3f_type *pl );

void	view_plane_g2v( view_type *vw, vec3d *N, vec3d *P, plane3f_type *plane );



	/* ViewCreate.c */
void	view_create( view_type *vw, vec3d *cp, float Lv, int row, int column );

	/* ViewCreateNormal.c */
void	view_create_noraml( view_type *vw, float rf, int row, int column );

void	view_init_normal( view_type *vw, int width, int height, int worldUnit, float w0, float h0 );


	/* ViewCamera.c */
void	view_camera_shift( view_type *bvw, float dn, float duv, config3d *cm,
                                                        view_type *vw );

	/* ViewObject.c */
void    view_object_shift( view_type *bvw, vec3d *P,
		float dn, float duv, config3d *cm, view_type *vw, vec3d *dp );

	/* ViewObject1.c */
void    view_object_shift1( view_type *bvw, vec3d *P, vec3d *N,
		float dn, float duv, config3d *cm, view_type *vw, vec3d *dp );



	// ViewEulerAngle.c

void	view_euler_angle( view_type *vw, float *roll, float *yaw, float *pitch  );

void	view_euler_angle_set( view_type *vw, float roll, float yaw, float pitch  );

void	view_euler_angle_set_by_vector_roll( view_type *vw, vec3d *N, float roll );


int		view_force_vertical( view_type *vw );

void	view_rotate( view_type *vw, float roll, float yaw, float pitch  );


float	view_euler_pitch_yaw_dpixel( view_type *vw, float pitch_yaw );

float	view_euler_roll_dpixel( view_type *vw, float roll, float x, float y );



	// ViewRoll.c
float	view_roll( view_type *vw );

void	view_roll_set( view_type *vw, float angle  );

void	view_roll_rotate( view_type *vw, float angle  );



	// ViewConfig.c
void	view_set_config( view_type *vw, config3d *dc );

void	view_set_configD( view_type *vw, config3d *dc, view_type *nvw );

void	view_set_configV( view_type *vw, vec3d *VR, config3d *c );

void	view_set_configVD( view_type *vw, vec3d *VR, config3d *dc, view_type *nvw );



	// ViewEulerV.c

void	view_eulerV_set( view_type *vw, vec3d *VR, float roll, float yaw, float pitch  );

void	view_eulerV( view_type *vw, vec3d *VR, float *roll, float *yaw, float *pitch  );

void	view_eulerV_set_by_vector_roll( view_type *vw, vec3d *VR, vec3d *N, float roll );


#ifdef __cplusplus
}
#endif

#endif
