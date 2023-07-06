#pragma once
#include "../base/jsonext.hpp"

/**
 * @brief 收到webt响应的包基础格式
 *
 */
JSONSTRUCT(_WEB_RESPONSE_PACK_BASE) {
    int Code = -1; //业务逻辑状态码
    std::string Msg; //文本提示信息
    std::string requestID; //请求参数中的请求ID
    long long TimeStamp = 0; //响应的时间戳
    int R = -1; //数字签名的随机密钥串索引
    std::string Sign; //签名
    std::string Data; //数据
};

/// <summary>
/// 用户信息
/// </summary>
/// <param name="UserID">用户ID</param>
/// <param name="UserName">用户名</param>
/// <param name="NickName">昵称</param>
/// <param name="AccessToken">访问Token</param>
/// <param name="Photo">用户头像</param>
/// <param name="UploadFileSize">用户已上传文件大小(单位Byte)</param>
/// <param name="UploadTotalSize">用户上传总容量限制(单位GB)</param>
JSONSTRUCT(_USER_DATA) {
    long UserID = 0;
    std::string UserName;
    std::string NickName;
    std::string AccessToken;
    std::string Photo;  
    float UploadFileSize = 0.f;//用户已上传文件大小
    int UploadTotalSize = 0;
};

/// <summary>
/// 登录信息
/// </summary>
/// <param name="UserData">用户信息</param>
JSONSTRUCT(_LOGIN_DATA) {
    _USER_DATA UserData;
};

/// <summary>
/// 分页信息
/// </summary>
/// <param name="PageCount">总页数</param>
/// <param name="CurrentPage">当前页号</param>
/// <param name="PageSize">每页的数据量</param>
/// <param name="DataCount">数据总量</param>
/// <param name="IsLastPage">是否是最后一页</param>
JSONSTRUCT(_PAGES) {
    int PageCount = 0; //总页数
    int CurrentPage = 0;//当前页号
    int PageSize = 0;//每页的数据量
    int DataCount = 0;//数据总量
    bool IsLastPage = true;//是否是最后一页
};




/// <summary>
/// 设备信息
/// </summary>
/// <param name="AuthToken">设备链接授权token</param>
/// <param name="UseWebRTC">是否使用TRC</param>
/// <param name="MobileDeviceID">设备唯一ID</param>
/// <param name="PhoneGUID">设备的32位唯一编码</param>
/// <param name="MobileName">手机名称</param>
/// <param name="PhysicalProperty">设备物理属性 1“虚拟设备  2：真实机器</param>
/// <param name="HostComputerID">所属宿主主机ID</param>
/// <param name="UseState">设备使用状态 2：空闲 1:分配 0：未创建</param>
/// <param name="CreateTime">创建时间,时间戳</param>
/// <param name="LastUpdateTime">最后更新时间，时间戳</param>
/// <param name="HealthState">设备健康状态 1:正常运行 0：故障  2：注销</param>
/// <param name="vgpus">vgpus</param>
/// <param name="RunMemory">运行内存</param>
/// <param name="IP">外网Ip</param>
/// <param name="IPPort">外网端口</param>
/// <param name="LocalIP">本地Ip</param>
/// <param name="LocalPort">内网端口</param>
/// <param name="ControlPort">反控端口</param>
/// <param name="AVPort">视频端口</param>
/// <param name="VoicePort">音频端口</param>
/// <param name="AdbPort">ADB调试服务起始端口</param>
/// <param name="WebRTCIP">Webrtc的服务的地址（只给app端用）</param>
/// <param name="WebRTCPort">Webrtc的服务的端口（只给app端用)</param>
/// <param name="IsRoot">是否root</param>
/// <param name="MultiMode">非多端</param>
/// <param name="MessagePort">消息端口</param>
/// <param name="DeviceSource">设备来源 0:内部设备 1 : 华为设备</param>
JSONSTRUCT(_MOBILEDEVICE) {
    std::string AuthToken; //设备链接授权token
    bool UseWebRTC = true; //是否使用TRC
    long MobileDeviceID = 0;//设备唯一ID
    std::string PhoneGUID;//设备的32位唯一编码
    std::string MobileName;//手机名称
    int PhysicalProperty = 0; //设备物理属性 1“虚拟设备  2：真实机器
    long HostComputerID = 0; //所属宿主主机ID
    int UseState = 0; //设备使用状态 2：空闲 1:分配 0：未创建 
    long CreateTime = 0; //创建时间,时间戳
    long LastUpdateTime = 0; //最后更新时间，时间戳
    int HealthState = 0; //设备健康状态 1:正常运行 0：故障  2：注销
    std::string vgpus; //vgpus
    std::string RunMemory; //运行内存
    std::string IP;//外网Ip
    int IPPort = 0;//外网端口
    std::string LocalIP; //本地Ip
    int LocalPort = 0; //内网端口
    int ControlPort = 0; //反控端口
    int AVPort = 0; //视频端口
    int VoicePort = 0; //音频端口
    int AdbPort = 0; //ADB调试服务起始端口
    std::string WebRTCIP; //Webrtc的服务的地址（只给app端用）
    int WebRTCPort = 0; //Webrtc的服务的端口（只给app端用)
    bool IsRoot = false; //是否root
    int MultiMode = 0; //非多端
    int MessagePort = 0; //消息端口
    int DeviceSource = 0; //设备来源 0:内部设备 1 : 华为设备
};

