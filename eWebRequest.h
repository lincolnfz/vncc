#pragma once
// base header
#include "base/base.h"
#include <boost/noncopyable.hpp>
#include <boost/serialization/singleton.hpp>
#include "base/threadpool.h"
#include "json.hpp"
#include <curl/curl.h>
#include <QObject>
#include<QMetaType>


std::string BuildJsWebData(const long UserID, const char* AccessToken, const char* pszData);
std::string BuildJsSignContent(const long UserID, const char* AccessToken, const char* pszData);
std::string BuildPostContent(const char* pszData);

class eWebRequest : public QObject, public virtual nbase::SupportWeakCallback, public boost::serialization::singleton<eWebRequest>
{
Q_OBJECT
public:
    
    typedef std::function<void(float, void*)> PROCESS_CALLBACK;
    struct DOWNLOADFILEINFO {
        int taskid = 0;
        CURL* curlhandle = nullptr;
        int httpstatus = 0;
        FILE* fp = nullptr;
        long long writesize = 0;
        float fPersent = 0.f;
        unsigned long ts = 0;

        curl_off_t nContentLength = -1;
        std::string	strUrl;
        std::wstring strConfigAppKey;
        std::wstring strTempFilePath;
        PROCESS_CALLBACK pcb;
        std::weak_ptr<nbase::WeakFlag> weakflag;
        eWebRequest* self = nullptr;
        void* user_arg = nullptr;
    };
    class eWebExchangeData {
    public:
        eWebExchangeData() {};
        virtual ~eWebExchangeData() {};
        enum class HttpReqType {
            HTTP_GET = 0,
            HTTP_POST = 1,
            HTTP_DOWNLOADFILE = 2,
        };
        std::string _strurl;
        std::string _postdata;
        std::vector<std::string> _httphead;
        std::vector<char> _response;
        std::wstring _localfilepath;
        std::string _data; //web���ظ��ֵ�json
        std::string _Msg;
        int _retcode = -1;
        int _httpcode = 0;
        int _req_times = 1; //�������
        unsigned short _timeout = 30;
        HttpReqType _reqtype = HttpReqType::HTTP_GET;
        unsigned int _client_key = 0;
        std::function<void(std::shared_ptr<eWebExchangeData>, void*)> _cb;
        void* _parm = nullptr;
        bool _bret = false;
        std::weak_ptr<nbase::WeakFlag> _weak_flag;
        std::unique_ptr<DOWNLOADFILEINFO> _downlaodfile;

        bool isSucc() {
            return _bret == true && _retcode == 0;
        }
    };
    typedef std::function<void(std::shared_ptr<eWebExchangeData>, void*)> WEB_CALLBACK;
    typedef std::function<void(std::wstring, std::wstring)> MD5_CALLBACK;

public:
    explicit eWebRequest(QObject *parent = nullptr);
	virtual ~eWebRequest();

signals:
    void webResponse_Signal(std::shared_ptr<eWebRequest::eWebExchangeData>);

private slots:
    void webResponse_handler(std::shared_ptr<eWebRequest::eWebExchangeData>);

public:
    void UpdatePregress(std::weak_ptr<nbase::WeakFlag>, PROCESS_CALLBACK, float, void*);
	bool Init();
    void Help_HTTP_POST(const char* url, nlohmann::json& json, void* user_arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK reply);
    void RequestWeb(std::shared_ptr<eWebExchangeData> sp, void* user_arg, WEB_CALLBACK reply);
    static bool briage_HttpsPostContent(std::shared_ptr<eWebExchangeData> sp,
        void* user_arg, WEB_CALLBACK reply, std::weak_ptr<nbase::WeakFlag> weakflag, eWebRequest* self);

    static bool briage_DownloadFile(std::shared_ptr<eWebExchangeData> sp,
        void* user_arg, WEB_CALLBACK reply, std::weak_ptr<nbase::WeakFlag> weakflag, eWebRequest* self);

    void DownloadFile(const char* url, const wchar_t* outfile, const int timeout, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, PROCESS_CALLBACK pcb, WEB_CALLBACK cb);

