#ifndef _PACKDEF_H
#define _PACKDEF_H
#include <vector>
#define SERVER_IP "192.168.1.110"
#define _DEFPORT 2020
typedef char PackType;

#define BEGIN_PROTOCOL_MAP static const ProtocolMap m_ProtocolMapEntries[]= \
{

#define END_PROTOCOL_MAP    {0,0}\
};

#define PM(X,Y)  {X,Y},

#define _DEF_SIZE 64
#define _DEF_SQLLEN 256

//=== 协议 ========================================================//

#define _DEF_PROTOCOL_BASE       50

//注册协议
#define _DEF_PROTOCOL_REGISTER_RQ  _DEF_PROTOCOL_BASE+1
#define _DEF_PROTOCOL_REGISTER_RS  _DEF_PROTOCOL_BASE+2

//登录协议
#define _DEF_PROTOCOL_LOGIN_RQ  _DEF_PROTOCOL_BASE+3
#define _DEF_PROTOCOL_LOGIN_RS  _DEF_PROTOCOL_BASE+4

//=== 数据包 ==========================================================//

//注册请求
struct REGISTER_RQ
{
	PackType m_nType;

};

//注册回复
struct REGISTER_RS
{
	PackType m_nType;

};


//登录请求
struct LOGIN_RQ
{
	PackType m_nType;

};

//登录回复
struct LOGIN_RS
{
	PackType m_nType;

};


#endif