/// <summary>
/// 手机信息
/// </summary>
/// <param name="ModelID">设备类型ID</param>
/// <param name="ModelName">设备类型名</param>
/// <param name="DeviceOrderID">设备订单ID</param>
/// <param name="MobileDeviceInfo">设备信息</param>
/// <param name="ExpireTime">过期剩余时间（秒）</param>
/// <param name="gID">分组ID</param>
/// <param name="bsel"></param>
/// <param name="BkImg">截图地址</param>
/// <param name="task"></param>
/// <param name="UserVpnConfigID">用户Vpn配置ID(0为用户没有设置Vpn)</param>
JSONSTRUCT(_USERMOBILEDEVICE) {
    long ModelID = 0; //类型ID
    std::string ModelName;
    long DeviceOrderID = 0;//订单ID
    _MOBILEDEVICE MobileDeviceInfo;
    long ExpireTime = 0;//过期剩余时间
    long gID = 0;
    bool bsel = false;
    std::wstring BkImg;
    struct TASK_STATE {
        bool bpending = false;
        unsigned long long ts = 0;
    } task;
    long UserVpnConfigID = 0;
};

/// <summary>
/// 用户当前有效的手机列表
/// </summary>
/// <param name="UserPhones">手机列表</param>
/// <param name="Page">分页信息</param>
JSONSTRUCT(_USERPHONES) {
    std::vector<_USERMOBILEDEVICE> UserPhones;
    _PAGES Page;
};

/// <summary>
/// 用户设备信息
/// </summary>
/// <param name="UserPhone">手机信息</param>
JSONSTRUCT(_USERPHONE) {
    _USERMOBILEDEVICE UserPhone;
};

/// <summary>
/// 分组信息
/// </summary>
/// <param name="ID">分组ID</param>
/// <param name="Name">分组名</param>
/// <param name="DeviceCount">分组下的设备数量</param>
JSONSTRUCT(_GROUP_INFO) {
    long ID = 0;
    std::string Name;
    int DeviceCount = 0;
};

//

/// <summary>
/// 分组对应设备数据
/// </summary>
/// <param name="ID">分组ID</param>
/// <param name="Name">分组名</param>
/// <param name="DeviceCount">分组下的设备数量</param>
/// <param name="MobileDeviceIDs">设备分组信息</param>
JSONSTRUCT(_GROUPINO_AGG_DATA) {
    long ID = 0;
    std::string Name;
    int DeviceCount = 0;
    std::list<long> MobileDeviceIDs;
};

/// <summary>
/// 用户设备分组信息
/// </summary>
/// <param name="GroupDatas">分组列表</param>
JSONSTRUCT(_GROUP_DATAS) {
    std::vector<_GROUP_INFO> GroupDatas;
};

/*
* MobileDeviceInfo 手机设备信息
* ExpireCountDown 剩余过期时间，单位秒（倒计时）
* ExpireTime 过期时间，时间戳
* Extendstring 设备的token格式 Token1,token1过期时间戳
* MultiMode 多端模式 0;兼容线上 1是多端 2是非多端
*/

/// <summary>
/// 连接云机的返回数据
/// </summary>
/// <param name="MobileDeviceInfo">设备信息</param>
/// <param name="ExpireCountDown">剩余过期时间，单位秒（倒计时）</param>
/// <param name="ExpireTime">过期时间，时间戳</param>
/// <param name="Extendstring">设备的token格式 Token1,token1过期时间戳</param>
/// <param name="MultiMode">多端模式 0;兼容线上 1是多端 2是非多端</param>
JSONSTRUCT(_CONNECT_INFO) {
    _MOBILEDEVICE MobileDeviceInfo;//设备信息
    long ExpireCountDown = 0;//剩余过期时间，单位秒（倒计时）
    long ExpireTime = 0;//过期时间，时间戳
    std::string Extendstring;//设备的token格式 Token1,token1过期时间戳
    int MultiMode = 0;//多端模式 0;兼容线上 1是多端 2是非多端
};

