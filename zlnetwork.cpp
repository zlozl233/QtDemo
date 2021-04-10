#include "zlnetwork.h"

QScopedPointer<TcpClient> TcpClient::self;
TcpClient *TcpClient::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new TcpClient);
        }
    }
    return self.data();
}

TcpClient::TcpClient(QObject *parent) : QTcpSocket(parent)
{
    tcpClient = new QTcpSocket(this);
    tcpClient->abort();
    connect(tcpClient, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpClient, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(disconnected()));
    connect(tcpClient,SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(readDate()));
}

TcpClient::~TcpClient()
{
  qDebug() <<"TcpClient已被析构";
}

bool TcpClient::connectTcpServer(const QString ip, int port)
{
//    virtual bool waitForConnected(int msecs = 30000)
//    virtual bool waitForDisconnected(int msecs = 30000)
//    virtual bool waitForBytesWritten(int msecs = 30000)
//    virtual bool waitForReadyRead(int msecs = 30000)
//    通过上述函数可以实现阻塞连接、断开连接、发送、接收数据内容
    tcpClient->connectToHost(ip,port);
    if (tcpClient->waitForConnected(100))  //阻塞连接100ms
        return true;
    else
        return false;
}

bool TcpClient::disconnectTcpServer()
{
    tcpClient->disconnectFromHost();
    if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000))   //断开连接
    {
        return true;
    }
    else
        return false;
}

void TcpClient::sendData(const QString &data)
{
   if (data.length() > 0)
   {
       tcpClient->write(data.toUtf8());
       tcpClient->flush();
   }
}

void TcpClient::sendData(const QByteArray &bytes)
{
    if (bytes.size() > 0) {
        tcpClient->write(bytes);
        tcpClient->flush();
    }

}

void TcpClient::sendData(const QJsonObject &jsonobj)
{
    if (jsonobj.size() > 0) {
        QJsonDocument document;
        document.setObject(jsonobj);
        QByteArray dataArray = document.toJson(QJsonDocument::Compact);
        tcpClient->write(dataArray);
        tcpClient->flush();
    }
}

void TcpClient::readDate()
{
    QByteArray buffer = tcpClient->readAll();
    if(!buffer.isEmpty())
    {
//        qDebug() << "收到服务端数据："+QString(buffer.data());
//        QString serversize = QString::number(buffer.size(),10);
//        qDebug() << "postdata.size: "+serversize;
        emit readSverDate(buffer);
    }
}

void TcpClient::connected()
{
//   qDebug() << "服务端连接";
   emit serverStart();

}
void TcpClient::disconnected()
{
//   qDebug() << "服务端断开";
   emit serverStop();
}

QScopedPointer<TcpServer> TcpServer::self;
TcpServer *TcpServer::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new TcpServer);
        }
    }
    return self.data();
}

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    tcpServer = new QTcpServer(this);
    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(doclientConnected()));

}

TcpServer::~TcpServer()
{
    qDebug() <<"TcpServer已被析构";
}

void TcpServer::sendData(const QString &data)
{
    if (data.length() > 0){
        foreach (QTcpSocket *client, tcpclients) {
            client->write(data.toUtf8());   //中文不乱码
            client->flush();
        }
    }
}

void TcpServer::sendData(const QString &ip, int port, const QString &data)
{
    if (data.length() <= 0){
        return;
    }
    foreach (QTcpSocket *client, tcpclients) {
        if (client->peerAddress().toString() == ip && client->peerPort() == port) {
             client->write(data.toUtf8());
             client->flush();
            break;
        }
    }

}

void TcpServer::sendData(const QByteArray &bytes)
{
    if (bytes.size() > 0) {
        foreach (QTcpSocket *client, tcpclients) {
            client->write(bytes);   //中文不乱码
             client->flush();
        }
    }
}

void TcpServer::sendData(const QString &ip, int port, const QByteArray &bytes)
{
    if (bytes.size() <= 0) {
        return;
    }
    foreach (QTcpSocket *client, tcpclients) {
        if (client->peerAddress().toString() == ip && client->peerPort() == port) {
             client->write(bytes);
              client->flush();
            break;
        }
    }
}

void TcpServer::sendData(const QJsonObject &jsonobj)
{
    if (jsonobj.size() > 0) {
        QJsonDocument document;
        document.setObject(jsonobj);
        QByteArray dataArray = document.toJson(QJsonDocument::Compact);
        foreach (QTcpSocket *client, tcpclients) {
            client->write(dataArray);   //中文不乱码
            client->flush();
        }
    }
}

void TcpServer::sendData(const QString &ip, int port, const QJsonObject &jsonobj)
{
     if (jsonobj.size() <= 0) {
         return;
     }
     QJsonDocument document;
     document.setObject(jsonobj);
     QByteArray dataArray = document.toJson(QJsonDocument::Compact);
     foreach (QTcpSocket *client, tcpclients) {
         if (client->peerAddress().toString() == ip && client->peerPort() == port) {
              client->write(dataArray);
              client->flush();
             break;
         }
     }
}


