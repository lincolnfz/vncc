#include "eInstructions.h"


bool isSuccResponse(_WEB_RESPONSE_PACK_BASE& data) {
	return data.Code == 0;
}

eInstructions::eInstructions() {

}

namespace nlohmann {

    void from_json(const json& j, _SCREENCAP& s)
    {
        OPTIONJSVALUE(image);
        OPTIONJSVALUE(vpnstatus);
    }

    void from_json(const json& j, MSGINFO& s)
    {
        OPTIONJSVALUE(ID);
        OPTIONJSVALUE(MsgType);
        OPTIONJSVALUE(MsgSource);
        OPTIONJSVALUE(Title);
        OPTIONJSVALUE(Content);
        OPTIONJSVALUE(MsgDate);
        OPTIONJSVALUE(MsgDisplay);
    }

    void from_json(const json& j, MSGDATA& s)
    {
        OPTIONJSVALUE(MsgData);
    }

    void from_json(const json& j, TYPE_SPECIALSET& s)
    {
        OPTIONJSVALUE(key);
        OPTIONJSVALUE(Domain);
        OPTIONJSVALUE(SLD);
        OPTIONJSVALUE(Protocol);
    }

    void from_json(const json& j, TYPE_DEFAULTSET& s)
    {
        OPTIONJSVALUE(Protocol);
        OPTIONJSVALUE(Domain);
    }

    void from_json(const json& j, DEFAULTSET& s)
    {
        OPTIONJSVALUE(DefaultSet);
        OPTIONJSVALUE(SpecialSet);
    }

    void from_json(const json& j, _WEB_RESPONSE_PACK_BASE& s) {
        OPTIONJSVALUE(Code);
        OPTIONJSVALUE(Msg);
        OPTIONJSVALUE(requestID);
        OPTIONJSVALUE(TimeStamp);
        OPTIONJSVALUE(R);
        OPTIONJSVALUE(Sign);
        OPTIONJSVALUE(Data);
    }

    void from_json(const json& j, _USER_DATA& s) {
        OPTIONJSVALUE(UserID);
        OPTIONJSVALUE(UserName);
        OPTIONJSVALUE(NickName);
        OPTIONJSVALUE(AccessToken);
        OPTIONJSVALUE(Photo);
        OPTIONJSVALUE(UploadFileSize);
        OPTIONJSVALUE(UploadTotalSize);
    }

    void from_json(const json& j, _LOGIN_DATA& s) {
        OPTIONJSVALUE(UserData);
    }

    void from_json(const json& j, _PAGES& s) {
        OPTIONJSVALUE(PageCount);
        OPTIONJSVALUE(CurrentPage);
        OPTIONJSVALUE(PageSize);
        OPTIONJSVALUE(DataCount);
        OPTIONJSVALUE(IsLastPage);
    }

    void from_json(const json& j, _MOBILEDEVICE& s) {
        OPTIONJSVALUE(UseWebRTC);
        OPTIONJSVALUE(MobileDeviceID);
        OPTIONJSVALUE(PhoneGUID);
        OPTIONJSVALUE(MobileName);
        OPTIONJSVALUE(PhysicalProperty);
        OPTIONJSVALUE(HostComputerID);
        OPTIONJSVALUE(UseState);
        OPTIONJSVALUE(CreateTime);
        OPTIONJSVALUE(LastUpdateTime);
        OPTIONJSVALUE(HealthState);
        OPTIONJSVALUE(vgpus);
        OPTIONJSVALUE(RunMemory);
        OPTIONJSVALUE(IP);
        OPTIONJSVALUE(IPPort);
        OPTIONJSVALUE(LocalIP);
        OPTIONJSVALUE(LocalPort);
        OPTIONJSVALUE(ControlPort);
        OPTIONJSVALUE(AVPort);
        OPTIONJSVALUE(VoicePort);
        OPTIONJSVALUE(AdbPort);
        OPTIONJSVALUE(WebRTCIP);
        OPTIONJSVALUE(WebRTCPort);
        OPTIONJSVALUE(IsRoot);
        OPTIONJSVALUE(MultiMode);
        OPTIONJSVALUE(MessagePort);
        OPTIONJSVALUE(DeviceSource);
        OPTIONJSVALUE(AuthToken);
    }