/// <summary>
/// 手机机型
/// </summary>
/// <param name="PhoneModel">手机型号</param>
/// <param name="SystemVersion">系统版本</param>
/// <param name="SerialNumber">序列号</param>
/// <param name="IMEI">IMEI</param>
/// <param name="IMSI">IMSI</param>
/// <param name="ICCID">ICCID</param>
/// <param name="WIFI">WIFI名称</param>
/// <param name="WIFIMAC">WIFIMAC</param>
/// <param name="AndroidID">AndroidID</param>
/// <param name="PhoneNumber">手机号</param>
/// <param name="Vendor">手机厂商</param>
/// <param name="OAID">OAID</param>
/// <param name="CountryCode">国家编码</param>
/// <param name="OperatorCode">手机运营商编码</param>
/// <param name="NetworkCode">运营商网络编码</param>
/// <param name="SchemeName">方案名</param>
/// <param name="Numeric">Mcc+Mnc</param>
/// <param name="CreateTime">创建时间</param>
/// <param name="IsSIM">是否启用SIM设置</param>
/// <param name="isHide"></param>
/// <param name="ID">设备ID</param>
/// <param name="DeviceOrderID">设备订单ID</param>
JSONSTRUCT(_PHONE_PARAM) {
    std::string PhoneModel;
    std::string SystemVersion;
    std::string SerialNumber;
    std::string IMEI;
    std::string IMSI;
    std::string ICCID;
    std::string WIFI;
    std::string WIFIMAC;
    std::string AndroidID;
    std::string PhoneNumber;
    std::string Vendor;
    std::string OAID;
    std::string CountryCode;
    std::string OperatorCode;
    std::string NetworkCode;
    std::string SchemeName;
    std::string Numeric;
    std::string CreateTime;
    bool IsSIM = false;
    bool isHide = false;
    long ID = 0;
    long DeviceOrderID = 0;
};

/// <summary>
/// 机型厂商
/// </summary>
/// <param name="Vendor">手机厂商</param>
/// <param name="PhoneParams">机型参数</param>
JSONSTRUCT(_DEVICE_LIBRARY) {
    std::string Vendor;
    std::vector<_PHONE_PARAM> PhoneParams;
};

/// <summary>
/// 机型库
/// </summary>
/// <param name="DeviceLibrarys">机型列表</param>
JSONSTRUCT(_DEVICE_LIBS) {
    std::vector<_DEVICE_LIBRARY> DeviceLibrarys;
};

/// <summary>
/// 指定机型数据
/// </summary>
/// <param name="">机型列表</param>
JSONSTRUCT(_PHONE_PARAM_INFO) {
    _PHONE_PARAM PhoneParamInfo;
};

/// <summary>
/// 用户一键新机方案--方案
/// </summary>
/// <param name="NewPhoneSchemes">方案列表</param>
JSONSTRUCT(_HISTORY_SLN) {
    std::vector<_PHONE_PARAM> NewPhoneSchemes;
};

/// <summary>
/// 设备类型信息
/// </summary>
/// <param name="ID">设备类型ID</param>
/// <param name="ShowOrder">显示顺序，倒序排序</param>
/// <param name="bicon_down">图标是否下载</param>
/// <param name="Name">设备类型名称</param>
/// <param name="IconPath">图标地址</param>
/// <param name="localPath">图标加载地址</param>
JSONSTRUCT(_MODEL_INFO) {
    long ID = 0;
    int ShowOrder = 0;
    bool bicon_down = false;
    std::string Name;
    std::string IconPath;
    std::wstring localPath;
};

/// <summary>
/// App打开web页面的url配置信息
/// </summary>
/// <param name="PageCode">页面编码 
/// 5001充值
/// 5002续费
/// 5003订单
/// 5004客服
/// 5005 Google登录
/// 5006 FaceBook登陆
/// 5008 QQ登陆
/// 5009 微信登陆
/// 5010  Pc激活码支付</param>
/// <param name="OpenUrl">url地址</param>
JSONSTRUCT(_APP_OPEN_PAGE_URL) {
    long PageCode = 0;
    std::string OpenUrl;
};

