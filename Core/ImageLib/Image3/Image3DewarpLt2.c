/*******************************
 ***   Image3Interpolate.c   ***
 *******************************/
#include	<math.h>

#include "Umath/LT2Type.h"

#include	"ImageType/ImageType.h"
#include	"Image3Tool.h"


// tim(x,y) = sim( lt(x,y) )
image_type *
image3_dewarp_lt2S( image_type *sim, lt2_type *lt, float scale, image_type *tim )
{
int	i,	j;
u_char	*sp,	*sp1;
u_char	*tp;
float	y,	x,	dx,	dy;
float	s1,	s2;
int	s;
int	iy,	ix;
float	iscale;
float	x0,	y0;
int	width,	height;

		
	iscale = 1.0 / scale;

	if( tim == NULL ){
		width = iscale * sim->column;
		height = iscale* sim->row;
		tim = image_create( height, width, 3, 1, NULL );
	}



	for( i = 0 ; i < tim->row ; i++ ){
		tp = IMAGE_PIXEL( tim, i, 0 );
		y0 = iscale*(i+0.5);
		for( j = 0 ; j < tim->column ; j++ ){

			x0 = iscale*(j+0.5);

			x = LT2_F1( *lt, x0, y0 );
			y = LT2_F2( *lt, x0, y0 );

			ix = x-0.5;
			iy = y-0.5;



			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				continue;
			}

			
			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			sp = IMAGE_PIXEL( sim, iy, ix );
			sp1 = sp + 3*sim->column;

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			// Red
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			//Green
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			// Blue
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;
		}
	}

	return( tim );
}



#define UNKNOWN_FLOW_THRESH 1e9


// tim(x,y) = sim( fim(x,y) )
image_type *
image3_dewarp_flow( image_type *sim, image_type *fim, image_type *im )
{
int	i,	j;
u_char	*sp,	*sp1;
u_char	*tp;
float	*fp;
float	y,	x,	dx,	dy;
float	s1,	s2;
int	s;
int	iy,	ix;


	im = image_recreate( im, sim->height, sim->width, 3, 1 );


	fp = fim->data_f;

	tp = im->data;

	for( i = 0 ; i < im->row ; i++ ){
		for( j = 0 ; j < sim->column ; j++ ){

			x = *fp++;
			y = *fp++;

			if( ABS(x) > UNKNOWN_FLOW_THRESH || ABS(y) > UNKNOWN_FLOW_THRESH ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				continue;
			}

			x += j+0.5;
			y += i+0.5;

			ix = x-0.5;
			iy = y-0.5;



			if( y < 0.5 || iy > sim->row-1-0.5 ||  x < 0.5 || x > sim->column-1-0.5 ){
				*tp++ = 0;
				*tp++ = 0;
				*tp++ = 0;
				continue;
			}


			dx = x - (ix+0.5);
			dy = y - (iy+0.5);

			sp = IMAGE_PIXEL( sim, iy, ix );
			sp1 = sp + 3*sim->column;

			dx = x - (ix+0.5);
			dy = y - (iy+0.5);


			// Red
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			//Green
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;

			// Blue
			s1 = (1-dx)* (*sp) + dx * (*(sp+3));
			s2 = (1-dx)* (*sp1) + dx * (*(sp1+3));
			s = (1-dy)*s1 + dy * s2 + 0.5;
			*tp++ = s;
			sp++; sp1++;
		}
	}

	return( im );
}





