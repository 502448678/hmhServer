#ifndef _PACKDEF_H
#define _PACKDEF_H
#include <vector>

#define BOOL bool
/////////////////////网络//////////////////////////////////////


#define SERVER_IP "101.200.180.58"
#define SERVER_PORT 2020
typedef char PackType;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BEGIN_PROTOCOL_MAP static const ProtocolMap m_ProtocolMapEntries[]= \
{

#define END_PROTOCOL_MAP    {0,0}\
};

#define PM(X,Y)  {X,Y},

#define DEF_SIZE 64
#define DEF_SQLLEN 256

//=== 协议 ========================================================//

#define DEF_PACK_BASE  (50)


//注册
#define  DEF_PACK_REGISTER_RQ    (DEF_PACK_BASE + 0)
#define  DEF_PACK_REGISTER_RS    (DEF_PACK_BASE + 1)

//登录
#define  DEF_PACK_LOGIN_RQ    (DEF_PACK_BASE + 2)
#define  DEF_PACK_LOGIN_RS    (DEF_PACK_BASE + 3)

//上传
#define DEF_PACK_UPLOAD_RQ    (DEF_PACK_BASE + 4)
#define DEF_PACK_UPLOAD_RS    (DEF_PACK_BASE + 5)

//上传文件块
#define DEF_PACK_UPLOAD_FILEBLOCK_RQ    (DEF_PACK_BASE + 6)
#define DEF_PACK_UPLOAD_FILEBLOCK_RS    (DEF_PACK_BASE + 7)

//下载
#define DEF_PACK_DOWNLOAD_RQ    (DEF_PACK_BASE + 8)
#define DEF_PACK_DOWNLOAD_RS    (DEF_PACK_BASE + 9)

//下载文件块
#define DEF_PACK_DOWNLOAD_FILEBLOCK_RQ    (DEF_PACK_BASE + 10)
#define DEF_PACK_DOWNLOAD_FILEBLOCK_RS    (DEF_PACK_BASE + 11)

//提交点击结果
#define DEF_PACK_PRESSLIKE_RQ  (DEF_PACK_BASE + 12)

//注册请求结果
#define userid_is_exist      0
#define register_sucess      1

//登录请求结果
#define userid_no_exist      0
#define password_error       1
#define login_sucess         2

//上传请求结果
#define file_is_exist        0
#define file_uploaded        1
#define file_uploadrq_sucess 2
#define file_upload_refuse   3

//上传回复结果
#define fileblock_failed     0
#define fileblock_success    1

//下载请求结果
#define file_downrq_failed   0
#define file_downrq_success  1

#define _downloadfileblock_fail  0
#define _downloadfileblock_success	1

#define DEF_PACK_COUNT (100)

#define MAX_PATH            (280 )
#define MAX_SIZE            (60  )
#define DEF_HOBBY_COUNT     (8  )
#define MAX_CONTENT_LEN     (4096 )

//=== 数据包 ==========================================================//

//登录请求
typedef struct STRU_LOGIN_RQ
{
    STRU_LOGIN_RQ()
    {
        m_nType = DEF_PACK_LOGIN_RQ;
        memset(m_szUser,0,MAX_SIZE);
        memset(m_szPassword,0,MAX_SIZE);
    }

    PackType m_nType;   //包类型
    char     m_szUser[MAX_SIZE] ; //用户ID
    char     m_szPassword[MAX_SIZE];  //密码
}STRU_LOGIN_RQ;


//登录回复
typedef struct STRU_LOGIN_RS
{
    STRU_LOGIN_RS()
    {
        m_nType= DEF_PACK_LOGIN_RS;
    }
    PackType m_nType;   //包类型
    int  m_UserID;
    int  m_lResult ; //注册结果

}STRU_REGISTERLOGIN_RS;


//注册请求
typedef struct STRU_REGISTER_RQ
{
    STRU_REGISTER_RQ()
    {
        m_nType = DEF_PACK_REGISTER_RQ;
        memset(m_szUser,0,MAX_SIZE);
        memset(m_szPassword,0,MAX_SIZE);
    }

    PackType m_nType;   //包类型
    char     m_szUser[MAX_SIZE] ; //用户ID
    char     m_szPassword[MAX_SIZE];  //密码
    int    food;
    int    funny;
    int    ennegy;
    int    dance;
    int    music;
    int    video;
    int    outside;
    int    edu;
}STRU_REGISTER_RQ;