/// <summary>
/// 统一开关配置
/// </summary>
/// <param name="ConfigCode">编码
/// 1001：文件管理
/// 1002：数据收集
/// 2001：Google包应用列表获取权限提示
/// 1003：错误日志上报统计开关
/// 1004：联系客服开关
/// 1007：Vpn显示开关</param>
/// <param name="IsOpen">是否开启  1：开启 0：关闭</param>
JSONSTRUCT(_SWITCH_CONFIG) {
    int ConfigCode = 0;
    int IsOpen = 0;
};

/// <summary>
/// 统一参数配置
/// </summary>
/// <param name="ConfigCode">编码
/// 1001：码率配置
/// 1002：GVIP分辨率
/// 1003：VIP分辨率
/// 1004：新码率配置
/// 1005：多开配置
/// 1006：H5码率配置
/// 2006：帧率切换
/// </param>
/// <param name="Value">下发值</param>
JSONSTRUCT(_PARAM_CONFIG) {
    int ConfigCode = 0;
    std::string Value;
};


JSONSTRUCT(_BITRATE) {
    int defBitrate = 1500;
    int bitrate1 = 1500;
    int bitrate2 = 2000;
    int bitrate3 = 3000;
};

/// <summary>
/// 分辨率配置
/// </summary>
/// <param name="ResolutionId">分辨率配置ID</param>
/// <param name="ResolutionName">名称</param>
/// <param name="ModelID">设备类型ID</param>
/// <param name="Width">宽</param>
/// <param name="Height">高</param>
/// <param name="MultiMode">多端模式 0兼容线上 1是多端 2是非多端</param>
/// <param name="HardwareWidth">宽</param>
/// <param name="HardwareHeight">高</param>
JSONSTRUCT(_RESOLUTION_SLN) {
    int ResolutionId = 0;
    std::string ResolutionName;
    int ModelID = 0;
    int Width = 0;
    int Height = 0;
    int MultiMode = 0;
    int HardwareWidth = 0;
    int HardwareHeight = 0;
};

