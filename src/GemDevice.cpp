
#include "GemSDK.h"

GemDevice::GemDevice(void* parent_class)
{
	m_parent_class	= parent_class;
}

GemDevice::~GemDevice()
{
	this->close(-1);
}

int GemDevice::open(void *)
{
	int ret;
	
	ret = BS2Device::open();

	return ret;
}

int GemDevice::close(int exit_status)
{
	int ret;
	
	ret = BS2Device::close(exit_status);

	return ret;
}

int GemDevice::parse(void* data, int size)
{
	int						result;
	int						stnum;
	int						fcnum;
	int						isConnected;
	BS2BlockHeader			header;
	BS2Message*				msg_body;
	BS2MessageInfo*			binfo;
	BS2TransactionInfo*		trinfo;
	BS2TransactionManager*	trmgr		= getTransactionManager();
	BS2MessageInfo*			binfo_get	= NULL;
	//GemDriver*				parent		= (GemDriver*)m_parent_class;

	isConnected = ((BS2Socket*)this->getDriver())->getStatus();

	switch(isConnected)
	{
	case 0:
		m_ParseStatus = 0;
		GemHsmsCall::Get()->process();
		break;

	case 2:
		//GemHsmsCall::Get()->process();

		result = m_receiver->getEventInfo(binfo_get);

		if( result == BEE_SUCCESS )
		{
			msg_body = binfo_get->getMessage();

			if( msg_body )
			{
				msg_body->get(&header);
				stnum = header.getStreamNum();
				fcnum = header.getFunctionNum();

				if(GemHsmsCall::Get()->isFunctionExist(stnum, fcnum + 1))
				{
					//GemHsmsCall::Get()->getCommunicateStatus();

					if(stnum == 1 && fcnum== 17)
					{
						GemHsmsCall::Get()->HsmsMessageAccess(stnum, fcnum+1, binfo_get);
					}
					else if(stnum == 1 && fcnum == 13)	
					{
						GemHsmsCall::Get()->HsmsMessageAccess(stnum, fcnum+1, binfo_get);
					}
					else if(GemHsmsCall::Get()->getOnlineStatus())
					{
						GemHsmsCall::Get()->HsmsMessageAccess(stnum, fcnum+1, binfo_get);
					}
					else
					{
						GemHsmsCall::Get()->HsmsMessageAccess(1, 0, binfo_get);
					}
				}
				else //process even function
				{
					if(stnum == 1 && fcnum == 14)
						GemHsmsCall::Get()->setCommunicateStatus(binfo_get);

					trinfo = trmgr->buffer(&header, TRANSACTION_RECV_PRIMARY);
							
					if( trinfo )
						trmgr->remove(trinfo);
				}
			}
		}

		if(m_ParseStatus <= 0 )							//auto send S1F13 when status change
		{		
			binfo = new BS2MessageInfo();
			GemHsmsCall::Get()->HsmsMessageAccess(1, 13, binfo);
			m_ParseStatus =1;
			delete binfo;
		}
		break;
	}

#if defined(_WIN32)
		::Sleep(50);
#else
		usleep(50000);
#endif

	result = BS2Device::parse(data, size); //nothing to do

	return result;
}

int GemDevice::put(ACE_Message_Block* mb, ACE_Time_Value* tv)
{
	int ret;
	
	ret = BS2Device::put(mb, tv);

	return ret;
}

int GemDevice::connected()
{
	int ret;
	
	ret = BS2Device::connected();

	return ret;
}

int GemDevice::disconnected()
{
	int ret;
	
	ret = BS2Device::disconnected();

	return ret;
}

int GemDevice::svc(void)
{
	int ret;
	
	ret = BS2Device::svc();

	return ret;
}