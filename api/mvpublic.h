#ifndef MVPUBLIC_H
#define MVPUBLIC_H

/**********************************************************************
 *@projectName    ApiTest
 *@brief          常用静态方法类
 * 1.存放一些常用且公用的方法
 * 2.获取桌面宽，高，屏幕数量，当前屏序号
 * 3.获取当前程序文件名称，所在路径
 * 4.获取随机因子，以及生成32和36位的UUID
 * 5.设置开启自启，系统时间，程序字符编码
 * 6.判断和获取本机IP和Mac地址
 * 7.一些进制，以及数据类型的之间的转化
 * 8.一些关于文件的操作，读，写，删等
 * 9.一些进程操作，杀进程，启动进程，查进程,开机自启进程，禁止重复启动进程, 重启进程，重启系统
 * 10. 可以执行CMD命令
 *
 *Author:         zlozl
 *Copyright:      zlozl
 *Email:          zlozl5566@163.com
 *QQ:             849414798
 *BlogAddress:    https://blog.csdn.net/ZLOZL
 *GiteeHome:      https://gitee.com/zlozl5566
 *Version:        V1.0.0.1
 *Create Date:    2021-01-09
**********************************************************************/
/* [Commit]
 * 11.可以执行Shell脚本，Linux下进程操作
 * 12.获取本机Mac地址
 *
 * Version:   V1.0.0.2
 * Date:      2021-01-20
 * Author:    zlozl
 */

/* [Commit]
 * 13.新增写本地文件,判断文件路径 无则新建
 * 14.新增一些文件操作
 *
 * Version:   V1.0.0.3
 * Date:      2021-03-22
 * Author:    zlozl
 */

/* [Commit]
 * 15.修复 QDataStream 写文件头部多四个字节
 *
 * Version:   V1.0.0.4
 * Date:      2021-03-24
 * Author:    zlozl
 */

#include <QObject>
#include <QtWidgets>
#include <QtNetwork>
#include <QUuid>
#include <QProcess>

//静态方法类
class MVPublic: public QObject
{
    Q_OBJECT
public:
    //桌面宽度高度
    static int deskWidth();
    static int deskHeight();
    //屏幕数量
    static int MonitorCount();
    //当前屏序号
    static int MonitorIndex();
    //程序本身文件名称
    static QString appName();
    //程序当前所在路径
    static QString appPath();

    //初始化随机数种子
    static void initRand();

    //产生UUID
    static QString getUUID_32();

    static QString getUUID_36();

    //初始化数据库
    static void initDb(const QString &dbName);
    //初始化文件,不存在则拷贝
    static void initFile(const QString &sourceName, const QString &targetName);
    //新建目录
    static void newDir(const QString &dirName);

    //设置编码
    static void setCode();
    //设置字体
    static void setFont();

    //设置延时,非阻塞,最大误差有100ms
    static void sleep(int msec);
    //设置延时,阻塞
    static void delay(int msec);

    //设置系统时间
    static void setSystemDateTime(const QString &year, const QString &month, const QString &day,
                                  const QString &hour, const QString &min, const QString &sec);
    //设置开机自启动
    static void runWithSystem(const QString &strName, const QString &strPath, bool autoRun = true);
    static void setAutoRunWithSystem(bool autoRun = true);

    //禁止进程重复启动
    static bool checkOnlyProcess(const QString &proName);

    //杀进程 0:阻塞 1：非阻塞
    static int killProcess(const QString &processName, const int mode = 0);

    //隔离方式启动进程
    static bool startDeatached(const QString &processUrl);
    //阻塞启动进程
    static int executeProcess(const QString &processUrl);
    //子进程方式启动进程
    static bool startProcess(const QString &processUrl, const int mesc = 3000);
    //判断进程是否存在
    static bool findProcess(const QString &processName);
    //重启进程
    static bool restartProcess(const QString &processName);
    //重启系统
    static bool rebootSystem();
    //执行CMD命令
    static QString executeCMD(const QString &tempcmd);
    //执行终端脚本
    static QString executeShell(const QString &tempshell);

    //判断是否是IP地址
    static bool isIP(const QString &ip);

    //获取机器第一个IPv4地址
    static QString getHostFirstIpAddress();

    //获取机器所有IP地址
    static QStringList getHostAllIpAdderss();

    //获取第一张网卡的Mac地址
    static QString getHostFirstMacAdderss();

    //获取机器所有网卡的Mac地址
    static QString getHostAllMacAdderss();

    //判断是否是MAC地址
    static bool isMac(const QString &mac);

    //判断是否是合法的电话号码
    static bool isTel(const QString &tel);

    //判断是否是合法的邮箱地址
    static bool isEmail(const QString &email);

    //16进制字符串转10进制
    static int strHexToDecimal(const QString &strHex);

    //10进制字符串转10进制
    static int strDecimalToDecimal(const QString &strDecimal);

    //2进制字符串转10进制
    static int strBinToDecimal(const QString &strBin);