/// <summary>
/// 系统预制接口
/// </summary>
/// <param name="ModelInfos">设备规格类型列表</param>
/// <param name="AppOpenPageUrls">App打开web页面的url配置信息</param>
/// <param name="SwitchConfigs">统一开关配置</param>
/// <param name="ParamConfigs">统一参数配置</param>
/// <param name="Phoneresolutionconfigs">分辨率配置</param>
/// <param name="BackupLimit">用户备份数据限额</param>
/// <param name="UploadFileSizeLimit">用户上传文件大小限制，单位是G</param>
/// <param name="StorageConfig">obs存储的配置文件</param>
JSONSTRUCT(_SYSINIT_INFO) {
    std::vector<_MODEL_INFO> ModelInfos;
    std::vector<_APP_OPEN_PAGE_URL> AppOpenPageUrls;
    std::vector<_SWITCH_CONFIG> SwitchConfigs;
    std::vector<_PARAM_CONFIG> ParamConfigs;
    std::vector<_RESOLUTION_SLN> Phoneresolutionconfigs;
    int BackupLimit = 0; //用户备份数据限额
    int UploadFileSizeLimit = 0; //用户上传文件大小限制，单位是G
    std::string StorageConfig;

    /// <summary>
    /// 获取url配置信息
    /// </summary>
    /// <param name="id">页面编码</param>
    /// <returns>url配置</returns>
    std::string GetUrlByCode(long id) {
        std::string strurl = "about:black";
        for (auto it : AppOpenPageUrls) {
            if (it.PageCode == id) {
                strurl = it.OpenUrl.c_str();
            }
        }
        return strurl;
    }

    /// <summary>
    /// 查找设备类型名称
    /// </summary>
    /// <param name="id">设备类型ID</param>
    /// <param name="name">用于保存类型名称</param>
    /// <returns>是否找到 true为找到，false为未找到</returns>
    bool FindModelName(long id, std::string& name) {
        bool bfind = false;
        for (auto it = ModelInfos.begin(); it != ModelInfos.end(); ++it) {
            if (it->ID == id) {
                name = it->Name;
                bfind = true;
                break;
            }
        }
        return bfind;
    }

    /// <summary>
    /// 获取码率配置
    /// </summary>
    /// <returns>jsondata</returns>
    std::string GetBitrate() {
        std::string str;
        for (auto it : ParamConfigs) {
            if (it.ConfigCode == 1004) {
                str = it.Value.c_str();
                break;
            }
        }
        return str;
    }

    /// <summary>
    /// 获取vip设备分辨率配置
    /// </summary>
    /// <returns>jsondata</returns>
    std::string GetVipResolution() {
        std::string str;
        for (auto it : ParamConfigs) {
            if (it.ConfigCode == 1003) {
                str = it.Value.c_str();
                break;
            }
        }
        return str;
    }

    /// <summary>
    /// 获取gvip设备分辨率配置
    /// </summary>
    /// <returns>jsondata</returns>
    std::string GetGVipResolution() {
        std::string str;
        for (auto it : ParamConfigs) {
            if (it.ConfigCode == 1002) {
                str = it.Value.c_str();
                break;
            }
        }
        return str;
    }

    /// <summary>
    /// 获取多开配置
    /// </summary>
    /// <returns>jsondata</returns>
    std::string GetMulitModeConfig() {
        std::string str;
        for (auto it : ParamConfigs) {
            if (it.ConfigCode == 1005) {
                str = it.Value.c_str();
                break;
            }
        }
        return str;
    }

    /// <summary>
    /// 获取是否使用多端下载功能
    /// </summary>
    /// <returns>true为使用，false为不使用</returns>
    bool GetApplyDownload()
    {
        bool isopen = false;
        for (auto it : SwitchConfigs)
        {
            if (it.ConfigCode == 1006)
            {
                isopen = it.IsOpen;
            }
        }
        return isopen;
    }

    /// <summary>
    /// 获取帧率配置
    /// </summary>
    /// <returns>jsondata</returns>
    std::string GetFps()
    {
        std::string str;
        for (auto it : ParamConfigs) {
            if (it.ConfigCode == 2006) {
                str = it.Value.c_str();
                break;
            }
        }
        return str;
    }

    /// <summary>
    /// 获取符合要求的分辨率配置信息
    /// </summary>
    /// <param name="modelid">设备类型ID</param>
    /// <param name="multiMode">多端模式</param>
    /// <returns>分辨率配置信息</returns>
    std::vector<_RESOLUTION_SLN> FilterResolution(int modelid, int multiMode) {
        std::vector<_RESOLUTION_SLN> vec;
        for (auto it : Phoneresolutionconfigs) {
            if (multiMode == -1) {
                if (it.ModelID == modelid) {
                    vec.push_back(it);
                }
            }
            else if (it.ModelID == modelid && it.MultiMode == multiMode) {
                vec.push_back(it);
            }
        }
        return vec;
    }

    /// <summary>
    /// 设置图标是否下载
    /// </summary>
    /// <param name="modelid">设备类型ID</param>
    void SetIconDown(int modelid) {
        auto it = ModelInfos.begin();
        for (; it != ModelInfos.end(); ++it) {
            if (it->ID == modelid) {
                it->bicon_down = true;
                break;
            }
        }
    }

    /// <summary>
    /// 获取图标
    /// </summary>
    /// <param name="modelid">设备类型ID</param>
    /// <param name="path">图标加载地址</param>
    /// <returns>true为获取到图标，false为未获取到图标</returns>
    bool GetIcon(int modelid, std::wstring& path) {
        bool ret = false;
        auto it = ModelInfos.begin();
        for (; it != ModelInfos.end(); ++it) {
            if (it->ID == modelid) {
                path = it->localPath;
                ret = it->bicon_down;
                break;
            }
        }
        return ret;
    }

    /// <summary>
    /// 获取是否使用VPN功能
    /// </summary>
    /// <returns>true为使用，false为不使用</returns>
    bool GetVPN()
    {
        bool isopen = false;
        for (auto it : SwitchConfigs)
        {
            if (it.ConfigCode == 1007)
            {
                isopen = it.IsOpen;
            }
        }
        return isopen;
    }
};

JSONSTRUCT(_NAVITE_JSPARAM) {
    std::string name;
    std::string param;
    std::string asyncID;
    std::string ddframe;
};

JSONSTRUCT(_OPEN_BROWSER_JSPARAM) {
    std::string url;
};

/// <summary>
/// 签名文件
/// </summary>
/// <param name="FileExists">文件是否已经存在 true:表示文件存在了不需要在上传了</param>
///  <param name="FilePath">签名后的完整地址 FileExists=true给的是下载地址</param>
///  <param name="ExpireSecond">有效时间倒计时，单位秒。默认10分钟</param>
///  <param name="Key">对应请求参数中FilePath的一个相对路径</param>
///  <param name="Md5">Md5</param>
///  <param name="fullpath">完整地址</param>
///  <param name="wfullpath"></param>
///  <param name="FileSize">文件大小</param>
///  <param name="FileType">文件类型</param>
///  <param name="FileClassify">文件分类</param>
///  <param name="arg"></param>
///  <param name="disable">是否禁用</param>
///  <param name="packagename">包名</param>
/// <param name="versioncode">版本号</param>
JSONSTRUCT(_SIGNFILEPATH) {
    bool FileExists = false;
    std::string FilePath;
    int ExpireSecond = 0; //有效时间倒计时，单位秒
    std::string Key;
    std::string Md5;
    std::string fullpath;
    std::wstring wfullpath;
    float FileSize = 0.f;
    std::string FileType;
    int FileClassify = 2;
    void* arg = nullptr;
    bool disable = false;
    std::string packagename;
    std::string versioncode;
};

