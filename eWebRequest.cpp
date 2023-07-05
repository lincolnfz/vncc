#include "eWebRequest.h"

// base header
#include "base/base.h"
//#include <curl/curl.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include "base/misctool.h"
#include "base/algorithm/Base64.h"
#include "base/algorithm/des.h"
#include "common/defThread.h"
#include "common/eInstructions.h"

#define RH_LOGIN "usercenter/login"
#define USER_LOGIN "uc/login"
#define USER_PHONES "uc/UserPhones"
#define USER_PHONE "uc/UserPhone"
#define ALL_GROUPS "device/Groups"
#define DEVICE_GROUPS "device/DeveiceGroups"
#define OP_GROUP "device/OpGroup"
#define GET_USER_DEVICE "device/GetUserDevice"
#define COMMAND_CHANNEL "device/CommandChannel"
#define CONNECT_PHONE "device/ConnectPhone"
#define PHONE_SCHEME "device/GetNewPhoneScheme"
#define GET_DEVICELIBRARY "device/GetDeviceLibrary"
#define RANDOM_DEVICE_LIBRARY "device/RandomDeviceLibrary"
#define SET_FUNCTION "device/SetFunction"
#define MOVE_DEVICE_GROUP "device/MoveDeveiceToGroup"
#define SYS_INIT "sys/SysInit"
#define CREATESIGNURL "sys/CreateSignUrl"
#define USERFILES "uc/userfiles"
#define GETEMAILCODE "uc/GetEmailCode"
#define REGISTER "uc/register"
#define FORGETPASS "uc/ForgotPass"
#define FILESUCCESS "sys/UploadFileSucess"
#define GETAPPVERSOIN "sys/GetAppVersion"
#define UPDATEUSERINFO "uc/UpdateUserInfo"
#define APPDATACOLLECT "sys/AppDataCollect"
#define CONFIGDATA "sys/ConfigData"
#define VALID_PATCH "device/GetPatchGUIDStatus"
#define RESULT_PATCH "device/GetInstallPatchPackageResult"
#define DELETE_CLOUD_FILE "sys/DeleteFile"
#define PN_CODE "uc/GetCode"
#define NEW_MOBILE "device/NewMobileDevice"
#define COUNTRY_CELLULAR "device/GetCountryMobileOperator"
#define LOG_COLLECT "sys/LogCollect"
#define GENERATE_QR "qrcode/CreateCode"
#define QUERY_QR "qrcode/GetUserInfo"
#define MSG_CENTER "sys/MsgCenter"
#define GET_APPS "appmarket/GetApps"
#define GET_DEVICE_TOKENS "device/GetDeviceTokens"
#define REFRESH_DEVICE_TOKEN "device/RefreshDeviceToken"
#define USER_VPN_LIST "device/UserVpnList"
#define OP_USER_VPN "device/OpUserVpn"


// 秘钥组
#ifdef _ECALC_VER
int g_deviceType = 8;
const std::string channel_name = "pc_main";
int g_RequestSource = 6;
std::string s_domain = "http://app.cloudecalc.com/";
std::string s_notify_domain = "http://notify.cloudecalc.com/";
char szPackName[] = "com.cloudecalc.mobile";
std::string GLOBLE_KEY[] = \
{"b5835174-6c62-40d0-b1af-0515346f7974", "6ed13599-f54a-4d02-a30e-da4bc1547450", \
"c99ab486-6d67-455f-a1ac-2a028651a177", "c03800a8-bb76-4b83-bdc5-a3c6825ccf69", \
"6995108e-bba9-47b3-87e3-36e6659412ef", "8eb551ad-749c-4b36-945f-3f78ed657a17", \
"33dad5d6-90d1-49f8-ad59-21acab489389", "0e6e254b-f497-4a16-a94e-adefceeddaf3"};
#elif _MAINLAND_VER
int g_deviceType = 8;
const std::string channel_name = "pc_main";
int g_RequestSource = 6;
std::string s_domain = "http://app.taoxyun.com/";
std::string s_notify_domain = "http://notify.taoxyun.com/";
char szPackName[] = "com.taoxinyun.mobile";
std::string GLOBLE_KEY[] = \
{"9e932e8f-6ad7-8c90-8c23-e696c3e23cb5",
"39bb8c0f-0c16-0f90-123e-6af27bc231db",
"04cf259e-9c1f-f7a7-84c2-f334e1ec3f4a",
"9626a5cf-b2d9-9fa8-22d7-ff6b6e70f6e7",
"5fa17582-ebb3-7a24-ed16-b05b2f1ff4b5",
"d20974d7-877a-b021-8e1c-adb7e9d19cac",
"22df7953-dc53-31b2-9518-cde2ad19e662",
"253c5188-f28f-f281-994f-07e393960a57"};
#elif _KAOPU_VER
int g_deviceType = 8;
const std::string channel_name = "pc_main";
int g_RequestSource = 6;
std::string s_domain = "http://papp.anytiktok.com/";
std::string s_notify_domain = "http://notify.cloudecalc.com/";
char szPackName[] = "com.cloudecalc.mobile";
std::string GLOBLE_KEY[] = \
{
	"ab1bcd29-b508-b8f6-7add-a3a453100263",
		"5e56f732-fbc3-77c7-3075-325ef5d25fb7",
		"0b712268-7887-e94f-ea41-b8f928fc9588",
		"2db44a8d-08c9-2fac-e8f8-42edcac1be48",
		"19061eef-5d57-8487-6792-15e980270d90",
		"e053c953-9d6e-0955-9c97-8ce49a7a994a",
		"ce5fd850-1c77-67b3-f65d-436713362f11",
		"b5cce23a-5c6a-20a6-7e69-92f76026a634"
};
#elif _MULTIDESK_VER
std::string channel_name = "win_main";
int g_RequestSource = 15;
int g_deviceType = 9;
std::string s_domain = "http://app.taoxyun.com/";
std::string s_notify_domain = "http://notify.cloudecalc.com/";
char szPackName[] = "win_main";
std::string GLOBLE_KEY[] = \
{"9e932e8f-6ad7-8c90-8c23-e696c3e23cb5",
"39bb8c0f-0c16-0f90-123e-6af27bc231db",
"04cf259e-9c1f-f7a7-84c2-f334e1ec3f4a",
"9626a5cf-b2d9-9fa8-22d7-ff6b6e70f6e7",
"5fa17582-ebb3-7a24-ed16-b05b2f1ff4b5",
"d20974d7-877a-b021-8e1c-adb7e9d19cac",
"22df7953-dc53-31b2-9518-cde2ad19e662",
"253c5188-f28f-f281-994f-07e393960a57"};
#else
int g_deviceType = 8;
int g_RequestSource = 6;
std::string s_domain = "http://app.cloudecalc.com/";
std::string s_notify_domain = "http://notify.cloudecalc.com/";
char szPackName[] = "com.cloudecalc.mobile";
std::string GLOBLE_KEY[] = \
{"b5835174-6c62-40d0-b1af-0515346f7974", "6ed13599-f54a-4d02-a30e-da4bc1547450", \
"c99ab486-6d67-455f-a1ac-2a028651a177", "c03800a8-bb76-4b83-bdc5-a3c6825ccf69", \
"6995108e-bba9-47b3-87e3-36e6659412ef", "8eb551ad-749c-4b36-945f-3f78ed657a17", \
"33dad5d6-90d1-49f8-ad59-21acab489389", "0e6e254b-f497-4a16-a94e-adefceeddaf3"};
#endif


