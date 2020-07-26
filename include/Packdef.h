#ifndef _PACKDEF_H
#define _PACKDEF_H
#include <vector>
#include "Queue.h"
#include "string.h"
#include "stdint.h"
#define BOOL bool
//=== 网络 ======================================================//
#define _DEF_PORT 2020
#define _DEF_SERVERIP "192.168.1.110"  

//=== 边界值 =====================================================//
#define _DEF_SIZE           (45  )
#define _DEF_BUFFERSIZE     (1000)
#define _DEF_SQLLEN         (256 )
#define _DEF_EPOLLSIZE      (4096)
#define _MAX_PATH           (280 )
#define _MAX_SIZE           (64  )
#define _DEF_HOBBY_COUNT    (8   )
#define _MAX_CONTENT_LEN    (4096)

//=== 协议映射 ===================================================//

#define BEGIN_PROTOCOL_MAP static const ProtocolMap m_ProtocolMapEntries[]= \
{
#define END_PROTOCOL_MAP    {0,0}\
};
#define PM(X,Y)  {X,Y},

//=== 协议 m_nType ========================================================//

#define _DEF_PROTOCOL_BASE                      (100)
//=== 1.注册 ===//
#define _DEF_PROTOCOL_REGISTER_RQ               (_DEF_PROTOCOL_BASE + 1)
#define _DEF_PROTOCOL_REGISTER_RS               (_DEF_PROTOCOL_BASE + 2)
//=== 2.登录 ===//
#define _DEF_PROTOCOL_LOGIN_RQ                  (_DEF_PROTOCOL_BASE + 3)
#define _DEF_PROTOCOL_LOGIN_RS                  (_DEF_PROTOCOL_BASE + 4)
//=== 3.上传 ===//
#define _DEF_PROTOCOL_UPLOAD_RQ                 (_DEF_PROTOCOL_BASE + 5)
#define _DEF_PROTOCOL_UPLOAD_RS                 (_DEF_PROTOCOL_BASE + 6)
//=== 4.上传文件块 ===//
#define _DEF_PROTOCOL_FILEBLOCK_RQ              (_DEF_PROTOCOL_BASE + 7)
#define _DEF_PROTOCOL_FILEBLOCK_RS              (_DEF_PROTOCOL_BASE + 8)
//=== 5.下载 ===//
#define _DEF_PROTOCOL_DOWNLOAD_RQ               (_DEF_PROTOCOL_BASE + 9)
#define _DEF_PROTOCOL_DOWNLOAD_RS               (_DEF_PROTOCOL_BASE +10)
//=== 6.下载文件块 ===//
#define _DEF_PROTOCOL_DOWNLOAD_FILEBLOCK_RQ     (_DEF_PROTOCOL_BASE +11)
#define _DEF_PROTOCOL_DOWNLOAD_FILEBLOCK_RS     (_DEF_PROTOCOL_BASE +12)

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
//上传请求结果
#define _file_is_exist        0
#define _file_uploaded        1
#define _file_uploadrq_sucess 2
#define _file_upload_refuse   3
//上传回复结果
#define _fileblock_failed     0
#define _fileblock_success    1

//下载请求结果
#define _file_downrq_failed   0
#define _file_downrq_success  1
//下载回复结果
#define _downloadfileblock_fail  0
#define _downloadfileblock_success	1

#define TRUE 1
#define FALSE 0

enum Sex{sex_male,sex_female};
//=== 数据包 ====================================================//
typedef char PackType;


//登录or注册请求包
typedef struct
{
    PackType  m_nType;
//  long long m_userid;
    char      m_useremail[_DEF_SIZE];
    char      m_username[_DEF_SIZE];
    char      m_szPassword[_DEF_SIZE];

}STRU_LOGIN_RQ,STRU_REGISTER_RQ;
//登录or注册回复包
//#pragma pack(4)
typedef struct
{
    PackType   m_nType;
    int        m_UserId;
    char       m_useremail[_DEF_SIZE];
    char       m_username[_DEF_SIZE];
    char       m_lResult;    //返回结果
}STRU_LOGIN_RS,STRU_REGISTER_RS;