/// <summary>
/// 文件签名
/// </summary>
/// <param name="SignFilePaths">签名文件列表</param>
JSONSTRUCT(_SIGNFILEPATHS) {
    std::vector<_SIGNFILEPATH> SignFilePaths;
};

/// <summary>
/// 文件信息
/// </summary>
/// <param name="UserFileID">文件Id</param>
/// <param name="FileName">文件名称</param>
/// <param name="FilePth">文件存放地址</param>
/// <param name="FileDomain">文件存放地址域名</param>
/// <param name="FileMd5">文件Md5</param>
/// <param name="FileSize">文件大小 单位Byte</param>
/// <param name="FileType">文件类型 例如 MP3,ZIP,jpg</param>
/// <param name="FileDesc">文件描述</param>
/// <param name="IsInstall">是否已经安装过</param>
/// <param name="FileExtendJson">文件扩展信息,app传上来，原封不动下发</param>
/// <param name="FileClassify">文件分类 1:应用  2：文件  3:备份数据</param>
/// <param name="MobileDeviceID">备份数据的机器ID FileClassify=3有效</param>
/// <param name="UploadChannel">上传渠道1；应用商店  2：用户云机 3：后台</param>
JSONSTRUCT(_FILEINFO) {
    long UserFileID = 0;
    std::string FileName;
    std::string FilePth;
    std::string FileDomain;
    std::string FileMd5;
    float FileSize = 0.f;
    std::string FileType;
    std::string FileDesc;
    bool IsInstall = false;
    std::string FileExtendJson;
    int FileClassify = 0;
    long MobileDeviceID = 0;
    int UploadChannel = 0;
};

/// <summary>
/// 用户上传的文件列表
/// </summary>
/// <param name="FileDatas">文件表</param>
/// <param name="Page">分页信息</param>
JSONSTRUCT(_USERFILES) {
    std::vector<_FILEINFO> FileDatas;
    _PAGES Page;
};

/// <summary>
/// 用户一键新机方案--分页
/// </summary>
/// <param name="Page">分页信息</param>
JSONSTRUCT(_HISTORY_SLN_PAGE) {
    _PAGES Page;
};

/// <summary>
/// 应用信息
/// </summary>
/// <param name="icon">app图标地址</param>
/// <param name="packageName">包名</param>
/// <param name="name">app名称</param>
/// <param name="isRoot">是否root</param>
JSONSTRUCT(APPINFO) {
    std::string icon;
    std::string packageName;
    std::string name;
    bool isRoot = false;
};

/// <summary>
/// 应用列表
/// </summary>
/// <param name="data">应用列表</param>
JSONSTRUCT(APPS) {
    std::vector<APPINFO> data;
};

JSONSTRUCT(WSDATA) {
    int type;
    std::string data;
};

/// <summary>
/// 应用区服信息
/// </summary>
/// <param name="AreaID">区服主键ID</param>
/// <param name="AreaName">名称</param>
/// <param name="ICON">app图标地址</param>
/// <param name="FileMd5">MD5</param>
/// <param name="AppDownUrl">下载地址</param>
/// <param name="AppSize">文件大小 单位MB</param>
/// <param name="AppVersion">版本号</param>
/// <param name="PackageName">包名</param>
/// <param name="InnerVersion">内部版本号</param>
JSONSTRUCT(AREASERVERINFO) {
    long AreaID = 0;
    std::string AreaName;
    std::string ICON;
    std::string FileMd5;
    std::string AppDownUrl;
    float AppSize = 0.f;
    std::string AppVersion;
    std::string PackageName;
    long InnerVersion = 0;
};

/// <summary>
/// 应用信息
/// </summary>
/// <param name="ID">主键ID</param>
/// <param name="Name">名称</param>
/// <param name="ICON">app图标地址</param>
/// <param name="AppType">应用类型 1:游戏 2：软件</param>
/// <param name="Areas">区服信息</param>
JSONSTRUCT(MARKETAPPINFO) {
    long ID = 0;
    std::string Name;
    std::string ICON;
    int AppType = 0;
    std::vector<AREASERVERINFO> Areas;
};

