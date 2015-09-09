/********************************************************
*							*
*		PerViewPrm.c				*
*							*
********************************************************/
#include	"Uprm/prm.h"
#include	"ViewType.h"


int	Type;

float	Angle,	Dis;

static vec3d   Ipn;	/* view plane normal    */

static vec3d   Icp;	/* center of projection */

static vec3d   Iv;	/* view up (v) vector       */

static int Fpn,	Fv,	Fw;

static box2d	Uv;
static int	Vpixel,	Upixel,	Vpixel0,	Upixel0;


static void	view_load_get_prm( GP_prm **prm, int *prm_no, void (**check)());


static GP_prm params[] = {
	{"V-type", (char *)&Type,  PRM_INTEGER, "%d", "2"},
	{"V-Ang", (char *)&Angle,  PRM_FLOAT, "%.2f", "15"},
	{"V-Dis", (char *)&Dis,  PRM_FLOAT, "%.2f", "1"},

	{"V-Cp-x", (char *)&Icp.x,  PRM_FLOAT, "%.2f", "2"},
	{"V-Cp-y", (char *)&Icp.y,  PRM_FLOAT, "%.2f", "0.5"},
	{"V-Cp-z", (char *)&Icp.z,  PRM_FLOAT, "%.2f", "3"},


	{"V-N-flag", (char *)&Fpn,  PRM_INTEGER, "%d", "0"},

	{"V-N-x", (char *)&Ipn.x,  PRM_FLOAT, "%.2f", "0"},
	{"V-N-y", (char *)&Ipn.y,  PRM_FLOAT, "%.2f", "0"},
	{"V-N-z", (char *)&Ipn.z,  PRM_FLOAT, "%.2f", "0"},


	{"V-v-flag", (char *)&Fv,  PRM_INTEGER, "%d", "0"},

	{"V-v-x", (char *)&Iv.x,  PRM_FLOAT, "%.2f", "1"},
	{"V-v-y", (char *)&Iv.y,  PRM_FLOAT, "%.2f", "1"},
	{"V-v-z", (char *)&Iv.z,  PRM_FLOAT, "%.2f", "1"},

	{"V-w-flag", (char *)&Fw,  PRM_INTEGER, "%d", "0"},
	{"V-U0", (char *)&Uv.x0,  PRM_FLOAT, "%.2f", "-1"},
	{"V-U1", (char *)&Uv.x1,  PRM_FLOAT, "%.2f", "1"},
	{"V-V0", (char *)&Uv.y0,  PRM_FLOAT, "%.2f", "-1"},
	{"V-V1", (char *)&Uv.y1,  PRM_FLOAT, "%.2f", "1"},

/*	view plane pixel */
	{"V-pixel", (char *)&Vpixel,  PRM_INTEGER, "%d", "100"},
	{"U-pixel", (char *)&Upixel,  PRM_INTEGER, "%d", "100"},
	{"V-p0", (char *)&Vpixel0,  PRM_INTEGER, "%d", "0"},
	{"U-p0", (char *)&Upixel0,  PRM_INTEGER, "%d", "0"},
};


static void
view_load_get_prm( GP_prm **prm, int *prm_no, void (**check)() )
{
	*prm = params;
	*prm_no = PRM_NUMBER(params);
	*check = NULL ;
}


void
view_loads( char *dir, char *fname, int min, int max,
				view_type *vw[], int *vw_no )
{
char	file[256],	name[256];
int	i;

	for( i = min, *vw_no = 0 ; max == -1 || i <= max ; i++, (*vw_no)++ ){

		sprintf( name, "%s-%d", fname, i );
		sprintf( file, "%s/%s.view", dir, name );

		if( gpFile_exist(file) == 0 )	return;

		vw[i] = VIEW_ALLOC();
		view_load( file, vw[i] );
	}
}



int
view_load( char *file, view_type *vw )
{
	if( gp_prm_init( file, view_load_get_prm ) < 0 )
		return( -1 );


	vw->type = Type;
	vw->angle = Angle;
	vw->dis = Dis;

	vw->uv = Uv;

	view_per_set( vw, &Icp, Fpn, &Ipn, Fv, &Iv, Fw );

	vw->Ua = Upixel/(vw->uv.x1 - vw->uv.x0 );
	vw->Ub = -vw->Ua * vw->uv.x0 - Upixel0;
	vw->Ua_inv = 1.0/vw->Ua;
	vw->Upixel = Upixel;


	vw->Va = Vpixel/(vw->uv.y0 - vw->uv.y1 );
	vw->Vb = -vw->Va * vw->uv.y1 - Vpixel0;
	vw->Va_inv = 1.0/vw->Va;
	vw->Vpixel = Vpixel;

	return( 1 );
}
