#include "ForwardMain.h"
#include "ui_ForwardMain.h"

#include <ForwardMain.h>

ForwardMain *pbackForwardMain = nullptr;
ForwardMain::ForwardMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForwardMain)
{
    ui->setupUi(this);
    ::pbackForwardMain = this;

    initFiles();
    initDB();

    connect(this,&ForwardMain::signal_forward_tcpCmd, this,&ForwardMain::slot_forward_tcpCmd);
    connect(this,&ForwardMain::signal_forward_writeMsg, this,&ForwardMain::slot_forward_writeMsg);


}

ForwardMain::~ForwardMain()
{
    delete ui;
}

QPair<bool, QStringList> ForwardMain::postJsonData(const QByteArray &postdata, const QStringList &keyList)
{
    QStringList postList;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(postdata, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << "解析postJsonData失败: "<<jsonError.error;
        return {false , postList};
    }
    // 解析Json
    if (doucment.isObject()) {
        QJsonObject obj = doucment.object();
        for (int i = 0; i < keyList.length(); i++) {
            if (obj.contains(keyList.at(i))) {
                QJsonValue value;
                value = obj.take(keyList.at(i));
                QString strValue = value.toVariant().toString();
                if (strValue != "")
                    postList.append(strValue);
            }
        }
        if (postList.length() == keyList.count())
            return {true , postList};
    }
    return {false , postList};
}

QPair<bool, QStringList> ForwardMain::postJsonData2(const QByteArray &postdata, const QStringList &keyList)
{
    QStringList postList;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(postdata, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        plog->sysLog("解析postJsonData2失败");
        qDebug() << "解析postJsonData2失败: "<<jsonError.error;
        return {false , postList};
    }
    if (doucment.isObject()) {
        QJsonObject obj = doucment.object();
        for (int i = 0; i < keyList.length(); i++) {
            if (obj.contains(keyList.at(i))) {
                QJsonValue value;
                value = obj.take(keyList.at(i));
                QString strValue = value.toVariant().toString();
                if (strValue != "")
                    postList.append(strValue);
            } else {
               postList.append("");
            }
        }
        if (postList.length() == keyList.count())
            return {true , postList};
    }
    return {false , postList};
}

QJsonObject ForwardMain::getresponseOKjsonobj()
{
    static QJsonObject responseOKjsonobj = { };
    if (responseOKjsonobj.isEmpty()) {
        QJsonObject datajsonobj;
        datajsonobj.insert("response","ok");
        QJsonObject jsonobj;
        jsonobj.insert("code",200);
        jsonobj.insert("data",datajsonobj);
        responseOKjsonobj = jsonobj;
    }
    return responseOKjsonobj;
}

QJsonObject ForwardMain::getResponseStatusobj()
{
    static QJsonObject responseStatusjsonobj = { };
    if (responseStatusjsonobj.isEmpty()) {
        QJsonObject datajsonobj;
        datajsonobj.insert("face device","正常");
        datajsonobj.insert("iris device","正常");
        datajsonobj.insert("fingerprint device","正常");
        datajsonobj.insert("reader device","正常");
        QJsonObject jsonobj;
        jsonobj.insert("code",200);
        jsonobj.insert("data",datajsonobj);
        responseStatusjsonobj = jsonobj;
    }
    return responseStatusjsonobj;
}

QJsonObject ForwardMain::getResponseVersionobj()
{
    static QJsonObject responseVersionjsonobj = { };
    if (responseVersionjsonobj.isEmpty()) {
        QJsonObject datajsonobj;
        datajsonobj.insert("mvcollectionservice",MVCOLLECTIONTL_VERSION);
        datajsonobj.insert("keepservice",MVKEEPSERVICE_VERSION);
        datajsonobj.insert("forwardservice",MVRECOTL_VERSION);
        datajsonobj.insert("fileservice",MVFORWARDERVICE_VERSION);
        QJsonObject jsonobj;
        jsonobj.insert("code",200);
        jsonobj.insert("data",datajsonobj);
        responseVersionjsonobj = jsonobj;
    }
    return responseVersionjsonobj;
}