//#define TEST 1
#ifdef TEST
const BYTE DES_KEY[] = { 20, 99, 100, 99, 111, 53, 101, 102 };
const BYTE DES_VI[] = { 50, 90, 109, 70, 119, 108, 111, 19 };
#else
const unsigned char DES_KEY[] = { 88, 27, 25, 80, 111, 36, 77, 91 };
const unsigned char DES_VI[] = { 66, 42, 89, 101, 33, 82, 65, 19 };
#endif

std::string g_str_ver = "1.0.0.01";
int g_i_ver = 10001;
std::string g_str_sn = "demoo";

long long time_stamp = 0;

std::string GetProxy(){
    return "";
}

std::wstring GetGloablLanguage(){
    return L"zh_cn";
}

std::string url_encode(const std::string& value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (std::string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
		std::string::value_type c = (*i);

		// Keep alphanumeric and other accepted characters intact
		if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
			continue;
		}

		// Any other characters are percent-encoded
		escaped << std::uppercase;
		escaped << '%' << std::setw(2) << int((unsigned char)c);
		escaped << std::nouppercase;
	}

	return escaped.str();
}

static std::mutex g_curl_lock;
CURLcode InitLibCurl() {
	std::unique_lock<std::mutex> lock(g_curl_lock);
	static bool bInit = false;
	static CURLcode res = CURL_LAST;
	if (!bInit) {
		res = curl_global_init(CURL_GLOBAL_ALL);
		bInit = true;
	}
	return res;
}

std::string DesAndBase64Encode(char* pszData)
{
    unsigned char* pbyEncrypt = (unsigned char*)calloc(strlen(pszData) + 9, 1);
	des_context ctx;
	des_set_key(&ctx, DES_KEY);
    int nEncryptLen = des_cbc_encrypt(&ctx, (const unsigned char*)pszData, strlen(pszData), pbyEncrypt, DES_VI);

	int nBase64Len = CBase64::Base64Encode(pbyEncrypt, nEncryptLen, nullptr, 0);
	char* pcsBase64 = (char*)calloc(nBase64Len + 1, sizeof(char));
    CBase64::Base64Encode(pbyEncrypt, nEncryptLen, (unsigned char*)pcsBase64, nBase64Len);

	std::string strGet = pcsBase64;
	free(pbyEncrypt);
	pbyEncrypt = nullptr;
	free(pcsBase64);
	pcsBase64 = nullptr;

	return strGet;
}

std::string DesAndBase64Decode(std::string strData)
{
	static char utf8flag[4] = { (char)0xEF, (char)0xBB, (char)0xBF, 0 };
	if (strData.substr(0, 3) == utf8flag)
	{
		strData.erase(0, 3);
	}

    int nBase64Len = CBase64::Base64Decode((const unsigned char*)strData.c_str(), strData.length(), nullptr, 0);
	char* pcsBase64 = (char*)calloc(nBase64Len + 1, sizeof(char));
	int nNew = CBase64::Base64Decode((const BYTE*)strData.c_str(), strData.length(), (BYTE*)pcsBase64, nBase64Len);

	char* pbyDecrypt = (char*)calloc(nBase64Len + 9, 1);
	std::string strGet;
	if (pbyDecrypt)
	{
		des_context ctx;
		des_set_key(&ctx, DES_KEY);
		int nEncryptLen = des_cbc_decrypt(&ctx, (const BYTE*)pcsBase64, nBase64Len, (BYTE*)pbyDecrypt, DES_VI);

		strGet = pbyDecrypt;
		//_DebugPrintA(strGet.c_str());
		free(pbyDecrypt);
	}

	free(pcsBase64);

	return strGet;
}

size_t headerwirtefunc(char* contents, size_t size, size_t nmemb, void* userp){
	size_t nThisPackSize = nmemb * size;
	eWebRequest::DOWNLOADFILEINFO* pData = (eWebRequest::DOWNLOADFILEINFO*)userp;
	CURLcode res = curl_easy_getinfo(pData->curlhandle, CURLINFO_RESPONSE_CODE, &pData->httpstatus);

	switch (pData->httpstatus)
	{
	case 200: {
        if(pData->fp == NULL){
            //_wfopen_s(&pData->fp, pData->strTempFilePath.c_str(), L"wb+");
        }
		//OutputDebugStringA("=====wb======\r\n");
		}
		break;
	case 206: {
        if (pData->fp == NULL){
            //_wfopen_s(&pData->fp, pData->strTempFilePath.c_str(), L"ab");
        }
		//OutputDebugStringA("=====ab======\r\n");
		}
		break;
		//可能发生跳转，需要重新设置回初始状态，避免状态错误
	case 301:
	case 302:
	case 307:
	case 308:
		pData->httpstatus = 0;
		break;
	default:
		break;
	}
	return nThisPackSize;
}

