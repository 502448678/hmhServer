#include "TCPNet.h"
#include "log.h"
#include "err_str.h"
IKernel * TCPNet::m_pKernel = NULL;

extern pthread_mutex_t alock;
extern pthread_mutex_t rlock;

TCPNet::TCPNet(IKernel *pKernel)
{
	s_socket = 0;
	m_threadpool = ThreadPool::GetInstance();
	m_pKernel = pKernel;
}

TCPNet::~TCPNet()
{
}

void TCPNet::addfd(int epollfd,int fd,int enable_et)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN;
	if(enable_et)
		event.events |= EPOLLET;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
}

bool TCPNet::InitNetWork()
{
	struct sockaddr_in saddr;
	bzero(&saddr,sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(_DEF_PORT);
	saddr.sin_addr.s_addr = inet_addr(_DEF_SERVERIP);

	s_socket = socket(AF_INET,SOCK_STREAM,0);
	if(bind(s_socket,(struct sockaddr*)&saddr,sizeof(saddr))==-1)
	{
		cout << "TCPNet bind error.." << endl;
		return false;
	}
	if(listen(s_socket,128)==-1)
	{
		cout << "TCPNet listen error.." << endl;
		return false;
		
	}


//	epollfd = epoll_create(_DEF_EPOLLSIZE);//创建epoll句柄，可监听的文件描述符数量为——DEF_EPOLLSIZE	
//	addfd(epollfd,s_socket,1);

//	m_threadpool->AddTask(Epoll_Jobs,(void*)this);
	epoll_event events[MAX_EVENTS];
	epollfds = epoll_create(_DEF_EPOLLSIZE);

	addfd(epollfds,s_socket,1);
	epollfd = epoll_create(_DEF_EPOLLSIZE);
	int num;

	cout << "Network init success.." << endl;
	LOG_INFO("%s","Network init success..\n");
	FlushLog();
	while(1)
	{
		num = epoll_wait(epollfds,events,EPOLLSIZE,-1);
		for(int i=0;i<num;i++)
		{
			struct sockaddr_in caddr;
			socklen_t size = sizeof(caddr);

			int c_socket = accept(s_socket,(struct sockaddr*)&caddr,&size);
			if(c_socket < 0)
			{
				cout << "connect failed" <<endl;
//				LOG_ERROR("%d", "Network connect failed..\n");
				continue;
			}
			else
			{
				cout << "epoll accept success.." << endl;
//				LOG_INFO("%s", "epoll accept success..\n");
			}
			addfd(epollfd,c_socket,1);
			m_threadpool->AddTask(Worker,(void*)this);
			
		}
	}

	return true;
}

void TCPNet::FlushLog()
{
	Log::get_instance()->flush();
}

void TCPNet::UnInitNetWork()
{
	//close(sockfd);
}
/*
void* TCPNet::Epoll_Jobs(void* arg)
{
	int ready;
	while(1)
	{
		if((ready = epoll_wait(epollfds,events,_DEF_EPOLLSIZE,-1))== -1)
			err_str("Epoll Call Failed:",-1);
		Epoll_Deal(ready);
		bzero(epollfds,sizeof(epollfds));

	}
}

void TCPNet::Epoll_Deal(int ready)
{
	for(int i=0;i<ready;i++)
	{
		int fd = epollfds[i].data.fd;
		if(s_socket == fd)
		{
			m_threadpool->AddTask(Accept_Deal,NULL);
		}
		else if(epollfds[i].events & EPOLLIN)
		{
		
			m_threadpool->AddTask(RecvUP,(void*)fd);
			removefd(fd);
		}
	}
}

void* TCPNet::Accept_Deal(void* arg)
{
	struct sockaddr_in clientaddr;
	int clientsize = sizeof(clientaddr);
	int clientfd;
	char ipstr[_DEF_IPSIZE];
	pthread_mutex_lock(&alock);
	if((clientfd == accept(s_socket,(struct sockaddr*)&clientaddr,(socklen_t*)&clientsize))==-1)
	{
		err_str("Custom thread Accept Error",-1);
	}
	pthread_mutex_unlock(&alock);
	addfd(epollfd,clientfd,1);
	cout << "Custom Thread TID:0x"<<(unsigned int)pthread_self()<<" Client IP:"<<inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,ipstr,sizeof(ipstr))<<" Client PORT:"<<ntohs(clientaddr.sin_port)<<endl;
	return 0;
}
*/
void* TCPNet::Worker(void* arg)
{
	cout << "work +1" <<endl;
//	LOG_INFO("%s","TCPNet Work+1..\n");
	TCPNet* pthis = (TCPNet*)arg;
	epoll_event events[MAX_EVENTS];
	int num = 0;

	while(1)
	{
		num = epoll_wait(pthis->epollfd,events,EPOLLSIZE,-1);

		cout <<"epoll_wait num:"<<num<<endl;

//		LOG_INFO("%s%d\n","epoll_wait num:",num);
		for(int i=0;i<num;i++)
		{
			//如果用户退出，移除sockfd  
			pthis->RecvUP(events[i],pthis->epollfd,events[i].data.fd);
		}
	}
}

void TCPNet::removefd(int epollfd,int fd)
{
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
	close(fd);
}

void TCPNet::RecvUP(epoll_event event,int epollfd,int c_socket)
{
	int RealReadNum;  //真正读到的大小
	int nPackSize;    //包大小
	char* psbuf = NULL;
	
	RealReadNum = recv(c_socket,(char*)&nPackSize,sizeof(int),0);
	if(RealReadNum <= 0)
	{
		//客户端下线或者错误
		removefd(epollfd,event.data.fd);
		//cout << "removefd .. " <<endl;
		LOG_INFO("%s:%d\n","removefd",event.data.fd);
		return;
	}
	psbuf = new char[nPackSize];
	int noffset = 0;
	while(nPackSize)
	{
		RealReadNum = recv(c_socket,psbuf+noffset,nPackSize,0);
		noffset += RealReadNum;
		nPackSize -= RealReadNum;
	}
	m_pKernel->DealData(c_socket,psbuf);

	cout << "Recv Data size:"<<RealReadNum<<endl;
//	LOG_INFO("%s%d\n","Recv data size:",RealReadNum);

	delete []psbuf;
	psbuf = NULL;
}

bool TCPNet::SendData(int sock,char* szbuf,int nlen)
{
	if(!szbuf || nlen <= 0)
		return false;
	//发送包大小
	if(send(sock,(const char*)&nlen,sizeof(int),0)<0)
		return false;
	//包内容
	if(send(sock,szbuf,nlen,0)<0)
		return false;
	cout <<"send data len:"<<nlen<<endl;
//	LOG_INFO("%s%d\n","Send data size:",nlen);
	return true;
}

