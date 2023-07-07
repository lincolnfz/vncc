#pragma once
#include "../base/jsonext.hpp"

/**
 * @brief �յ�webt��Ӧ�İ�������ʽ
 *
 */
JSONSTRUCT(_WEB_RESPONSE_PACK_BASE) {
    int Code = -1; //ҵ���߼�״̬��
    std::string Msg; //�ı���ʾ��Ϣ
    std::string requestID; //��������е�����ID
    long long TimeStamp = 0; //��Ӧ��ʱ���
    int R = -1; //����ǩ���������Կ������
    std::string Sign; //ǩ��
    std::string Data; //����
};

/// <summary>
/// �û���Ϣ
/// </summary>
/// <param name="UserID">�û�ID</param>
/// <param name="UserName">�û���</param>
/// <param name="NickName">�ǳ�</param>
/// <param name="AccessToken">����Token</param>
/// <param name="Photo">�û�ͷ��</param>
/// <param name="UploadFileSize">�û����ϴ��ļ���С(��λByte)</param>
/// <param name="UploadTotalSize">�û��ϴ�����������(��λGB)</param>
JSONSTRUCT(_USER_DATA) {
    long UserID = 0;
    std::string UserName;
    std::string NickName;
    std::string AccessToken;
    std::string Photo;  
    float UploadFileSize = 0.f;//�û����ϴ��ļ���С
    int UploadTotalSize = 0;
};

/// <summary>
/// ��¼��Ϣ
/// </summary>
/// <param name="UserData">�û���Ϣ</param>
JSONSTRUCT(_LOGIN_DATA) {
    _USER_DATA UserData;
};

/// <summary>
/// ��ҳ��Ϣ
/// </summary>
/// <param name="PageCount">��ҳ��</param>
/// <param name="CurrentPage">��ǰҳ��</param>
/// <param name="PageSize">ÿҳ��������</param>
/// <param name="DataCount">��������</param>
/// <param name="IsLastPage">�Ƿ������һҳ</param>
JSONSTRUCT(_PAGES) {
    int PageCount = 0; //��ҳ��
    int CurrentPage = 0;//��ǰҳ��
    int PageSize = 0;//ÿҳ��������
    int DataCount = 0;//��������
    bool IsLastPage = true;//�Ƿ������һҳ
};