size_t downloadwirtefunc(void* contents, size_t size, size_t nmemb, void* userp) {
	eWebRequest::DOWNLOADFILEINFO* pData = (eWebRequest::DOWNLOADFILEINFO*)userp;
	size_t nThisPackSize = nmemb * size;
	if (!pData->fp)
		return 0;
	fwrite(contents, nThisPackSize, 1, pData->fp);

	pData->writesize += nThisPackSize;
	return nThisPackSize;
}

int my_progress_callback(void* userp, double dltotal, double dlnow, double ultotal, double ulnow) {
    eWebRequest::DOWNLOADFILEINFO* pData = (eWebRequest::DOWNLOADFILEINFO*)userp;
    unsigned long ts = getNowTickMs();
	//bool bfin = (100.0 * dlnow / dltotal) == 0 ? true : false;
	if (dltotal > 0 && ((ts - pData->ts) > 1000))
	{
		pData->ts = ts;
		pData->fPersent = 100.0 * dlnow / dltotal;
		//printf("persent[%f]\n", pData->fPersent);
		//wchar_t debugmsg[261] = { 0 };
		//swprintf_s(debugmsg, 260, L"%s - %f\n", pData->strTempFilePath.c_str(), pData->fPersent);
		//OutputDebugString(debugmsg);

        nbase::StdClosure fn = nbase::Bind(&eWebRequest::UpdatePregress, pData->self, pData->weakflag, pData->pcb, pData->fPersent, pData->user_arg);
        nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, fn);
    }
	return 0;
}

size_t postwirtefunc(void* contents, size_t size, size_t nmemb, void* userp) {
	char* v = (char*)contents;
	size_t nThisPackSize = nmemb * size;
	std::vector<char>* pvecData = (std::vector<char>*)userp;
	pvecData->insert(pvecData->end(), v, v + nThisPackSize);

	return nThisPackSize;
}

bool HttpsPostContent(const char* lpUrl, const char* lpData, std::vector<char>* pvecContent, int* iRetcode, bool* out_ret,
        std::vector<std::string>* pvecHeader = nullptr, long dwTimeOut = 30,
		const wchar_t* lpszLocalFilePath = nullptr,
		eWebRequest::DOWNLOADFILEINFO* pDownloadInfo = nullptr) {
	bool bret = false;
	*out_ret = bret;
	*iRetcode = -1;
	CURL* curl;
	CURLcode res = CURLE_OK;
	CURLU* ccurl = curl_url();
	CURLUcode cr = curl_url_set(ccurl, CURLUPART_URL, lpUrl, CURLU_URLENCODE);
	if (cr != CURLUE_OK) {
		curl_url_cleanup(ccurl);
		*out_ret = bret;
		return bret;
	}

	//先解析url
	char* pUrlHost = NULL;
	curl_url_get(ccurl, CURLUPART_HOST, &pUrlHost, 0);
	size_t nHostLen = strlen(pUrlHost);
	for (size_t i = 0; i < nHostLen; i++) {
		pUrlHost[i] = tolower(pUrlHost[i]);
	}
	curl_url_set(ccurl, CURLUPART_HOST, pUrlHost, 0);
	curl_free(pUrlHost);

	char* pLastUrl = NULL;
	curl_url_get(ccurl, CURLUPART_URL, &pLastUrl, CURLU_DEFAULT_SCHEME);
	char* pUrlPath = NULL;
	curl_url_get(ccurl, CURLUPART_PATH, &pUrlPath, 0);

	std::string strUrlPath = pUrlPath; //下载文件使用
	curl_free(pUrlPath);

	std::string strUrl = pLastUrl;
	curl_free(pLastUrl);

	curl_url_cleanup(ccurl);

	struct curl_slist* headlist = NULL;
	curl = curl_easy_init();
	if (curl) {
        std::string proxy = GetProxy();
		if (proxy.size()) {
			curl_easy_setopt(curl, CURLOPT_PROXY, proxy.c_str());
		}
		curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_HTTP);

		/* First set the URL that is about to receive our POST. */
		curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());

		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		if (dwTimeOut > 0)
		{
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, dwTimeOut);
		}
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);

		/* Now specify we want to POST data */
		if (lpData && lpData[0]) {
			curl_easy_setopt(curl, CURLOPT_POST, 1L);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, lpData);
		}

		if (lpszLocalFilePath && wcslen(lpszLocalFilePath) > 0 && pDownloadInfo != nullptr) {
			pDownloadInfo->strConfigAppKey;
			pDownloadInfo->curlhandle = curl;
			pDownloadInfo->strUrl = strUrl;
			pDownloadInfo->strTempFilePath = lpszLocalFilePath;
			pDownloadInfo->strTempFilePath.append(L".tmp~");

			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerwirtefunc);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, pDownloadInfo);

			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, downloadwirtefunc);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, pDownloadInfo);

			curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, my_progress_callback);
			curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, pDownloadInfo);
			curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
		}
		else if (pvecContent) {
			pvecContent->clear();
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, postwirtefunc);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, pvecContent);
		}
#ifdef _DEBUG
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
#endif

		if (pvecHeader && pvecHeader->size() > 0) {
			for (size_t i = 0; i < pvecHeader->size(); i++) {
				headlist = curl_slist_append(headlist, (*pvecHeader)[i].c_str());
			}
		}

		if (headlist) {
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headlist);
		}

		/*
		  If you use POST to a HTTP 1.1 server, you can send data without knowing
		  the size before starting the POST if you use chunked encoding. You
		  enable this by adding a header like "Transfer-Encoding: chunked" with
		  CURLOPT_HTTPHEADER. With HTTP 1.0 or without chunked transfer, you must
		  specify the size in the request.
		*/

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		*iRetcode = CURLE_OK;
		if (res != CURLE_OK) {
			*iRetcode = res;
			//printLogMsg(eLOG_LEVEL_INFO, "curl", "httppost err: %d", res);
		}
		else {
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, iRetcode);
			if (lpszLocalFilePath) {
				if (304 == *iRetcode || 200 == *iRetcode || 206 == *iRetcode) {
					bret = true;
				}
			}
			else if (200 == *iRetcode)
			{
				bret = true;
			}
		}

		if (headlist) {
			curl_slist_free_all(headlist);
		}

		if (pDownloadInfo && pDownloadInfo->fp) {
			fclose(pDownloadInfo->fp);
			if (bret) {
                //bret = MoveFileExW(pDownloadInfo->strTempFilePath.c_str(),
                //	lpszLocalFilePath,
                //	MOVEFILE_REPLACE_EXISTING);
			}
		}

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	pvecContent->push_back('\0');
	*out_ret = bret;
	return bret;
}

