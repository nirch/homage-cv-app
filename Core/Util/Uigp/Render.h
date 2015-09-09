#ifndef		_RENDER_H_
#define		_RENDER_H_

#include	"Uigp/igp.h"
#include	"Uview/ViewType.h"

#include	"GpType/GpType.h"

#include	"SklType/LnkType.h"


	/* RenderAgp.c */
void	render_agp( view_type *base, gp_array *agp, float depth0, float depth1,
			view_type *vw, gp_array *nagp );


	// RenderAuxTool.c
void	render_point( view_type *base, vec2d *p, float depth,
				view_type *vw, vec2d *np, float *ndepth );

void	render_point_inv( view_type *vw, vec2d *np, float depth,
                                                view_type *base, vec2d *p );


//void	renderP_point( view_type *base, vec2d *p, float depth,
//			  view_type *vw, vec2d *np, float *ndepth );




	// RenderPr.c
void	render_pr_agp( view_type *base, gp_array *agp,
						view_type *vw, gp_array *nagp );

void	render_prP_agp( view_type *base, gp_array *agp,
				view_type *vw, gp_array *nagp );


	// RenderPrAux.c
void	render_pr_aux( gp_array *agp );




	// RenderPl.c
void	render_pl_agp( view_type *base, gp_array *agp,
						view_type *vw, gp_array *nagp );


	// RenderPlAux.c
void	render_pl_aux( gp_array *agp, lnk_type *link[], int no );




	// RenderPlw.c
void	render_plw_agp( view_type *base, gp_array *agp,
				view_type *vw, gp_array *nagp );

	// RenderPlwAux.c
void	render_plw_aux( gp_array *agp, lnk_type **aL[], int nL[], int no );



	/* hyputA.c */
float	hypotA( float x, float y );



	/* RenderAgpClear.c */
void	render_agp_clear( gp_array *agp, int bgp, int row, int column, mbr_type Sbr);



	/* RenderAux.c */
void	render_aux_free( gp_array *agp );






	// RenderAuxPosition.c   use by SklData and DepthData
void	render_aux_position( gp_array *agp );

void	render_aux_position_restore( gp_array *agp );



#endif
