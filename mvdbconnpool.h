#ifndef MVDBCONNPOOL_H
#define MVDBCONNPOOL_H

/**********************************************************************
 *@projectName    dbCRUD1
 *@brief          数据库连接池
    * 数据库连接池特点:
    * 获取连接时不需要了解连接的名字，连接池内部维护连接的名字
    * 支持多线程，保证获取到的连接一定是没有被其他线程正在使用
    * 按需创建连接,可以创建多个连接,可以控制连接的数量
    * 连接被复用，不是每次都重新创建一个新的连接（连接的创建是一个很消耗资源的过程）
    * 连接断开了后会自动重连
    * 当无可用连接时，获取连接的线程会等待一定时间尝试继续获取，直到取到有效连接或者超时返回一个无效的连接
    * 关闭连接很简单
 *Author:         zlozl
 *Copyright:      zlozl
 *Email:          zlozl5566@163.com
 *BlogAddress:    https://blog.csdn.net/ZLOZL
 *GiteeHome:      https://gitee.com/zlozl5566
 *Version:        V1.0.0.1
 *Date:           2020-09-05
**********************************************************************/
#include <QObject>
#include <QtSql>

class MVDBConnPool
{
public:

    enum DbType {
        DbType_Sqlite = 0,      //sqlite数据库
        DbType_MySql = 1,       //mysql数据库
        DbType_Oracle = 2,      //oracle数据库
        DbType_SqlServer = 3,   //sqlserver数据库
        DbType_Access = 4,      //access数据库
    };
    /*
     * @brief 获取数据库连接，连接使用完后不需要手动关闭，数据库连接池会在使用此连接的线程结束后自动关闭连接。
     * 传入的连接名 connectionName 默认为空 (内部会为连接名基于线程的信息创建一个唯一的前缀)，
     * 如果同一个线程需要使用多个不同的数据库连接，可以传入不同的 connectionName
     *
     * @param emdbtype 数据库类型 connectionName 连接的名字
     * @return 返回 连接bool，数据库连接
    */
    static QPair<bool, QSqlDatabase> openConnection(MVDBConnPool::DbType emdbtype, const QString &connectionName = "");

    //sqlite
    static void setSqliteUrl(const QString &url);
    //mysql
    static void setMysqlHostName(const QString &hostname);
    static void setMysqlDatabaseName(const QString &dbname);
    static void setMysqlUserName(const QString &username);
    static void setMysqlPassword(const QString &password);

private:
    // 创建数据库连接
    static QPair<bool, QSqlDatabase> createConnection(MVDBConnPool::DbType emdbtype, const QString &connectionName);

protected:
    static QString sqlite_url;
    static QString mysql_hostname;
    static QString mysql_databasename;
    static QString mysql_username;
    static QString mysql_password;

};

#endif // MVDBCONNPOOL_H
