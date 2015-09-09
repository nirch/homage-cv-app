/***********************************************
 ***    ViewPer.c: perspective projection    ***
 ***********************************************/
#include	<math.h>
#include	"Uvec/Vec3d.h"
#include	"Ujet2/Jet2Type.h"
#include	"Ujet2/Jet23Type.h"

#include	"ViewType.h"


void
view_per_set( view_type *vw, vec3d *cp, int Fpn, vec3d *pn,
					int Fv, vec3d *v, int Fw )
{
float	t;

	vw->cp = *cp;

	/* set  Vpn */
	if( Fpn == 0 ){
		vw->N.x = pn->x - cp->x;
		vw->N.y = pn->y - cp->y;
		vw->N.z = pn->z - cp->z;
	}
	else	vw->N = *pn;


	t = 1/VEC3D_NORM( vw->N );
	vw->N.x *= t;
	vw->N.y *= t;
	vw->N.z *= t;







	/* set  V */
	if( Fv == 0 ){
		if( vw->N.x != 0 || vw->N.y != 0 ){
			VEC3D_SET( vw->V, 0, 0, 1 );
		}
		else	VEC3D_SET( vw->V, 0, 1, 0 );
	}
	else	vw->V = *v;


	t = -VEC3D_INNER( vw->V, vw->N );
        vw->V.x += t * vw->N.x;
        vw->V.y += t * vw->N.y;
        vw->V.z += t * vw->N.z;

	t = 1/VEC3D_NORM( vw->V );
	vw->V.x *= t;
	vw->V.y *= t;
	vw->V.z *= t;


	/* set U */
	VEC3D_PRODUCT( vw->N, vw->V, vw->U );



	if( Fw == 0 ){
		t = vw->dis * sin( vw->angle * M_PI/180.0 );
		vw->uv.x0 = vw->uv.y0 = -t;
		vw->uv.x1 = vw->uv.y1 =  t;
	}



	vw->INcp = VEC3D_INNER( vw->N, vw->cp );
	vw->IUcp = VEC3D_INNER( vw->U, vw->cp );
	vw->IVcp = VEC3D_INNER( vw->V, vw->cp );
}



void
view_set_pixel( view_type *vw, int row, int col )
{
    vw->Upixel = col;
    vw->Ua = vw->Upixel/(vw->uv.x1 - vw->uv.x0 );
    vw->Ub = -vw->Ua * vw->uv.x0;
    vw->Ua_inv = 1.0/vw->Ua;


    vw->Vpixel = row;
    vw->Va = vw->Vpixel/(vw->uv.y0 - vw->uv.y1 );
    vw->Vb = -vw->Va * vw->uv.y1;
    vw->Va_inv = 1.0/vw->Va;
}