/// <summary>
/// �豸��Ϣ
/// </summary>
/// <param name="AuthToken">�豸������Ȩtoken</param>
/// <param name="UseWebRTC">�Ƿ�ʹ��TRC</param>
/// <param name="MobileDeviceID">�豸ΨһID</param>
/// <param name="PhoneGUID">�豸��32λΨһ����</param>
/// <param name="MobileName">�ֻ�����</param>
/// <param name="PhysicalProperty">�豸�������� 1�������豸  2����ʵ����</param>
/// <param name="HostComputerID">������������ID</param>
/// <param name="UseState">�豸ʹ��״̬ 2������ 1:���� 0��δ����</param>
/// <param name="CreateTime">����ʱ��,ʱ���</param>
/// <param name="LastUpdateTime">������ʱ�䣬ʱ���</param>
/// <param name="HealthState">�豸����״̬ 1:�������� 0������  2��ע��</param>
/// <param name="vgpus">vgpus</param>
/// <param name="RunMemory">�����ڴ�</param>
/// <param name="IP">����Ip</param>
/// <param name="IPPort">�����˿�</param>
/// <param name="LocalIP">����Ip</param>
/// <param name="LocalPort">�����˿�</param>
/// <param name="ControlPort">���ض˿�</param>
/// <param name="AVPort">��Ƶ�˿�</param>
/// <param name="VoicePort">��Ƶ�˿�</param>
/// <param name="AdbPort">ADB���Է�����ʼ�˿�</param>
/// <param name="WebRTCIP">Webrtc�ķ���ĵ�ַ��ֻ��app���ã�</param>
/// <param name="WebRTCPort">Webrtc�ķ���Ķ˿ڣ�ֻ��app����)</param>
/// <param name="IsRoot">�Ƿ�root</param>
/// <param name="MultiMode">�Ƕ��</param>
/// <param name="MessagePort">��Ϣ�˿�</param>
/// <param name="DeviceSource">�豸��Դ 0:�ڲ��豸 1 : ��Ϊ�豸</param>
JSONSTRUCT(_MOBILEDEVICE) {
    std::string AuthToken; //�豸������Ȩtoken
    bool UseWebRTC = true; //�Ƿ�ʹ��TRC
    long MobileDeviceID = 0;//�豸ΨһID
    std::string PhoneGUID;//�豸��32λΨһ����
    std::string MobileName;//�ֻ�����
    int PhysicalProperty = 0; //�豸�������� 1�������豸  2����ʵ����
    long HostComputerID = 0; //������������ID
    int UseState = 0; //�豸ʹ��״̬ 2������ 1:���� 0��δ���� 
    long CreateTime = 0; //����ʱ��,ʱ���
    long LastUpdateTime = 0; //������ʱ�䣬ʱ���
    int HealthState = 0; //�豸����״̬ 1:�������� 0������  2��ע��
    std::string vgpus; //vgpus
    std::string RunMemory; //�����ڴ�
    std::string IP;//����Ip
    int IPPort = 0;//�����˿�
    std::string LocalIP; //����Ip
    int LocalPort = 0; //�����˿�
    int ControlPort = 0; //���ض˿�
    int AVPort = 0; //��Ƶ�˿�
    int VoicePort = 0; //��Ƶ�˿�
    int AdbPort = 0; //ADB���Է�����ʼ�˿�
    std::string WebRTCIP; //Webrtc�ķ���ĵ�ַ��ֻ��app���ã�
    int WebRTCPort = 0; //Webrtc�ķ���Ķ˿ڣ�ֻ��app����)
    bool IsRoot = false; //�Ƿ�root
    int MultiMode = 0; //�Ƕ��
    int MessagePort = 0; //��Ϣ�˿�
    int DeviceSource = 0; //�豸��Դ 0:�ڲ��豸 1 : ��Ϊ�豸
};

