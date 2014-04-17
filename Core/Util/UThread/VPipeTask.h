/***
***    author:		Yoram Elichai   yorame@gmail.com
***    Copyright (C) 2000 - All Rights Reserved
***/
#ifndef __VPIPE_TASK__H_
#define __VPIPE_TASK__H_


#include "UThread/VThread.h"
#include "Uthread/VSemaphore.h"






class CVPipeTask :public CVThread
{

public:
	CVPipeTask();

	~CVPipeTask(void);


	int Start();

	int SetIn();

	int WaitForOut();


private:
	void		Run();


private:


	CVSemaphore m_semIn;
	CVSemaphore m_semOut;
};

#endif __VPIPE_TASK__H_