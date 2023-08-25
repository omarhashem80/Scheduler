#ifndef _UI
#define _UI
#include"Scheduler.h"
class UI
{
private :
	static int mode;
public:
	static void updatainterface(Scheduler*sptr, Processor** PRL, LinkedQueue<Process*>TERM, LinkedQueue<Process*>BLK,LinkedQueue<Process*>Waiting);
};
#endif