std::string BuildJsWebData(const long UserID, const char* AccessToken, const char* pszData) {
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(pszData, nullptr, true);
	}
	catch (nlohmann::json::parse_error& e) {

	}
	catch (...) {

	}
	json["UserID"] = UserID;
	json["AccessToken"] = AccessToken;
	json["DeviceType"] = g_deviceType;
	json["IMEI"] = "WIN_IMEI";
	char tmp[30] = { 0 };
    sprintf(tmp, "%d", g_i_ver);
	json["Version"] = tmp;
	json["VersionName"] = g_str_ver.c_str();
	json["ChannelName"] = channel_name;
	json["DeviceCode"] = g_str_sn;
	json["PackageType"] = szPackName;
	json["RequestSource"] = g_RequestSource;
	json["property"] = "WIN_property";
	std::wstring strlan = GetGloablLanguage();
	nbase::StringReplaceAll(L"_", L"-", strlan);
	json["Language"] = nbase::UTF16ToUTF8(strlan);
	std::string strout = json.dump();
	return strout;
}

std::string BuildJsSignContent(const long UserID, const char* AccessToken, const char* pszData) {
	nlohmann::json data;
	try
	{
		data = nlohmann::json::parse(pszData, nullptr, true);
	}
	catch (nlohmann::json::parse_error& e)
	{

	}
	std::string outdata = BuildJsWebData(UserID, AccessToken, data.dump().c_str());

	srand((unsigned long)time(nullptr));
	int nR = rand() % 8;//
	std::string reqid = generate_guid(); //请求的唯一ID
	unsigned long long nTimeStamp = 0; //时间戳(豪秒)

    long long time = abs(getNowTickMs() - time_stamp);
	nTimeStamp = nTimeStamp + time;
	if (time_stamp == 0)
	{
        nTimeStamp = getNowTickMs();
	}
	std::string strtmp = string_sprintf("%s%d%s%lld%s", outdata.c_str(), nR, reqid.c_str(),
		nTimeStamp, GLOBLE_KEY[nR].c_str());
	std::string strSign = GetStringSha256(strtmp.c_str());
	nlohmann::json root;
	root["r"] = nR;
	root["sign"] = strSign;
	root["timeStamp"] = nTimeStamp;
	root["requestID"] = reqid;
	root["data"] = outdata.c_str();
	std::string strout = root.dump();
	return strout;
}

std::string BuildPostContent(const char* pszData) {
	nlohmann::json json;
	try {
		json = nlohmann::json::parse(pszData, nullptr, true);
	}
	catch (nlohmann::json::parse_error& e) {

	}
	catch (...) {

	}

	json["IMEI"] = "WIN_IMEI";
	json["DeviceType"] = g_deviceType;
	char tmp[30] = { 0 };
    sprintf(tmp, "%d", g_i_ver);
	json["Version"] = tmp;
	json["VersionName"] = "1.0.0.1";
	json["ChannelName"] = channel_name;
	json["DeviceCode"] = "WIN";
	json["PackageType"] = szPackName;
	json["RequestSource"] = g_RequestSource;
	json["property"] = "WIN_property";
	std::wstring strlan = GetGloablLanguage();
	nbase::StringReplaceAll(L"_", L"-", strlan);
	json["Language"] = nbase::UTF16ToUTF8(strlan);

	srand((unsigned long)time(nullptr));
	//std::string strGet = DesAndBase64Encode((char*)pszData);  //加密
	std::string strGet = json.dump();  //不加密

	int nDeviceType = 3; //中控进程类型
	unsigned long long nTimeStamp = 0; //时间戳(微秒)
	std::string strIMEI = "LINUX_IMEI"; //手机IMEI码
	int nVersion = g_i_ver; //内部版本号
	std::string strVersionName = g_str_ver; //App版本号
	std::string strChannelName = channel_name; //App渠道名称
	std::string strDeviceCode = "LINUX"; //设备唯一码
	std::string strPackageType = "main"; //包名(默认main)
	std::string reqid = generate_guid(); //请求的唯一ID

	int nR = rand() % 8;//

    long long time = abs(getNowTickMs() - time_stamp);
	nTimeStamp = nTimeStamp + time;
	if (time_stamp == 0)
	{
        nTimeStamp = getNowTickMs();
	}

	//开始做签名
	std::string strtmp = string_sprintf("%s%d%s%lld%s", strGet.c_str(), nR, reqid.c_str(),
		nTimeStamp, GLOBLE_KEY[nR].c_str() );
	std::string strSign = GetStringSha256(strtmp.c_str());
	//strSign.clear();  //******************************现在测试不做签名,加密
	//签名ok

	std::string strDst = string_sprintf("r=%d&sign=%s&data=%s&ChannelName=%s&DeviceCode=%s&DeviceType=%d&IMEI=%s&PackageType=%s&TimeStamp=%lld&Version=%d&VersionName=%s&RequestID=%s",
		nR, strSign.c_str(), url_encode(strGet.c_str()).c_str(),
		//nR, strSign.c_str(), strGet.c_str(),
		strChannelName.c_str(), strDeviceCode.c_str(), nDeviceType, strIMEI.c_str(), strPackageType.c_str(),
		nTimeStamp, nVersion, strVersionName.c_str(), reqid.c_str());

	return strDst;
}

eWebRequest::eWebRequest():_executor(1){
	_str_domain = s_domain;
}