void ForwardMain::writeMsg(const QString &msg, const QColor &color)
{
    if (App::isShowGUI) {
        emit signal_forward_writeMsg(msg,color);
    } else {
        plog->sysLog(msg);
    }
}

void ForwardMain::syslog(const QString &info)
{
    plog->sysLog(info);
    qDebug()<<info;
}

void ForwardMain::slot_forward_tcpCmd(const QString &tempcmd)
{
    ptcpsever_forward->sendData(tempcmd);
}

void ForwardMain::slot_forward_writeMsg(const QString &msg, const QColor &color)
{
    QString current_dt = QDateTime::currentDateTime().toString("[yyyy-MM-dd-HH-mm-ss-zzz] ");
    if (ui->textmain->document()->lineCount() > 100) {
        ui->textmain->setText("");
    }
    ui->textmain->setTextColor(QColor(color));
    ui->textmain->append(current_dt+msg);
    plog->sysLog(msg);
}

void ForwardMain::slot_receiveData(const QString &ip, int port, const QByteArray &data)
{
    plog->sysLog("收到终端协议["+ip+":"+QString::number(port,10)+"] data: "+QString(data));
    //开始解析客户端指令
    QString cmd = QString(data);
    if (cmd.indexOf("#") != 0) {
        writeMsg("非法指令不解析: "+cmd,QColor(255,0,0));
        return;
    }
}

void ForwardMain::slot_clientConnected(const QString &ip, int port)
{
    plog->sysLog("监测到["+ip+":"+QString::number(port,10)+"],已连接");
    if (ip == App::HostIP) {
        m_isCollectionTLConn = true;
        writeMsg("采集终端连接");
    }
}

void ForwardMain::slot_clientDisconnected(const QString &ip, int port)
{
    plog->sysLog("监测到["+ip+":"+QString::number(port,10)+"],断开连接");
    if (ip == App::HostIP) {
        m_isCollectionTLConn = false;
        writeMsg("采集终端断开...",QColor(255,0,0));
    }
}

void ForwardMain::doreadUdpClientData(const QString &ip, int port, const QByteArray &data)
{
    QTimer::singleShot(1, this, [=]
    {
        m_heart_udpserver->sendData(ip,port,data);
    });
}

void ForwardMain::initFiles()
{
    plog = MVLog::GetInstance();
    plog->openLogFile("ForwardServiceLog");
    plog->clearLogFile(App::logFileDir,30);
    App::HostIP = MVPublic::getHostFirstIpAddress();
    plog->sysLog("获取到本机IP: "+App::HostIP);
}

void ForwardMain::initDB()
{
    MVDBConnPool::setSqliteUrl(qApp->applicationDirPath() + "/MVCollection.db");
    const auto dbvalue = MVDBConnPool::openConnection(MVDBConnPool::DbType_Sqlite,"collect");
    if (!dbvalue.first) {
        writeMsg("连接本地采集库失败",QColor(255,0,0));
    } else {
        m_db_collect = dbvalue.second;
    };
}

void ForwardMain::startHttpServer()
{

}

void ForwardMain::startTcpServer()
{
    bool ok = ptcpsever_forward->startTcpServer(QHostAddress(App::HostIP), 23311);
    if (!ok) {
        writeMsg("转发TcpServer 启动异常",QColor(255,0,0));
    }
}

void ForwardMain::startUdpServer()
{
    bool ok = m_heart_udpserver->startUdpServer("127.0.0.1", KEEP_FORWARD_PORT);
    if (!ok) {
        writeMsg("终端UDP心跳服务 启动异常",QColor(255,0,0));
    }
}

int ForwardMain::deviceReboot()
{
    writeMsg("收到设备重启请求");
    QString tmpcmd = "#^set^devreboot^";
    emit signal_forward_tcpCmd(tmpcmd);
    return 1;
}

int ForwardMain::systemReboot()
{
    writeMsg("收到系统重启请求");
    QString tmpcmd = "#^set^sysreboot^";
    emit signal_forward_tcpCmd(tmpcmd);
    return 1;
}

