#ifndef CFreeLineDraw_h
#define CFreeLineDraw_h


#include "Uln/LnType.h"
#include "Uln/PlnType.h"
#include "UGeoApp/GeoApp.h"





class CFreeLineDraw
{
private:
	class CGpw *m_gpw;
	gapp_type	*m_Gapp;
	ln_type		*m_Link, *m_PolyLink, *m_LastLink;
	int m_type;
	int m_No;

	vec2f_type m_BeginVec;
	vec2f_type m_EndVec;


	int	m_color;

public:


	CFreeLineDraw( vec2f_type *p, CGpw *gpw );
	~CFreeLineDraw();

	void	Draw();

	void	Move( vec2f_type *p );
    
	ln_type *Finish( vec2d point);
	ln_type *GetLine();

	void GetEndDirection( vec2d *tanget );

	bool	test1( vec2d *p );
	bool	test2( vec2d *p );

	gapp_prm m_Fgprm;
	void setFgprmUnit();
	float m_Unit;// how many agp units are in 1 pixel unit.
};



#endif //CFreeLineDraw_h