#ifndef MVGLOBAL_H
#define MVGLOBAL_H

#include <QString>

/*终端软件版本号*/
const QString  MVRECOTL_VERSION         =  "V1.0.0.1";
const QString  MVMAGSERVICE_VERSION     =  "V1.0.0.1";
const QString  MVBUSSERVICE_VERSION     =  "V1.0.0.1";
const QString  MVKEEPSERVICE_VERSION    =  "V1.0.0.1";
const QString  MVFILESERVICE_VERSION    =  "V1.0.0.1";

const QString  MVCOLLECTIONTL_VERSION    =  "V1.0.0.1";
const QString  MVFORWARDERVICE_VERSION    =  "V1.0.0.1";

/*终端软件名称*/
#if defined(LINUX)
const QString MVRECOTL_NAME       = "MVRecoTL";
const QString MVBUSSERVICE_NAME   = "BusinessService";
const QString MVMAGSERVICE_NAME   = "ManageService";
const QString MVKEEPSERVICE_NAME  = "KeepService";
const QString MVFILESERVICE_NAME  = "FileService";

const QString MVCOLLECTTL_NAME       = "MVCollectionTL";
const QString MVFORWARDSERVICE_NAME    = "ForwardService";
#else
const QString MVRECOTL_NAME       = "MVRecoTL.exe";
const QString MVBUSSERVICE_NAME   = "BusinessService.exe";
const QString MVMAGSERVICE_NAME   = "ManageService.exe";
const QString MVKEEPSERVICE_NAME  = "KeepService.exe";
const QString MVFILESERVICE_NAME  = "FileService.exe";

const QString MVCOLLECTTL_NAME       = "MVCollectionTL.exe";
const QString MVFORWARDSERVICE_NAME    = "ForwardService.exe";
#endif


/*服务配置*/
//qml测试
#define  QMLTEST_SETTING  0
//arm
#define  ARMTEST_SETTING  1
//全流程测试
#define  ALLTEST_SETTING  0

//中科虹霸
#define  HBIRIS_SETTING  1

//先牛虹膜
#define  XNIRIS_SETTING  0

//IB指纹仪
#define  IBSCAN_SETTING  1

//证件阅读器
#define  IDCARD_SETTING  1

/*服务端口配置*/
const int KEEP_RECO_PORT = 40001;
const int KEEP_MANAGE_PORT = 40002;
const int KEEP_BUSSINESS_PORT = 40003;
const int KEEP_FILE_PORT = 40004;

const int KEEP_COLLECT_PORT = 50001;
const int KEEP_FORWARD_PORT = 50002;

/*配置文件设置*/
#define  CONFIG_SETTING  1


//人脸视频宽度，高度
const int FACE_VEDIO_WIDTH = 640;
const int FACE_VEDIO_HEIGHT = 480;
//红外视频
const int FACEIR_VEDIO_WIDTH = 250;
const int FACEIR_VEDIO_HEIGHT = 354;

//虹膜视频宽度，高度
const int IRIS_VEDIO_WIDTH = 800;
const int IRIS_VEDIO_HEIGHT = 440;

//虹膜特征值长度
const int IRIS_FEATURE_SIZE = 1536;

/*extern 全局变量，建议.h声明 .cpp定义使用*/
//实际写入的虹膜特征数量
extern int g_irisTemplate_index;
//实际写入的人脸特征数量
extern int g_faceTemplate_index;

//虹膜处理超时(ms)
const int IRIS_TIMEOVER = 3 * 1000;

//虹膜最小距离(mm)
const int IRIS_MINDISTANCE = 500;
//虹膜最大距离
const int IRIS_MAXDISTANCE = 600;
//人脸检测最大距离
const int FACE_MAXDISTANCE = 1000;

/*设置注册和识别的默认路径*/
//人脸注册照片,特征值路径
const QString FACE_ENROLL_IMAGEPATH = "/CollectTemp/face/";
const QString FACE_ENROLL_FEATUREPATH = "/CollectTemp/face/";
//虹膜注册照片,特征值路径
const QString IRIS_ENROLL_IMAGELEFT_PATH = "/CollectTemp/iris/";
const QString IRIS_ENROLL_FEATURELEFT_PATH = "/CollectTemp/iris/";
const QString IRIS_ENROLL_IMAGERIGHT_PATH = "/CollectTemp/iris/";
const QString IRIS_ENROLL_FEATURERIGHT_PATH = "/CollectTemp/iris/";
//指纹注册照片,特征值路径
const QString FINGER_ENROLL_IMAGELEFT_PATH = "/CollectTemp/finger/";
const QString FINGER_ENROLL_FEATURELEFT_PATH = "/CollectTemp/finger/";
const QString FINGER_ENROLL_IMAGERIGHT_PATH = "/CollectTemp/finger/";
const QString FINGER_ENROLL_FEATURERIGHT_PATH = "/CollectTemp/finger/";
//证件采集信息路径
const QString CRED_ENROLL_PATH = "/CollectTemp/certificate/";


/*设置注册成功保存图像尺寸*/
const int FACE_ENROLL_WIDTH = 180;
const int FACE_ENROLL_HEIGHT = 230;

const int IRIS_ENROLL_WIDTH = 254;
const int IRIS_ENROLL_HEIGHT = 192;

const int FIGNER_ENROLL_WIDTH = 180;
const int FIGNER_ENROLL_HEIGHT = 230;


/*全局枚举变量*/
//认证方式
enum MVAuthMode
{
    IrisAuth = 1,    	//虹膜
    FaceAuth = 2,	    //人脸
    FingerAuth = 3,     //指纹
    FingerVeinAuth = 4,     //指静脉
};
//采集类型
enum MVCollMode
{
    IdleColl = 0,       //空闲
    IrisColl = 1,    	//虹膜
    FaceColl = 2,	    //人脸
    FingerColl = 3,     //指纹
    FingerVeinColl = 4,     //指静脉
    CredentialColl = 5, //证件信息采集
};

//异常代码枚举
enum ErrorCode {
    ErrorCode_1001 = -1001,
    ErrorCode_1002 = -1002,
    ErrorCode_1003 = -1003,
    ErrorCode_1004 = -1004,
    ErrorCode_1005 = -1005,
};

#endif // MVGLOBAL_H
