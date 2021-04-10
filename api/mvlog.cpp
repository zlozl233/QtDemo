#include "mvlog.h"
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QDateTime>

MVLog *MVLog::GetInstance()
{
    static MVLog *plog = nullptr;
    if (plog == nullptr) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        plog = new MVLog();
    }
    return plog;
}

void MVLog::openLogFile(const QString &fileName, const QString &filePath)
{
    App::logFileName = fileName;
    App::logFileDir = QString("%1/%2").arg(filePath).arg(fileName)+"/";
    qDebug() << "logFileDir=="+App::logFileDir;
    QDir dir(App::logFileDir);
    if (!dir.exists()) {
        dir.mkpath(App::logFileDir);
    }
    App::logFileNamePath = App::logFileDir + QDateTime::currentDateTime().toString("yyyyMMdd")+".log";
    sysLog("***************"+App::appName+"程序启动,打开日志***************");
}

void MVLog::clearLogFile(const QString &filePath, const int &saveNum)
{
//    QFile::remove(qApp->applicationDirPath()+"/Debuginfo.log");
//    QFile::remove(qApp->applicationDirPath()+"/"+App::appName+"_debinfo.log");
    QDir dir(filePath);
    QStringList filters;  //设置过滤器
    filters<<"*.log";  //过滤留下log文件
    dir.setNameFilters(filters);
    QFileInfoList fileList = dir.entryInfoList(QDir::Files,QDir::Name); //过滤当前路径下的文件并按日期小到大排序
    if (fileList.count() > saveNum) {
        QStringList logfilelist;
        for(int i=0;i<fileList.count()-saveNum;i++)
        {
            logfilelist.append(fileList[i].filePath());
            QFile::remove(logfilelist[i]);   //删除多余最老的日志文件
        }
    }
}

void MVLog::sysLog(const QString &info)
{
    QString current_dt = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss:zzz]  ");
    QString nowLogFilePath = App::logFileDir + QDateTime::currentDateTime().toString("yyyyMMdd")+".log";
    if (App::logFileNamePath != nowLogFilePath) {   //日期变了就新建一个log
        App::logFileNamePath = nowLogFilePath;
    }
    QFile file(nowLogFilePath);
    file.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    QTextStream stream(&file);
    stream << current_dt << info << NEWLINE;
    stream.flush();
    file.flush();
    file.close();
}

void MVLog::closeLog()
{
    sysLog("***************"+App::appName+"程序退出,关闭日志***************");
}

/*
  * @brief 定向输出编译器调试信息
  * 请在main函数中加入下面判断条件是否定向输出调试信息
  *     if (App::isOutPutDebuginfo) {
  *     void MessageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg);
  *      qInstallMessageHandler(MessageOutPut);
  *   }
  * @param
  *
  * @return
  * 调试信息会输出在程序启动路径下的debinfo.log,并且重启程序会自动删除
  *
*/
void MessageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString text;
   switch(type)
   {
   case QtDebugMsg:
       text = QString("Debug:");
       break;

   case QtWarningMsg:
       text = QString("Warning:");
       break;

   case QtCriticalMsg:
       text = QString("Critical:");
       break;

   case QtFatalMsg:
       text = QString("Fatal:");
       break;

   case QtInfoMsg:
       text = QString("Info:");
       break;

   default:
       break;
   }
   //日志写到文件
   QString current_dt = QDateTime::currentDateTime().toString("[yyyy-MM-dd-HH-mm-ss-zzz] ");
   QString message = QString("%1%2   %3").arg(current_dt).arg(text).arg(msg);
//   QString LOG_FILE  =  qApp->applicationDirPath()+"/debinfo.log";
   QString LOG_FILE  =  qApp->applicationDirPath()+"/"+App::appName+"_debinfo.log";

   QFile debfile(LOG_FILE);
   debfile.open(QIODevice::WriteOnly | QIODevice::Append);
//日志文件大小大于1m 就清空
   if (debfile.size() > 1024000) {
       debfile.close();
       debfile.open(QIODevice::WriteOnly | QIODevice::Truncate);
       qDebug() << "<<<<<<<<<<<<<<<<<<<清空调试信息日志>>>>>>>>>>>>>>>>>>>>";
   }
   QTextStream text_stream(&debfile);
   text_stream << message << "\r\n";
   text_stream.flush();   //刷新缓冲区
   debfile.flush();
   //不建议频繁打开关闭日志文件，IO性能瓶颈很大
   debfile.close();
}
