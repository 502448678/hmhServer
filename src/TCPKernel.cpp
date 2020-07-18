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
	PM(DEF_PACK_LOGIN_RQ,&TCPKernel::LoginRq)
	PM(DEF_PACK_REGISTER_RQ,&TCPKernel::RegisterRq)
	END_PROTOCOL_MAP

bool TCPKernel::Open()
{
	pthread_mutex_init(&mutex,NULL);
	//连接数据库
	if(!m_sql.ConnectMySql((char*)"localhost",(char*)"root",(char*)"199874",(char*)"hmh_server"))
	{
		cout <<"MySql connect error.." <<endl;
		return false;
	}
	cout <<"MySql connect success.." <<endl;
	//加载线程池
	if(!m_threadpool->InitThreadPool(100,2,100))
	{
		cout <<"ThreadPool init error.." <<endl;
		return false;
	}
	cout <<"ThreadPool init success.." <<endl;
	//连接网络
	if(!m_pTCPNet->InitNetWork())
	{
		cout << "Network init error.."<<endl;
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