//注册回复
typedef struct STRU_REGISTER_RS
{
    STRU_REGISTER_RS()
    {
        m_nType= DEF_PACK_REGISTER_RS;
    }
    PackType m_nType;   //包类型
    int  m_lResult ; //注册结果

}STRU_REGISTER_RS;


//下载文件请求

typedef struct STRU_DOWNLOAD_RQ
{
    STRU_DOWNLOAD_RQ()
    {
        m_nType = DEF_PACK_DOWNLOAD_RQ;
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
        m_nType = DEF_PACK_DOWNLOAD_RS;
        m_nFileId = 0;
        memset(m_szFileName , 0 ,MAX_PATH);
        memset(m_rtmp , 0 ,MAX_PATH);
    }
    PackType m_nType;   //包类型
    int      m_nFileId;
    int64_t  m_nFileSize;
    int      m_nVideoId;
    char     m_szFileName[MAX_PATH]; 
    char     m_rtmp[MAX_PATH];

}STRU_DOWNLOAD_RS;

//文件块请求

typedef struct STRU_DOWNLOAD_FILEBLOCK_RQ
{
    STRU_DOWNLOAD_FILEBLOCK_RQ()
    {
        m_nType = DEF_PACK_DOWNLOAD_FILEBLOCK_RQ;
        m_nUserId = 0;
        m_nFileId =0;
        m_nBlockLen =0;
        memset(m_szFileContent,0,MAX_CONTENT_LEN);
    }
    PackType m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_nFileId;
    int    m_nBlockLen;
    char     m_szFileContent[MAX_CONTENT_LEN];
}STRU_DOWNLOAD_FILEBLOCK_RQ;


//文件块回复包
typedef struct STRU_DOWNLOAD_FILEBLOCK_RS
{
    STRU_DOWNLOAD_FILEBLOCK_RS()
    {
        m_nType = DEF_PACK_DOWNLOAD_FILEBLOCK_RS;
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

//上传文件请求
typedef struct STRU_UPLOAD_RQ
{
    STRU_UPLOAD_RQ()
    {
        m_nType = DEF_PACK_UPLOAD_RQ;
        m_nFileId = 0;
        m_UserId = 0;
        m_nVideoId = 0;
        memset(m_szFileName , 0 ,MAX_PATH);
    }
    PackType m_nType;   //包类型
    int      m_UserId;
    int      m_nFileId;
    int      m_nVideoId;
    int64_t  m_nFileSize;
    char     m_szHobby[DEF_HOBBY_COUNT];
    char     m_szFileName[MAX_PATH];
    char     m_szFileType[MAX_SIZE];


}STRU_UPLOAD_RQ;

//上传文件成功回复
typedef struct STRU_UPLOAD_RS
{
    STRU_UPLOAD_RS()
    {
        m_nType = DEF_PACK_UPLOAD_RS;
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
        m_nType = DEF_PACK_UPLOAD_FILEBLOCK_RQ;
        m_nUserId = 0;
        m_nFileId =0;
        m_nVideoId = 0;
        m_nBlockLen =0;
        memset(m_szFileContent,0,MAX_CONTENT_LEN);
    }
    PackType m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_nFileId;
    int    m_nVideoId;
    int    m_nBlockLen;
    char   m_szFileContent[MAX_CONTENT_LEN];

}STRU_UPLOAD_FILEBLOCK_RQ;

//视频点赞
typedef struct STRU_PRESSLIKE_RQ
{
    STRU_PRESSLIKE_RQ()
    {
        m_nType = DEF_PACK_PRESSLIKE_RQ;
        m_nUserId = 0;
        m_nVideoId = 0;
    }
    PackType m_nType;   //包类型
    int    m_nUserId; //用户ID
    int    m_nVideoId;

}STRU_PRESSLIKE_RQ;


#endif

