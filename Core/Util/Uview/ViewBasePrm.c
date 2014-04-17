/*************************
 ***   BaseViewPrm.c   ***
 *************************/
#include	"Uprm/prm.h"
#include	"ViewType.h"


static vec3d   Ipn;    /* view plane normal    */

static vec3d   Icp;    /* center of projection */

static vec3d   Iv;    /* view up (v) vector       */

static int Fpn,	Fv,	Fw;
static int	Vpixel,	Upixel,	Vpixel0,	Upixel0;


view_type	Bvw;


static GP_prm params[] = {
	{"V-type", (char *)&Bvw.type,  PRM_INTEGER, "%d", "2"},
	{"V-Ang", (char *)&Bvw.angle,  PRM_FLOAT, "%.2f", "30"},
	{"V-Dis", (char *)&Bvw.dis,  PRM_FLOAT, "%.2f", "1000"},

	{"V-Cp-x", (char *)&Icp.x,  PRM_FLOAT, "%.2f", "0"},
	{"V-Cp-y", (char *)&Icp.y,  PRM_FLOAT, "%.2f", "0"},
	{"V-Cp-z", (char *)&Icp.z,  PRM_FLOAT, "%.2f", "1000.0"},


	{"V-pn-flag", (char *)&Fpn,  PRM_INTEGER, "%d", "0"},

	{"V-pn-x", (char *)&Ipn.x,  PRM_FLOAT, "%.2f", "0"},
	{"V-pn-y", (char *)&Ipn.y,  PRM_FLOAT, "%.2f", "0"},
	{"V-pn-z", (char *)&Ipn.z,  PRM_FLOAT, "%.2f", "0"},


	{"V-v-flag", (char *)&Fv,  PRM_INTEGER, "%d", "0"},

	{"V-v-x", (char *)&Iv.x,  PRM_FLOAT, "%.2f", "0"},
	{"V-v-y", (char *)&Iv.y,  PRM_FLOAT, "%.2f", "1"},
	{"V-v-z", (char *)&Iv.z,  PRM_FLOAT, "%.2f", "0"},

	{"V-w-flag", (char *)&Fw,  PRM_INTEGER, "%d", "1"},
	{"V-U0", (char *)&Bvw.uv.x0,  PRM_FLOAT, "%.2f", "-1"},
	{"V-U1", (char *)&Bvw.uv.x1,  PRM_FLOAT, "%.2f", "1"},
	{"V-V0", (char *)&Bvw.uv.y0,  PRM_FLOAT, "%.2f", "-1"},
	{"V-V1", (char *)&Bvw.uv.y1,  PRM_FLOAT, "%.2f", "1"},

/*	view plane pixel */
	{"V-pixel", (char *)&Vpixel,  PRM_INTEGER, "%d", "100"},
	{"U-pixel", (char *)&Upixel,  PRM_INTEGER, "%d", "100"},
	{"V-p0", (char *)&Vpixel0,  PRM_INTEGER, "%d", "0"},
	{"U-p0", (char *)&Upixel0,  PRM_INTEGER, "%d", "0"},
};


void
view_base_get_prm( GP_prm **prm, int *prm_no, void (**check)() )
{
	*prm    = params;
	*prm_no  = PRM_NUMBER(params);
	*check  = NULL ;
}


void
view_base_init( char *file )
{
	gp_prm_init( file, view_base_get_prm );

	view_base_set_lprm();
}


void
view_base_set_lprm( )
{
	view_per_set( &Bvw, &Icp, Fpn, &Ipn, Fv, &Iv, Fw );


	Bvw.Ua = Upixel/(Bvw.uv.x1 - Bvw.uv.x0 );
	Bvw.Ub = -Bvw.Ua * Bvw.uv.x0 - Upixel0;
	Bvw.Ua_inv = 1.0/Bvw.Ua;
	Bvw.Upixel = Upixel;


	Bvw.Va = Vpixel/(Bvw.uv.y0 - Bvw.uv.y1 );
	Bvw.Vb = -Bvw.Va * Bvw.uv.y1 - Vpixel0;
	Bvw.Va_inv = 1.0/Bvw.Va;
	Bvw.Vpixel = Vpixel;
}
