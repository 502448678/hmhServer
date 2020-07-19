#ifndef _TCPKERNEL_H
#define _TCPKERNEL_H

#include "IKernel.h"
#include "mysql.h"
#include "ThreadPool.h"
#include "INet.h"
#include "Packdef.h"
#include "TCPNet.h"
#include <vector>
#include <pthread.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <ctime>
#include <functional>
#include <set>
#include <cstdlib>
#include <unistd.h>
#include "RedisTools.h"

using namespace std;

class TCPKernel;
typedef void (TCPKernel::*PFUN)(int,char*);

struct ProtocolMap
{
	PackType m_nType;
	PFUN m_pfun;
};

class TCPKernel:public IKernel
{
public:
	TCPKernel();
	~TCPKernel();
public:
	bool Open();
	void Close();
	void DealData(int sock,char* szbuf);
public:
	void RegisterRq(int sock,char* szbuf);
	void LoginRq(int sock,char* szbuf);

public:
	INet* m_pTCPNet;
	ThreadPool* m_threadpool;
	pthread_mutex_t mutex;
public:
	MySql m_sql;

};

#endif
