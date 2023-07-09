#pragma once
//#include <windows.h>
//#include <process.h>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/serialization/singleton.hpp>
#include <mutex>

/**
     * 启动第三方apk
     */
#define CMD_ACTION_START_APP  100000

     /**
      * 获取云机的设备信息
      */
#define CMD_ACTION_GET_DEVICEINFO  100001

      /**
       * 安装应用市场
       */
#define CMD_ACTION_INSTALL_APP_MAKET  100002

       /**
        * 获取已安装的app信息
        */
#define CMD_ACTION_GET_INSTALLED_APP_INFO  100003

        /**
         * rootApp
         */
#define CMD_ACTION_ROOT_APP  100004

         /**
          * 获取顶部的app
          */
#define CMD_ACTION_GET_TOP_APP  100005

          /**
           * 打开App
           */
#define CMD_ACTION_RUN_APP  100006

           /**
            * 停止App
            */
#define CMD_ACTION_STOP_APP  100007

            /**
             * 连接云机准备
             */
#define CMD_ACTION_PREPARE_CONNECT  100008

             /**
              * 旋转
              */
#define CMD_ACTION_ROTATE  100009

              /**
               * 发送广播
               */
#define CMD_ACTION_SEND_BROADCASTRECEIVER  100010


               /**
                * 安装app
                */
#define CMD_INSTALL_APP  100011

                /**
                 * 多端命令客户端请求
                 */
#define CMD_ACTION_MULTI_CLIENT  100012

                 /**
                  * 扫描云机外二维码回调
                  */
#define CMD_ATION_SCAN_QRCODE_RESULT  100013

                  /**
                   * 切换语言
                   */
#define CMD_ACTION_CHANG_LANGUAGE  100014

                   /**
                    * 多端预处理
                    */
#define CMD_ACTION_MULTI_PREPARE  100015

                    /**
                     * 截图
                     */
#define CMD_ACTION_SCREENCAP  100016


                     /**
                      * 设置剪切板
                      */
#define CMD_ACTION_SET_CLIP_BOARD  100017

                      /**
                       * 查询root状态
                       */
#define CMD_ACTION_QUERY_ROOT_STATE  100018

                       /**
                        * 执行shell命令
                        */
#define CMD_ACTION_SHELL  100019



                        /**
                         * 截图上传云
                         */
#define CMD_ACTION_SCREEP_OBB  100020

                         /**
                          * 下载并安装
                          */

#define CMD_ACTION_DOWNLOAD_INSTALL_INFO  100021


                          /**
                           * 心跳
                           */
#define CMD_ACTION_HEART  100027


                           /**
                            * 心跳
                            */
#define CMD_ACTION_GP_HEART  100022


                           /**
                                * 杀死客户端
                                */
#define CMD_ACTION_CLOSE_CLIENT  110001
                                /**
                                 * 云机内部旋转停止外部
                                 */
#define CMD_ACTION_ROATE_CLIENT  110002

                                 /**
                                 * 反控
                                 */
#define CMD_ACTION_TOUCH  100023

                                 /**
                                 * 输入法
                                 */
#define CMD_ACTION_INPUT  100024

                                 /**
                                 * 握手
                                 */
#define CMD_HAND_SHARK  100025


                                 /**
                                 * 设备过期
                                 */
#define CMD_DEVICE_EXPRIE  100026


                                 /**
                                      * 卸载app
                                      */
#define CMD_ACTION_UNINSTALL_APP 100032

/**
 * 清理后台
 */
#define CMD_ACTION_CLEAR_PROCESS 100033

 /**
* 批量清理app数据
*/
#define CMD_ACTION_CLEAR_APP_BATCH 100035

/**
     * 授予权限或解除
     */
#define CMD_ACTION_APP_PERMISSION  100037

/**
     * 打开外部上传的文件
     */
#define CMD_ACTION_OUTSIZE_OPEN_UP_FILE 100038

/**
     * 剪切板返回
     */
#define CMD_ACTION_COPY_CUT_RESULT 100039


/**
     * 客户端复制到粘贴板命令
     */
#define CMD_ACTION_IME_SET_CLIP_BOARD 120001
/**
 * 云手机复制到粘贴板命令
 */
#define CMD_ACTION_IME_COPY_CLIP_BOARD 120002
/**
 * 云手机剪切到粘贴板命令
 */
#define CMD_ACTION_IME_CUT_CLIP_BOARD 120003
/**
 * 云手机剪切到粘贴板命令
 */
#define CMD_ACTION_IME_PASTE_CLIP_BOARD 120004
/**
 * 输入文字
 */
#define CMD_ACTION_IME_INPUT_TEXT 120005
/**
 * 输入特殊按键，如回退删除
 */
#define CMD_ACTION_IME_SEND_KEY_EVENT 120006
/**
 * 输入法状态
 */
#define CMD_ACTION_IME_INPUT_STATE 120007

/**
* 摇一摇
*/
#define CMD_ACTION_SHAKE 100028


/**
* 打开云机相机
*/
#define CMD_ACTION_OPEN_CAMERA 100030

/**
     * 杀死客户端
     */
#define CMD_ACTION_CLOSE_CLIENT 110001


/**
* 打开图片 
*/
#define CMD_OPEN_PIC 140003


/**
* QR 数据
*/
#define CMD_QR_DATA 140002


/**
     * 切换输入法
     */
#define CMD_ACTION_CUT_IME 100041


class ePrivateProto : public boost::serialization::singleton<ePrivateProto>
{
public:
    ePrivateProto() {
        //InitializeCriticalSection(&_cs);
    };
    virtual ~ePrivateProto() {
        //DeleteCriticalSection(&_cs);
    };

    std::string Connect(const char* lang, const char* token, const int deviceLevel, const char* lbh_token);

    std::string SwitchLang(const char* lang, const char* lbh_token);

    std::string Heartbeat(const char* lbh_token);

    std::string HighVolume();

    std::string Home();

    std::string Back();

    std::string AppSwitch();

    std::string LowVolume();

    std::string SetClipBoard(const char* data, const char* lbh_token);

    std::string GetInstallAppInfo(const char* lbh_token);

    std::string MouseAction(float x, float y, int action);

    std::string RootApps(std::vector<std::string>& changeapps, std::vector<std::string>& rootapps, const char* lbh_token);

    std::string GetDeviceInfo(const char* lbh_token);

    std::string ScreenCaptrue(int width, int height, bool bzip, float Resolution, float Definition, bool isSaveAlbum, const char* lbh_token);

    std::string GenNormal(const int cmdType, const std::string& data, const char* lbh_token);

private:
    void GenOutProto(const unsigned long reqId, const int cmdType, const std::string& data, const char* lbh_token, std::string& out);
    unsigned long _reqid = 0;
    //CRITICAL_SECTION _cs;
    std::mutex _cs;
};

// 使用typedef以及宏来简化使用
typedef boost::serialization::singleton<ePrivateProto> singleton_Proto; // 使用模板的方式只允许单个实例
#define sProto singleton_Proto::get_mutable_instance() // 非const实例
#define sProto_const singleton_Proto::get_const_instance() // const实例
