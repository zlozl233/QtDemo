#include "mvpublic.h"

#if defined(Q_OS_WIN)
#include <windows.h>
#include <tlhelp32.h>
#endif
#ifdef Q_OS_LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#endif

int MVPublic::deskWidth()
{
    //没有必要每次都获取,只有当变量为空时才去获取一次
    static int width = 0;
    if (width == 0) {
        width = QGuiApplication::screens().at(0)->availableGeometry().width();
    }

    return width;
}

int MVPublic::deskHeight()
{
    //没有必要每次都获取,只有当变量为空时才去获取一次
    static int height = 0;
    if (height == 0) {
        height = QGuiApplication::screens().at(0)->availableGeometry().height();
    }

    return height;
}

int MVPublic::MonitorCount()
{
   static int count = 0;
   if (count == 0) {
//       QDesktopWidget * deskTop = QApplication::desktop();
//        count = deskTop->screenCount();
     count = QGuiApplication::screens().count();
   }
   return count;
}

int MVPublic::MonitorIndex()
{
    static int index = -1;
    if (index == -1) {
        QDesktopWidget * deskTop = QApplication::desktop();
         index = deskTop->screenNumber (); // 参数是一个QWidget*
    }
    return index;
}

QString MVPublic::appName()
{
    //没有必要每次都获取,只有当变量为空时才去获取一次
    static QString name;
    if (name.isEmpty()) {
        name = qApp->applicationFilePath();
        QStringList list = name.split("/");
        name = list.at(list.count() - 1).split(".").at(0);
    }

    return name;
}

QString MVPublic::appPath()
{
#ifdef Q_OS_ANDROID
    return QString("/sdcard/Android/%1").arg(appName());
#else
    return qApp->applicationDirPath();
#endif
}

void MVPublic::initRand()
{
    //初始化随机数种子
    QTime t = QTime::currentTime();
    qsrand(t.msec() + t.second() * 1000);
}

QString MVPublic::getUUID_32()
{
    QUuid uuid = QUuid::createUuid();
    QString strId = uuid.toString();
    return strId.remove("{").remove("}").remove("-").toUpper();
}

QString MVPublic::getUUID_36()
{
    QUuid uuid = QUuid::createUuid();
    QString strId = uuid.toString();
    return strId.remove("{").remove("}").toUpper();
}

void MVPublic::initDb(const QString &dbName)
{
    initFile(QString(":/%1.db").arg(appName()), dbName);
}

void MVPublic::initFile(const QString &sourceName, const QString &targetName)
{
    //判断文件是否存在,不存在则从资源文件复制出来
    QFile file(targetName);
    if (!file.exists() || file.size() == 0) {
        file.remove();
        MVPublic::copyFile(sourceName, targetName);
    }
}

void MVPublic::newDir(const QString &dirName)
{
    QString strDir = dirName;

    //如果路径中包含斜杠字符则说明是绝对路径
    //linux系统路径字符带有 /  windows系统 路径字符带有 :/
    if (!strDir.startsWith("/") && !strDir.contains(":/")) {
        strDir = QString("%1/%2").arg(MVPublic::appPath()).arg(strDir);
//        qDebug() << strDir;
    }

    QDir dir(strDir);
    if (!dir.exists()) {
        dir.mkpath(strDir);
    }
}

void MVPublic::setCode()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#if _MSC_VER
    QTextCodec *codec = QTextCodec::codecForName("gbk");
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
#endif
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
#else
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif
}

void MVPublic::setFont()
{
//    QFont font;
//    font.setFamily("Microsoft Yahei");
//    font.setPixelSize(14);
//    app.setFont(font);
}

void MVPublic::sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

void MVPublic::delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < dieTime) {
    }
}

void MVPublic::setSystemDateTime(const QString &year, const QString &month, const QString &day, const QString &hour, const QString &min, const QString &sec)
{
#ifdef Q_OS_WIN
    QProcess p(0);
    p.start("cmd");
    p.waitForStarted();
    p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
    p.start("cmd");
    p.waitForStarted();
    p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
#else
    QString cmd = QString("date %1%2%3%4%5.%6").arg(month).arg(day).arg(hour).arg(min).arg(year).arg(sec);
    system(cmd.toLatin1());
    system("hwclock -w");
#endif
}

void MVPublic::runWithSystem(const QString &strName, const QString &strPath, bool autoRun)
{
#ifdef Q_OS_WIN
    //当前用户设置开机自启
    QString setCurrUser = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    //所有用户设置开机自启
    QString setAllUser = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings reg(setAllUser, QSettings::NativeFormat);
    if (autoRun) {
        qDebug()<<"AppName: "<<strName<<"AppPath: "<<strPath<<"设置开机自启";
        reg.setValue(strName, strPath);
    }
    else {
        qDebug()<<"AppName: "<<strName<<"AppPath: "<<strPath<<"取消开机自启";
        reg.setValue(strName, "");
    }
#endif
}

void MVPublic::setAutoRunWithSystem(bool autoRun)
{
#ifdef Q_OS_WIN
    //当前用户设置开机自启
    QString setCurrUser = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    //所有用户设置开机自启
    QString setAllUser = "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    QSettings reg(setAllUser, QSettings::NativeFormat);
    const QString strAppName = QApplication::applicationName();
    const QString strAppPath = QDir::toNativeSeparators(QApplication::applicationFilePath());
    if (autoRun) {
        qDebug()<<"AppName: "<<strAppName<<"AppPath: "<<strAppPath<<"设置开机自启";
        reg.setValue(strAppName, strAppPath);
    }
    else {
        qDebug()<<"AppName: "<<strAppName<<"AppPath: "<<strAppPath<<"取消开机自启";
        reg.setValue(strAppName, "");
    }
#endif
#ifdef Q_OS_LINUX
#endif
}

bool MVPublic::checkOnlyProcess(const QString &proName)
{
#ifdef Q_OS_WIN
    //  创建互斥量
    const wchar_t* wstr = reinterpret_cast<const wchar_t *>(proName.utf16());
    HANDLE m_hMutex  =  CreateMutex(NULL, FALSE,  wstr );
    //  检查错误代码
    if  (GetLastError()  ==  ERROR_ALREADY_EXISTS)  {
        //  如果已有互斥量存在则释放句柄并复位互斥量
        qDebug()<<proName+"进程已存在，禁止重复启动";
        CloseHandle(m_hMutex);
        m_hMutex  =  NULL;
        //  程序退出
        return  false;
    }
    else
        return true;
#endif
#ifdef Q_OS_LINUX
    return true;
    /*
    //proName为文件路径 eg: "/tmp/lockfile"
//    const char filename[]  = proName;
    const char* wstr = reinterpret_cast<const char *>(proName.utf16());
    int fd = open (wstr, O_WRONLY | O_CREAT , 0644);
    int flock = lockf(fd, F_TLOCK, 0 );
    if (fd == -1) {
            perror("open lockfile/n");
            return false;
    }
    //给文件加锁
    if (flock == -1) {
            perror("lock file error/n");
            return false;
    }
    //程序退出后，文件自动解锁
    return true;
    */
#endif

}

int MVPublic::killProcess(const QString &processName, const int mode)
{
    if (mode == 0) {
#ifdef Q_OS_WIN
        QStringList list;
        list<<"/im"<<processName<<"/f";
        return QProcess::execute(QLatin1String("taskkill"), list);
#endif
#ifdef Q_OS_LINUX
        return QProcess::execute("killall -9 "+processName);
#endif
    } else if (mode == 1) {
#ifdef Q_OS_WIN
        QProcess mypro;
        QStringList list;
        list<<"/im"<<processName<<"/f";
        return QProcess::execute(QLatin1String("taskkill"), list);
#endif
#ifdef Q_OS_LINUX
        QProcess mypro;
        mypro.start("killall -9 "+processName);
        return 1;
#endif
    }
    return 0;
}

bool MVPublic::startDeatached(const QString &processUrl)
{
#ifdef Q_OS_WIN
    QProcess mypro;
    qDebug() << "隔离方式启动进程: "<<processUrl;
    return mypro.startDetached(processUrl);
#endif
#ifdef Q_OS_LINUX
    QProcess mypro;
    qDebug() << "隔离方式启动进程: "<<processUrl;
    return mypro.startDetached("sudo ./"+processUrl);
//    system("sudo ./BusinessService");
#endif
}

int MVPublic::executeProcess(const QString &processUrl)
{
#ifdef Q_OS_WIN
    QProcess mypro;
    qDebug() << "阻塞方式启动进程: "<<processUrl;
    return mypro.execute(processUrl);
#endif
#ifdef Q_OS_LINUX
    QProcess mypro;
    qDebug() << "阻塞方式启动进程: "<<processUrl;
    return mypro.execute("sudo ./"+processUrl);
#endif
}

bool MVPublic::startProcess(const QString &processUrl,  const int msecs)
{
#ifdef Q_OS_WIN
    QProcess mypro;
    qDebug() << "子进程方式启动进程: "<<processUrl;
    mypro.start(processUrl);
    return mypro.waitForStarted(msecs);
#endif
#ifdef Q_OS_LINUX
    QProcess mypro;
    qDebug() << "子进程方式启动进程: "<<processUrl;
    mypro.start("sudo ./"+processUrl);
    return mypro.waitForStarted(msecs);
#endif
}

bool MVPublic::findProcess(const QString &processName)
{
#ifdef Q_OS_WIN
    PROCESSENTRY32 processEntry32;
    HANDLE toolHelp32Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (((int)toolHelp32Snapshot) != -1)
    {
        processEntry32.dwSize = sizeof(processEntry32);
        if (Process32First(toolHelp32Snapshot, &processEntry32))
        {
            do
            {
                int iLen = 2 * wcslen(processEntry32.szExeFile);
                char* chRtn = new char[iLen + 1];
                //转换成功返回为非负值
                wcstombs(chRtn, processEntry32.szExeFile, iLen + 1);
                if (strcmp(processName.toStdString().c_str(), chRtn) == 0)
                {
                    return true;
                }
            } while (Process32Next(toolHelp32Snapshot, &processEntry32));
        }
        CloseHandle(toolHelp32Snapshot);
    }
    return false;
#elif defined Q_OS_LINUX
    QProcess mypro;
    QStringList args;
    args << "-c";
    args << "ps -e -ocmd|grep -v grep| grep -v $0 | grep "+processName+" |wc -l";
//    mypro.start("sh",args);
    QString scmd = "ps -e -ocmd|grep -v grep| grep -v $0 | grep "+processName+" |wc -l";
//    qDebug()<<"scmd: "<<scmd;
//    mypro.start(scmd);
//    mypro.start("ps -a | grep "+processName);
    mypro.start("bash", QStringList() << "-c" << scmd);
    mypro.waitForFinished();
    QString output = mypro.readAllStandardOutput();
    QStringList list = output.split("\n");
    qDebug()<<"list0: "<<list.at(0);
    if (list.at(0) == "0")
        return false;
    else
        return true;
#endif

}

bool MVPublic::restartProcess(const QString &processName)
{
#ifdef Q_OS_WIN
    QProcess mypro;
    qDebug()<< ("重启进程");
//    mypro.start("shutdown  -f -r -t 0");
    return true;
#elif defined Q_OS_LINUX
    QProcess mypro;
    qDebug()<< ("重启进程");
//    mypro.start("sudo reboot");
    return true;
#endif
}

bool MVPublic::rebootSystem()
{
#ifdef Q_OS_WIN
    QProcess mypro;
    mypro.start("cmd", QStringList()<<"/c"<<"shutdown  -f -r -t 3");
    mypro.waitForStarted();
    mypro.waitForFinished();
    qDebug()<<QString::fromLocal8Bit(mypro.readAllStandardOutput());
    return true;
#elif defined Q_OS_LINUX
    QProcess mypro;
    qDebug()<< ("重启系统");
    mypro.start("sudo reboot");
    return true;
#endif
}


/* 执行CMD命令
 * 常用CMD脚本命令：
 * 重启程序： ping 127.0.0.1 -n 3&& start D:\MVProject\MVService\code\FaceReco\FaceReco_bin\bin\BusinessService.exe
 *
*/
QString MVPublic::executeCMD(const QString &tempcmd)
{
    QProcess p(nullptr);
    p.start("cmd", QStringList()<<"/c"<<tempcmd);
    p.waitForStarted();
    p.waitForFinished();
    QString output = QString::fromLocal8Bit(p.readAllStandardOutput());
    return  output;
}

QString MVPublic::executeShell(const QString &tempshell)
{
#ifdef Q_OS_WIN
    QProcess p(nullptr);
    p.start("cmd", QStringList()<<"/c"<<tempshell);
    p.waitForStarted();
    p.waitForFinished();
    QString output = QString::fromLocal8Bit(p.readAllStandardOutput());
    return  output;
#elif defined Q_OS_LINUX
    QProcess p(nullptr);
    p.start("bash", QStringList()<<"-c"<<tempshell);
    p.waitForStarted();
    p.waitForFinished();
    QString output = QString::fromLocal8Bit(p.readAllStandardOutput());
    return  output;
#endif
}


bool MVPublic::isIP(const QString &ip)
{
    QRegExp RegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    return RegExp.exactMatch(ip);
}

QString MVPublic::getHostFirstIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     return strIpAddress;
}

QStringList MVPublic::getHostAllIpAdderss()
{
    //获取本机所有IP
    QStringList ips;
    QList<QNetworkInterface> netInterfaces = QNetworkInterface::allInterfaces();
    foreach(const QNetworkInterface  &netInterface, netInterfaces) {
        //移除虚拟机和抓包工具的虚拟网卡
        QString humanReadableName = netInterface.humanReadableName().toLower();
        if(humanReadableName.startsWith("vmware network adapter") || humanReadableName.startsWith("npcap loopback adapter")) {
            continue;
        }
        //过滤当前网络接口
        bool flag = (netInterface.flags() == (QNetworkInterface::IsUp | QNetworkInterface::IsRunning | QNetworkInterface::CanBroadcast | QNetworkInterface::CanMulticast));
        if(flag) {
            QList<QNetworkAddressEntry> addrs = netInterface.addressEntries();
            foreach(QNetworkAddressEntry addr, addrs) {
                //只取出IPV4的地址
                if(addr.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                    QString ip4 = addr.ip().toString();
                    if(ip4 != "127.0.0.1") {
                        ips.append(ip4);
                    }
                }
            }
        }
    }
    return ips;
}

QString MVPublic::getHostFirstMacAdderss()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
        int nCnt = nets.count();
        QString strMacAddr = "";
        for(int i = 0; i < nCnt; i ++)
        {
            // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
            if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
            {
                strMacAddr = nets[i].hardwareAddress();
                break;
            }
        }
        return strMacAddr;
}


bool MVPublic::isMac(const QString &mac)
{
    QRegExp RegExp("^[A-F0-9]{2}(-[A-F0-9]{2}){5}$");
    return RegExp.exactMatch(mac);
}

bool MVPublic::isTel(const QString &tel)
{
    if (tel.length() != 11) {
        return false;
    }

    if (!tel.startsWith("13") && !tel.startsWith("14") && !tel.startsWith("15") && !tel.startsWith("18")) {
        return false;
    }

    return true;
}

bool MVPublic::isEmail(const QString &email)
{
    if (!email.contains("@") || !email.contains(".com")) {
        return false;
    }

    return true;
}

int MVPublic::strHexToDecimal(const QString &strHex)
{
    bool ok;
    return strHex.toInt(&ok, 16);
}

int MVPublic::strDecimalToDecimal(const QString &strDecimal)
{
    bool ok;
    return strDecimal.toInt(&ok, 10);
}

int MVPublic::strBinToDecimal(const QString &strBin)
{
    bool ok;
    return strBin.toInt(&ok, 2);
}

QString MVPublic::strHexToStrBin(const QString &strHex)
{
    uchar decimal = strHexToDecimal(strHex);
    QString bin = QString::number(decimal, 2);
    uchar len = bin.length();

    if (len < 8) {
        for (int i = 0; i < 8 - len; i++) {
            bin = "0" + bin;
        }
    }

    return bin;
}

QString MVPublic::decimalToStrBin1(int decimal)
{
    QString bin = QString::number(decimal, 2);
    uchar len = bin.length();

    if (len <= 8) {
        for (int i = 0; i < 8 - len; i++) {
            bin = "0" + bin;
        }
    }

    return bin;
}

QString MVPublic::decimalToStrBin2(int decimal)
{
    QString bin = QString::number(decimal, 2);
    uchar len = bin.length();

    if (len <= 16) {
        for (int i = 0; i < 16 - len; i++) {
            bin = "0" + bin;
        }
    }

    return bin;
}

QString MVPublic::decimalToStrHex(int decimal)
{
    QString temp = QString::number(decimal, 16);
    if (temp.length() == 1) {
        temp = "0" + temp;
    }

    return temp;
}

QByteArray MVPublic::intToByte(int i)
{
    QByteArray result;
    result.resize(4);
    result[3] = (uchar)(0x000000ff & i);
    result[2] = (uchar)((0x0000ff00 & i) >> 8);
    result[1] = (uchar)((0x00ff0000 & i) >> 16);
    result[0] = (uchar)((0xff000000 & i) >> 24);
    return result;
}

QByteArray MVPublic::intToByteRec(int i)
{
    QByteArray result;
    result.resize(4);
    result[0] = (uchar)(0x000000ff & i);
    result[1] = (uchar)((0x0000ff00 & i) >> 8);
    result[2] = (uchar)((0x00ff0000 & i) >> 16);
    result[3] = (uchar)((0xff000000 & i) >> 24);
    return result;
}

int MVPublic::byteToInt(const QByteArray &data)
{
    int i = data.at(3) & 0x000000ff;
    i |= ((data.at(2) << 8) & 0x0000ff00);
    i |= ((data.at(1) << 16) & 0x00ff0000);
    i |= ((data.at(0) << 24) & 0xff000000);
    return i;
}

int MVPublic::byteToIntRec(const QByteArray &data)
{
    int i = data.at(0) & 0x000000ff;
    i |= ((data.at(1) << 8) & 0x0000ff00);
    i |= ((data.at(2) << 16) & 0x00ff0000);
    i |= ((data.at(3) << 24) & 0xff000000);
    return i;
}

quint32 MVPublic::byteToUInt(const QByteArray &data)
{
    quint32 i = data.at(3) & 0x000000ff;
    i |= ((data.at(2) << 8) & 0x0000ff00);
    i |= ((data.at(1) << 16) & 0x00ff0000);
    i |= ((data.at(0) << 24) & 0xff000000);
    return i;
}

quint32 MVPublic::byteToUIntRec(const QByteArray &data)
{
    quint32 i = data.at(0) & 0x000000ff;
    i |= ((data.at(1) << 8) & 0x0000ff00);
    i |= ((data.at(2) << 16) & 0x00ff0000);
    i |= ((data.at(3) << 24) & 0xff000000);
    return i;
}

QByteArray MVPublic::ushortToByte(ushort i)
{
    QByteArray result;
    result.resize(2);
    result[1] = (uchar)(0x000000ff & i);
    result[0] = (uchar)((0x0000ff00 & i) >> 8);
    return result;
}

QByteArray MVPublic::ushortToByteRec(ushort i)
{
    QByteArray result;
    result.resize(2);
    result[0] = (uchar) (0x000000ff & i);
    result[1] = (uchar) ((0x0000ff00 & i) >> 8);
    return result;
}

int MVPublic::byteToUShort(const QByteArray &data)
{
    int i = data.at(1) & 0x000000FF;
    i |= ((data.at(0) << 8) & 0x0000FF00);

    if (i >= 32768) {
        i = i - 65536;
    }

    return i;
}

int MVPublic::byteToUShortRec(const QByteArray &data)
{
    int i = data.at(0) & 0x000000FF;
    i |= ((data.at(1) << 8) & 0x0000FF00);

    if (i >= 32768) {
        i = i - 65536;
    }

    return i;
}

void MVPublic::QStringConverPChar(const QString &intStr, const char *outchar)
{
    std::string str = intStr.toStdString();
    const char* ch = str.c_str();
    outchar = ch;
}


QString MVPublic::qstringToBase64(const QString &string)
{
    QByteArray ba;
    ba=string.toUtf8();           //QByteArray
    ba=ba.toBase64();             //Base64
    char * cx=ba.data();          //char *
    QString b64qs1=QString(cx);   //QString
    return b64qs1;
}

QString MVPublic::base64ToQString(const QString &bs64str)
{
    QByteArray ba;
    std::string stdStr = bs64str.toStdString();  //std::string
    ba=QByteArray(stdStr.c_str() );              //QByteArray
    ba=ba.fromBase64(ba);                        //unBase64
    QString qs1=QString::fromUtf8(ba);           //QString
    return  qs1;
}

QByteArray MVPublic::readBinAryFile(const QString &fileurl)
{
    QByteArray bafile;
    QFile file(fileurl);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return bafile;
    }
    QDataStream in(&file);
//    in.setVersion(QDataStream::Qt_5_9);
    in >> bafile;
    qDebug() <<"bafile.size: " <<bafile.size();
    file.close();
    return bafile;
}

QByteArray MVPublic::readTextFile(const QString &fileurl)
{
    QByteArray bafile;
    QFile file(fileurl);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
        return bafile;
    }
    bafile = file.readAll();
    file.close();
    return bafile;
}

bool MVPublic::writeByteToBinAryFile(const QByteArray &data, const QString &fileurl)
{
    QFile file(fileurl);
    //以只读的方式打开，并清空原文件内容
    if (!file.open(QIODevice :: WriteOnly | QIODevice :: Truncate)) {
        return false;
    }
    QDataStream out(&file);
    //设置数据流的版本，以便在不同版本的Qt间进行数据传递
//    out.setVersion(QDataStream::Qt_5_9);
    out.writeRawData(data,data.size());
//    out << data;
    file.close();
    return true;
}

bool MVPublic::writeByteToBinAryFile(const QByteArray &data, const QString &filepath, const QString &fileName)
{
    newDir(filepath);
    QFile file(filepath+fileName);
    //以只读的方式打开，并清空原文件内容
    if (!file.open(QIODevice :: WriteOnly | QIODevice :: Truncate)) {
        return false;
    }
    QDataStream out(&file);
    //设置数据流的版本，以便在不同版本的Qt间进行数据传递
//    out.setVersion(QDataStream::Qt_5_9);   
     //这样头部就不会多出来四个字节了
    out.writeRawData(data,data.size());
//    out << data;
    file.close();
    return true;
}

bool MVPublic::writeByteToTestFile(const QByteArray &data, const QString &fileurl)
{
    QFile file(fileurl);
    //以只读的方式打开，并清空原文件内容
    if (!file.open(QIODevice :: WriteOnly | QIODevice :: Truncate)) {
        return false;
    }
    QTextStream out(&file);
    out << data;
    file.close();
    return true;
}

bool MVPublic::writeByteToTestFile(const QByteArray &data, const QString &filepath, const QString &fileName)
{
    newDir(filepath);
    QFile file(filepath+fileName);
    //以只读的方式打开，并清空原文件内容
    if (!file.open(QIODevice :: WriteOnly | QIODevice :: Truncate)) {
        return false;
    }
    QTextStream out(&file);
    out << data;
    file.close();
    return true;
}

QByteArray MVPublic::jsonObjToByte(QJsonObject object)
{
    QJsonObject json;
    QJsonDocument doc = QJsonDocument(object);
    return doc.toJson();
}

QString MVPublic::getXorEncryptDecrypt(const QString &str, char key)
{
    QByteArray data = str.toLatin1();
    int size = data.size();
    for (int i = 0; i < size; i++) {
        data[i] = data[i] ^ key;
    }

    return QLatin1String(data);
}

uchar MVPublic::getOrCode(const QByteArray &data)
{
    int len = data.length();
    uchar result = 0;

    for (int i = 0; i < len; i++) {
        result ^= data.at(i);
    }

    return result;
}

uchar MVPublic::getCheckCode(const QByteArray &data)
{
    int len = data.length();
    uchar temp = 0;

    for (uchar i = 0; i < len; i++) {
        temp += data.at(i);
    }

    return temp % 256;
}

QString MVPublic::getValue(quint8 value)
{
    QString result = QString::number(value);
    if (result.length() <= 1) {
        result = QString("0%1").arg(result);
    }
    return result;
}