/// <summary>
/// �ֻ���Ϣ
/// </summary>
/// <param name="ModelID">�豸����ID</param>
/// <param name="ModelName">�豸������</param>
/// <param name="DeviceOrderID">�豸����ID</param>
/// <param name="MobileDeviceInfo">�豸��Ϣ</param>
/// <param name="ExpireTime">����ʣ��ʱ�䣨�룩</param>
/// <param name="gID">����ID</param>
/// <param name="bsel"></param>
/// <param name="BkImg">��ͼ��ַ</param>
/// <param name="task"></param>
/// <param name="UserVpnConfigID">�û�Vpn����ID(0Ϊ�û�û������Vpn)</param>
JSONSTRUCT(_USERMOBILEDEVICE) {
    long ModelID = 0; //����ID
    std::string ModelName;
    long DeviceOrderID = 0;//����ID
    _MOBILEDEVICE MobileDeviceInfo;
    long ExpireTime = 0;//����ʣ��ʱ��
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
/// �û���ǰ��Ч���ֻ��б�
/// </summary>
/// <param name="UserPhones">�ֻ��б�</param>
/// <param name="Page">��ҳ��Ϣ</param>
JSONSTRUCT(_USERPHONES) {
    std::vector<_USERMOBILEDEVICE> UserPhones;
    _PAGES Page;
};

/// <summary>
/// �û��豸��Ϣ
/// </summary>
/// <param name="UserPhone">�ֻ���Ϣ</param>
JSONSTRUCT(_USERPHONE) {
    _USERMOBILEDEVICE UserPhone;
};

/// <summary>
/// ������Ϣ
/// </summary>
/// <param name="ID">����ID</param>
/// <param name="Name">������</param>
/// <param name="DeviceCount">�����µ��豸����</param>
JSONSTRUCT(_GROUP_INFO) {
    long ID = 0;
    std::string Name;
    int DeviceCount = 0;
};

//

/// <summary>
/// �����Ӧ�豸����
/// </summary>
/// <param name="ID">����ID</param>
/// <param name="Name">������</param>
/// <param name="DeviceCount">�����µ��豸����</param>
/// <param name="MobileDeviceIDs">�豸������Ϣ</param>
JSONSTRUCT(_GROUPINO_AGG_DATA) {
    long ID = 0;
    std::string Name;
    int DeviceCount = 0;
    std::list<long> MobileDeviceIDs;
};

/// <summary>
/// �û��豸������Ϣ
/// </summary>
/// <param name="GroupDatas">�����б�</param>
JSONSTRUCT(_GROUP_DATAS) {
    std::vector<_GROUP_INFO> GroupDatas;
};

/*
* MobileDeviceInfo �ֻ��豸��Ϣ
* ExpireCountDown ʣ�����ʱ�䣬��λ�루����ʱ��
* ExpireTime ����ʱ�䣬ʱ���
* Extendstring �豸��token��ʽ Token1,token1����ʱ���
* MultiMode ���ģʽ 0;�������� 1�Ƕ�� 2�ǷǶ��
*/

/// <summary>
/// �����ƻ��ķ�������
/// </summary>
/// <param name="MobileDeviceInfo">�豸��Ϣ</param>
/// <param name="ExpireCountDown">ʣ�����ʱ�䣬��λ�루����ʱ��</param>
/// <param name="ExpireTime">����ʱ�䣬ʱ���</param>
/// <param name="Extendstring">�豸��token��ʽ Token1,token1����ʱ���</param>
/// <param name="MultiMode">���ģʽ 0;�������� 1�Ƕ�� 2�ǷǶ��</param>
JSONSTRUCT(_CONNECT_INFO) {
    _MOBILEDEVICE MobileDeviceInfo;//�豸��Ϣ
    long ExpireCountDown = 0;//ʣ�����ʱ�䣬��λ�루����ʱ��
    long ExpireTime = 0;//����ʱ�䣬ʱ���
    std::string Extendstring;//�豸��token��ʽ Token1,token1����ʱ���
    int MultiMode = 0;//���ģʽ 0;�������� 1�Ƕ�� 2�ǷǶ��
};

/// <summary>
/// �ֻ�����
/// </summary>
/// <param name="PhoneModel">�ֻ��ͺ�</param>
/// <param name="SystemVersion">ϵͳ�汾</param>
/// <param name="SerialNumber">���к�</param>
/// <param name="IMEI">IMEI</param>
/// <param name="IMSI">IMSI</param>
/// <param name="ICCID">ICCID</param>
/// <param name="WIFI">WIFI����</param>
/// <param name="WIFIMAC">WIFIMAC</param>
/// <param name="AndroidID">AndroidID</param>
/// <param name="PhoneNumber">�ֻ���</param>
/// <param name="Vendor">�ֻ�����</param>
/// <param name="OAID">OAID</param>
/// <param name="CountryCode">���ұ���</param>
/// <param name="OperatorCode">�ֻ���Ӫ�̱���</param>
/// <param name="NetworkCode">��Ӫ���������</param>
/// <param name="SchemeName">������</param>
/// <param name="Numeric">Mcc+Mnc</param>
/// <param name="CreateTime">����ʱ��</param>
/// <param name="IsSIM">�Ƿ�����SIM����</param>
/// <param name="isHide"></param>
/// <param name="ID">�豸ID</param>
/// <param name="DeviceOrderID">�豸����ID</param>
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
/// ���ͳ���
/// </summary>
/// <param name="Vendor">�ֻ�����</param>
/// <param name="PhoneParams">���Ͳ���</param>
JSONSTRUCT(_DEVICE_LIBRARY) {
    std::string Vendor;
    std::vector<_PHONE_PARAM> PhoneParams;
};

/// <summary>
/// ���Ϳ�
/// </summary>
/// <param name="DeviceLibrarys">�����б�</param>
JSONSTRUCT(_DEVICE_LIBS) {
    std::vector<_DEVICE_LIBRARY> DeviceLibrarys;
};

/// <summary>
/// ָ����������
/// </summary>
/// <param name="">�����б�</param>
JSONSTRUCT(_PHONE_PARAM_INFO) {
    _PHONE_PARAM PhoneParamInfo;
};

/// <summary>
/// �û�һ���»�����--����
/// </summary>
/// <param name="NewPhoneSchemes">�����б�</param>
JSONSTRUCT(_HISTORY_SLN) {
    std::vector<_PHONE_PARAM> NewPhoneSchemes;
};

/// <summary>
/// �豸������Ϣ
/// </summary>
/// <param name="ID">�豸����ID</param>
/// <param name="ShowOrder">��ʾ˳�򣬵�������</param>
/// <param name="bicon_down">ͼ���Ƿ�����</param>
/// <param name="Name">�豸��������</param>
/// <param name="IconPath">ͼ���ַ</param>
/// <param name="localPath">ͼ����ص�ַ</param>
JSONSTRUCT(_MODEL_INFO) {
    long ID = 0;
    int ShowOrder = 0;
    bool bicon_down = false;
    std::string Name;
    std::string IconPath;
    std::wstring localPath;
};

/// <summary>
/// App��webҳ���url������Ϣ
/// </summary>
/// <param name="PageCode">ҳ����� 
/// 5001��ֵ
/// 5002����
/// 5003����
/// 5004�ͷ�
/// 5005 Google��¼
/// 5006 FaceBook��½
/// 5008 QQ��½
/// 5009 ΢�ŵ�½
/// 5010  Pc������֧��</param>
/// <param name="OpenUrl">url��ַ</param>
JSONSTRUCT(_APP_OPEN_PAGE_URL) {
    long PageCode = 0;
    std::string OpenUrl;
};

/// <summary>
/// ͳһ��������
/// </summary>
/// <param name="ConfigCode">����
/// 1001���ļ�����
/// 1002�������ռ�
/// 2001��Google��Ӧ���б��ȡȨ����ʾ
/// 1003��������־�ϱ�ͳ�ƿ���
/// 1004����ϵ�ͷ�����
/// 1007��Vpn��ʾ����</param>
/// <param name="IsOpen">�Ƿ���  1������ 0���ر�</param>
JSONSTRUCT(_SWITCH_CONFIG) {
    int ConfigCode = 0;
    int IsOpen = 0;
};

/// <summary>
/// ͳһ��������
/// </summary>
/// <param name="ConfigCode">����
/// 1001����������
/// 1002��GVIP�ֱ���
/// 1003��VIP�ֱ���
/// 1004������������
/// 1005���࿪����
/// 1006��H5��������
/// 2006��֡���л�
/// </param>
/// <param name="Value">�·�ֵ</param>
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
/// �ֱ�������
/// </summary>
/// <param name="ResolutionId">�ֱ�������ID</param>
/// <param name="ResolutionName">����</param>
/// <param name="ModelID">�豸����ID</param>
/// <param name="Width">��</param>
/// <param name="Height">��</param>
/// <param name="MultiMode">���ģʽ 0�������� 1�Ƕ�� 2�ǷǶ��</param>
/// <param name="HardwareWidth">��</param>
/// <param name="HardwareHeight">��</param>
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
/// ϵͳԤ�ƽӿ�
/// </summary>
/// <param name="ModelInfos">�豸��������б�</param>
/// <param name="AppOpenPageUrls">App��webҳ���url������Ϣ</param>
/// <param name="SwitchConfigs">ͳһ��������</param>
/// <param name="ParamConfigs">ͳһ��������</param>
/// <param name="Phoneresolutionconfigs">�ֱ�������</param>
/// <param name="BackupLimit">�û����������޶�</param>
/// <param name="UploadFileSizeLimit">�û��ϴ��ļ���С���ƣ���λ��G</param>
/// <param name="StorageConfig">obs�洢�������ļ�</param>
JSONSTRUCT(_SYSINIT_INFO) {
    std::vector<_MODEL_INFO> ModelInfos;
    std::vector<_APP_OPEN_PAGE_URL> AppOpenPageUrls;
    std::vector<_SWITCH_CONFIG> SwitchConfigs;
    std::vector<_PARAM_CONFIG> ParamConfigs;
    std::vector<_RESOLUTION_SLN> Phoneresolutionconfigs;
    int BackupLimit = 0; //�û����������޶�
    int UploadFileSizeLimit = 0; //�û��ϴ��ļ���С���ƣ���λ��G
    std::string StorageConfig;

    /// <summary>
    /// ��ȡurl������Ϣ
    /// </summary>
    /// <param name="id">ҳ�����</param>
    /// <returns>url����</returns>
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
    /// �����豸��������
    /// </summary>
    /// <param name="id">�豸����ID</param>
    /// <param name="name">���ڱ�����������</param>
    /// <returns>�Ƿ��ҵ� trueΪ�ҵ���falseΪδ�ҵ�</returns>
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
    /// ��ȡ��������
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
    /// ��ȡvip�豸�ֱ�������
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
    /// ��ȡgvip�豸�ֱ�������
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
    /// ��ȡ�࿪����
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
    /// ��ȡ�Ƿ�ʹ�ö�����ع���
    /// </summary>
    /// <returns>trueΪʹ�ã�falseΪ��ʹ��</returns>
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
    /// ��ȡ֡������
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
    /// ��ȡ����Ҫ��ķֱ���������Ϣ
    /// </summary>
    /// <param name="modelid">�豸����ID</param>
    /// <param name="multiMode">���ģʽ</param>
    /// <returns>�ֱ���������Ϣ</returns>
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
    /// ����ͼ���Ƿ�����
    /// </summary>
    /// <param name="modelid">�豸����ID</param>
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
    /// ��ȡͼ��
    /// </summary>
    /// <param name="modelid">�豸����ID</param>
    /// <param name="path">ͼ����ص�ַ</param>
    /// <returns>trueΪ��ȡ��ͼ�꣬falseΪδ��ȡ��ͼ��</returns>
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
    /// ��ȡ�Ƿ�ʹ��VPN����
    /// </summary>
    /// <returns>trueΪʹ�ã�falseΪ��ʹ��</returns>
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
/// ǩ���ļ�
/// </summary>
/// <param name="FileExists">�ļ��Ƿ��Ѿ����� true:��ʾ�ļ������˲���Ҫ���ϴ���</param>
///  <param name="FilePath">ǩ�����������ַ FileExists=true���������ص�ַ</param>
///  <param name="ExpireSecond">��Чʱ�䵹��ʱ����λ�롣Ĭ��10����</param>
///  <param name="Key">��Ӧ���������FilePath��һ�����·��</param>
///  <param name="Md5">Md5</param>
///  <param name="fullpath">������ַ</param>
///  <param name="wfullpath"></param>
///  <param name="FileSize">�ļ���С</param>
///  <param name="FileType">�ļ�����</param>
///  <param name="FileClassify">�ļ�����</param>
///  <param name="arg"></param>
///  <param name="disable">�Ƿ����</param>
///  <param name="packagename">����</param>
/// <param name="versioncode">�汾��</param>
JSONSTRUCT(_SIGNFILEPATH) {
    bool FileExists = false;
    std::string FilePath;
    int ExpireSecond = 0; //��Чʱ�䵹��ʱ����λ��
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
/// �ļ�ǩ��
/// </summary>
/// <param name="SignFilePaths">ǩ���ļ��б�</param>
JSONSTRUCT(_SIGNFILEPATHS) {
    std::vector<_SIGNFILEPATH> SignFilePaths;
};

/// <summary>
/// �ļ���Ϣ
/// </summary>
/// <param name="UserFileID">�ļ�Id</param>
/// <param name="FileName">�ļ�����</param>
/// <param name="FilePth">�ļ���ŵ�ַ</param>
/// <param name="FileDomain">�ļ���ŵ�ַ����</param>
/// <param name="FileMd5">�ļ�Md5</param>
/// <param name="FileSize">�ļ���С ��λByte</param>
/// <param name="FileType">�ļ����� ���� MP3,ZIP,jpg</param>
/// <param name="FileDesc">�ļ�����</param>
/// <param name="IsInstall">�Ƿ��Ѿ���װ��</param>
/// <param name="FileExtendJson">�ļ���չ��Ϣ,app��������ԭ�ⲻ���·�</param>
/// <param name="FileClassify">�ļ����� 1:Ӧ��  2���ļ�  3:��������</param>
/// <param name="MobileDeviceID">�������ݵĻ���ID FileClassify=3��Ч</param>
/// <param name="UploadChannel">�ϴ�����1��Ӧ���̵�  2���û��ƻ� 3����̨</param>
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
/// �û��ϴ����ļ��б�
/// </summary>
/// <param name="FileDatas">�ļ���</param>
/// <param name="Page">��ҳ��Ϣ</param>
JSONSTRUCT(_USERFILES) {
    std::vector<_FILEINFO> FileDatas;
    _PAGES Page;
};

/// <summary>
/// �û�һ���»�����--��ҳ
/// </summary>
/// <param name="Page">��ҳ��Ϣ</param>
JSONSTRUCT(_HISTORY_SLN_PAGE) {
    _PAGES Page;
};

/// <summary>
/// Ӧ����Ϣ
/// </summary>
/// <param name="icon">appͼ���ַ</param>
/// <param name="packageName">����</param>
/// <param name="name">app����</param>
/// <param name="isRoot">�Ƿ�root</param>
JSONSTRUCT(APPINFO) {
    std::string icon;
    std::string packageName;
    std::string name;
    bool isRoot = false;
};

/// <summary>
/// Ӧ���б�
/// </summary>
/// <param name="data">Ӧ���б�</param>
JSONSTRUCT(APPS) {
    std::vector<APPINFO> data;
};

JSONSTRUCT(WSDATA) {
    int type;
    std::string data;
};

/// <summary>
/// Ӧ��������Ϣ
/// </summary>
/// <param name="AreaID">��������ID</param>
/// <param name="AreaName">����</param>
/// <param name="ICON">appͼ���ַ</param>
/// <param name="FileMd5">MD5</param>
/// <param name="AppDownUrl">���ص�ַ</param>
/// <param name="AppSize">�ļ���С ��λMB</param>
/// <param name="AppVersion">�汾��</param>
/// <param name="PackageName">����</param>
/// <param name="InnerVersion">�ڲ��汾��</param>
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
/// Ӧ����Ϣ
/// </summary>
/// <param name="ID">����ID</param>
/// <param name="Name">����</param>
/// <param name="ICON">appͼ���ַ</param>
/// <param name="AppType">Ӧ������ 1:��Ϸ 2�����</param>
/// <param name="Areas">������Ϣ</param>
JSONSTRUCT(MARKETAPPINFO) {
    long ID = 0;
    std::string Name;
    std::string ICON;
    int AppType = 0;
    std::vector<AREASERVERINFO> Areas;
};

/// <summary>
/// Ӧ���б�
/// </summary>
/// <param name="AppInfos">Ӧ���б�</param>
/// <param name="Page">��ҳ��Ϣ</param>
JSONSTRUCT(MARKETAPP) {
    std::vector<MARKETAPPINFO> AppInfos;
    _PAGES Page;
};

/// <summary>
/// �ϴ��ļ��仯��Ϣ
/// </summary>
/// <param name="UploadFileSize">�û���ʹ�ÿռ��С</param>
/// <param name="UploadFileId">�ϴ��ļ�ID</param>
JSONSTRUCT(UPLOADFILESUCC) {
    float UploadFileSize = 0.f;
    long UploadFileId = 0;
};

/// <summary>
/// ������Ϣ
/// </summary>
/// <param name="AppName">App�汾����</param>
/// <param name="AppVersion">App�汾</param>
/// <param name="AppVersionCode">App�汾Code������1.1.1</param>
/// <param name="AppUrl">����ַ</param>
/// <param name="AppSize">����С</param>
/// <param name="UpdateType">�������� 1��ǿ�Ƹ��� 2����ѡ����</param>
/// <param name="UpdateContent">��������</param>
/// <param name="UrlType">��ַ���� 1:���� 2����ת</param>
JSONSTRUCT(GETAPPVERSION) {
    std::string AppName;
    int AppVersion = 0;
    std::string AppVersionCode;
    std::string AppUrl;
    std::string AppSize;
    int UpdateType = 2; //�������� 1��ǿ�Ƹ��� 2����ѡ����
    std::string UpdateContent;
    int UrlType = 1; //��ַ���� 1:���� 2����ת
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
/// ��Ϣ��Ϣ
/// </summary>
/// <param name="ID">��ϢID</param>
/// <param name="MsgType">��Ϣ���� 1:ƽ̨��Ϣ  2��������Ϣ</param>
/// <param name="MsgSource">��Ϣ��Դ  2:App 13:Server  14:Admin</param>
/// <param name="Title">����</param>
/// <param name="Content">��Ϣ����</param>
/// <param name="MsgDate">��Ϣʱ�䣬��ʽ��yyyy-MM-dd HH-mm-ss</param>
/// <param name="MsgDisplay">��Ϣչʾ��ʽ  0:��  1:��ʾ</param>
JSONSTRUCT(MSGINFO)
{
    long ID = 0;//��ϢID
    int MsgType = 0;//��Ϣ���� 1:ƽ̨��Ϣ  2��������Ϣ
    int MsgSource = 0;//��Ϣ��Դ  2:App 13:Server  14:Admin
    std::string Title;//����
    std::string Content;//��Ϣ����
    std::string MsgDate;//��Ϣʱ�䣬��ʽ��yyyy-MM-dd HH-mm-ss
    int MsgDisplay = 0;//��Ϣչʾ��ʽ  0:��  1:��ʾ
};
JSONSTRUCT(DEVICEAUTHTOKEN)
{
    std::string AccessToken;
    long ExpireTime = 0;
    long ExpireCountDown = 0;
};

/// <summary>
/// ��Ϣ�б�
/// </summary>
/// <param name="MsgData">��Ϣ�б�</param>
JSONSTRUCT(MSGDATA)
{
    std::vector<MSGINFO> MsgData;//��Ϣ�б�
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
/// vpn����
/// </summary>
/// <param name="ID">vpnID</param>
/// <param name="Name">vpn����</param>
JSONSTRUCT(VPNCLASSIFY) {
    int ID = 0;
    std::string Name;
};

/// <summary>
/// �û�VPN��Ϣ
/// </summary>
/// <param name="ID">vpnID</param>
/// <param name="Name">�û�Vpn����</param>
/// <param name="Classify">vpn����</param>
/// <param name="Address">��ַ</param>
/// <param name="Prot">�˿�</param>
/// <param name="Account">�˺�</param>
/// <param name="Password">����</param>
JSONSTRUCT(USERVPN)
{
    long ID = 0;
    std::string Name; //�û�Vpn����
    int Classify = 0; //����
    std::string Address; //��ַ
    int Prot = 0; //�˿�
    std::string Account; //�˺�
    std::string Password; //����
};

/// <summary>
/// �û�VPN�б�
/// </summary>
/// <param name="UserVpnList">�û�����Vpn�б�</param>
/// <param name="VpnClassifyList">Vpn�����б�</param>
JSONSTRUCT(_USERVPNLIST)
{
    std::vector<USERVPN> UserVpnList;
    std::vector<VPNCLASSIFY> VpnClassifyList;
};

/// <summary>
/// �����豸����Vpn��Ϣ
/// </summary>
/// <param name="DeviceOrderID">�豸����ID</param>
/// <param name="UserVpnConfigID">�û�vpn����ID</param>
JSONSTRUCT(DEVICEORDERVPN)
{
    long DeviceOrderID = 0;
    long UserVpnConfigID = 0;
};

/// <summary>
/// ����Vpn
/// </summary>
/// <param name="SetDeviceOrderVpn">�����豸����Vpn</param>
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