eWebRequest::~eWebRequest() {

}

void eWebRequest::UpdatePregress(std::weak_ptr<nbase::WeakFlag> weakflag, PROCESS_CALLBACK pcb, float persent, void* user_arg) {
	if (!(weakflag.expired())) {
		pcb(persent, user_arg);
	}
}

bool eWebRequest::Init() {
	CURLcode code = InitLibCurl();
	bool ret = code == CURLE_OK;
	if (!ret) {
		//printLogMsg(eLOG_LEVEL_ERROR, "eWebInterface", "curl init fail: %d", code);
	}
	else {
		//printLogMsg(eLOG_LEVEL_INFO, "eWebInterface", "curl init: %d", code);
	}
	return ret;
}

void eWebRequest::Help_HTTP_POST(const char* url, nlohmann::json& json, void* user_arg, std::weak_ptr<nbase::WeakFlag> weakflag,  eWebRequest::WEB_CALLBACK reply) {
	//BuildWebReqPack(json);
	std::shared_ptr<eWebRequest::eWebExchangeData> sp = std::make_shared<eWebRequest::eWebExchangeData>();
	sp->_reqtype = eWebRequest::eWebExchangeData::HttpReqType::HTTP_POST;
	sp->_strurl = url;
	sp->_weak_flag = weakflag;
	std::string strjson = json.dump();
	if (json.empty()) {
		strjson = "{}";
	}
	sp->_postdata = BuildPostContent(strjson.c_str());
	std::wstring strlan = GetGloablLanguage();
	nbase::StringReplaceAll(L"_", L"-", strlan);
	std::string language = nbase::UTF16ToUTF8(nbase::StringPrintf(L"Accept-Language: %s", strlan.c_str()));
	sp->_httphead.push_back(language);
	RequestWeb(sp, user_arg, reply);
}

void eWebRequest::RequestWeb(std::shared_ptr<eWebRequest::eWebExchangeData> sp, void* user_arg, eWebRequest::WEB_CALLBACK reply) {
	if (sp->_reqtype == eWebRequest::eWebExchangeData::HttpReqType::HTTP_POST) {
		this->GetWeakFlag();
		_executor.commit(eWebRequest::briage_HttpsPostContent, sp, user_arg, reply, this->GetWeakFlag(), this);
	}
}


bool eWebRequest::briage_HttpsPostContent(std::shared_ptr<eWebRequest::eWebExchangeData> sp,
	void* user_arg, eWebRequest::WEB_CALLBACK reply, std::weak_ptr<nbase::WeakFlag> weakflag, eWebRequest* self) {

	bool ret = HttpsPostContent(sp->_strurl.c_str(), sp->_postdata.c_str(), &sp->_response, &sp->_httpcode, &sp->_bret,
		&sp->_httphead, sp->_timeout, sp->_localfilepath.c_str());
    /*if (!ret && sp->_req_times < 3 && !weakflag.expired()) {
		if (sp->_reqtype == eWebRequest::eWebExchangeData::HttpReqType::HTTP_POST) {
			++(sp->_req_times);
			self->_executor.commit(eWebRequest::briage_HttpsPostContent, sp, user_arg, reply, weakflag, self);
		}
		return ret;
    }*/
	if (reply) {
		if (sp->_response.size() <= 1) {
			sp->_response.clear();
			sp->_response.push_back('n');
			sp->_response.push_back('u');
			sp->_response.push_back('l');
			sp->_response.push_back('l');
			sp->_response.push_back('\0');
		}
		sp->_cb = reply;
		sp->_parm = user_arg;
		nlohmann::json jv;

		try {
			jv = nlohmann::json::parse(&(sp->_response[0]), nullptr, true);
		}
		catch (nlohmann::json::parse_error& e) {
			
		}
		catch (...) {
			
		}

		_WEB_RESPONSE_PACK_BASE res_base;
		if (!jv.is_discarded()) {
			QuickGetJsonData(jv, res_base);
			sp->_response.clear();
			sp->_retcode = res_base.Code;
			std::swap(sp->_Msg, res_base.Msg);
			std::swap(sp->_data, res_base.Data); //交换数据
			time_stamp = res_base.TimeStamp;
			sWebReq._web_timer_ts = res_base.TimeStamp;
		}
		else {
			
		}
		//std::shared_ptr<eEmuManagerData> sp_msg(std::make_shared<eEmuManagerData>());
		//sp_msg->SetWebPackage(sp);
		//eEmuManager::getInstance()->WriteInQueue_Msg(sp_msg);
        nbase::StdClosure fn = self->ToWeakCallback(nbase::Bind([](eWebRequest* self, std::shared_ptr<eWebRequest::eWebExchangeData> sp)->void {
            if ( !(sp->_weak_flag.expired()) && sp->_cb) {
				void* arg = sp->_parm;
				std::function<void(std::shared_ptr<eWebExchangeData>, void*)> cb = sp->_cb;
				cb(sp, arg);
			}
            }, self, sp));
		
		if (!weakflag.expired() && !(sp->_weak_flag.expired())) {
			//dummy	
            nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, fn);
		}
	}

	return ret;
}

bool eWebRequest::briage_DownloadFile(std::shared_ptr<eWebExchangeData> sp,
	void* user_arg, WEB_CALLBACK reply, std::weak_ptr<nbase::WeakFlag> weakflag, eWebRequest* self) {
	sp->_retcode = 0;
	bool ret = HttpsPostContent(sp->_strurl.c_str(), sp->_postdata.c_str(), &sp->_response, &sp->_httpcode, &sp->_bret,
		&sp->_httphead, sp->_timeout, sp->_localfilepath.c_str(), sp->_downlaodfile.get());

    if (reply) {
		sp->_cb = reply;
		sp->_parm = user_arg;
	}

    nbase::StdClosure fn =  self->ToWeakCallback(nbase::Bind([](eWebRequest* self, std::shared_ptr<eWebRequest::eWebExchangeData> sp)->void {
		if (!(sp->_weak_flag.expired()) && sp->_cb) {
			void* arg = sp->_parm;
			std::function<void(std::shared_ptr<eWebExchangeData>, void*)> cb = sp->_cb;
			cb(sp, arg);
		}
        }, self, sp));
	if (!weakflag.expired() && !(sp->_weak_flag.expired())) {
        nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, fn);
    }

	return ret;
}