    void from_json(const json& j, _USERMOBILEDEVICE& s) {
        OPTIONJSVALUE(ModelID);
        OPTIONJSVALUE(ModelName);
        OPTIONJSVALUE(DeviceOrderID);
        OPTIONJSVALUE(MobileDeviceInfo);
        OPTIONJSVALUE(ExpireTime);
        OPTIONJSVALUE(UserVpnConfigID);

    }

    void from_json(const json& j, _USERPHONES& s) {
        OPTIONJSVALUE(UserPhones);
        OPTIONJSVALUE(Page);
    }

    void from_json(const json& j, _GROUP_INFO& s) {
        OPTIONJSVALUE(ID);
        OPTIONJSVALUE(Name);
        OPTIONJSVALUE(DeviceCount);
    }

    void from_json(const json& j, _GROUPINO_AGG_DATA& s) {
        OPTIONJSVALUE(ID);
        OPTIONJSVALUE(Name);
        OPTIONJSVALUE(DeviceCount);
        OPTIONJSVALUE(MobileDeviceIDs);
    }

    void from_json(const json& j, _GROUP_DATAS& s) {
        OPTIONJSVALUE(GroupDatas);
    }

    void from_json(const json& j, _CONNECT_INFO& s) {
        OPTIONJSVALUE(MobileDeviceInfo);
        OPTIONJSVALUE(ExpireCountDown);
        OPTIONJSVALUE(ExpireTime);
        OPTIONJSVALUE(Extendstring);
        OPTIONJSVALUE(MultiMode);
    }

    void from_json(const json& j, _PHONE_PARAM& s) {
        OPTIONJSVALUE(PhoneModel);
        OPTIONJSVALUE(SystemVersion);
        OPTIONJSVALUE(SerialNumber);
        OPTIONJSVALUE(IMEI);
        OPTIONJSVALUE(IMSI);
        OPTIONJSVALUE(ICCID);
        OPTIONJSVALUE(WIFI);
        OPTIONJSVALUE(WIFIMAC);
        OPTIONJSVALUE(AndroidID);
        OPTIONJSVALUE(PhoneNumber);
        OPTIONJSVALUE(Vendor);
        OPTIONJSVALUE(OAID);
        OPTIONJSVALUE(CountryCode);
        OPTIONJSVALUE(OperatorCode);
        OPTIONJSVALUE(NetworkCode);
        OPTIONJSVALUE(SchemeName);
        OPTIONJSVALUE(IsSIM);
        OPTIONJSVALUE(ID);
        OPTIONJSVALUE(Numeric);
        OPTIONJSVALUE(CreateTime);
        OPTIONJSVALUE(isHide);
        OPTIONJSVALUE(DeviceOrderID);
    }

    void from_json(const json& j, _DEVICE_LIBRARY& s) {
        OPTIONJSVALUE(Vendor);
        OPTIONJSVALUE(PhoneParams);
    }

    void from_json(const json& j, _DEVICE_LIBS& s) {
        OPTIONJSVALUE(DeviceLibrarys);
    }

    void from_json(const json& j, _PHONE_PARAM_INFO& s) {
        OPTIONJSVALUE(PhoneParamInfo);
    }

    void from_json(const json& j, _MODEL_INFO& s) {
        OPTIONJSVALUE(ID);
        OPTIONJSVALUE(ShowOrder);
        OPTIONJSVALUE(Name);
        OPTIONJSVALUE(IconPath);
    }

    void from_json(const json& j, _APP_OPEN_PAGE_URL& s) {
        OPTIONJSVALUE(PageCode);
        OPTIONJSVALUE(OpenUrl);
    }

    void from_json(const json& j, _SWITCH_CONFIG& s) {
        OPTIONJSVALUE(ConfigCode);
        OPTIONJSVALUE(IsOpen);
    }

    void from_json(const json& j, _PARAM_CONFIG& s) {
        OPTIONJSVALUE(ConfigCode);
        OPTIONJSVALUE(Value);
    }