    /*��¼*/
    void Login(const int LoginType, const char* UniqueID, const char* NickName,
        const char* Photo, const char* EmailAccount, const char* Password,
        const char* AccessToken, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    void RHLogin(const char* Username, const char* Password,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*ʹ���ֻ���֤���¼*/
    void MobileVerifyLogin(const char* MobilePhone, const char* Code, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*ʹ���ֻ��ŵ�¼*/
    void MobileLogin(const char* MobilePhone, const char* Password, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);
    
    /*��ȡ�û���ǰ��Ч���ֻ��б�*/
    void UserPhones(const long UserID, const char* AccessToken, const int CurrentPage, const int PageSize, 
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ�û��豸��Ϣ*/
    void UserPhone(const long UserID, const char* AccessToken, const long MobileDeviceID, const long DeviceOrderID,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ�û��豸������Ϣ*/
    void Groups(const long UserID, const char* AccessToken, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�����豸�����ȡ�豸��Ϣ*/
    void DeveiceGroups(const long UserID, const char* AccessToken, const long GroupID, const int CurrentPage, const int PageSize,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�û��豸�������*/
    void OpGroup(const long UserID, const char* AccessToken, const int OP, const long GroupID, const char* GroupName, 
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ�û�ָ���豸*/
    void GetUserDevice(const long UserID, const char* AccessToken, std::vector<long> &ids, std::vector<long>& orderids,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸����ͨ��--�����豸*/
    void CommandReboot(const long UserID, const char* AccessToken, 
        std::vector<long>& ids, std::vector<long>& orderids, bool IsUpdateResolution, int width, int height, int dpi, int resolutionID,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸����ͨ��--�ָ���������*/
    void CommandRecoveFactory(const long UserID, const char* AccessToken,
        std::vector<long>& ids, std::vector<long>& orderids, bool IsRetain,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�û����������豸*/
    void ConnectPhone(const long UserID, const char* AccessToken, long DeviceOrderID,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ�û�һ���»�����*/
    void GetNewPhoneScheme(const long UserID, const char* AccessToken, const int CurrentPage, const int PageSize, const long deviceid,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ���Ϳ�*/
    void GetDeviceLibrary(const long UserID, const char* AccessToken,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�����ȡ���Ϳ���ָ����������*/
    void RandomDeviceLibrary(const long UserID, const char* AccessToken,
        const char* Vendor, const char* PhoneModel,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸����ͨ��--һ���»�����̨��*/
    void EnableNewDeviceInfo(const long UserID, const char* AccessToken, nlohmann::json& info,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸����ͨ��--һ���»���������*/
    void EnableNewDeviceInfo(const long UserID, const char* AccessToken,
        std::vector<long> ids, std::vector<long> orderids, const char* manufactuer, const char* deviccetype,
        const char* countrycode, const char* operatorcode, const char* networkcode, bool isSIM,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸���ù���*/
    void SetFunction(const long UserID, const char* AccessToken, 
        long MobileDeviceID, long DeviceOrderID, int Func, std::string Name, int State,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�ƶ��豸������*/
    void MoveDeveiceToGroup(const long UserID, const char* AccessToken, 
        long ToGroupID, std::vector<long> MobileDeviceIDs, std::vector<long> DeviceOrderIDs, long FromGroupID,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*Ԥ�ƽӿ�*/
    void SysInit(void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ��Ѷ�ϴ������ļ�ǩ��*/
    void CreateSignUrl(std::vector<std::string>&, std::string&,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, MD5_CALLBACK md5db, WEB_CALLBACK cb);

    /*�û��ļ��б�*/
    void UserFiles(const long UserID, const char* AccessToken, const int FileClassify, const char* SearchKey,
        const int CurrentPage, const int PageSize,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);
    /*�豸����ͨ��--��װAPK*/
    void InatallApp(const long UserID, const char* AccessToken,
        const long userfileid, std::vector<long>& MobileDeviceIDs, std::vector<long>& DeviceOrderIDs,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸����ͨ��--ROOT*/
    void RootDevice(const long UserID, const char* AccessToken,
        std::vector<long>& MobileDeviceIDs, std::vector<long>& DeviceOrderIDs, bool broot,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*����������֤��*/
    void GetEmailCode(const char* Email, int CodeType, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*����ע���˺�*/
    void Register(const char* email, const char* pwd, const char* pncode, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�ֻ���ע���˺�*/
    void RegisterByPhone(const char* email, const char* pwd, const char* pncode, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*���ã����ǣ����루���䣩*/
    void ForgotPass(const char* email, const char* pwd, const char* pncode, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*���ã����ǣ����루�ֻ��ţ�*/
    void MobilePwdRst(const char* acc, const char* pwd, const char* pncode, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡӦ���г�Ӧ���б�*/
    void GetApps(const char* SearchKey, const int AppType, const int CurrentPage, const int PageSize, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�ļ��ϴ����֪ͨ*/
    void UploadFileSucess(const long UserID, const char* AccessToken, const char* FileName,
        const char* FilePath, const char* md5, const float FileSize, const char* FileType,
        const char* FileExtendJson, const int UploadChannel, const int FileClassify, const char* PackageName, const char* FileDesc, const char* FileVersion,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�޸��û���Ϣ*/
    void UpdateUserInfo(const long UserID, const char* AccessToken, const char* UpdateInfos, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*APP���½ӿ�*/
    void GetAppVersion(void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��������ռ��ӿ�*/
    void AppDataCollect(const long UserID, const char* AccessToken,
        const char* DataType, const char* Data,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��־�����ռ��ӿ�*/
    void LogCollect(const long UserID, const char* AccessToken,
        const char* DataType, const char* Data,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ�ϴ�obs����*/
    void ConfigData(void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸����ͨ��--���Ͳ�����*/
    void Patch(const long UserID, const char* AccessToken, std::vector<long>& devices, std::vector<long>& orderids, std::vector<std::string>& patch, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�жϲ�������������Ƿ���ȷ*/
    void Check_Valid_Patch(const long UserID, const char* AccessToken, std::vector<std::string>& patch, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ��װ���������*/
    void Check_Patch(const long UserID, const char* AccessToken, std::vector<std::string>& task, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*ɾ���ļ�*/
    void Delete_Cloud_file(const long UserID, const char* AccessToken, long UserFileID, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�����ֻ���֤��*/
    void PnCode(const char* phoneNum, const int codeModel, const int codeType, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸���ϻ��»�*/
    void Change_BreakDevice(const long UserID, const char* AccessToken, const long deviceid, const long orderid, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*�豸���ù���*/
    void Batch_Rename(const long UserID, const char* AccessToken, std::vector<long> ids, std::vector<std::wstring> names,
        void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ���Ҽ��ֻ���Ӫ��*/
    void CountryCellular(const long UserID, const char* AccessToken, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ά������*/
    void GenerateQr(void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    /*��ȡ�û�*/
    void QueryQr(std::string token, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    void Get_Device_Token(const long UserID, const char* AccessToken, std::vector<long>& orderids, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    void Refresh_Device_Token(const long UserID, const char* AccessToken, long orderid, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    void setDomain(std::string& domain)
    {
        _str_domain = domain;
    }

    void MsgCenter(const long UserID, const char* AccessToken, const int CurrentPage, const int PageSize, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb);

    long long web_timer_ts(){
        return _web_timer_ts;
    }

private:
	threadpool _executor; //�̳߳ؿ�4���߳�,���Զ�����
    std::string _str_domain;
    long long _web_timer_ts = 0;
    
};

Q_DECLARE_METATYPE(std::shared_ptr<eWebRequest::eWebExchangeData>);

// ʹ��typedef�Լ�������ʹ��
typedef boost::serialization::singleton<eWebRequest> singleton_webRequest; // ʹ��ģ��ķ�ʽֻ������ʵ��
#define sWebReq singleton_webRequest::get_mutable_instance() // ��constʵ��
#define sWebReq_const singleton_webRequest::get_const_instance() // constʵ��
