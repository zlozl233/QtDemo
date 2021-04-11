#include "ForwardMain.h"

#include <QApplication>
#include "crash_exception.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef Q_OS_WIN
    //防止进程重复启动
    QSharedMemory singleton(a.applicationName());
    if(!singleton.create(1))
    {
      QMessageBox::warning(NULL, "提示", "程序已运行!");
      return false;
    }
    //加入崩溃dump文件功能
    SetUnhandledExceptionFilter(ExceptionFilter);
#else

#endif
    App::appName = MVPublic::appName();
    App::appPath = MVPublic::appPath();
    App::configFile = QString("%1/%2.ini").arg(App::appPath).arg(App::appName);
    //App::readConfig();
    if (App::isOutPutDebuginfo) {
        void MessageOutPut(QtMsgType type, const QMessageLogContext &context, const QString &msg);
        qInstallMessageHandler(MessageOutPut);
    }
    ForwardMain w;
    if (App::isShowGUI) {
        w.setWindowTitle("终端转发服务"+MVFORWARDERVICE_VERSION);
        w.showMinimized();
    } else {
        //隐藏界面后台挂起服务
        w.setWindowFlags(Qt::Tool);
    }
    return a.exec();
}