/// <summary>
/// 应用列表
/// </summary>
/// <param name="AppInfos">应用列表</param>
/// <param name="Page">分页信息</param>
JSONSTRUCT(MARKETAPP) {
    std::vector<MARKETAPPINFO> AppInfos;
    _PAGES Page;
};

/// <summary>
/// 上传文件变化信息
/// </summary>
/// <param name="UploadFileSize">用户已使用空间大小</param>
/// <param name="UploadFileId">上传文件ID</param>
JSONSTRUCT(UPLOADFILESUCC) {
    float UploadFileSize = 0.f;
    long UploadFileId = 0;
};

/// <summary>
/// 更新信息
/// </summary>
/// <param name="AppName">App版本名称</param>
/// <param name="AppVersion">App版本</param>
/// <param name="AppVersionCode">App版本Code，例如1.1.1</param>
/// <param name="AppUrl">包地址</param>
/// <param name="AppSize">包大小</param>
/// <param name="UpdateType">更新类型 1：强制更新 2：可选更新</param>
/// <param name="UpdateContent">更新内容</param>
/// <param name="UrlType">地址类型 1:下载 2：跳转</param>
JSONSTRUCT(GETAPPVERSION) {
    std::string AppName;
    int AppVersion = 0;
    std::string AppVersionCode;
    std::string AppUrl;
    std::string AppSize;
    int UpdateType = 2; //更新类型 1：强制更新 2：可选更新
    std::string UpdateContent;
    int UrlType = 1; //地址类型 1:下载 2：跳转
};

JSONSTRUCT(BASE_PROTO) {
    int cmdType = 0;
    int device = 0;
    unsigned int reqid = 0;
    bool isSuccess = false;
    std::string stt;
    std::string data;
};

JSONSTRUCT(HOST_DOMAIN) {
    std::string RsHost;
    std::string RsfHost;
    std::string SrcUpHosts;
};

JSONSTRUCT(QRDATA) {
    long id = 0;
    int code = 0;
    std::string data;
};

JSONSTRUCT(ERROR_PATCH) {
    std::vector<std::string> ErrorPatchGUIDs;
};

JSONSTRUCT(PATCH_TASK_GUID) {
    std::string BatchGUID;
};

JSONSTRUCT(PATCH_RESULT_ITEM) {
    long MobileDeviceID = 0;
    int PStatus = 0;
    long DeviceOrderID = 0;
};

JSONSTRUCT(PATCH_RESULT_ARRAY) {
    std::vector<PATCH_RESULT_ITEM> InstallResult;
};

JSONSTRUCT(_BITRATE_ITEM) {
    std::string name;
    int bitrate = 1500;
};

JSONSTRUCT(_BITRATE_ROOT) {
    std::string type;
    std::vector<_BITRATE_ITEM> qList;
};

JSONSTRUCT(_RESOLUTION) {
    int width = 1280;
    int height = 720;
    int dpi = 330;
};

JSONSTRUCT(_MULTI_MODE_ITEM) {
    int width = 360;
    int height = 640;
    int fps = 10;
    int conntype = 1;
    int datatype = 4;
    int bitrate = 600;
    bool autobit = false;
};

JSONSTRUCT(_LANG) {
    std::string lang;
    std::string value;
};

JSONSTRUCT(_MULTI_MODE) {
    std::vector<_LANG> name;
    _MULTI_MODE_ITEM data;
};

JSONSTRUCT(_NETWORK) {
    std::string NetworkName;
    std::string NetworkCode;
    std::string Numeric;
};

JSONSTRUCT(_ISP) {
    std::string OperatorName;
    std::string OperatorCode;
    std::string NetworkCode;
    std::string Numeric;
    std::vector<_NETWORK> NetworkList;
};

JSONSTRUCT(_COUNTRY_CELLULAR) {
    std::string CountryName;
    std::string CountryCode;
    std::vector<_ISP> MobileOperatorList;
};

JSONSTRUCT(_COUNTRY_CELLULAR_ARRAY) {
    std::vector<_COUNTRY_CELLULAR> CountryList;
};

JSONSTRUCT(TYPE_DEFAULTSET) {
    std::string Protocol;
    std::string Domain;
};

JSONSTRUCT(TYPE_SPECIALSET) {
    std::string key;
    std::string Domain;
    std::string SLD;
    std::string Protocol;
};

JSONSTRUCT(DEFAULTSET) {
    TYPE_DEFAULTSET DefaultSet;
    std::vector<TYPE_SPECIALSET> SpecialSet;
};