void eWebRequest::DownloadFile(const char* url, const wchar_t* outfile, const int timeout, void* arg,
	std::weak_ptr<nbase::WeakFlag> weakflag, PROCESS_CALLBACK pcb, WEB_CALLBACK cb) {

	std::shared_ptr<eWebRequest::eWebExchangeData> sp = std::make_shared<eWebRequest::eWebExchangeData>();
	sp->_reqtype = eWebRequest::eWebExchangeData::HttpReqType::HTTP_DOWNLOADFILE;
	sp->_strurl = url;
	sp->_weak_flag = weakflag;
	sp->_localfilepath = outfile;
	sp->_timeout = timeout;
	sp->_downlaodfile = std::make_unique<DOWNLOADFILEINFO>();
	sp->_downlaodfile->pcb = pcb;
	sp->_downlaodfile->weakflag = weakflag;
	sp->_downlaodfile->self = this;
	sp->_downlaodfile->user_arg = arg;
	_executor.commit(eWebRequest::briage_DownloadFile, sp, arg, cb, this->GetWeakFlag(), this);
}

void eWebRequest::Login(const int LoginType, const char* UniqueID, const char* NickName,
						const char* Photo, const char* EmailAccount, const char* Password,
						const char* AccessToken, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	bool bRet = false;
	nlohmann::json req;
	req["LoginType"] = LoginType;
	req["UniqueID"] = UniqueID;
	req["NickName"] = NickName;
	req["Photo"] = Photo;
	req["EmailAccount"] = EmailAccount;
	req["Password"] = Password;
	req["AccessToken"] = AccessToken;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), USER_LOGIN);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::RHLogin(const char* Username, const char* Password,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserName"] = Username;
	req["Password"] = Password;

	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), RH_LOGIN);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}


void eWebRequest::MobileVerifyLogin(const char* MobilePhone, const char* Code, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["LoginType"] = 9;
	req["MobilePhone"] = MobilePhone;
	req["Code"] = Code;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), USER_LOGIN);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::MobileLogin(const char* MobilePhone, const char* Password, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["LoginType"] = 6;
	req["MobilePhone"] = MobilePhone;
	req["Password"] = Password;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), USER_LOGIN);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::UserPhones(const long UserID, const char* AccessToken, const int CurrentPage, const int PageSize, 
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb ) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CurrentPage"] = CurrentPage;
	req["PageSize"] = PageSize;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), USER_PHONES);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::UserPhone(const long UserID, const char* AccessToken, const long MobileDeviceID, const long DeviceOrderID,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["MobileDeviceID"] = MobileDeviceID;
	req["DeviceOrderID"] = DeviceOrderID;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), USER_PHONE);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Groups(const long UserID, const char* AccessToken, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), ALL_GROUPS);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::DeveiceGroups(const long UserID, const char* AccessToken, const long GroupID, const int CurrentPage, const int PageSize,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["GroupID"] = GroupID;
#ifdef _MULTIDESK_VER
	req["ModelID"] = 3; //桃心t30设备编号
#else
	req["ModelID"] = 0;
#endif
	req["CurrentPage"] = CurrentPage;
	req["PageSize"] = PageSize;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), DEVICE_GROUPS);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::OpGroup(const long UserID, const char* AccessToken, const int OP, const long GroupID, const char* GroupName,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["OP"] = OP;
	if (OP == 2 || OP == 3) {
		req["GroupID"] = GroupID;
	}
	if (OP == 1 || OP == 3) {
		req["GroupName"] = GroupName;
	}
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), OP_GROUP);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::GetUserDevice(const long UserID, const char* AccessToken, std::vector<long>& ids, std::vector<long>& orderids,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["MobileDeviceIDs"] = ids;
	req["DeviceOrderIDs"] = orderids;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), GET_USER_DEVICE);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::CommandReboot(const long UserID, const char* AccessToken,
	std::vector<long>& ids, std::vector<long>& orderids, bool IsUpdateResolution, int width, int height, int dpi, int resolutionID,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CommandType"] = 3;
	nlohmann::json command;
	command["MobileDevices"] = ids;
	command["DeviceOrderIDs"] = orderids;
	if (IsUpdateResolution) {
		command["IsUpdateResolution"] = IsUpdateResolution;
		command["Width"] = width;
		command["Height"] = height;
		char sztmp[10] = { 0 };
        sprintf(sztmp, "%d", dpi);
		command["Dpi"] = sztmp;
		if (resolutionID > 0) {
			command["ResolutionId"] = resolutionID;
		}
	}
	req["Command"] = command;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), COMMAND_CHANNEL);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::CommandRecoveFactory(const long UserID, const char* AccessToken,
	std::vector<long>& ids, std::vector<long>& orderids, bool IsRetain,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CommandType"] = 4;
	nlohmann::json command;
	command["MobileDevices"] = ids;
	command["DeviceOrderIDs"] = orderids;
	command["IsRetain"] = IsRetain;
	req["Command"] = command;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), COMMAND_CHANNEL);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::EnableNewDeviceInfo(const long UserID, const char* AccessToken, nlohmann::json& info,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CommandType"] = 8;
	req["Command"] = info;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), COMMAND_CHANNEL);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::EnableNewDeviceInfo(const long UserID, const char* AccessToken,
	std::vector<long> ids, std::vector<long> orderids, const char* manufactuer, const char* deviccetype,
	const char* countrycode, const char* operatorcode, const char* networkcode, bool isSIM,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req, cmd;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CommandType"] = 8;
	cmd["MobileDeviceIDs"] = ids;
	cmd["DeviceOrderIDs"] = orderids;
	cmd["IsSIM"] = isSIM;
	if (manufactuer) {
		cmd["Manufactuer"] = manufactuer;
	}
	if (deviccetype) {
		cmd["DevicceType"] = deviccetype;
	}
	if (countrycode)
	{	
		cmd["CountryCode"] = countrycode;
		if (operatorcode)
		{
			cmd["OperatorCode"] = operatorcode;			
		}
		if (networkcode)
		{
			cmd["NetworkCode"] = networkcode;
		}
	}
	req["Command"] = cmd;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), COMMAND_CHANNEL);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::ConnectPhone(const long UserID, const char* AccessToken, long DeviceOrderID,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["DeviceOrderID"] = DeviceOrderID;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), CONNECT_PHONE);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::GetNewPhoneScheme(const long UserID, const char* AccessToken,
	const int CurrentPage, const int PageSize, const long deviceid,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CurrentPage"] = CurrentPage;
	req["PageSize"] = PageSize;
	req["DeviceOrderID"] = deviceid;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), PHONE_SCHEME);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::GetDeviceLibrary(const long UserID, const char* AccessToken,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), GET_DEVICELIBRARY);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::RandomDeviceLibrary(const long UserID, const char* AccessToken,
	const char* Vendor, const char* PhoneModel,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["Vendor"] = Vendor;
	req["PhoneModel"] = PhoneModel;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), RANDOM_DEVICE_LIBRARY);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::SetFunction(const long UserID, const char* AccessToken,
	long MobileDeviceID, long DeviceOrderID, int Func, std::string Name, int State,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["MobileDeviceID"] = MobileDeviceID;
	req["DeviceOrderID"] = DeviceOrderID;
	req["Func"] = Func;
	req["Name"] = Name;
	req["State"] = State;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), SET_FUNCTION);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::MoveDeveiceToGroup(const long UserID, const char* AccessToken,
	long ToGroupID, std::vector<long> MobileDeviceIDs, std::vector<long> DeviceOrderIDs, long FromGroupID,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["ToGroupID"] = ToGroupID;
	req["MobileDeviceIDs"] = MobileDeviceIDs;
	req["DeviceOrderIDs"] = DeviceOrderIDs;
	req["FromGroupID"] = FromGroupID;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), MOVE_DEVICE_GROUP);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::SysInit(void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	//req["dummy"] = "";
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), SYS_INIT);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