//上传文件请求
typedef struct STRU_UPLOAD_RQ
{
    STRU_UPLOAD_RQ()
    {
        m_nType = _DEF_PROTOCOL_UPLOAD_RQ;
        m_nFileId = 0;
        m_UserId = 0;
        m_nVideoId = 0;
        memset(m_szFileName , 0 ,_MAX_PATH);
    }
    PackType m_nType;   //包类型
    int      m_UserId;
    int      m_nFileId;
    int      m_nVideoId;
    int64_t  m_nFileSize;
    char     m_szHobby[_DEF_HOBBY_COUNT];
    char     m_szFileName[_MAX_PATH];
    char     m_szFileType[_MAX_SIZE];
}STRU_UPLOAD_RQ;
//上传文件成功回复
typedef struct STRU_UPLOAD_RS
{
    STRU_UPLOAD_RS()
    {
        m_nType = _DEF_PROTOCOL_UPLOAD_RS;
        m_nResult = 0;
    }
    PackType m_nType;   //包类型
    int      m_nResult;

}STRU_UPLOAD_RS;
//上传文件块请求
typedef struct STRU_UPLOAD_FILEBLOCK_RQ
{
    STRU_UPLOAD_FILEBLOCK_RQ()
    {
        m_nType = _DEF_PROTOCOL_FILEBLOCK_RQ;
        m_nUserId = 0;
        m_nFileId =0;
        m_nVideoId = 0;
        m_nBlockLen =0;
        memset(m_szFileContent,'\0',_MAX_CONTENT_LEN);
    }
    PackType m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_nFileId;
    int    m_nVideoId;
    int    m_nBlockLen;
    char   m_szFileContent[_MAX_CONTENT_LEN];

}STRU_UPLOAD_FILEBLOCK_RQ;

//下载文件请求
typedef struct STRU_DOWNLOAD_RQ
{
    STRU_DOWNLOAD_RQ()
    {
        m_nType = _DEF_PROTOCOL_DOWNLOAD_RQ;
        m_nUserId = 0;
    }
    PackType   m_nType;   //包类型
    int    m_nUserId; //用户ID

}STRU_DOWNLOAD_RQ;
//下载文件回复
typedef struct STRU_DOWNLOAD_RS
{
    STRU_DOWNLOAD_RS()
    {
        m_nType = _DEF_PROTOCOL_DOWNLOAD_RS;
        m_nFileId = 0;
        memset(m_szFileName , 0 ,_MAX_PATH);
        memset(m_rtmp , 0 ,_MAX_PATH);
    }
    PackType m_nType;   //包类型
    int      m_nFileId;
    int64_t  m_nFileSize;
    int      m_nVideoId;
    char     m_szFileName[_MAX_PATH];
    char     m_rtmp[_MAX_PATH];

}STRU_DOWNLOAD_RS;
//下载文件块请求
typedef struct STRU_DOWNLOAD_FILEBLOCK_RQ
{
    STRU_DOWNLOAD_FILEBLOCK_RQ()
    {
        m_nType = _DEF_PROTOCOL_DOWNLOAD_FILEBLOCK_RQ;
        m_nUserId = 0;
        m_nFileId =0;
        m_nBlockLen =0;
        memset(m_szFileContent,'\0',_MAX_CONTENT_LEN);
    }
    PackType m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_nFileId;
    int    m_nBlockLen;
    char   m_szFileContent[_MAX_CONTENT_LEN];
}STRU_DOWNLOAD_FILEBLOCK_RQ;


//下载文件块回复
typedef struct STRU_DOWNLOAD_FILEBLOCK_RS
{
    STRU_DOWNLOAD_FILEBLOCK_RS()
    {
        m_nType = _DEF_PROTOCOL_DOWNLOAD_FILEBLOCK_RS;
        m_nUserId = 0;
        m_nFileId =0;
        m_nBlockLen =0;
        m_nResult = 0;
    }
    PackType m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_nFileId;
    int     m_nResult;
    int64_t    m_nBlockLen;
}STRU_DOWNLOAD_FILEBLOCK_RS;


#endif

