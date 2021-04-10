#ifndef ZLNETWORK_H
#define ZLNETWORK_H
/**
*@projectName MyNetwork
*@brief  version V1.0.0.1
* 1: 四种模式，tcp客户端、tcp服务器、udp客户端、udp服务器
* 2：支持多个客户端连接并发
* 3：支持服务端单播，广播，指定断开，全部断开
* 4: 采用单线程
* 2020-06-03 version V1.0.0.2 新增：
* 5: TCP传输 json对象 byte数组  自定义结构体
*
* 2020-11-20 version V1.0.0.3 新增：
* 6: UDP传输字节数组
* 7：TCP在发送数据时使用flush及时刷新
*
*author zlozl
*date 2020-06-03
*/
#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QTcpServer>
#include <QUdpSocket>
#include <QMutex>
#include <QJsonObject>
#include <QJsonDocument>


class TcpClient :public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    ~TcpClient();
    static TcpClient *Instance();
private:
    static QScopedPointer<TcpClient> self;
    QTcpSocket *tcpClient;
public:
    bool connectTcpServer(const QString ip, int port);
    bool disconnectTcpServer();
    void sendData(const QString &data);
    void sendData(const QByteArray &bytes);
    void sendData(const QJsonObject &jsonobj);
    void sentData(const QJsonArray &jsonarray);
    void sentData(const QImage &image);

signals:
    void readSverDate(const QByteArray &data);
    void serverStart();
    void serverStop();


private slots:
    void readDate();
    void connected();
    void disconnected();

};

class TcpServer :public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = nullptr);
    ~TcpServer();
    static TcpServer *Instance();
private:
    //*智能指针,在其生命期结束后会自动删除它所指的对象
    static QScopedPointer<TcpServer> self;
    QTcpServer *tcpServer;
    QList<QTcpSocket *> tcpclients;
    QTcpSocket *currentClient;
public:
    bool startTcpServer(const QHostAddress &sIP, int iPort);
    void stopTcpServer();
    //指定断开
    void stopTcpServer(const QString &ip, int port);
    //指定连接发送数据   c++ 形参不同，形成函数重载
    void sendData(const QString &ip, int port, const QString &data);
    void sendData(const QString &data);
    void sendData(const QString &ip, int port, const QByteArray &bytes);
    void sendData(const QByteArray &bytes);
    void sendData(const QString &ip, int port, const QJsonObject &jsonobj);
    void sendData(const QJsonObject &jsonobj);
    void sendData(const QString &ip, int port, const QJsonArray &jsonarray);
    void sentData(const QJsonArray &jsonarray);
    void sendData(const QString &ip, int port, const QImage &image);
    void sentData(const QImage &image);


signals:
    void receiveData(const QString &ip, int port, const QByteArray &data);
    void clientConnected(const QString &ip, int port);
    void clientDisconnected(const QString &ip, int port);

private slots:

    void doclientConnected();
    void doclientDisconnected();
    void doreceiveData();
};







#if 1
//UDP客户端
class UdpClient :public QUdpSocket
{
    Q_OBJECT
public:
    explicit UdpClient(QObject *parent = nullptr);
    ~UdpClient();
    static UdpClient *Instance();
private:
    QUdpSocket *udpclient;
    static QScopedPointer<UdpClient> self;
public:
    void sendData(const QString &ip, int port, const QString &data);
    void sendData(const QString &ip, int port, const QByteArray &byte);
signals:
    void doreadSverData(const QString &ip, int port, const QByteArray &data);

private slots:
    void readSverData();
};

//UDP服务端
class UdpServer :public QUdpSocket
{
   Q_OBJECT
public:
    explicit UdpServer(QObject *parent = nullptr);
    ~UdpServer();
    static UdpServer *Instance();
private:
   QUdpSocket *udpserver;
    static QScopedPointer<UdpServer> self;
public:
   bool startUdpServer(const QString &ip, int port);
   void stopUdpServer();
   void sendData(const QString &ip, int port, const QString &data);
   void sendData(const QString &ip, int port, const QByteArray &byte);

signals:
   void doreadClientData(const QString &ip, int port, const QByteArray &data);

private slots:
   void readClientData();
};
#endif

#endif // ZLNETWORK_H
