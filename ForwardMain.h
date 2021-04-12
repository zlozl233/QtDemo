#ifndef FORWARDMAIN_H
#define FORWARDMAIN_H

//import Qt
#include <QWidget>
#include <QDebug>

//import zlozl
#include "app.h"
#include "mvpublic.h"
#include "mvlog.h"
#include "zlnetwork.h"
#include "WebSocketServerManager.h"
#include "mvdbconnpool.h"

//import JQLibrary
#include "JQHttpServer.h"
#include "JQNet.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ForwardMain; }
QT_END_NAMESPACE

class ForwardMain : public QWidget
{
    Q_OBJECT

public:
    ForwardMain(QWidget *parent = nullptr);
    ~ForwardMain();

public:
    //一些json数据解析，封装
    //解析post过来的json请求体，postJsonData 为非空字段 postJsonData2 允许字段为空
    QPair <bool, QStringList > postJsonData(const QByteArray &postdata, const QStringList &keyList);
    QPair <bool, QStringList > postJsonData2(const QByteArray &postdata, const QStringList &keyList);
    //直接ok
    QJsonObject getresponseOKjsonobj();
    //设备状态
    QJsonObject getResponseStatusobj();
    //软件版本
    QJsonObject getResponseVersionobj();
    //写本地日志+GUI msg
    void writeMsg(const QString &msg, const QColor &color = QColor(0,0,0));
    //写本地日志+qDebug
    void syslog(const QString &info);
/*
 ****************************************************************************************************
 * 通过信号与槽来进行跨线程传输/转发
 ****************************************************************************************************
*/
signals:
    void signal_forward_tcpCmd(const QString &tempcmd);
    void signal_forward_writeMsg(const QString &msg, const QColor &color = QColor(0,0,0));
    //void signal_forward_soundPlay(const QString &voiname);

private slots:
    void slot_forward_tcpCmd(const QString &tempcmd);
    void slot_forward_writeMsg(const QString &msg, const QColor &color = QColor(0,0,0));
   // void slot_forward_soundPlay(const QString &voiname);

/*
 ****************************************************************************************************
 * socket 通讯
 ****************************************************************************************************
*/
private slots:
    //TCPServer <=> 终端服务
    void slot_receiveData(const QString &ip, int port, const QByteArray &data);
    void slot_clientConnected(const QString &ip, int port);
    void slot_clientDisconnected(const QString &ip, int port);

    //UDP心跳检测
    void doreadUdpClientData(const QString &ip, int port, const QByteArray &data);

private:
    /*
    ****************************************************************************************************
    * 初始化
    ****************************************************************************************************
    */
    void initFiles();
    void initDB();
    //开启Http服务
    void startHttpServer();
    //开启TCP转发服务
    void startTcpServer();
    //开启UDP心跳服务
    void startUdpServer();
    //开启WebSocket服务
    void startWebSocket();

    /*
    ****************************************************************************************************
    * 流程控制
    ****************************************************************************************************
    */
    //设备是否启用
    bool m_isdeviceEnable = true;
    //终端是否连接
    bool m_isCollectionTLConn = false;
    //设备重启
    int deviceReboot();
    //系统重启
    int systemReboot();
private:
    Ui::ForwardMain *ui;
    //日志
    MVLog *plog = nullptr;
    //数据库——采集
    QSqlDatabase m_db_collect;
    //一些服务
    JQHttpServer::TcpServerManage *ptcpServerManage = nullptr;
    TcpServer *ptcpsever_forward = nullptr;
    //心跳服务
    UdpServer *m_heart_udpserver = nullptr;

    /*多种服务输出*/
    WebSocketServerManager *myWebSocketServer = nullptr;

};
#endif // FORWARDMAIN_H
