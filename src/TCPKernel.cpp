#include "TCPKernel.h"

TCPKernel::TCPKernel()
{
	m_pTCPNet = new TCPNet(this);
	m_threadpool = ThreadPool::GetInstance();
}
TCPKernel::~TCPKernel()
{
	if(m_pTCPNet)
	{
		delete m_pTCPNet;
		m_pTCPNet = NULL;
	}
}

//协议映射表
	BEGIN_PROTOCOL_MAP
	PM(_DEF_PROTOCOL_LOGIN_RQ,&TCPKernel::LoginRq)
	PM(_DEF_PROTOCOL_REGISTER_RQ,&TCPKernel::RegisterRq)
	END_PROTOCOL_MAP

bool TCPKernel::Open()
{
	pthread_mutex_init(&mutex,NULL);
	//连接数据库
	if(!m_sql.ConnectMySql((char*)"localhost",(char*)"root",(char*)"199874",(char*)"hmh_server"))
	{
		return false;
	}
	//加载线程池
	if(!m_threadpool->InitThreadPool(100,2,100))
	{
		return false;
	}
	//连接网络
	if(!m_pTCPNet->InitNetWork())
	{
		return false;
	}
	return true;
}

void TCPKernel::Close()
{
	m_pTCPNet->UnInitNetWork();
	m_threadpool->DestroyThreadPool();
}

void TCPKernel::DealData(int sock,char* szbuf)
{
	PackType* pType = (PackType*)szbuf;
	int i=0;
	while(1)
	{
		if(m_ProtocolMapEntries[i].m_nType == *pType)
		{
			(this->*m_ProtocolMapEntries[i].m_pfun)(sock,szbuf);
			break;
		}
		else if(m_ProtocolMapEntries[i].m_nType == 0 || m_ProtocolMapEntries[i].m_pfun == 0)
			break;
		i++;
	}
}
	
void TCPKernel::LoginRq(int sock,char* szbuf)
{
	
}

void TCPKernel::RegisterRq(int sock,char* szbuf)
{

}
