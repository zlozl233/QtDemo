#include "mvdbconnpool.h"

QString MVDBConnPool::sqlite_url = "";
QString MVDBConnPool::mysql_hostname = "127.0.0.1";
QString MVDBConnPool::mysql_databasename = "test";
QString MVDBConnPool::mysql_username = "root";
QString MVDBConnPool::mysql_password = "123456";


QPair<bool, QSqlDatabase> MVDBConnPool::openConnection(MVDBConnPool::DbType emdbtype, const QString &connectionName)
{
    // 1. 创建连接的全名: 基于线程的地址和传入进来的 connectionName，因为同一个线程可能申请创建多个数据库连接
    // 2. 如果连接已经存在，复用它，而不是重新创建
    //    2.1 返回连接前访问数据库，如果连接断开，可以重新建立连接 (测试: 关闭数据库几分钟后再启动，再次访问数据库)
    // 3. 如果连接不存在，则创建连接
    // 4. 线程结束时，释放在此线程中创建的数据库连接

    // [1] 创建连接的全名: 基于线程的地址和传入进来的 connectionName，因为同一个线程可能申请创建多个数据库连接
    QString baseConnectionName = "conn_" + QString::number(quint64(QThread::currentThread()), 16);
    QString fullConnectionName = baseConnectionName +"_"+ connectionName;
//    qDebug() << fullConnectionName;
    if (QSqlDatabase::contains(fullConnectionName)) {
        // [2] 如果连接已经存在，复用它，而不是重新创建
        QSqlDatabase existingDb = QSqlDatabase::database(fullConnectionName);

        // [2.1] 返回连接前访问数据库，如果连接断开，可以重新建立连接 (测试: 关闭数据库几分钟后再启动，再次访问数据库)
        QSqlQuery query("SELECT 1", existingDb);

        if (query.lastError().type() != QSqlError::NoError && !existingDb.open()) {
            qDebug().noquote() << "Open datatabase error:" << existingDb.lastError().text();
            return {false,QSqlDatabase()};
        }

        return {true,existingDb};
    } else {
        // [3] 如果连接不存在，则创建连接
        if (qApp != nullptr) {
            // [4] 线程结束时，释放在此线程中创建的数据库连接
            QObject::connect(QThread::currentThread(), &QThread::finished, qApp, [fullConnectionName] {
                if (QSqlDatabase::contains(fullConnectionName)) {
                    QSqlDatabase::removeDatabase(fullConnectionName);
                    qDebug().noquote() << QString("Connection deleted: %1").arg(fullConnectionName);
                }
            });
        }

        return createConnection(emdbtype,fullConnectionName);
    }
}

void MVDBConnPool::setSqliteUrl(const QString &url)
{
    sqlite_url = url;
}

void MVDBConnPool::setMysqlHostName(const QString &hostname)
{
    mysql_hostname = hostname;
}

void MVDBConnPool::setMysqlDatabaseName(const QString &dbname)
{
    mysql_databasename = dbname;
}

void MVDBConnPool::setMysqlUserName(const QString &username)
{
    mysql_username = username;
}

void MVDBConnPool::setMysqlPassword(const QString &password)
{
    mysql_password = password;
}

QPair<bool, QSqlDatabase> MVDBConnPool::createConnection(MVDBConnPool::DbType emdbtype, const QString &connectionName)
{
    static int sn = 0;
    QSqlDatabase db;
    // 创建一个新的数据库连接
//    qDebug() << "支持数据库的驱动:"<<QSqlDatabase::drivers();
    switch (emdbtype) {
    case DbType_Sqlite:
    {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
        db.setDatabaseName(sqlite_url);
        break;
    }
    case DbType_MySql:
    {
        db = QSqlDatabase::addDatabase("QMYSQL", connectionName);
        db.setHostName(mysql_hostname);
        db.setDatabaseName(mysql_databasename);
        db.setUserName(mysql_username);
        db.setPassword(mysql_password);
        break;
    }
    case DbType_Oracle:
    {
        break;
    }
    case DbType_SqlServer:
    {
        break;
    }
    case DbType_Access:
    {
        break;
    }
    default:
        qDebug () << "不是可支持的数据库类型";
        break;
    }

    if (db.open()) {
        qDebug().noquote() << QString("Connection created: %1, sn: %2").arg(connectionName).arg(++sn);
        return {true,db};
    } else {
        qDebug().noquote() << "Create connection error:" << db.lastError().text();
        return {false,QSqlDatabase()};
    }
}
