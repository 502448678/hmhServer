#include "TCPKernel.h"
#include "Queue.h"
#include "log.h"
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
	LOG_INFO("%s\n","========== hPlayer Server Begin Running .. =============");
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
		LOG_ERROR("%s","MySql connect error..\n");
		return false;
	}
	cout <<"MySql connect success.." <<endl;
	LOG_INFO("%s","MySql connect success..\n");
	//加载线程池
	if(!m_threadpool->InitThreadPool(100,5,100))
	{
		cout <<"ThreadPool init error.." <<endl;
		LOG_ERROR("%s","ThreadPool init error..\n");
		return false;
	}
	cout <<"ThreadPool init success.." <<endl;
	LOG_INFO("%s","ThreadPool init success..\n");
	//连接网络
	if(!m_pTCPNet->InitNetWork())
	{
		cout << "Network init error.."<<endl;
		LOG_ERROR("%s","Network init error..\n");
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
//	LOG_INFO("%s","DealData ..\n");
	PackType* pType = (PackType*)szbuf;
	int i=0;
	while(1)
	{

		if(m_ProtocolMapEntries[i].m_nType == *pType)
		{
			cout << "m_ProtocalMapEntries["<<i<<"]" <<endl;
//			LOG_INFO("%s[%d]\n","m_ProtocalMapEntries",i);
			(this->*m_ProtocolMapEntries[i].m_pfun)(sock,szbuf);
			break;
		}
		if(m_ProtocolMapEntries[i].m_nType == 0 && m_ProtocolMapEntries[i].m_pfun == 0)
		{
			//break;
			cout << "not m_nType to deal.."<<endl;	
//			LOG_WARN("%s", "not m_nType to deal..\n");
			return;
		}
		i++;
	}
}

void TCPKernel::RegisterRq(int clientfd,char* szbuf)
{
	cout << "clientfd:"<<clientfd<<" ===> "<<"RegisterRq"<<endl;
	LOG_INFO("%s%d ===> %s\n","clientfd:",clientfd,"RegisterRq");
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
	LOG_INFO("%s\n",szsql);
	m_sql.SelectMySql(szsql , 1 , lstStr );


	//如果没有可以插入
	if(lstStr.size() == 0)  
	{
		
		sprintf(szsql , "insert into t_userdata(email,name,password) values('%s','%s','%s');",rq->m_useremail,rq->m_username , rq->m_szPassword);

		cout << szsql << endl;
		LOG_INFO("%s\n",szsql);
		if(m_sql.UpdateMySql( szsql ))
		{
			
			rs.m_lResult = _register_success;
		}
		else{
			rs.m_lResult = _register_userid_is_exist;

		}
	}
		

	//如果存在
	else{
		rs.m_lResult = _register_userid_is_exist;
		cout << "user to register has already exist.." << endl;
//		LOG_INFO("%s","user to register has already exist..\n");
	}
	

	lstStr.clear();
//	cout << "register result:"<<rs.m_lResult<<endl;
	m_pTCPNet->SendData( clientfd , (char*)&rs , sizeof(rs) );
	Log::get_instance()->flush();
	
}
	
void TCPKernel::LoginRq(int clientfd,char* szbuf)
{

	cout << "clientfd:"<<clientfd<<" ===> "<<"LoginRq"<<endl;	
	LOG_INFO("%s%d ===> %s\n","clientfd:",clientfd,"LoginRq");
	STRU_LOGIN_RQ * rq = (STRU_LOGIN_RQ *)szbuf;
	STRU_LOGIN_RS rs;
	rs.m_nType = _DEF_PROTOCOL_LOGIN_RS;
	

	rs.m_lResult = _login_noexist;
	//查数据库
	char szsql[_DEF_SQLLEN] = {0};
	bzero( szsql,sizeof(szsql));

	list<string> lstStr;
	//判斷 是否存在
	sprintf( szsql , "select password from t_userdata where email = '%s';",rq->m_useremail);
	cout <<szsql<<endl;
	LOG_INFO("%s\n",szsql);


	m_sql.SelectMySql(szsql , 1 , lstStr);
	
	if( lstStr.size() == 0 )
	{
		rs.m_lResult = _login_noexist;
		
		LOG_INFO("%s\n","login user not exist");
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
			//cout << "UserEmail:" << rs.m_useremail;
			rs.m_lResult = _login_success;
			
			//登录成功

			char username_sql[_DEF_SQLLEN] = {0};
			sprintf(username_sql,"select name from t_userdata where email = '%s';",rq->m_useremail);
			cout << username_sql;
			LOG_INFO("%s\n",username_sql);

			m_sql.SelectMySql(username_sql,1,lstStr);
			char* username_rs = (char*)lstStr.front().c_str();
			LOG_INFO("%s%s\n","Login success..  login rs name:",username_rs);
			strcpy(rs.m_username ,(const char*)username_rs);

			char userid_sql[_DEF_SQLLEN] = {0};
			sprintf(userid_sql,"select id from t_userdata where email = '%s';",rq->m_useremail);
			cout << userid_sql;
			LOG_INFO("%s\n",userid_sql);

			m_sql.SelectMySql(username_sql,1,lstStr);
			char* userid_rs = (char*)lstStr.front().c_str();
			LOG_INFO("%s%s\n","login rs id:",username_rs);
			rs.m_UserId = atoi(userid_rs);
			// write into redis  用戶id --> 用戶socket ， 用戶名字   id=1 --> 7 , zhangsan  list
			// 超時 5s  心跳 3秒一次發送  超時這個key自動消失
/*			char sztmp[100];
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
			*/
		}
	}
	lstStr.clear();

//	cout << "login result:"<<rs.m_lResult<<endl;
	m_pTCPNet->SendData( clientfd , (char*)&rs , sizeof(rs) );
	Log::get_instance()->flush();
	
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

