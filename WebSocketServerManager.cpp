#include "WebSocketServerManager.h"
#include <QDebug>
#include <QWebSocket>
 
WebSocketServerManager::WebSocketServerManager(QString serverName,
                                               QWebSocketServer::SslMode secureMode,
                                               QObject *parent)
    : QObject(parent),
      _serverName(serverName),
      _sslMode(secureMode),
      _running(false),
      _pWebSocketServer(0)
{
}
 
WebSocketServerManager::~WebSocketServerManager()
{
    if(_pWebSocketServer != 0)
    {
        _pWebSocketServer->deleteLater();
        _pWebSocketServer = 0;
    }
}
 
bool WebSocketServerManager::running() const
{
    return _running;
}
 
void WebSocketServerManager::slot_start(QHostAddress hostAddress, qint32 port)
{
    if(_running)
    {
        qDebug() << __FILE__ << __LINE__
                 << "Failed to" << __FUNCTION__ << "it's already running...";
        return;
    }
    if(!_pWebSocketServer)
    {
        _pWebSocketServer = new QWebSocketServer(_serverName, _sslMode, 0);
        connect(_pWebSocketServer, SIGNAL(newConnection()), this, SLOT(slot_newConnection()));
        connect(_pWebSocketServer, SIGNAL(closed()), this, SLOT(slot_closed()));
        connect(_pWebSocketServer, SIGNAL(serverError(QWebSocketProtocol::CloseCode)),
                this             , SLOT(slot_serverError(QWebSocketProtocol::CloseCode)));
    }
    _listenHostAddress = hostAddress;
    _listenPort = port;
    _pWebSocketServer->listen(_listenHostAddress, _listenPort);
    _running = true;
}
 
void WebSocketServerManager::slot_stop()
{
    if(!_running)
    {
        qDebug() << __FILE__ << __LINE__
                 << "Failed to" << __FUNCTION__
                 << ", it's not running...";
        return;
    }
    _running = false;
    _pWebSocketServer->close();
}
 
void WebSocketServerManager::slot_sendData(QString ip, qint32 port, QString message)
{
    QString key = QString("%1-%2").arg(ip).arg(port);
    if(_hashIpPort2PWebSocket.contains(key))
    {
        _hashIpPort2PWebSocket.value(key)->sendTextMessage(message);
    }
}

void WebSocketServerManager::slot_sendData(QString ip, qint32 port, QByteArray data)
{
    QString key = QString("%1-%2").arg(ip).arg(port);
    if(_hashIpPort2PWebSocket.contains(key))
    {
        _hashIpPort2PWebSocket.value(key)->sendBinaryMessage(data);
    }
}

void WebSocketServerManager::slot_sendData(QString message)
{
    QList<QString> kList = _hashIpPort2PWebSocket.keys();

    for(int i=0; i<kList.count(); i++)
    {
//        qDebug() << kList[i];
        _hashIpPort2PWebSocket.value(kList[i])->sendTextMessage(message);
    }

}

void WebSocketServerManager::slot_sendData(QByteArray data)
{
    QList<QString> kList = _hashIpPort2PWebSocket.keys();

    for(int i=0; i<kList.count(); i++)
    {
//        qDebug() << kList[i];
        _hashIpPort2PWebSocket.value(kList[i])->sendBinaryMessage(data);
    }
}
 
void WebSocketServerManager::slot_newConnection()
{
    QWebSocket *pWebSocket = _pWebSocketServer->nextPendingConnection();
    connect(pWebSocket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
    connect(pWebSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this      , SLOT(slot_error(QAbstractSocket::SocketError)));
    // 既会触发frame接收也会触发message接收
//    connect(pWebSocket, SIGNAL(textFrameReceived(QString,bool)),
//            this      , SLOT(slot_textFrameReceived(QString,bool)));
    connect(pWebSocket, SIGNAL(textMessageReceived(QString)),
            this      , SLOT(slot_textMessageReceived(QString)));
    connect(pWebSocket, SIGNAL(binaryMessageReceived(QByteArray)),
            this      , SLOT(slot_binaryReceived(QByteArray)));

    _hashIpPort2PWebSocket.insert(QString("%1-%2").arg(pWebSocket->peerAddress().toString())
                                  .arg(pWebSocket->peerPort()),
                                  pWebSocket);
    qDebug() << __FILE__ << __LINE__ << pWebSocket->peerAddress().toString() << pWebSocket->peerPort();
    emit signal_conncted(pWebSocket->peerAddress().toString(), pWebSocket->peerPort());
}
 
void WebSocketServerManager::slot_serverError(QWebSocketProtocol::CloseCode closeCode)
{
    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
    if(!pWebSocket)
    {
        return;
    }
    emit signal_error(pWebSocket->peerAddress().toString(), pWebSocket->peerPort(), _pWebSocketServer->errorString());
}
 
void WebSocketServerManager::slot_closed()
{
    QList<QWebSocket *> _listWebSocket = _hashIpPort2PWebSocket.values();
    for(int index = 0; index < _listWebSocket.size(); index++)
    {
        _listWebSocket.at(index)->close();
    }
    _hashIpPort2PWebSocket.clear();
    emit signal_close();
}
 
void WebSocketServerManager::slot_disconnected()
{
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
    if(!pWebSocket)
    {
        return;
    }
    qDebug() << __FILE__ << __LINE__ << __FUNCTION__;
    emit signal_disconncted(pWebSocket->peerAddress().toString(), pWebSocket->peerPort());
    _hashIpPort2PWebSocket.remove(QString("%1-%2").arg(pWebSocket->peerAddress().toString())
                                                  .arg(pWebSocket->peerPort()));
}
 
void WebSocketServerManager::slot_error(QAbstractSocket::SocketError error)
{
    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
    if(!pWebSocket)
    {
        return;
    }
    emit signal_error(pWebSocket->peerAddress().toString(), pWebSocket->peerPort(), pWebSocket->errorString());
}
 
void WebSocketServerManager::slot_textFrameReceived(const QString &frame, bool isLastFrame)
{
    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
    if(!pWebSocket)
    {
        return;
    }
    qDebug() << __FILE__ << __LINE__ << frame << isLastFrame;
    emit signal_textFrameReceived(pWebSocket->peerAddress().toString(), pWebSocket->peerPort(), frame, isLastFrame);
}
 
void WebSocketServerManager::slot_textMessageReceived(const QString &message)
{
    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
    if(!pWebSocket)
    {
        return;
    }
    emit signal_textMessageReceived(pWebSocket->peerAddress().toString(), pWebSocket->peerPort(), message);
}

void WebSocketServerManager::slot_binaryReceived(const QByteArray &data)
{
    QWebSocket *pWebSocket = dynamic_cast<QWebSocket *>(sender());
    if(!pWebSocket)
    {
        return;
    }
    emit signal_binaryDataReceived(pWebSocket->peerAddress().toString(), pWebSocket->peerPort(), data);
}