    void from_json(const json& j, _RESOLUTION_SLN& s) {
        OPTIONJSVALUE(ResolutionId);
        OPTIONJSVALUE(ResolutionName);
        OPTIONJSVALUE(ModelID);
        OPTIONJSVALUE(Width);
        OPTIONJSVALUE(Height);
        OPTIONJSVALUE(MultiMode);
        OPTIONJSVALUE(HardwareWidth);
        OPTIONJSVALUE(HardwareHeight);
    }

    void from_json(const json& j, _SYSINIT_INFO& s) {
        OPTIONJSVALUE(ModelInfos);
        OPTIONJSVALUE(AppOpenPageUrls);
        OPTIONJSVALUE(SwitchConfigs);
        OPTIONJSVALUE(ParamConfigs);
        OPTIONJSVALUE(BackupLimit);
        OPTIONJSVALUE(UploadFileSizeLimit);
        OPTIONJSVALUE(StorageConfig);
        OPTIONJSVALUE(Phoneresolutionconfigs);
    }

    void from_json(const json& j, _BITRATE& s) {
        OPTIONJSVALUE(defBitrate);
        OPTIONJSVALUE(bitrate1);
        OPTIONJSVALUE(bitrate2);
        OPTIONJSVALUE(bitrate3);
    }

    void from_json(const json& j, _NAVITE_JSPARAM& s) {
        OPTIONJSVALUE(name);
        OPTIONJSVALUE(param);
        OPTIONJSVALUE(asyncID);
        OPTIONJSVALUE(ddframe);
    }

    void from_json(const json& j, _OPEN_BROWSER_JSPARAM& s) {
        OPTIONJSVALUE(url);
    }

    void from_json(const json& j, _SIGNFILEPATH& s) {
        OPTIONJSVALUE(FileExists);
        OPTIONJSVALUE(FilePath);
        OPTIONJSVALUE(ExpireSecond);
        OPTIONJSVALUE(Key);
        OPTIONJSVALUE(Md5);
    }

    void from_json(const json& j, _SIGNFILEPATHS& s) {
        OPTIONJSVALUE(SignFilePaths);
    }

    void from_json(const json& j, _FILEINFO& s) {
        OPTIONJSVALUE(UserFileID);
        OPTIONJSVALUE(FileName);
        OPTIONJSVALUE(FilePth);
        OPTIONJSVALUE(FileDomain);
        OPTIONJSVALUE(FileMd5);
        OPTIONJSVALUE(FileSize);
        OPTIONJSVALUE(FileType);
        OPTIONJSVALUE(FileDesc);
        OPTIONJSVALUE(IsInstall);
        OPTIONJSVALUE(FileExtendJson);
        OPTIONJSVALUE(FileClassify);
        OPTIONJSVALUE(MobileDeviceID);
        OPTIONJSVALUE(UploadChannel);
    }

    void from_json(const json& j, _USERFILES& s) {
        OPTIONJSVALUE(FileDatas);
        OPTIONJSVALUE(Page);
    }

    void from_json(const json& j, APPINFO& s) {
        OPTIONJSVALUE(icon);
        OPTIONJSVALUE(packageName);
        OPTIONJSVALUE(isRoot);
        OPTIONJSVALUE(name);
    }

    void from_json(const json& j, APPS& s) {
        OPTIONJSVALUE(data);
    }

    void from_json(const json& j, WSDATA& s) {
        OPTIONJSVALUE(type);
        OPTIONJSVALUE(data);
    }

    void from_json(const json& j, AREASERVERINFO& s) {
        OPTIONJSVALUE(AreaID);
        OPTIONJSVALUE(AreaName);
        OPTIONJSVALUE(ICON);
        OPTIONJSVALUE(FileMd5);
        OPTIONJSVALUE(AppDownUrl);
        OPTIONJSVALUE(AppSize);
        OPTIONJSVALUE(AppVersion);
        OPTIONJSVALUE(PackageName);
        OPTIONJSVALUE(InnerVersion);
    }

