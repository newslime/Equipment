#ifndef GEMHSMS_SECSDEVICE_H
#define GEMHSMS_SECSDEVICE_H

#ifndef GEMSDK_PROTECT
#pragma message("please include GemSDK.h instead of me")
#endif

class GemDevice: public BS2Device
{
 public:
    GemDevice(void* parent_class);
    virtual ~GemDevice();
	virtual int open(void * = 0);
    virtual int close(int = 0);
	virtual int parse(void* data, int size);
    virtual int put(ACE_Message_Block *mb, ACE_Time_Value *tv = 0);
    virtual int svc(void);
    virtual int connected();
    virtual int disconnected();
	
//////////////
	void*				m_parent_class;
	//wxCriticalSection	m_mutex_gemdevice;

private:
	int m_ParseStatus;
};

#endif