void TcpServer::doclientConnected()
{
    currentClient = tcpServer->nextPendingConnection();
    tcpclients.append(currentClient);
    QString port = QString::number(currentClient->peerPort(),10);
    qDebug() << "客户端["+currentClient->peerAddress().toString()+":"+port+"]"+"连接成功";
    connect(currentClient,SIGNAL(readyRead()),this,SLOT(doreceiveData()));
    connect(currentClient,SIGNAL(disconnected()),this,SLOT(doclientDisconnected()));
    emit clientConnected(currentClient->peerAddress().toString(),currentClient->peerPort());

}
void TcpServer::doclientDisconnected()
{
    for (int i=0; i<tcpclients.length(); i++)   //这里会把所有的离线客户端全部清除
    {
        if (tcpclients[i]->state() == QAbstractSocket::UnconnectedState)
        {
            QString ip = tcpclients[i]->peerAddress().toString();
            int port = tcpclients[i]->peerPort();
            tcpclients[i]->destroyed();
            tcpclients.removeAt(i);
            qDebug() << ip+QString::number(port,10)+"客户端已离线";
            emit clientDisconnected(ip,port);
        }
    }
}

void TcpServer::doreceiveData()
{
    for(int i=0; i<tcpclients.length(); i++)
    {
        QByteArray buffer = tcpclients[i]->readAll();
        if(buffer.isEmpty())
            continue;
        QString ip = tcpclients[i]->peerAddress().toString();
        int port = tcpclients[i]->peerPort();
//        qDebug() << "收到客户端["+ip+":"+ QString::number(port,10)+"]"+"data=="+buffer;
        emit receiveData(ip,port,buffer);

    }

}
bool TcpServer::startTcpServer(const QHostAddress &sIP, int iPort)
{
    bool ok = tcpServer->listen(sIP,iPort);
//    qDebug() << "TCP服务启动成功";
    return ok;
}

void TcpServer::stopTcpServer()
{
    foreach (QTcpSocket *client, tcpclients) {
        client->disconnectFromHost();
    }
//    qDebug() << "TCP服务暂停成功";
}
void TcpServer::stopTcpServer(const QString &ip, int port)
{
    foreach (QTcpSocket *client, tcpclients) {
        if (client->peerAddress().toString() == ip && client->peerPort() == port) {
            client->disconnectFromHost();
            break;
        }
    }
}



QScopedPointer<UdpClient> UdpClient::self;
UdpClient *UdpClient::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new UdpClient);
        }
    }
    return self.data();
}

UdpClient::UdpClient(QObject *parent) : QUdpSocket(parent)
{
    udpclient = new QUdpSocket(this);
    connect(udpclient, SIGNAL(readyRead()), this, SLOT(readSverData()));
}

UdpClient::~UdpClient()
{
    qDebug() << "UdpClient被析构";
}

void UdpClient::readSverData()
{
    QHostAddress host;
    quint16 port;
    QByteArray data;

    while (udpclient->hasPendingDatagrams()) {
        data.resize(udpclient->pendingDatagramSize());
        udpclient->readDatagram(data.data(), data.size(), &host, &port);

        QString ip = host.toString();
        ip = ip.replace("::ffff:", "");
        if (ip.isEmpty()) {
            continue;
        }
//        QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(buffer);
        emit doreadSverData(ip,port,data);
     }
}

void UdpClient::sendData(const QString &ip, int port, const QString &data)
{
   QByteArray buff = data.toUtf8();
   udpclient->writeDatagram(buff, QHostAddress(ip), port);
}

void UdpClient::sendData(const QString &ip, int port, const QByteArray &byte)
{
       udpclient->writeDatagram(byte, QHostAddress(ip), port);
}

//udpserver
QScopedPointer<UdpServer> UdpServer::self;
UdpServer *UdpServer::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new UdpServer);
        }
    }
    return self.data();
}
UdpServer::UdpServer(QObject *parent): QUdpSocket(parent)
{
    udpserver = new QUdpSocket(this);
    connect(udpserver, SIGNAL(readyRead()), this, SLOT(readClientData()));
}

UdpServer::~UdpServer()
{
    qDebug() << "UdpServer被析构";
}

void UdpServer::readClientData()
{
    QHostAddress host;
    quint16 port;
    QByteArray data;

    while (udpserver->hasPendingDatagrams()) {
        data.resize(udpserver->pendingDatagramSize());
        udpserver->readDatagram(data.data(), data.size(), &host, &port);

        QString ip = host.toString();
        ip = ip.replace("::ffff:", "");
        if (ip.isEmpty()) {
            continue;
        }
//        QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(buffer);
        emit doreadClientData(ip,port,data);
     }
}

bool UdpServer::startUdpServer(const QString &ip, int port)
{
    bool ok =udpserver->bind(QHostAddress(ip),port);
    if (ok){
        return true;
    } else {
        return false;
    }
}
void UdpServer::stopUdpServer()
{
    udpserver->abort();
}
void UdpServer::sendData(const QString &ip, int port, const QString &data)
{
    QByteArray buff = data.toUtf8();
    udpserver->writeDatagram(buff,QHostAddress(ip),port);
}

void UdpServer::sendData(const QString &ip, int port, const QByteArray &byte)
{
    udpserver->writeDatagram(byte,QHostAddress(ip),port);
}

