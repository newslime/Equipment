
#include "GemSDK.h"

GemSender::GemSender(void* parent_class, BS2Device* device) : BS2Sender(device)
{
	m_parent_class = parent_class;
}

GemSender::~GemSender()
{

}

int GemSender::open(void *)
{
	int ret;
	
	ret = BS2Sender::open();

	return ret;
}

int GemSender::close(u_long exit_status)
{
	int ret;
	
	ret = BS2Sender::close(exit_status);

	return ret;
}

int GemSender::parse(BCHAR* data, int size)
{
	int ret;
	
	ret = BS2Sender::parse(data, size);

	return ret;
}

// enqueue the message
int GemSender::put(ACE_Message_Block *mb, ACE_Time_Value *tv)
{
	int ret;
	
	ret = BS2Sender::put(mb, tv);

	return ret;
}

// send the message
int GemSender::send(BS2Message* msg, void* ptr)
{
	int ret;
	
	ret = BS2Sender::send(msg, ptr);

	return ret;
}

// send the message
int GemSender::dispatch(BS2OStream * buff, void * ptr)
{
	int ret;
	
	ret = BS2Sender::dispatch(buff, ptr);

	return ret;
}

// Receive data from message queue.
int GemSender::svc(void)
{
	int ret;
	
	ret = BS2Sender::svc();


	return ret;
}

   
