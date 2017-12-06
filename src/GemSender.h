#ifndef GEMHSMS_SENDER_H
#define GEMHSMS_SENDER_H

class GemSender: public BS2Sender
{
 public:
    GemSender(void* parent_class, BS2Device* device);
    virtual ~GemSender();

	virtual int open(void * = 0);
    virtual int close(u_long = 0);

    virtual int parse(BCHAR * data, int size);
    virtual int put(ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
    virtual int send(BS2Message * msg, void * ptr = NULL);
	virtual int dispatch(BS2OStream * buff, void * ptr = NULL);
    virtual int svc(void);

	void*	m_parent_class;
};

#endif