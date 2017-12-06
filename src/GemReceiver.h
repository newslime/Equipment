#ifndef GEMHSMS_RECEIVER_H
#define GEMHSMS_RECEIVER_H

class GemReceiver: public BS2Receiver
{
 public:
    GemReceiver(void* parent_class, BS2Device* device);
    virtual ~GemReceiver();

	virtual int open(void * = 0);
    virtual int close(u_long = 0);
	
    virtual int parse(BCHAR * data, int size);
    virtual int put(ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
	virtual int dispatch(BCHAR * data, int size);
    virtual int svc(void);

	void*	m_parent_class;
};

#endif