// tim(x,y) = sim( lt(x,y) )
image_type *
image3_dewarp_flow_forword( image_type *sim, image_type *fim, image_type *im )
{
image_type	*wim,	*tim[3];
float	*tp,	*wp,	*tp0,	*tp1,	*tp2;
u_char	*sp;
int	tmp,	x,	y;
float	fx,	fy,	*fp;
float	dst_x,	dst_y,	rem_x,	rem_y,	w;
int	idst_x,	idst_y;
	
	im = image_recreate( im, sim->row, sim->column, 3, 1 );


	wim = image_create( sim->row, sim->column, 4, 1, NULL );
	imageF_const( wim, 0 );


	tim[0] = image_create( sim->row, sim->column, 4, 1, NULL );
	imageF_const( tim[0], 0 );
	tim[1] = image_create( sim->row, sim->column, 4, 1, NULL );
	imageF_const( tim[1], 0 );
	tim[2] = image_create( sim->row, sim->column, 4, 1, NULL );
	imageF_const( tim[2], 0 );




	fp = fim->data_f;
	sp = sim->data;
	for( y = 0 ; y < sim->row ; y++ ){
		for( x = 0 ; x < sim->column ; x++, sp+=3 ){
			fx = *fp++;
			fy = *fp++;

			if( ABS(fx) > UNKNOWN_FLOW_THRESH || ABS(fy) > UNKNOWN_FLOW_THRESH ){
				continue;
			}

			dst_x = x+0.5 + fx;
			dst_y = y+0.5 + fy;

	
			idst_x = ((int)(dst_x+1)) - 1;
			idst_y = ((int)(dst_y+1)) - 1;




			rem_x = dst_x - idst_x;
			rem_y = dst_y - idst_y;
	


			if (idst_x>=0 && idst_x<tim[0]->column){
				if (idst_y>=0 && idst_y<tim[0]->row){
					wp = (float*)IMAGE_PIXEL( wim, idst_y, idst_x );
					w = (1-rem_x)*(1-rem_y);
					*wp += w;


					tp = (float*)IMAGE_PIXEL( tim[0], idst_y, idst_x );
					*tp += w*sp[0];
					tp = (float*)IMAGE_PIXEL( tim[1], idst_y, idst_x );
					*tp += w*sp[1];
					tp = (float*)IMAGE_PIXEL( tim[2], idst_y, idst_x );
					*tp += w*sp[2];
				}

				if ((idst_y+1)>=0 && (idst_y+1)<tim[0]->row){
					wp = (float*)IMAGE_PIXEL( wim, idst_y+1, idst_x );
					w = (1-rem_x)*rem_y;

					tp = (float*)IMAGE_PIXEL( tim[0], idst_y+1, idst_x );
					*tp += w*sp[0];
					tp = (float*)IMAGE_PIXEL( tim[1], idst_y+1, idst_x );
					*tp += w*sp[1];
					tp = (float*)IMAGE_PIXEL( tim[2], idst_y+1, idst_x );
					*tp += w*sp[2];
					*wp += w;
				}
			}
			if (idst_x+1>=0 && idst_x+1<tim[0]->column )
			{
				if (idst_y>=0 && idst_y<tim[0]->row)
				{
					wp = (float*)IMAGE_PIXEL( wim, idst_y, idst_x+1 );
					w = rem_x*(1-rem_y);


					tp = (float*)IMAGE_PIXEL( tim[0], idst_y, idst_x+1 );
					*tp += w*sp[0];
					tp = (float*)IMAGE_PIXEL( tim[1], idst_y, idst_x+1 );
					*tp += w*sp[1];
					tp = (float*)IMAGE_PIXEL( tim[2], idst_y, idst_x+1 );
					*tp += w*sp[2];

					*wp += w;
				}
				if ((idst_y+1)>=0 && (idst_y+1)<tim[0]->row)
				{
					wp = (float*)IMAGE_PIXEL( wim, idst_y+1, idst_x+1 );
					w = rem_x*rem_y;

					tp = (float*)IMAGE_PIXEL( tim[0], idst_y+1, idst_x+1 );
					*tp += w*sp[0];
					tp = (float*)IMAGE_PIXEL( tim[1], idst_y+1, idst_x+1 );
					*tp += w*sp[1];
					tp = (float*)IMAGE_PIXEL( tim[2], idst_y+1, idst_x+1 );
					*tp += w*sp[2];

					*wp += w;
				}
			}
		}
	}




	tp0 = (float *)tim[0]->data;
	tp1 = (float *)tim[1]->data;
	tp2 = (float *)tim[2]->data;


	wp = (float *)wim->data;
	sp = im->data;
	for( y = 0 ; y < sim->row ; y++ ){
		for( x = 0 ; x < sim->column ; x++, sp += 3, tp0++, tp1++, tp2++, wp++ ){
			if( *wp != 0 ){
				tmp = *tp0 / *wp;
				sp[0] = PUSH_TO_RANGE( tmp, 0, 255 );


				tmp = *tp1 / *wp;
				sp[1] = PUSH_TO_RANGE( tmp, 0, 255 );

				tmp = *tp2 / *wp;
				sp[2] = PUSH_TO_RANGE( tmp, 0, 255 );

			}
			else	*sp = 0;
		}
	}





	image_destroy( tim[0], 1 );
	image_destroy( tim[1], 1 );
	image_destroy( tim[2], 1 );



	image_destroy( wim, 1 );

	return( im );
}