//函数功能：计算CRC16
//参数1：*data 16位CRC校验数据，
//参数2：len   数据流长度
//参数3：init  初始化值
//参数4：table 16位CRC查找表

//逆序CRC计算
quint16 MVPublic::getRevCrc_16(quint8 *data, int len, quint16 init, const quint16 *table)
{
    quint16 cRc_16 = init;
    quint8 temp;

    while(len-- > 0) {
        temp = cRc_16 >> 8;
        cRc_16 = (cRc_16 << 8) ^ table[(temp ^ *data++) & 0xff];
    }

    return cRc_16;
}

//正序CRC计算
quint16 MVPublic::getCrc_16(quint8 *data, int len, quint16 init, const quint16 *table)
{
    quint16 cRc_16 = init;
    quint8 temp;

    while(len-- > 0) {
        temp = cRc_16 & 0xff;
        cRc_16 = (cRc_16 >> 8) ^ table[(temp ^ *data++) & 0xff];
    }

    return cRc_16;
}

//Modbus CRC16校验
quint16 MVPublic::getModbus16(quint8 *data, int len)
{
    //MODBUS CRC-16表 8005 逆序
    const quint16 table_16[256] = {
        0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
        0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
        0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
        0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
        0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
        0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
        0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
        0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
        0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
        0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
        0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
        0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
        0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
        0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
        0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
        0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
        0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
        0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
        0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
        0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
        0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
        0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
        0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
        0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
        0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
        0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
        0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
        0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
        0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
        0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
        0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
        0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
    };

    return getCrc_16(data, len, 0xFFFF, table_16);
}

//CRC16校验
QByteArray MVPublic::getCRCCode(const QByteArray &data)
{
    quint16 result = getModbus16((quint8 *)data.data(), data.length());
    return MVPublic::ushortToByteRec(result);
}

QString MVPublic::byteArrayToAsciiStr(const QByteArray &data)
{
    QString temp;
    int len = data.size();

    for (int i = 0; i < len; i++) {
        //0x20为空格,空格以下都是不可见字符
        char b = data.at(i);

        if (0x00 == b) {
            temp += QString("\\NUL");
        } else if (0x01 == b) {
            temp += QString("\\SOH");
        } else if (0x02 == b) {
            temp += QString("\\STX");
        } else if (0x03 == b) {
            temp += QString("\\ETX");
        } else if (0x04 == b) {
            temp += QString("\\EOT");
        } else if (0x05 == b) {
            temp += QString("\\ENQ");
        } else if (0x06 == b) {
            temp += QString("\\ACK");
        } else if (0x07 == b) {
            temp += QString("\\BEL");
        } else if (0x08 == b) {
            temp += QString("\\BS");
        } else if (0x09 == b) {
            temp += QString("\\HT");
        } else if (0x0A == b) {
            temp += QString("\\LF");
        } else if (0x0B == b) {
            temp += QString("\\VT");
        } else if (0x0C == b) {
            temp += QString("\\FF");
        } else if (0x0D == b) {
            temp += QString("\\CR");
        } else if (0x0E == b) {
            temp += QString("\\SO");
        } else if (0x0F == b) {
            temp += QString("\\SI");
        } else if (0x10 == b) {
            temp += QString("\\DLE");
        } else if (0x11 == b) {
            temp += QString("\\DC1");
        } else if (0x12 == b) {
            temp += QString("\\DC2");
        } else if (0x13 == b) {
            temp += QString("\\DC3");
        } else if (0x14 == b) {
            temp += QString("\\DC4");
        } else if (0x15 == b) {
            temp += QString("\\NAK");
        } else if (0x16 == b) {
            temp += QString("\\SYN");
        } else if (0x17 == b) {
            temp += QString("\\ETB");
        } else if (0x18 == b) {
            temp += QString("\\CAN");
        } else if (0x19 == b) {
            temp += QString("\\EM");
        } else if (0x1A == b) {
            temp += QString("\\SUB");
        } else if (0x1B == b) {
            temp += QString("\\ESC");
        } else if (0x1C == b) {
            temp += QString("\\FS");
        } else if (0x1D == b) {
            temp += QString("\\GS");
        } else if (0x1E == b) {
            temp += QString("\\RS");
        } else if (0x1F == b) {
            temp += QString("\\US");
        } else if (0x7F == b) {
            temp += QString("\\x7F");
        } else if (0x5C == b) {
            temp += QString("\\x5C");
        } else if (0x20 >= b) {
            temp += QString("\\x%1").arg(decimalToStrHex((quint8)b));
        } else {
            temp += QString("%1").arg(b);
        }
    }

    return temp.trimmed();
}

