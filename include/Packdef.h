#ifndef _PACKDEF_H
#define _PACKDEF_H
#include <vector>
#include "Queue.h"

#define BOOL bool
//=== 网络 ======================================================//
#define _DEF_PORT 2020
#define _DEF_SERVERIP "192.168.1.110"  

//=== 边界值 =====================================================//
#define _DEF_SIZE 45
#define _DEF_BUFFERSIZE 1000
#define _DEF_SQLLEN 256
#define _DEF_EPOLLSIZE 4096
#define _DEF_IPSIZE 16

//=== 协议映射 ===================================================//

#define BEGIN_PROTOCOL_MAP static const ProtocolMap m_ProtocolMapEntries[]= \
{
#define END_PROTOCOL_MAP    {0,0}\
};
#define PM(X,Y)  {X,Y},

//=== 协议 ========================================================//

#define _DEF_PROTOCOL_BASE 100
//1.注册
#define _DEF_PROTOCOL_REGISTER_RQ _DEF_PROTOCOL_BASE +1
#define _DEF_PROTOCOL_REGISTER_RS _DEF_PROTOCOL_BASE +2
//2.登录
#define _DEF_PROTOCOL_LOGIN_RQ _DEF_PROTOCOL_BASE +3
#define _DEF_PROTOCOL_LOGIN_RS _DEF_PROTOCOL_BASE +4
//3.编辑个人信息
#define _DEF_PROTOCOL_EDITPERSONINFO_RQ _DEF_PROTOCOL_BASE +5
#define _DEF_PROTOCOL_EDITPERSONINFO_RS _DEF_PROTOCOL_BASE +6
//4.获取作者结构体信息
#define _DEF_PROTOCOL_GETAUTHORINFO_RQ _DEF_PROTOCOL_BASE +7
#define _DEF_PROTOCOL_GETAUTHORINFO_RS _DEF_PROTOCOL_BASE +8
//5.获取作者视频流的头
#define _DEF_PROTOCOL_GETAUTHORVIDEOSTREAMHEAD_RQ _DEF_PROTOCOL_BASE +9
#define _DEF_PROTOCOL_GETAUTHORVIDEOSTREAMHEAD_RS _DEF_PROTOCOL_BASE +10
//6.获取作者视频流
#define _DEF_PROTOCOL_GETAUTHORVIDEOSTREAM_RQ _DEF_PROTOCOL_BASE +11
#define _DEF_PROTOCOL_GETAUTHORVIDEOSTREAM_RS _DEF_PROTOCOL_BASE +12
//7.上传视频流的头
#define _DEF_PROTOCOL_UPLOADVIDEOHEAD_RQ _DEF_PROTOCOL_BASE +13
#define _DEF_PROTOCOL_UPLOADVIDEOHEAD_RS _DEF_PROTOCOL_BASE +14
//8.上传视频流
#define _DEF_PROTOCOL_UPLOADVIDEOSTREAM_RQ _DEF_PROTOCOL_BASE +15
#define _DEF_PROTOCOL_UPLOADVIDEOSTREAM_RS _DEF_PROTOCOL_BASE +16
//9.点赞
#define _DEF_PROTOCOL_GIVENIDEOEVALUATE_RQ _DEF_PROTOCOL_BASE +17
#define _DEF_PROTOCOL_GIVENIDEOEVALUATE_RS _DEF_PROTOCOL_BASE +18
//10.退出
#define _DEF_PROTOCOL_QUITAUTHOR_RQ   _DEF_PROTOCOL_BASE +19
#define _DEF_PROTOCOL_QUITAUTHOR_RS   _DEF_PROTOCOL_BASE +20


//=== m_lResult回复状态 =====================================//
//注册状态
#define _register_fail 0
#define _register_success 1
#define _register_userid_is_exist 2
//登录状态
#define _login_noexist 0
#define _login_password_err 1
#define _login_success 2
#define _login_user_online 3
//设置个人信息状态
#define _EDITPERSONINFO_fail 0
#define _EDITPERSONINFO_success 1
//获取自己或作者信息状态
#define _GETAUTHORINFO_fail 0
#define _GETAUTHORINFO_success 1
//点赞状态
#define _GIVENIDEOEVALUATE_fail 0
#define _GIVENIDEOEVALUATE_success 1
////退出状态
#define _QUITAUTHOR_fail 0
#define _QUITAUTHOR_success 1
 