    void to_json(json& j, AREASERVERINFO& s) {
        TOJSVALUE(AreaID);
        TOJSVALUE(AreaName);
        TOJSVALUE(ICON);
        TOJSVALUE(FileMd5);
        TOJSVALUE(AppDownUrl);
        TOJSVALUE(AppSize);
        TOJSVALUE(AppVersion);
        TOJSVALUE(PackageName);
        TOJSVALUE(InnerVersion);
    }

    void from_json(const json& j, MARKETAPPINFO& s) {
        OPTIONJSVALUE(ID);
        OPTIONJSVALUE(Name);
        OPTIONJSVALUE(ICON);
        OPTIONJSVALUE(AppType);
        OPTIONJSVALUE(Areas);
    }

    void from_json(const json& j, MARKETAPP& s) {
        OPTIONJSVALUE(AppInfos);
        OPTIONJSVALUE(Page);
    }

    void from_json(const json& j, UPLOADFILESUCC& s) {
        OPTIONJSVALUE(UploadFileSize);
        OPTIONJSVALUE(UploadFileId);
    }

    void from_json(const json& j, GETAPPVERSION& s) {
        OPTIONJSVALUE(AppName);
        OPTIONJSVALUE(AppVersion);
        OPTIONJSVALUE(AppVersionCode);
        OPTIONJSVALUE(AppUrl);
        OPTIONJSVALUE(AppSize);
        OPTIONJSVALUE(UpdateType);
        OPTIONJSVALUE(UpdateContent);
        OPTIONJSVALUE(UrlType);
    }

    void from_json(const json& j, BASE_PROTO& s) {
        OPTIONJSVALUE(cmdType);
        OPTIONJSVALUE(device);
        OPTIONJSVALUE(stt);
        OPTIONJSVALUE(isSuccess);
        OPTIONJSVALUE(data);
        OPTIONJSVALUE(reqid);
    }

    void from_json(const json& j, HOST_DOMAIN& s) {
        OPTIONJSVALUE(RsHost);
        OPTIONJSVALUE(RsfHost);
        OPTIONJSVALUE(SrcUpHosts);
    }

    void from_json(const json& j, QRDATA& s) {
        OPTIONJSVALUE(id);
        OPTIONJSVALUE(code);
        OPTIONJSVALUE(data);
    }

    void from_json(const json& j, ERROR_PATCH& s) {
        OPTIONJSVALUE(ErrorPatchGUIDs);
    }

    void from_json(const json& j, PATCH_TASK_GUID& s) {
        OPTIONJSVALUE(BatchGUID);
    }

    void from_json(const json& j, PATCH_RESULT_ITEM& s) {
        OPTIONJSVALUE(MobileDeviceID);
        OPTIONJSVALUE(PStatus);
        OPTIONJSVALUE(DeviceOrderID);
    }

    void from_json(const json& j, PATCH_RESULT_ARRAY& s) {
        OPTIONJSVALUE(InstallResult);
    }

    void from_json(const json& j, _BITRATE_ITEM& s) {
        OPTIONJSVALUE(name);
        OPTIONJSVALUE(bitrate);
    }

    void from_json(const json& j, _BITRATE_ROOT& s) {
        OPTIONJSVALUE(type);
        OPTIONJSVALUE(qList);
    }

    void from_json(const json& j, _RESOLUTION& s) {
        OPTIONJSVALUE(width);
        OPTIONJSVALUE(height);
        OPTIONJSVALUE(dpi);
    }

    void from_json(const json& j, _MULTI_MODE_ITEM& s) {
        OPTIONJSVALUE(width);
        OPTIONJSVALUE(height);
        OPTIONJSVALUE(fps);
        OPTIONJSVALUE(conntype);
        OPTIONJSVALUE(datatype);
        OPTIONJSVALUE(bitrate);
        OPTIONJSVALUE(autobit);
    }

    void from_json(const json& j, _LANG& s) {
        OPTIONJSVALUE(lang);
        OPTIONJSVALUE(value);
    }

    void from_json(const json& j, _MULTI_MODE& s) {
        OPTIONJSVALUE(name);
        OPTIONJSVALUE(data);
    }

