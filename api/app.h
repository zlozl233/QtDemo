#ifndef APP_H
#define APP_H

#include <QtCore>
#include <QtGui>

#include "mvglobal.h"

class App
{
public:
    //使用静态变量一定要记得先初始化
    //日志文件变量
    static QString appName;             //程序名称
    static QString appPath;             //程序路径
    static int deskWidth;               //屏幕宽度
    static int deskHeight;              //屏幕高度
    static QString logFileNamePath;     //当前日志文件路径加名称
    static QString logFileDir;          //当前日志文件路径
    static QString logFileName;         //日志文件夹名称
    static QString HostIP;              //主机IP
    static QString TZLXContains;        //特征类型分类
    static QString qmlImageUrl;         //qmlImageSouces路径

#if CONFIG_SETTING
    //读写配置参数及其他操作
    static QString configFile;          //配置文件路径
    static QString faceKey;             //人脸算法授权码
    static float faceThreshold;         //人脸阈值
    static bool isLiveness;             //是否启用活体检测
    static float livnessThreshold;      //活体阈值
    static int livnessMode;             //活体检测等级(0: 普通(IR和RGB任意过) 1: 中级(IR过) 2：高级(RGB过) 3: 最高级(IR+RBG都过))

    static bool isFaceDete;             //是否启用人脸属性检测
    static bool isMaskTip;              //是否口罩提示
    static bool isFaceTrack;            //是否启用人脸追踪
    static bool isFace106Key;           //是否进行特征点检测
    static bool isFaceReco;             //是否启用人脸识别
    static bool isScenePhoto;           //是否保存现场照片
    static bool isFaceVedio;            //是否输出RGB视频

    static bool isIrisReco;             //是否启用虹膜识别
    static bool isIrisVedio;            //是否输出虹膜视频

    static int waiting;                 //比对通过等待几秒
    static int facereco_interval;       //人脸识别间隔(人脸识别一轮匹配全部失败)
    static int livedete_interval;       //活体检测间隔(活体检测false)
    static int irisreco_interval;       //虹膜识别间隔
    static int faceInterval;            //相同人员几秒内不允许人脸重复比对
    static int irisInterval;            //相同人员几秒内不允许虹膜重复比对
    static int idleMode;                //几秒后切换为空闲模式
    static int recoMode;                //识别模式 1：兼容人脸 2：信任虹膜

    static QString HWControl;           //硬件控制串口号
    static int faceMinY;                //人脸最佳Y轴最小值-虹膜
    static int faceMaxY;                //人脸最佳Y轴最大值-虹膜
    static int faceEnrollMinY;          //人脸最佳Y轴最小值-人脸
    static int faceEnrollMaxY;          //人脸最佳Y轴最大值-人脸

    /*生物采集一体机*/
    static bool isFaceCollect;          //人脸采集
    static bool isIrisCollect;          //虹膜采集
    static bool isFingerCollect;        //指纹采集
    static bool isCredCollect;          //证件采集
    static int enrollOverTime;          //采集超时时间(秒)

    static bool isAutoRun;              //是否开机自启
    static bool isShowGUI;              //是否显示界面
    static bool isOutPutDebuginfo;      //是否输出debug日志

    static void readConfig();           //读取配置参数
    static void writeConfig();          //写入配置参数
    static void newConfig();            //以初始值新建配置文件
    static bool checkConfig();          //校验配置文件
#endif

};

#endif // APP_H
