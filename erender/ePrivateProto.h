#pragma once
//#include <windows.h>
//#include <process.h>
#include <string>
#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/serialization/singleton.hpp>
#include <mutex>

/**
     * ����������apk
     */
#define CMD_ACTION_START_APP  100000

     /**
      * ��ȡ�ƻ����豸��Ϣ
      */
#define CMD_ACTION_GET_DEVICEINFO  100001

      /**
       * ��װӦ���г�
       */
#define CMD_ACTION_INSTALL_APP_MAKET  100002

       /**
        * ��ȡ�Ѱ�װ��app��Ϣ
        */
#define CMD_ACTION_GET_INSTALLED_APP_INFO  100003

        /**
         * rootApp
         */
#define CMD_ACTION_ROOT_APP  100004

         /**
          * ��ȡ������app
          */
#define CMD_ACTION_GET_TOP_APP  100005

          /**
           * ��App
           */
#define CMD_ACTION_RUN_APP  100006

           /**
            * ֹͣApp
            */
#define CMD_ACTION_STOP_APP  100007

            /**
             * �����ƻ�׼��
             */
#define CMD_ACTION_PREPARE_CONNECT  100008

             /**
              * ��ת
              */
#define CMD_ACTION_ROTATE  100009

              /**
               * ���͹㲥
               */
#define CMD_ACTION_SEND_BROADCASTRECEIVER  100010


               /**
                * ��װapp
                */
#define CMD_INSTALL_APP  100011

                /**
                 * �������ͻ�������
                 */
#define CMD_ACTION_MULTI_CLIENT  100012

                 /**
                  * ɨ���ƻ����ά��ص�
                  */
#define CMD_ATION_SCAN_QRCODE_RESULT  100013

                  /**
                   * �л�����
                   */
#define CMD_ACTION_CHANG_LANGUAGE  100014

                   /**
                    * ���Ԥ����
                    */
#define CMD_ACTION_MULTI_PREPARE  100015

                    /**
                     * ��ͼ
                     */
#define CMD_ACTION_SCREENCAP  100016


                     /**
                      * ���ü��а�
                      */
#define CMD_ACTION_SET_CLIP_BOARD  100017

                      /**
                       * ��ѯroot״̬
                       */
#define CMD_ACTION_QUERY_ROOT_STATE  100018

                       /**
                        * ִ��shell����
                        */
#define CMD_ACTION_SHELL  100019



                        /**
                         * ��ͼ�ϴ���
                         */
#define CMD_ACTION_SCREEP_OBB  100020

                         /**
                          * ���ز���װ
                          */

#define CMD_ACTION_DOWNLOAD_INSTALL_INFO  100021


                          /**
                           * ����
                           */
#define CMD_ACTION_HEART  100027


                           /**
                            * ����
                            */
#define CMD_ACTION_GP_HEART  100022


                           /**
                                * ɱ���ͻ���
                                */
#define CMD_ACTION_CLOSE_CLIENT  110001
                                /**
                                 * �ƻ��ڲ���תֹͣ�ⲿ
                                 */
#define CMD_ACTION_ROATE_CLIENT  110002

                                 /**
                                 * ����
                                 */
#define CMD_ACTION_TOUCH  100023

                                 /**
                                 * ���뷨
                                 */
#define CMD_ACTION_INPUT  100024

                                 /**
                                 * ����
                                 */
#define CMD_HAND_SHARK  100025


                                 /**
                                 * �豸����
                                 */
#define CMD_DEVICE_EXPRIE  100026


                                 /**
                                      * ж��app
                                      */
#define CMD_ACTION_UNINSTALL_APP 100032

/**
 * �����̨
 */
#define CMD_ACTION_CLEAR_PROCESS 100033

 /**
* ��������app����
*/
#define CMD_ACTION_CLEAR_APP_BATCH 100035

/**
     * ����Ȩ�޻���
     */
#define CMD_ACTION_APP_PERMISSION  100037

/**
     * ���ⲿ�ϴ����ļ�
     */
#define CMD_ACTION_OUTSIZE_OPEN_UP_FILE 100038

/**
     * ���а巵��
     */
#define CMD_ACTION_COPY_CUT_RESULT 100039


/**
     * �ͻ��˸��Ƶ�ճ��������
     */
#define CMD_ACTION_IME_SET_CLIP_BOARD 120001
/**
 * ���ֻ����Ƶ�ճ��������
 */
#define CMD_ACTION_IME_COPY_CLIP_BOARD 120002
/**
 * ���ֻ����е�ճ��������
 */
#define CMD_ACTION_IME_CUT_CLIP_BOARD 120003
/**
 * ���ֻ����е�ճ��������
 */
#define CMD_ACTION_IME_PASTE_CLIP_BOARD 120004
/**
 * ��������
 */
#define CMD_ACTION_IME_INPUT_TEXT 120005
/**
 * �������ⰴ���������ɾ��
 */
#define CMD_ACTION_IME_SEND_KEY_EVENT 120006
/**
 * ���뷨״̬
 */
#define CMD_ACTION_IME_INPUT_STATE 120007

/**
* ҡһҡ
*/
#define CMD_ACTION_SHAKE 100028


/**
* ���ƻ����
*/
#define CMD_ACTION_OPEN_CAMERA 100030

/**
     * ɱ���ͻ���
     */
#define CMD_ACTION_CLOSE_CLIENT 110001


/**
* ��ͼƬ 
*/
#define CMD_OPEN_PIC 140003


/**
* QR ����
*/
#define CMD_QR_DATA 140002


/**
     * �л����뷨
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

// ʹ��typedef�Լ�������ʹ��
typedef boost::serialization::singleton<ePrivateProto> singleton_Proto; // ʹ��ģ��ķ�ʽֻ������ʵ��
#define sProto singleton_Proto::get_mutable_instance() // ��constʵ��
#define sProto_const singleton_Proto::get_const_instance() // constʵ��
