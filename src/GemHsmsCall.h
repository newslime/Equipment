
#ifndef GEM_HSMSCALL_H
#define GEM_HSMSCALL_H

//using namespace std;

typedef struct _TraceData
{
	string			TRID;
	int				DSPER;
	int				TOTSMP;
	int				REPGSZ;
	vector<b_value> SVIDList;
	time_t			oritime;
	int				frequency;
} TraceData;

class GemHsmsCall
{
public:
	typedef void(GemHsmsCall::*HsmsReponse)(BS2MessageInfo*);

	static GemHsmsCall*	Get();
	static void			Free();

    GemHsmsCall();
    virtual ~GemHsmsCall();

	void	Initialize(char* hostname, int port, int mode);
	void	Deinitialize();
	void	InitCall();

	void	process();
	//void	SetGemSender(void* sender);
	bool	isFunctionExist(int stnum, int fcnum);

	void	HsmsMessageAccess(int stnum, int fcnum, BS2MessageInfo* binfo);
	void	HSMS_S1F0(BS2MessageInfo* info);
	void	HSMS_S1F4(BS2MessageInfo* info);
	void 	HSMS_S1F13(BS2MessageInfo* info);
	void 	HSMS_S1F14(BS2MessageInfo* info);
	void 	HSMS_S1F16(BS2MessageInfo* info);
	void 	HSMS_S1F18(BS2MessageInfo* info);
	void 	HSMS_S2F24(BS2MessageInfo* info);
	void 	HSMS_S2F34(BS2MessageInfo* info);
	void 	HSMS_S2F36(BS2MessageInfo* info);
	void 	HSMS_S2F38(BS2MessageInfo* info);

	void 	HSMS_S6F1(TraceData& traceData);
	void 	HSMS_S6F11(string ceid);
	void	SendS9F7FormatError(BS2MessageInfo* info, string errStr);

	map<string, string>	getCEIDMap() { return m_ceidMap; }

	void setCommunicateStatus(BS2MessageInfo* info);

	const bool getCommunicateStatus() const
	{ return m_CommunicateStatus; }

	const bool getOnlineStatus() const
	{ return m_OnlineStatus; }

protected:
	GemDevice*		m_gemhsms_device;
	GemSender*		m_gemhsms_sender;
	GemReceiver*	m_gemhsms_receiver;

	HsmsReponse		m_hsmsrep[21][50];
	
	wxCriticalSection	m_mutex_hsmscall;
	
	int	parseTraceData(BS2Message* msg, TraceData& traceData);
	int	parseSVIDList();
	int	parseCEIDList();

	void processS6F1();
	
	map<string, TraceData>	m_traceDataMap;
	map<string, string>		m_svidMap;
	map<string, string>		m_ceidMap;

	char			m_ExePath[MAX_PATH];
	unsigned int	m_DataID;
	bool			m_OnlineStatus;
	bool			m_CommunicateStatus;
};

#endif // HOST_HSMSCALL_H