/*
void eWebRequest::CreateSignUrl(std::vector<std::string>& FilePath, std::string& RequestMethod, 
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, MD5_CALLBACK md5db, WEB_CALLBACK cb) {
	//std::vector<std::string> filepath;
	//std::string method = RequestMethod;
    nbase::StdClosure fn = nbase::Bind([=](std::vector<std::string> FilePath, std::string RequestMethod,
		void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, MD5_CALLBACK md5db, WEB_CALLBACK cb)->void {
			std::vector<std::string> FileMd5;
			std::vector<std::string> FileNames;
			std::vector<int> FileType;
			for (auto it : FilePath) {
				std::wstring wfilepath = nbase::UTF8ToUTF16(it);
				std::wstring wfilename;
				std::string strmd5 = get_file_md5(wfilepath.c_str());
				FileMd5.push_back(strmd5);
				nbase::FilePathApartFileName(wfilepath, wfilename);
				std::wstring file_ext;
				get_FileSuffix(wfilepath, file_ext);
				file_ext = nbase::MakeLowerString(file_ext);
				int ftype = 1001;
				if (file_ext.compare(L"apk") == 0 || file_ext.compare(L"xapk") == 0) {
					ftype = 1002;
				}
				//wfilename = nbase::StringPrintf(L"%s_%s.%s", wfilename.c_str(), nbase::UTF8ToUTF16(strmd5).c_str(), file_ext.c_str());
				wfilename = nbase::UTF8ToUTF16(strmd5);
				if (file_ext.size()) {
					wfilename = nbase::StringPrintf(L"%s.%s", nbase::UTF8ToUTF16(strmd5).c_str(), file_ext.c_str());
				}
				FileNames.push_back(nbase::UTF16ToUTF8(wfilename));
				FileType.push_back(ftype);

				std::wstring md5 = nbase::UTF8ToUTF16(strmd5);
				StdClosure fn = nbase::Bind([=](std::wstring file, std::wstring md5)->void {
					md5db(file, md5);
					}, wfilepath, md5);
				nbase::ThreadManager::PostTask((int)ThreadId::kThreadUI, ToWeakCallback(fn));
			}
			nlohmann::json req;
			req["FilePath"] = FileNames;
			req["RequestMethod"] = RequestMethod;
			req["FileMd5"] = FileMd5;
			//req["FileType"] = FileType;
			std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), CREATESIGNURL);
			this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
		}, FilePath, RequestMethod, arg, weakflag, md5db, cb);
	nbase::ThreadManager::PostTask((int)ThreadId::kThreadGlobalMisc, ToWeakCallback(fn));
}
*/

