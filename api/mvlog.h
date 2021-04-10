#ifndef MVLog_H
#define MVLog_H

#ifdef Q_OS_WIN
#define NEWLINE "\r\n"
#else
#define NEWLINE "\n"
#endif

/**********************************************************************
 *@projectName    MVRecoTL
 *@brief          设计模式之——单例模式(日志类，线程是安全的)
 *Author:         zlozl
 *Copyright:      zlozl
 *Email:          zlozl5566@163.com
 *QQ:             849414798
 *BlogAddress:    https://blog.csdn.net/ZLOZL
 *GiteeHome:      https://gitee.com/zlozl5566
 *Version:        V1.0.0.1
 *Create Date:    2021-01-19
**********************************************************************/

/* 如何需要定向输出编译器日志，下面段代码放到mian函数中去
 * 日志文件大小大于1m 就清空
  App::appName = MVPublic::appName();
  App::appPath = MVPublic::appPath();
  App::configFile = QString("%1/%2.ini").arg(App::appPath).arg(App::appName);
  App::readConfig();
  if (App::isOutPutDebuginfo) {
    void MessageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    qInstallMessageHandler(MessageOutPut);
  }
*/

#include <QObject>
#include <QDebug>
#include "app.h"

class MVLog : public QObject
{
    Q_OBJECT
private:
    MVLog() { }                          //防止外部调用构造创建对象
    ~MVLog(){ }                          //析构函数
    MVLog(const MVLog &);                //阻止拷贝创建对象
    MVLog& operator = (const MVLog &);   //阻止赋值对象
public:
    static MVLog *GetInstance();
    //打开日志并创建日志文件夹  默认为程序启动路径
    void openLogFile(const QString &fileName, const QString &filePath = QCoreApplication::applicationDirPath());
    //清理日志文件下日志数量  默认保留90天日志
    void clearLogFile(const QString &filePath, const int &saveNum = 90);
    //写消息到日志，新的一天会重新写一个log文件
    void sysLog(const QString &info);
    void closeLog();
 /*[TODO]
 *Date:            2020-09-29
 * 日志超过90天就删除最老，而且不用重启软件触发，自动删除
 * debug调试信息文本输出太多就要分开写入，删除时候全部删除
 * debug调试日志最好重新写一个文件夹下，方便删除管理
 *
 */
};

#endif // MVLog_H
