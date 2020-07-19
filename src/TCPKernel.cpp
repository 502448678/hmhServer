#include "TCPKernel.h"
#include "Queue.h"
#include "err_str.h"
#include <map>
#include <iostream>
#include <list>
using namespace std;

Queue* FileQueue = 0;
RedisTool * g_Redis = 0;
FILE *m_pFile;
int m_nFileSize;
int m_nPos;

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

//=== 协议映射表  ===================================================//

	BEGIN_PROTOCOL_MAP
	PM(_DEF_PROTOCOL_LOGIN_RQ,&TCPKernel::LoginRq)
	PM(_DEF_PROTOCOL_REGISTER_RQ,&TCPKernel::RegisterRq)
	END_PROTOCOL_MAP

//===================================================================//

bool TCPKernel::Open()
{
	g_Redis = new RedisTool;

    	m_pFile = NULL;
   	m_nFileSize = 0;
    	m_nPos = 0;
//    int j= 1234;
//    for( int i = 0 ; i < 30 ; ++i)
//    {
//        g_VideoPort[i]=j++;
//    }
   	q_Init(&FileQueue);

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
	cout << "DealData .."<<endl;
	PackType* pType = (PackType*)szbuf;
	int i=0;
	while(1)
	{

		if(m_ProtocolMapEntries[i].m_nType == *pType)
		{
			cout << "m_ProtocalMapEntries["<<i<<"]" <<endl;
			(this->*m_ProtocolMapEntries[i].m_pfun)(sock,szbuf);
			break;
		}
		else if(m_ProtocolMapEntries[i].m_nType == 0 || m_ProtocolMapEntries[i].m_pfun == 0)
		{
			//break;
			cout << "not m_nType to deal.."<<endl;	
			return;
		}
		i++;
	}
}

void TCPKernel::RegisterRq(int clientfd,char* szbuf)
{
	cout << "clientfd:"<<clientfd<<" ===> "<<"RegisterRq"<<endl;
	STRU_REGISTER_RQ * rq = (STRU_REGISTER_RQ *)szbuf;
	STRU_REGISTER_RS rs;
	rs.m_nType = _DEF_PROTOCOL_REGISTER_RS;

	rs.m_lResult = _register_fail;
	//查数据库
	char szsql[_DEF_SQLLEN] = {0};
	bzero( szsql,sizeof(szsql));

	
	list<string> lstStr;
	//查询数据库中是否有这个人
	sprintf( szsql , "select email from t_userdata where email = '%s';",rq->m_useremail);
	cout << szsql <<endl;

	if(m_sql.SelectMySql(szsql , 1 , lstStr ))

	//如果存在
//	if(lstStr.size() > 0 )
	{
		rs.m_lResult = _register_userid_is_exist;
		cout << "user to register has already exist.." << endl;
	}

	//如果没有可以插入
	else  
	{
		
		sprintf(szsql , "insert into t_userdata(email,name,password) values('%s','%s','%s');",rq->m_useremail,rq->m_username , rq->m_szPassword);

		cout << szsql << endl;
		
		m_sql.UpdateMySql( szsql );
		
		rs.m_lResult = _register_success;
	}
	lstStr.clear();
	cout << "register result:"<<rs.m_lResult<<endl;
	m_pTCPNet->SendData( clientfd , (char*)&rs , sizeof(rs) );

	
}
	
void TCPKernel::LoginRq(int clientfd,char* szbuf)
{

	cout << "clientfd:"<<clientfd<<" ===> "<<"LoginRq"<<endl;	
	STRU_LOGIN_RQ * rq = (STRU_LOGIN_RQ *)szbuf;
	STRU_LOGIN_RS rs;
	rs.m_nType = _DEF_PROTOCOL_LOGIN_RS;

	rs.m_lResult = _login_noexist;
	//查数据库
	char szsql[_DEF_SQLLEN] = {0};
	bzero( szsql,sizeof(szsql));

	list<string> lstStr;
	//判斷 是否存在
	sprintf( szsql , "select password from t_UserData where email = '%s';",rq->m_useremail);
	cout <<szsql<<endl;

	m_sql.SelectMySql(szsql , 1 , lstStr);

	if( lstStr.size() == 0 )
	{
		rs.m_lResult = _login_noexist;
	}
	else
	{
		rs.m_lResult = _login_password_err;

		string strPassword = lstStr.front();
		lstStr.pop_front();
		

		if( 0 == strcmp(strPassword.c_str(),rq->m_szPassword))
		{
			lstStr.clear();
			bzero(&szsql,sizeof(szsql));

			//char * id = (char*)q_Pop(pQueue);
			//rs.m_userid = atoi(id);
			cout << "UserEmail:" << rs.m_useremail;
			rs.m_lResult = _login_success;

			// write into redis  用戶id --> 用戶socket ， 用戶名字   id=1 --> 7 , zhangsan  list
			// 超時 5s  心跳 3秒一次發送  超時這個key自動消失
			char sztmp[100];
			sprintf(sztmp , "%d",clientfd);
			string strfd = sztmp;

			sprintf( sztmp , "email=%s" , rs.m_useremail);
			string strID = sztmp;

			if( g_Redis->isHashKeyExists(strID) )
			{
				rs.m_lResult = _login_user_online;
			}else
			{
				g_Redis->SetHashValue(  strID,  "fd" , strfd );
				g_Redis->SetHashValue(  strID,  "email" , rq->m_useremail );
				g_Redis->SetHashValue(  strID,  "port" , "0" );
			}
		}
	}
	lstStr.clear();

	cout << "login result:"<<rs.m_lResult<<endl;
	m_pTCPNet->SendData( clientfd , (char*)&rs , sizeof(rs) );

	
}

/*
void HeartRq(int clientfd ,char* szbuf,int nlen)
{
	//   printf("clientfd:%d HeartRq\n", clientfd);

	STRU_HEART_RQ * rq = (STRU_HEART_RQ*) szbuf;

	char sztmp[100];
	sprintf( sztmp , "id=%d" , rq->m_UserID);

	string strID;
	strID = sztmp;

	g_Redis->SetExpire(strID , 6);
}


*/