void eWebRequest::UserFiles(const long UserID, const char* AccessToken, const int FileClassify, const char* SearchKey,
	const int CurrentPage, const int PageSize,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["FileClassify"] = FileClassify;
	req["SearchKey"] = SearchKey;
	req["CurrentPage"] = CurrentPage;
	req["PageSize"] = PageSize;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), USERFILES);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::InatallApp(const long UserID, const char* AccessToken,
	const long userfileid, std::vector<long>& MobileDeviceIDs, std::vector<long>& DeviceOrderIDs,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CommandType"] = 12;
	nlohmann::json command;
	command["UserFileID"] = userfileid;
	command["MobileDeviceIDs"] = MobileDeviceIDs;
	command["DeviceOrderIDs"] = DeviceOrderIDs;
	req["Command"] = command;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), COMMAND_CHANNEL);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::RootDevice(const long UserID, const char* AccessToken,
	std::vector<long>& MobileDeviceIDs, std::vector<long>& DeviceOrderIDs, bool broot,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CommandType"] = 19;
	nlohmann::json command;
	command["MobileDevices"] = MobileDeviceIDs;
	command["DeviceOrderIDs"] = DeviceOrderIDs;
	command["RootStaus"] = broot == true ? 1 : 0;

	req["Command"] = command;
	

	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), COMMAND_CHANNEL);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::GetEmailCode(const char* Email, int CodeType, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["Email"] = Email;
	req["CodeType"] = CodeType;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), GETEMAILCODE);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Register(const char* email, const char* pwd, const char* pncode, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["EmailAccount"] = email;
	req["PassWord"] = pwd;
	req["EmailCode"] = pncode;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), REGISTER);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::RegisterByPhone(const char* email, const char* pwd, const char* pncode, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["MobilePhone"] = email;
	req["PassWord"] = pwd;
	req["Code"] = pncode;
	req["RegisterType"] = 6;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), REGISTER);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::ForgotPass(const char* email, const char* pwd, const char* pncode, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["Email"] = email;
	req["PassWord"] = pwd;
	req["EmailCode"] = pncode;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), FORGETPASS);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::MobilePwdRst(const char* acc, const char* pwd, const char* pncode, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["MobilePhone"] = acc;
	req["PassWord"] = pwd;
	req["Code"] = pncode;
	req["ForgotType"] = 6;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), FORGETPASS);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::GetApps(const char* SearchKey, const int AppType, const int CurrentPage, const int PageSize,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["PhoneGUID"] = "";
	req["AppType"] = AppType;
	req["SearchKey"] = SearchKey;
	req["CurrentPage"] = CurrentPage;
	req["PageSize"] = PageSize;
	std::string str_url = string_sprintf("%s%s", s_notify_domain.c_str(), GET_APPS);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::UploadFileSucess(const long UserID, const char* AccessToken, const char* FileName,
	const char* FilePath, const char* md5, const float FileSize, const char* FileType,
	const char* FileExtendJson, const int UploadChannel, const int FileClassify, const char* PackageName, const char* FileDesc, const char* FileVersion,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["FileName"] = FileName;
	req["FilePath"] = FilePath;
	req["FileMd5"] = md5;
	req["FileSize"] = FileSize;
	req["FileType"] = FileType;
	req["FileExtendJson"] = FileExtendJson;
	req["UploadChannel"] = UploadChannel;
	req["FileClassify"] = FileClassify;
	req["PackageName"] = PackageName;
	req["FileVersion"] = FileVersion;
	req["FileDesc"] = FileDesc;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), FILESUCCESS);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::GetAppVersion(void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), GETAPPVERSOIN);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::UpdateUserInfo(const long UserID, const char* AccessToken, const char* UpdateInfos, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["UpdateInfos"] = UpdateInfos;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), UPDATEUSERINFO);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::AppDataCollect(const long UserID, const char* AccessToken,
	const char* DataType, const char* Data,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["DataType"] = DataType;
	req["Data"] = Data;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), APPDATACOLLECT);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::LogCollect(const long UserID, const char* AccessToken,
	const char* DataType, const char* Data,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["DataType"] = DataType;
	req["Data"] = Data;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), LOG_COLLECT);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::ConfigData(void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), CONFIGDATA);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Patch(const long UserID, const char* AccessToken,
	std::vector<long>& devices, std::vector<long>& orderids, std::vector<std::string>& patch, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CommandType"] = 20;
	nlohmann::json command;
	//command["MobileDevices"] = devices;
	command["DeviceOrderIDs"] = orderids;
	command["PatchGUIDs"] = patch;

	req["Command"] = command;

	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), COMMAND_CHANNEL);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Check_Valid_Patch(const long UserID, const char* AccessToken, std::vector<std::string>& patch, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["PatchGUIDs"] = patch;

	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), VALID_PATCH);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Check_Patch(const long UserID, const char* AccessToken, std::vector<std::string>& task, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["BatchGUIDs"] = task;

	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), RESULT_PATCH);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Delete_Cloud_file(const long UserID, const char* AccessToken, long UserFileID, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["UserFileID"] = UserFileID;

	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), DELETE_CLOUD_FILE);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::PnCode(const char* phoneNum, const int codeModel, const int codeType, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["MobilePhone"] = phoneNum;
	req["GetCodeModel"] = codeModel;
	req["CodeType"] = codeType;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), PN_CODE);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Change_BreakDevice(const long UserID, const char* AccessToken, const long deviceid, const long orderid, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["FaildMobileDeviceID"] = deviceid;
	req["FaildDeviceOrderID"] = orderid;

	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), NEW_MOBILE);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Batch_Rename(const long UserID, const char* AccessToken, std::vector<long> ids, std::vector<std::wstring> names,
	void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["Func"] = 1;
	req["OperationType"] = 1;
	if (ids.size() == names.size()) {
		nlohmann::json MobileDeviceList;
		int n = ids.size();
		for (int i = 0; i < n; ++i) {
			nlohmann::json item;
			item["DeviceOrderID"] = ids[i];
			item["Name"] = nbase::UTF16ToUTF8(names[i]);
			MobileDeviceList.push_back(item);
		}
		req["MobileDeviceList"] = MobileDeviceList;
	}
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), SET_FUNCTION);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::CountryCellular(const long UserID, const char* AccessToken, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), COUNTRY_CELLULAR);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::GenerateQr(void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["DeviceCode"] = g_str_sn;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), GENERATE_QR);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::QueryQr(std::string token, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["CodeToken"] = token;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), QUERY_QR);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::MsgCenter(const long UserID, const char* AccessToken, const int CurrentPage, const int PageSize
	, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb)
{
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["CurrentPage"] = CurrentPage;
	req["Pagesize"] = PageSize;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), MSG_CENTER);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Get_Device_Token(const long UserID, const char* AccessToken, std::vector<long>& orderids, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["DeviceOrderIDs"] = orderids;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), GET_DEVICE_TOKENS);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}

void eWebRequest::Refresh_Device_Token(const long UserID, const char* AccessToken, long orderid, void* arg, std::weak_ptr<nbase::WeakFlag> weakflag, WEB_CALLBACK cb) {
	nlohmann::json req;
	req["UserID"] = UserID;
	req["AccessToken"] = AccessToken;
	req["DeviceOrderID"] = orderid;
	std::string str_url = string_sprintf("%s%s", _str_domain.c_str(), REFRESH_DEVICE_TOKEN);
	this->Help_HTTP_POST(str_url.c_str(), req, arg, weakflag, cb);
}
