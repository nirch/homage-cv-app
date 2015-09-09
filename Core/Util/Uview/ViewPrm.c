/********************************************************
*							*
*		PerViewPrm.c				*
*							*
********************************************************/
#include	"Uprm/prm.h"
#include	"ViewType.h"


static vec3d   Ipn;	/* view plane normal    */

static vec3d   Icp;	/* center of projection */

static vec3d   Iv;	/* view up (v) vector       */

static int Fpn,	Fv,	Fw;

static int	Vpixel,	Upixel,	Vpixel0,	Upixel0;


view_type	Vw;


static GP_prm params[] = {
	{"V-type", (char *)&Vw.type,  PRM_INTEGER, "%d", "2"},
	{"V-Ang", (char *)&Vw.angle,  PRM_FLOAT, "%.2f", "15"},
	{"V-Dis", (char *)&Vw.dis,  PRM_FLOAT, "%.2f", "1"},

	{"V-Cp-x", (char *)&Icp.x,  PRM_FLOAT, "%.2f", "2"},
	{"V-Cp-y", (char *)&Icp.y,  PRM_FLOAT, "%.2f", "0.5"},
	{"V-Cp-z", (char *)&Icp.z,  PRM_FLOAT, "%.2f", "3"},


	{"V-pn-flag", (char *)&Fpn,  PRM_INTEGER, "%d", "0"},

	{"V-pn-x", (char *)&Ipn.x,  PRM_FLOAT, "%.2f", "0"},
	{"V-pn-y", (char *)&Ipn.y,  PRM_FLOAT, "%.2f", "0"},
	{"V-pn-z", (char *)&Ipn.z,  PRM_FLOAT, "%.2f", "0"},


	{"V-v-flag", (char *)&Fv,  PRM_INTEGER, "%d", "0"},

	{"V-v-x", (char *)&Iv.x,  PRM_FLOAT, "%.2f", "1"},
	{"V-v-y", (char *)&Iv.y,  PRM_FLOAT, "%.2f", "1"},
	{"V-v-z", (char *)&Iv.z,  PRM_FLOAT, "%.2f", "1"},

	{"V-w-flag", (char *)&Fw,  PRM_INTEGER, "%d", "0"},
	{"V-U0", (char *)&Vw.uv.x0,  PRM_FLOAT, "%.2f", "-1"},
	{"V-U1", (char *)&Vw.uv.x1,  PRM_FLOAT, "%.2f", "1"},
	{"V-V0", (char *)&Vw.uv.y0,  PRM_FLOAT, "%.2f", "-1"},
	{"V-V1", (char *)&Vw.uv.y1,  PRM_FLOAT, "%.2f", "1"},

/*	view plane pixel */
	{"V-pixel", (char *)&Vpixel,  PRM_INTEGER, "%d", "100"},
	{"U-pixel", (char *)&Upixel,  PRM_INTEGER, "%d", "100"},
	{"V-p0", (char *)&Vpixel0,  PRM_INTEGER, "%d", "0"},
	{"U-p0", (char *)&Upixel0,  PRM_INTEGER, "%d", "0"},
};


void
view_per_get_prm( GP_prm **prm, int *prm_no, void (**check)() )
{
	*prm = params;
	*prm_no = PRM_NUMBER(params);
	*check = NULL ;
}


void
view_per_init( char *file )
{
	gp_prm_init( file, view_per_get_prm );
	view_per_set_lprm();
}


void
view_per_set_lprm()
{
	view_per_set( &Vw, &Icp, Fpn, &Ipn, Fv, &Iv, Fw );

	Vw.Ua = Upixel/(Vw.uv.x1 - Vw.uv.x0 );
	Vw.Ub = -Vw.Ua * Vw.uv.x0 - Upixel0;
	Vw.Ua_inv = 1.0/Vw.Ua;
	Vw.Upixel = Upixel;


	Vw.Va = Vpixel/(Vw.uv.y0 - Vw.uv.y1 );
	Vw.Vb = -Vw.Va * Vw.uv.y1 - Vpixel0;
	Vw.Va_inv = 1.0/Vw.Va;
	Vw.Vpixel = Vpixel;

}


//void
//view_per_set_in_prm( view_type *vw )
//{
//	Icp = vw->cp;
//	if( Fpn == 0 )
//		Ipn = vw->rp;
//	else	Ipn = vw->N;
//}