    void from_json(const json& j, _NETWORK& s) {
        OPTIONJSVALUE(NetworkName);
        OPTIONJSVALUE(NetworkCode);
        OPTIONJSVALUE(Numeric);
    }

    void from_json(const json& j, _ISP& s) {
        OPTIONJSVALUE(OperatorName);
        OPTIONJSVALUE(OperatorCode);
        OPTIONJSVALUE(NetworkList);
        OPTIONJSVALUE(NetworkCode);
        OPTIONJSVALUE(Numeric);
    }

    void from_json(const json& j, _COUNTRY_CELLULAR& s) {
        OPTIONJSVALUE(CountryName);
        OPTIONJSVALUE(CountryCode);
        OPTIONJSVALUE(MobileOperatorList);
    }

    void from_json(const json& j, _COUNTRY_CELLULAR_ARRAY& s) {
        OPTIONJSVALUE(CountryList);
    }

    void from_json(const json& j, _HISTORY_SLN& s) {
        OPTIONJSVALUE(NewPhoneSchemes);
    }

    void from_json(const json& j, _HISTORY_SLN_PAGE& s) {
        OPTIONJSVALUE(Page);
    }

    void from_json(const json& j, FRAMERATE& s) {
        OPTIONJSVALUE(fps);
    }

    void from_json(const json& j, CODETOKEN& s) {
        OPTIONJSVALUE(CodeToken);
    }

    void from_json(const json& j, DEVICEAUTHTOKEN& s) {
        OPTIONJSVALUE(AccessToken);
        OPTIONJSVALUE(ExpireTime);
        OPTIONJSVALUE(ExpireCountDown);
    }

    void from_json(const json& j, MOBILEDEVICETOKEN& s) {
        OPTIONJSVALUE(DeviceOrderID);
        OPTIONJSVALUE(DeviceAuthToken);
    }

    void from_json(const json& j, MOBILEDEVICETOKENS& s) {
        OPTIONJSVALUE(MobileDeviceTokens);
    }

    void from_json(const json& j, VPNCLASSIFY& s) {
        OPTIONJSVALUE(ID);
        OPTIONJSVALUE(Name);
    }

    void from_json(const json& j, USERVPN& s) {
        OPTIONJSVALUE(ID);
        OPTIONJSVALUE(Name);
        OPTIONJSVALUE(Classify);
        OPTIONJSVALUE(Address);
        OPTIONJSVALUE(Prot);
        OPTIONJSVALUE(Account);
        OPTIONJSVALUE(Password);
    }

    void to_json(json& j, USERVPN& s) {
        TOJSVALUE(ID);
        TOJSVALUE(Name);
        TOJSVALUE(Classify);
        TOJSVALUE(Address);
        TOJSVALUE(Prot);
        TOJSVALUE(Account);
        TOJSVALUE(Password);
    }

    void to_json(json& j, DEVICEORDERVPN& s) {
        TOJSVALUE(DeviceOrderID);
        TOJSVALUE(UserVpnConfigID);
    }

    void from_json(const json& j, _USERVPNLIST& s) {
        OPTIONJSVALUE(UserVpnList);
        OPTIONJSVALUE(VpnClassifyList);
    }

    void from_json(const json& j, DEVICEORDERVPN& s) {
        OPTIONJSVALUE(DeviceOrderID);
        OPTIONJSVALUE(UserVpnConfigID);
    }

    void from_json(const json& j, SETVPN& s) {
        OPTIONJSVALUE(SetDeviceOrderVpn);
    }

    void from_json(const json& j, _WEBSOCKETINFO& s) {
        OPTIONJSVALUE(Ip);
        OPTIONJSVALUE(Port);
    }

    void from_json(const json& j, _RHLOGINRET& s) {
        OPTIONJSVALUE(UserData);
        OPTIONJSVALUE(DeviceData);
        OPTIONJSVALUE(WebSocketData);
    }

    void from_json(const json& j, _PLAYER_CMD& s){
        OPTIONJSVALUE(winid);
    }
}
