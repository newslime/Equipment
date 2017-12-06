
#include "GemSDK.h"

GemReceiver::GemReceiver(void* parent_class, BS2Device* device) : BS2Receiver(device)
{
	m_parent_class = parent_class;
}

GemReceiver::~GemReceiver()
{

}

int GemReceiver::open(void *)
{
	int ret;
	
	ret = BS2Receiver::open();

	return ret;
}

int GemReceiver::close(u_long exit_status)
{
	int ret;
	
	ret = BS2Receiver::close(exit_status);

	return ret;
}

int GemReceiver::parse(BCHAR* data, int size)
{
	int ret;
	
	ret = BS2Receiver::parse(data, size);
	
	return ret;
}

int GemReceiver::put(ACE_Message_Block* mb, ACE_Time_Value* tv)
{
	int ret;
	
	ret = BS2Receiver::put(mb, tv);

	return ret;
}

int GemReceiver::dispatch(BCHAR* data, int size)
{
	int ret;
	
	ret = BS2Receiver::dispatch(data, size);

	return ret;
}

int GemReceiver::svc(void)
{
	int ret;
	
	ret = BS2Receiver::svc();

	return ret;
}

