#include	<math.h>
#include	"ImageType/ImageType.h"
#include	"ImageType/ImageTool.h"




void image_add_logo( image_type *sim, image_type *logo, int x, int y, int opacity )
{
u_int	*sp;
u_int	*tp;
int	i,	j,	align;
int	t;
float	dt;
int	color,	r,	g,	b,	r1,	g1,	b1,	r2,	g2,	b2,	r3,	g3,	b3;

	dt = opacity / 100.0;

	sp = IMAGE4_PIXEL( sim, y, x );
	tp = (u_int *)logo->data;
	align = sim->column - logo->column;

	for( i = 0 ; i < logo->row ; i++, sp += align )
	{
		for( j = 0 ; j < logo->column ; j++ )
		{
			t = ( *tp >> 24 ) & 0xff;
			if( t == 255 ){
				sp++;
				tp++;
				continue;
			}


			if( opacity == 100 && t == 0 )
				*sp++ = *tp++ & 0x00FFFFFF;

			else {
				r = IMAGE4_RED( *sp );
				g = IMAGE4_GREEN( *sp );	
				b = IMAGE4_BLUE( *sp );
				r1 = IMAGE4_RED( *tp & 0x00FFFFFF );
				g1 = IMAGE4_GREEN( *tp & 0x00FFFFFF );	
				b1 = IMAGE4_BLUE( *tp & 0x00FFFFFF );

				dt = (opacity / 100.0 ) * ((255-t) / 255.0 ); 
			
				r2 = r + (r1-r)*dt;
				g2 = g + (g1-g)*dt;
				b2 = b + (b1-b)*dt;
			
				r3 = PUSH_TO_RANGE( r2, 0, 255 );
				g3 = PUSH_TO_RANGE( g2, 0, 255 );
				b3 = PUSH_TO_RANGE( b2, 0, 255 );

				color = IMAGE4_RGB( r3, g3, b3 );

				*sp = color;
				sp++;
				tp++;
			}

		}
	}
}