QByteArray MVPublic::hexStrToByteArray(const QString &str)
{
    QByteArray senddata;
    int hexdata, lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len / 2);
    char lstr, hstr;

    for (int i = 0; i < len;) {
        hstr = str.at(i).toLatin1();
        if (hstr == ' ') {
            i++;
            continue;
        }

        i++;
        if (i >= len) {
            break;
        }

        lstr = str.at(i).toLatin1();
        hexdata = convertHexChar(hstr);
        lowhexdata = convertHexChar(lstr);

        if ((hexdata == 16) || (lowhexdata == 16)) {
            break;
        } else {
            hexdata = hexdata * 16 + lowhexdata;
        }

        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }

    senddata.resize(hexdatalen);
    return senddata;
}

char MVPublic::convertHexChar(char ch)
{
    if ((ch >= '0') && (ch <= '9')) {
        return ch - 0x30;
    } else if ((ch >= 'A') && (ch <= 'F')) {
        return ch - 'A' + 10;
    } else if ((ch >= 'a') && (ch <= 'f')) {
        return ch - 'a' + 10;
    } else {
        return (-1);
    }
}

QByteArray MVPublic::asciiStrToByteArray(const QString &str)
{
    QByteArray buffer;
    int len = str.length();
    QString letter;
    QString hex;

    for (int i = 0; i < len; i++) {
        letter = str.at(i);

        if (letter == "\\") {
            i++;
            letter = str.mid(i, 1);

            if (letter == "x") {
                i++;
                hex = str.mid(i, 2);
                buffer.append(strHexToDecimal(hex));
                i++;
                continue;
            } else if (letter == "N") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "U") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "L") {           //NUL=0x00
                        buffer.append((char)0x00);
                        continue;
                    }
                } else if (hex == "A") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "K") {           //NAK=0x15
                        buffer.append(0x15);
                        continue;
                    }
                }
            } else if (letter == "S") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "O") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "H") {           //SOH=0x01
                        buffer.append(0x01);
                        continue;
                    } else {                    //SO=0x0E
                        buffer.append(0x0E);
                        i--;
                        continue;
                    }
                } else if (hex == "T") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "X") {           //STX=0x02
                        buffer.append(0x02);
                        continue;
                    }
                } else if (hex == "I") {        //SI=0x0F
                    buffer.append(0x0F);
                    continue;
                } else if (hex == "Y") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "N") {           //SYN=0x16
                        buffer.append(0x16);
                        continue;
                    }
                } else if (hex == "U") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "B") {           //SUB=0x1A
                        buffer.append(0x1A);
                        continue;
                    }
                }
            } else if (letter == "E") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "T") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "X") {           //ETX=0x03
                        buffer.append(0x03);
                        continue;
                    } else if (hex == "B") {    //ETB=0x17
                        buffer.append(0x17);
                        continue;
                    }
                } else if (hex == "O") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "T") {           //EOT=0x04
                        buffer.append(0x04);
                        continue;
                    }
                } else if (hex == "N") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "Q") {           //ENQ=0x05
                        buffer.append(0x05);
                        continue;
                    }
                } else if (hex == "M") {        //EM=0x19
                    buffer.append(0x19);
                    continue;
                } else if (hex == "S") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "C") {           //ESC=0x1B
                        buffer.append(0x1B);
                        continue;
                    }
                }
            } else if (letter == "A") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "C") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "K") {           //ACK=0x06
                        buffer.append(0x06);
                        continue;
                    }
                }
            } else if (letter == "B") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "E") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "L") {           //BEL=0x07
                        buffer.append(0x07);
                        continue;
                    }
                } else if (hex == "S") {        //BS=0x08
                    buffer.append(0x08);
                    continue;
                }
            } else if (letter == "C") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "R") {               //CR=0x0D
                    buffer.append(0x0D);
                    continue;
                } else if (hex == "A") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "N") {           //CAN=0x18
                        buffer.append(0x18);
                        continue;
                    }
                }
            } else if (letter == "D") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "L") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "E") {           //DLE=0x10
                        buffer.append(0x10);
                        continue;
                    }
                } else if (hex == "C") {
                    i++;
                    hex = str.mid(i, 1);

                    if (hex == "1") {           //DC1=0x11
                        buffer.append(0x11);
                        continue;
                    } else if (hex == "2") {    //DC2=0x12
                        buffer.append(0x12);
                        continue;
                    } else if (hex == "3") {    //DC3=0x13
                        buffer.append(0x13);
                        continue;
                    } else if (hex == "4") {    //DC2=0x14
                        buffer.append(0x14);
                        continue;
                    }
                }
            } else if (letter == "F") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "F") {               //FF=0x0C
                    buffer.append(0x0C);
                    continue;
                } else if (hex == "S") {        //FS=0x1C
                    buffer.append(0x1C);
                    continue;
                }
            } else if (letter == "H") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "T") {               //HT=0x09
                    buffer.append(0x09);
                    continue;
                }
            } else if (letter == "L") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "F") {               //LF=0x0A
                    buffer.append(0x0A);
                    continue;
                }
            } else if (letter == "G") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "S") {               //GS=0x1D
                    buffer.append(0x1D);
                    continue;
                }
            } else if (letter == "R") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "S") {               //RS=0x1E
                    buffer.append(0x1E);
                    continue;
                }
            } else if (letter == "U") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "S") {               //US=0x1F
                    buffer.append(0x1F);
                    continue;
                }
            } else if (letter == "V") {
                i++;
                hex = str.mid(i, 1);

                if (hex == "T") {               //VT=0x0B
                    buffer.append(0x0B);
                    continue;
                }
            } else if (letter == "\\") {
                //如果连着的是多个\\则对应添加\对应的16进制0x5C
                buffer.append(0x5C);
                continue;
            } else {
                //将对应的\[前面的\\也要加入
                buffer.append(0x5C);
                buffer.append(letter.toLatin1());
                continue;
            }
        }

        buffer.append(str.mid(i, 1).toLatin1());

    }

    return buffer;
}