#define TRUE 1
#define FALSE 0
 
enum Sex{sex_male,sex_female};
//=== 数据包 ====================================================//
typedef char PackType;


//登录or注册请求包
typedef struct
{
	PackType  m_nType;
	long long m_userid;
	char      m_useremail[_DEF_SIZE];
	char	  m_username[_DEF_SIZE];
	char      m_szPassword[_DEF_SIZE];

}STRU_LOGIN_RQ,STRU_REGISTER_RQ;
//登录or注册回复包
#pragma pack(4)
typedef struct
{
	PackType   m_nType;
	char       m_useremail[_DEF_SIZE];
	char       m_username[_DEF_SIZE];
	char       m_lResult;    //返回结果
}STRU_LOGIN_RS,STRU_REGISTER_RS;



//设置个人信息请求包
struct STRU_EDITPERSONINFO_RQ
{
	PackType   m_nType;
	long long  m_userid;      //用户ID
	char       m_szName[_DEF_SIZE];//昵称
	char       m_szSignature [_DEF_SIZE];//签名
	enum Sex 	   sex;              //性别
};
//设置个人信息回复包
typedef struct
{
	PackType   m_nType;
	long long  m_userid;      //用户ID
	char       m_lResult;    //返回结果
}STRU_EDITPERSONINFO_RS;

//获取自己或作者信息请求包
typedef struct
{
	PackType   m_nType;
	char m_username[_DEF_SIZE];
}STRU_GETAUTHORINFO_RQ;
//获取自己或作者信息回复包
typedef struct
{
	PackType   m_nType;
	char       m_username[_DEF_SIZE];
	char	   szbuf[_DEF_BUFFERSIZE];
	int 	   nNum_praise;  //作者总赞
}STRU_GETAUTHORINFO_RS;

//视频信息
typedef struct STRU_VIDEOInfo
{
	int nNum_praise;
	char m_videoname[_DEF_SIZE];
	char m_username[_DEF_SIZE];
	int videotype;
}STRU_VIDEOINFO;

//获取视频流的头or视频流请求包
typedef struct
{
	PackType   m_nType;
	char m_username[_DEF_SIZE];
}STRU_GETAUTHORVIDEOSTREAM_RQ,STRU_GETAUTHORVIDEOSTREAMHEAD_RQ;
//获取视频流的头or视频流回复包
typedef struct
{
	PackType   m_nType;
	long long n_len;   //视频流大小
	STRU_VIDEOINFO su_videoinfo;
}STRU_GETAUTHORVIDEOSTREAMHEAD_RS;

typedef struct
{
	PackType   m_nType;
	char szbuf[_DEF_BUFFERSIZE];    //装视频的名或视频流信息
	long long n_len;   //视频流大小
}STRU_GETAUTHORVIDEOSTREAM_RS;

//上传视频流请求
#pragma pack(4)
typedef struct NODE_1
{
	PackType   m_nType;
	STRU_VIDEOINFO su_videoinfo;
	long long n_len;   //视频流大小
}STRU_UPLOADVIDEOHEAD_RQ;
#pragma pack(4)
typedef struct NODE_2
{
	PackType   m_nType;
	char szbuf[_DEF_BUFFERSIZE];
	long long n_len;   //视频流大小
}STRU_UPLOADVIDEOSTREAM_RQ;
//上传视频流回复包
typedef struct STRU_UPLOADVIDEOHEAD_RS
{
	PackType   m_nType;
}STRU_UPLOADVIDEOSTREAM_RS;


//点赞请求包
typedef struct STRU_GIVENIDEOEVALUATE_Rq
{
	PackType   m_nType;
	STRU_VIDEOINFO su_videoinfo;   //点赞对象
}STRU_GIVENIDEOEVALUATE_RQ;
//点赞回复包
typedef struct STRU_GIVENIDEOEVALUATE_Rs
{
	PackType   m_nType;
	char       m_lResult;    //返回结果
}STRU_GIVENIDEOEVALUATE_RS;


//退出登录请求包
typedef struct STRU_QUITAUTHOR_Rq
{
	PackType  m_nType;
	long long m_userid;
}STRU_QUITAUTHOR_RQ;
//退出登录回复包
typedef struct STRU_QUITAUTHOR_Rs
{
	PackType  m_nType;
	long long m_userid;
	char      m_lResult;    //返回结果
}STRU_qUITAUTHOR_RS;

#endif