    //16进制字符串转2进制字符串
    static QString strHexToStrBin(const QString &strHex);

    //10进制转2进制字符串一个字节
    static QString decimalToStrBin1(int decimal);

    //10进制转2进制字符串两个字节
    static QString decimalToStrBin2(int decimal);

    //10进制转16进制字符串,补零.
    static QString decimalToStrHex(int decimal);


    //int转字节数组
    static QByteArray intToByte(int i);
    static QByteArray intToByteRec(int i);

    //字节数组转int
    static int byteToInt(const QByteArray &data);
    static int byteToIntRec(const QByteArray &data);
    static quint32 byteToUInt(const QByteArray &data);
    static quint32 byteToUIntRec(const QByteArray &data);

    //ushort转字节数组
    static QByteArray ushortToByte(ushort i);
    static QByteArray ushortToByteRec(ushort i);

    //字节数组转ushort
    static int byteToUShort(const QByteArray &data);
    static int byteToUShortRec(const QByteArray &data);

    //QByteArray QString 转化
//    static QString ByteToQString(const QByteArray &data);

    //QString Char* 转化
    static void QStringConverPChar(const QString &intStr, const char *outchar);

    //QByteArray QImage 转化
//    static QImage ByteConverQImage(const QByteArray &data);

    //QString Base64 加密 解密
    static QString qstringToBase64(const QString &string);
    static QString base64ToQString(const QString &bs64str);

    //读取二进制文件到字节数组
    static QByteArray readBinAryFile(const QString &fileurl);
    //读取普通文本到字节数组
    static QByteArray readTextFile(const QString &fileurl);
    //注意：写入的文件会多4个字节(文件头)【已修复】 字节数组写入到二进制文件(会清空原有文件数据)
    static bool writeByteToBinAryFile(const QByteArray &data, const QString &fileurl);
    static bool writeByteToBinAryFile(const QByteArray &data, const QString &filepath, const QString &fileName);
    //字节数组写入到普通文件(会清空原有文件数据)一般推荐使用这样方法
    static bool writeByteToTestFile(const QByteArray &data, const QString &fileurl);
    static bool writeByteToTestFile(const QByteArray &data, const QString &filepath, const QString &fileName);


    //json对象转字节数组
    static QByteArray jsonObjToByte(QJsonObject object);
    //字节数组转json对象
//    static QJsonObject byteToJsonObj();

    //异或加密算法
    static QString getXorEncryptDecrypt(const QString &str, char key);

    //异或校验
    static uchar getOrCode(const QByteArray &data);

    //计算校验码
    static uchar getCheckCode(const QByteArray &data);

    //字符串补全
    static QString getValue(quint8 value);

    //CRC校验
    static quint16 getRevCrc_16(quint8 *data, int len, quint16 init, const quint16 *table);
    static quint16 getCrc_16(quint8 *data, int len, quint16 init, const quint16 *table);
    static quint16 getModbus16(quint8 *data, int len);
    static QByteArray getCRCCode(const QByteArray &data);


    //字节数组转Ascii字符串
    static QString byteArrayToAsciiStr(const QByteArray &data);

    //16进制字符串转字节数组
    static QByteArray hexStrToByteArray(const QString &str);
    static char convertHexChar(char ch);

    //Ascii字符串转字节数组
    static QByteArray asciiStrToByteArray(const QString &str);

    //字节数组转16进制字符串
    static QString byteArrayToHexStr(const QByteArray &data);

    //获取保存的文件
    static QString getSaveName(const QString &filter, QString defaultDir = QCoreApplication::applicationDirPath());

    //获取选择的文件
    static QString getFileName(const QString &filter, QString defaultDir = QCoreApplication::applicationDirPath());

    //获取选择的文件集合
    static QStringList getFileNames(const QString &filter, QString defaultDir = QCoreApplication::applicationDirPath());

    //获取选择的目录
    static QString getFolderName();

    //获取文件名,含拓展名
    static QString getFileNameWithExtension(const QString &strFilePath);

    //获取选择文件夹中的文件
    static QStringList getFolderFileNames(const QStringList &filter);

    //文件夹是否存在
    static bool folderIsExist(const QString &strFolder);

    //文件是否存在
    static bool fileIsExist(const QString &strFile);

    //复制文件
    static bool copyFile(const QString &sourceFile, const QString &targetFile);

    //删除文件夹下所有文件
    static void deleteDirectory(const QString &path);
    //删除单独一个文件
    static void deleteFile(const QString &fileurl);

    //判断IP地址及端口是否在线
    static bool ipLive(const QString &ip, int port, int timeout = 1000);

    //获取网页所有源代码
    static QString getHtml(const QString &url);

    //获取本机公网IP地址
    static QString getNetIP(const QString &webCode);

    //获取本机IP
    static QString getLocalIP();

    //Url地址转为IP地址
    static QString urlToIP(const QString &url);

    //判断是否通外网
    static bool isWebOk();

};
#endif // MVPUBLIC_H