QString MVPublic::byteArrayToHexStr(const QByteArray &data)
{
    QString temp = "";
    QString hex = data.toHex();

    for (int i = 0; i < hex.length(); i = i + 2) {
        temp += hex.mid(i, 2) + " ";
    }

    return temp.trimmed().toUpper();
}

QString MVPublic::getSaveName(const QString &filter, QString defaultDir)
{
    return QFileDialog::getSaveFileName(0, "选择文件", defaultDir , filter);
}

QString MVPublic::getFileName(const QString &filter, QString defaultDir)
{
    return QFileDialog::getOpenFileName(0, "选择文件", defaultDir , filter);
}

QStringList MVPublic::getFileNames(const QString &filter, QString defaultDir)
{
    return QFileDialog::getOpenFileNames(0, "选择文件", defaultDir, filter);
}

QString MVPublic::getFolderName()
{
    return QFileDialog::getExistingDirectory();
}

QString MVPublic::getFileNameWithExtension(const QString &strFilePath)
{
    QFileInfo fileInfo(strFilePath);
    return fileInfo.fileName();
}

QStringList MVPublic::getFolderFileNames(const QStringList &filter)
{
    QStringList fileList;
    QString strFolder = QFileDialog::getExistingDirectory();

    if (strFolder.length() != 0) {
        QDir myFolder(strFolder);

        if (myFolder.exists()) {
            fileList = myFolder.entryList(filter);
        }
    }

    return fileList;
}

bool MVPublic::folderIsExist(const QString &strFolder)
{
    QDir tempFolder(strFolder);
    return tempFolder.exists();
}

bool MVPublic::fileIsExist(const QString &strFile)
{
    QFile tempFile(strFile);
    return tempFile.exists();
}

bool MVPublic::copyFile(const QString &sourceFile, const QString &targetFile)
{
    bool ok;
    ok = QFile::copy(sourceFile, targetFile);
    //将复制过去的文件只读属性取消
    ok = QFile::setPermissions(targetFile, QFile::WriteOwner);
    return ok;
}

void MVPublic::deleteDirectory(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return;
    }

    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    QFileInfoList fileList = dir.entryInfoList();

    foreach (QFileInfo fi, fileList) {
        if (fi.isFile()) {
            fi.dir().remove(fi.fileName());
        } else {
            deleteDirectory(fi.absoluteFilePath());
            dir.rmdir(fi.absoluteFilePath());
        }
    }
}

void MVPublic::deleteFile(const QString &fileurl)
{
   if (!fileIsExist(fileurl))
       return;
   QFileInfo FileInfo(fileurl);
   if (FileInfo.isFile()) //如果是文件
       QFile::remove(fileurl);
}

bool MVPublic::ipLive(const QString &ip, int port, int timeout)
{
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost(ip, port);
    //超时没有连接上则判断不在线
    return tcpClient.waitForConnected(timeout);
}

QString MVPublic::getHtml(const QString &url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    return QString(responseData);
}

QString MVPublic::getNetIP(const QString &webCode)
{
    QString web = webCode;
    web = web.replace(' ', "");
    web = web.replace("\r", "");
    web = web.replace("\n", "");
    QStringList list = web.split("<br/>");
    QString tar = list.at(3);
    QStringList ip = tar.split("=");
    return ip.at(1);
}

QString MVPublic::getLocalIP()
{
    QStringList ips;
    QList<QHostAddress> addrs = QNetworkInterface::allAddresses();
    foreach (QHostAddress addr, addrs) {
        QString ip = addr.toString();
        if (MVPublic::isIP(ip)) {
            ips << ip;
        }
    }

    //优先取192开头的IP,如果获取不到IP则取127.0.0.1
    QString ip = "127.0.0.1";
    foreach (QString str, ips) {
        if (str.startsWith("192.168.1") || str.startsWith("192")) {
            ip = str;
            break;
        }
    }

    return ip;
}

QString MVPublic::urlToIP(const QString &url)
{
    QHostInfo host = QHostInfo::fromName(url);
    return host.addresses().at(0).toString();
}

bool MVPublic::isWebOk()
{
    //能接通百度IP说明可以通外网
    return ipLive("115.239.211.112", 80);
}
