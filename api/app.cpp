#include "app.h"

QString App::appName = "MVAPP";
int App::deskWidth = 0;
int App::deskHeight = 0;
QString App::logFileName = "MVLOG";
QString App::HostIP = "127.0.0.1";

QString App::TZLXContains = "012";

/*默认配置设置*/
#if defined(Q_OS_WIN)
QString App::appPath = "C:/MVAPP/";
QString App::logFileNamePath = "C:/MVLOG/MVAPP.log";
QString App::logFileDir = "C:/MVLOG/";
QString App::qmlImageUrl = "file:///";
#endif

#if defined(Q_OS_LINUX)
QString App::appPath = "usr/MVAPP/";
QString App::logFileNamePath = "usr/MVLOG/MVAPP.log";
QString App::logFileDir = "usr/MVLOG/";
QString App::qmlImageUrl = "file://";
#endif

#if CONFIG_SETTING

QString App::configFile = "config.ini";
bool App::isAutoRun = false;
bool App::isShowGUI = true;
bool App::isOutPutDebuginfo = false;
QString App::faceKey = "2505947106180";
float App::faceThreshold = 0.8500;
bool App::isLiveness = true;
float App::livnessThreshold = 0.9150;
int App::livnessMode = 0;
bool App::isFaceDete = false;
bool App::isMaskTip = false;
bool App::isFaceTrack = false;
bool App::isFace106Key = false;
bool App::isFaceReco = false;
bool App::isScenePhoto = false;
bool App::isFaceVedio = true;

bool App::isIrisReco = false;
bool App::isIrisVedio = false;
int App::recoMode = 1;

//生物采集一体机
bool App::isFaceCollect = true;
bool App::isIrisCollect = true;
bool App::isFingerCollect = false;
bool App::isCredCollect = false;

int App::enrollOverTime = 20;

int App::waiting = 1;
//ms
int App::facereco_interval = 100;
int App::irisreco_interval = 100;
int App::livedete_interval = 100;

int App::faceInterval = 2;
int App::irisInterval = 2;
int App::idleMode = 10;

QString App::HWControl = "COM2";
int App::faceMinY = 240;
int App::faceMaxY = 270;
int App::faceEnrollMinY = 100;
int App::faceEnrollMaxY = 400;

void App::readConfig()
{
//    if (!checkConfig()) {
//        return;
//    }
    QSettings set(App::configFile, QSettings::IniFormat);
    set.beginGroup("AppConfig");
    if (set.value("HostIP").toString() != "")
    App::HostIP = set.value("HostIP").toString();
    App::faceKey = set.value("faceKey").toString();
    App::faceThreshold = set.value("faceThreshold").toFloat();
    App::isLiveness = set.value("liveness").toBool();
    App::livnessThreshold = set.value("livnessThreshold").toFloat();
    App::livnessMode = set.value("livnessMode").toInt();
    App::isFaceDete = set.value("faceDete").toBool();
    App::isMaskTip = set.value("maskTip").toBool();
    App::isFaceTrack = set.value("faceTrack").toBool();
    App::isFace106Key = set.value("face106Key").toBool();
    App::isFaceReco = set.value("faceReco").toBool();
    App::isFaceVedio = set.value("faceVedio").toBool();
    App::isScenePhoto = set.value("scenePhoto").toBool();

    App::isIrisReco = set.value("irisReco").toBool();
    App::isIrisVedio = set.value("irisVedio").toBool();
    App::recoMode = set.value("recoMode").toInt();

    App::waiting = set.value("waiting").toInt();
    App::facereco_interval = set.value("facereco_interval").toInt();
    App::livedete_interval = set.value("livedete_interval").toInt();
    App::irisreco_interval = set.value("irisreco_interval").toInt();

    App::faceInterval = set.value("faceInterval").toInt();
    App::irisInterval = set.value("irisInterval").toInt();
    App::idleMode = set.value("idleMode").toInt();

    App::HWControl = set.value("HWControl").toString();
    App::faceMinY = set.value("faceMinY").toInt();
    App::faceMaxY = set.value("faceMaxY").toInt();

    App::faceEnrollMinY = set.value("faceEnrollMinY").toInt();
    App::faceEnrollMaxY = set.value("faceEnrollMaxY").toInt();

    App::isFaceCollect = set.value("isFaceCollect").toBool();
    App::isIrisCollect = set.value("isIrisCollect").toBool();
    App::isFingerCollect = set.value("isFingerCollect").toBool();
    App::isCredCollect = set.value("isCredCollect").toBool();
    App::enrollOverTime = set.value("enrollOverTime").toInt();

    App::isAutoRun = set.value("isAutoRun").toBool();
    App::isShowGUI = set.value("isShowGUI").toBool();
    set.endGroup();

    set.beginGroup("DebugOut");
    App::isOutPutDebuginfo = set.value("DebugOut").toBool();
    set.endGroup();
}

void App::writeConfig()
{
    QSettings set(App::configFile, QSettings::IniFormat);
    set.beginGroup("AppConfig");
    set.setValue("HostIP", App::HostIP);
    set.setValue("faceKey", App::faceKey);
    set.setValue("faceThreshold", App::faceThreshold);
    set.setValue("liveness",App::isLiveness);
    set.setValue("livnessThreshold", App::livnessThreshold);
    set.setValue("livnessMode", App::livnessMode);
    set.setValue("faceDete",App::isFaceDete);
    set.setValue("maskTip",App::isMaskTip);
    set.setValue("faceTrack",App::isFaceTrack);
    set.setValue("face106Key",App::isFace106Key);
    set.setValue("faceReco",App::isFaceReco);
    set.setValue("faceVedio",App::isFaceVedio);
    set.setValue("scenePhoto",App::isScenePhoto);

    set.setValue("irisReco",App::isIrisReco);
    set.setValue("irisVedio",App::isIrisVedio);
    set.setValue("recoMode", App::recoMode);

    set.setValue("isFaceCollect",App::isFaceCollect);
    set.setValue("isIrisCollect",App::isIrisCollect);
    set.setValue("isFingerCollect",App::isFingerCollect);
    set.setValue("isCredCollect",App::isCredCollect);

    set.setValue("waiting", App::waiting);

    set.setValue("facereco_interval",App::facereco_interval);
    set.setValue("irisreco_interval",App::irisreco_interval);
    set.setValue("facereco_interval",App::facereco_interval);
    set.setValue("faceInterval", App::faceInterval);
    set.setValue("irisInterval", App::irisInterval);
    set.setValue("idleMode", App::idleMode);
    set.setValue("HWControl", App::HWControl);
    set.setValue("faceMinY", App::faceMinY);
    set.setValue("faceMaxY", App::faceMaxY);

    set.setValue("isAutoRun", App::isAutoRun);
    set.endGroup();

    set.beginGroup("DebugOut");
    set.setValue("DebugOut", App::isOutPutDebuginfo);
    set.endGroup();
}

void App::newConfig()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#endif
    writeConfig();
}

bool App::checkConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(App::configFile);
    if (file.size() == 0) {
        newConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly)) {
        bool ok = true;
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2) {
                if (list.at(1) == "") {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok) {
            newConfig();
            return false;
        }
    } else {
        newConfig();
        return false;
    }

    return true;
}

#endif