JSONSTRUCT(FRAMERATE)
{
    int fps = 30;
};

JSONSTRUCT(CODETOKEN)
{
    std::string CodeToken;
};

/// <summary>
/// 消息信息
/// </summary>
/// <param name="ID">消息ID</param>
/// <param name="MsgType">消息类型 1:平台消息  2：个人消息</param>
/// <param name="MsgSource">消息来源  2:App 13:Server  14:Admin</param>
/// <param name="Title">标题</param>
/// <param name="Content">消息内容</param>
/// <param name="MsgDate">消息时间，格式：yyyy-MM-dd HH-mm-ss</param>
/// <param name="MsgDisplay">消息展示方式  0:无  1:提示</param>
JSONSTRUCT(MSGINFO)
{
    long ID = 0;//消息ID
    int MsgType = 0;//消息类型 1:平台消息  2：个人消息
    int MsgSource = 0;//消息来源  2:App 13:Server  14:Admin
    std::string Title;//标题
    std::string Content;//消息内容
    std::string MsgDate;//消息时间，格式：yyyy-MM-dd HH-mm-ss
    int MsgDisplay = 0;//消息展示方式  0:无  1:提示
};
JSONSTRUCT(DEVICEAUTHTOKEN)
{
    std::string AccessToken;
    long ExpireTime = 0;
    long ExpireCountDown = 0;
};

/// <summary>
/// 消息列表
/// </summary>
/// <param name="MsgData">消息列表</param>
JSONSTRUCT(MSGDATA)
{
    std::vector<MSGINFO> MsgData;//消息列表
};


JSONSTRUCT(MOBILEDEVICETOKEN)
{
    long DeviceOrderID = 0;
    DEVICEAUTHTOKEN DeviceAuthToken;
};

JSONSTRUCT(MOBILEDEVICETOKENS)
{
    std::vector<MOBILEDEVICETOKEN> MobileDeviceTokens;
};

/// <summary>
/// vpn分类
/// </summary>
/// <param name="ID">vpnID</param>
/// <param name="Name">vpn名称</param>
JSONSTRUCT(VPNCLASSIFY) {
    int ID = 0;
    std::string Name;
};

/// <summary>
/// 用户VPN信息
/// </summary>
/// <param name="ID">vpnID</param>
/// <param name="Name">用户Vpn名称</param>
/// <param name="Classify">vpn分类</param>
/// <param name="Address">地址</param>
/// <param name="Prot">端口</param>
/// <param name="Account">账号</param>
/// <param name="Password">密码</param>
JSONSTRUCT(USERVPN)
{
    long ID = 0;
    std::string Name; //用户Vpn名称
    int Classify = 0; //分类
    std::string Address; //地址
    int Prot = 0; //端口
    std::string Account; //账号
    std::string Password; //密码
};

/// <summary>
/// 用户VPN列表
/// </summary>
/// <param name="UserVpnList">用户已有Vpn列表</param>
/// <param name="VpnClassifyList">Vpn分类列表</param>
JSONSTRUCT(_USERVPNLIST)
{
    std::vector<USERVPN> UserVpnList;
    std::vector<VPNCLASSIFY> VpnClassifyList;
};

/// <summary>
/// 设置设备订单Vpn信息
/// </summary>
/// <param name="DeviceOrderID">设备订单ID</param>
/// <param name="UserVpnConfigID">用户vpn配置ID</param>
JSONSTRUCT(DEVICEORDERVPN)
{
    long DeviceOrderID = 0;
    long UserVpnConfigID = 0;
};

/// <summary>
/// 设置Vpn
/// </summary>
/// <param name="SetDeviceOrderVpn">设置设备订单Vpn</param>
JSONSTRUCT(SETVPN)
{
    std::vector<DEVICEORDERVPN> SetDeviceOrderVpn;
};

JSONSTRUCT(_SCREENCAP)
{
    std::string image;
    std::string vpnstatus;
};

JSONSTRUCT(_WEBSOCKETINFO) {
    std::string Ip;
    int Port = 0;
};

JSONSTRUCT(_RHLOGINRET) {
    _USER_DATA UserData;
    _MOBILEDEVICE DeviceData;
    _WEBSOCKETINFO WebSocketData;
};

bool isSuccResponse(_WEB_RESPONSE_PACK_BASE&);

namespace nlohmann{
    void from_json(const json& j, _WEB_RESPONSE_PACK_BASE& s);
    void from_json(const json& j, _RHLOGINRET& s);
}

class eInstructions
{
public:
    eInstructions();
};

