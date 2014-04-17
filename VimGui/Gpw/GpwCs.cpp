
#include        <math.h>

#include	"GpwType.h"


void
gpw_cs( gpw_type *gpw, float x, float y )
{
int	wx,	wy;
int	color;

	gpw_cs_unshow( gpw );

	gpw_xy2win( gpw, x, y, &wx, &wy );

	if( wx < gpw->WXmin-10 || wx >= gpw->WXmax+10 ||
			wy < gpw->WYmin-10 || wy >= gpw->WYmax+10 )
		return;


	gpw->cs_x = wx;
	gpw->cs_y = wy;



 	gpw->cs[0] = GetPixel( gpw->dc, wx-1, wy);
	gpw->cs[1] = GetPixel( gpw->dc, wx, wy-1);
	gpw->cs[2] = GetPixel( gpw->dc, wx, wy);
	gpw->cs[3] = GetPixel( gpw->dc, wx, wy+1);
	gpw->cs[4] = GetPixel( gpw->dc, wx+1, wy);

	color = GREEN;
	SetPixel( gpw->dc, wx-1, wy, (COLORREF)color );
	SetPixel( gpw->dc, wx, wy-1, (COLORREF)color );
	SetPixel( gpw->dc, wx, wy, (COLORREF)color );
	SetPixel( gpw->dc, wx, wy+1, (COLORREF)color );
	SetPixel( gpw->dc, wx+1, wy, (COLORREF)color );


	gpw->Fcs = 1;
 
}
 


	

void
gpw_cs_unshow( gpw_type *gpw )
{
int	wx,	wy;

	if( gpw->Fcs == 0 )	return;

	wx = gpw->cs_x;
	wy = gpw->cs_y;
	SetPixel( gpw->dc, wx-1, wy, (COLORREF)gpw->cs[0] );
	SetPixel( gpw->dc, wx, wy-1, (COLORREF)gpw->cs[1] );
	SetPixel( gpw->dc, wx, wy, (COLORREF)gpw->cs[2] );
	SetPixel( gpw->dc, wx, wy+1, (COLORREF)gpw->cs[3] );
	SetPixel( gpw->dc, wx+1, wy, (COLORREF)gpw->cs[4] );


	gpw->Fcs = 0;
}