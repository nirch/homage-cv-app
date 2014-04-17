/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __VMOUSE_EVENT_
#define __VMOUSE_EVENT_


#include "UThread/VThread.h"



class CVMouseEvent :public CVThread
{
private:

public:
	CVMouseEvent( );
	~CVMouseEvent(void);


	int	Move( int x, int y );

private:
	void		Run();

	int MoveTo( int x, int y );
	int SetPos ( int x, int y );


private:

	CVEvent	m_eTimer;
	int m_wait;

	int	m_dt;

	int	m_x0;
	int	m_y0;

	int m_inX;
	int m_inY;
};

#endif __AVI_WRITER__H_