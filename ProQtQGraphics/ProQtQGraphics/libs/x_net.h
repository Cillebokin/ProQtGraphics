#ifndef X_NET_H
#define X_NET_H

#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#include "time.h"
#include "stdlib.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <windows.h>
#include <Shlobj.h>
#endif
#include "unistd.h"
using namespace std;

//为了和将来用到的程序中公用一个头文件，创建dll时用到的是__declspec(dllexport)，而使用dll时用到__declspec(dllimport)，完全可以各自写一下
#define X_NETSHARED_EXPORT __declspec(dllexport)
#ifdef X_NETSHARED_EXPORT
#else
#define X_NETSHARED_EXPORT __declspec(dllimport)
#endif

//类型定义
typedef unsigned char                        XNetByte;
typedef unsigned char                        uchar;
typedef unsigned short                       UInt16;

//XNet服务端口
#define 	ServicePort                      2323
#define     ServiceIp                        "127.0.0.1"

//操作结果（仅告知操作是否成功）
#define		XNetCommResult                   int
#define		XNetCommResult_Err               -1      // XNet串口通讯错误
#define		XNetCommResult_IPErr             -2      // IP地址设置错误
#define		XNetCommResult_StartXNetErr      -3      // 启动XNet错误
#define		XNetCommResult_InitWinSockErr    -4      // 初始化WinSock错误
#define		XNetCommResult_XNetCommunErr     -5      // XNet通讯错误
#define		XNetCommResult_SetComPortErr     -6      // 设置端口号129错误
#define		XNetCommResult_Success            0      // XNet串口通讯成功

//XNet数据类型为读，或写
#define     XNET_READ_OP                      1      // 读
#define     XNET_WRITE_OP                     2      // 写
#define     XNET_AUTO_FIND                    3      // 自动寻找

//XNet数据类型为寄存器，或线圈
#define     XNET_AUTO_OP                      0      // 自动寻找
#define     XNET_REGS_OP                      1      // 寄存器类型
#define     XNET_COILS_OP                     2      // 线圈类型

//使用组帧时，XNet数据类型的表示
#define     XNET_COMBINE(bitlen, type, addr) ((((int)(bitlen)) << 24) | (((int)(type)) << 16) | (addr))
#define     XNET_BITLEN(combine)             (((combine) >> 24) & 0xFF)
#define     XNET_TYPE(combine)               (((combine) >> 16) & 0xFF)
#define     XNET_ADDR(combine)               ((combine) & 0xFFFF)

/** ******************************** 版本号***************************** **/
// 第一个字节表示大版本	（增加大型功能时该值增加）
// 第二个字节表示投产版本	（每次投产时，该值增加，后面两个字节清零，表示测试部门已经验证过，属于稳定版本）
// 第三个字节表示测试版本	（每次给测试部门时，该值增加，表示研发内部测试通过，后面字节清零）
// 第四个字节表示代码上传版本	（每次上传版本增加1）
#define     PLATFORM_CFG_VERSION             "02.00.02.03"

/** 数据的宏定义 **/
#define     CHAR_ARRAY_PATH_NUM              100     // 启动XNet服务使用的数组大小
#define     REGS_BUFFER_LENGTH               20000   // 寄存器缓冲区数组大小
#define     REGS_NUM_SIZE                    9200    // 寄存器长度
#define     COILS_BUFFER_LENGTH              9000    // 线圈缓冲区数组大小
#define     COILS_NUM_SIZE                   65535   // 线圈长度
#define     XNET_BITLEN_M                    1       // 数据类型为线圈
#define     XNET_BITLEN_MAXLEN_M             640     // 类型为线圈时，组帧最大长度为640
#define     XNET_BITLEN_D                    16      // 数据类型为寄存器
#define     XNET_BITLEN_MAXLEN_D             40      // 类型为寄存器时，组帧最大长度为40
#define     XNETREAD_MAXLEN                  400     // XNETRead函数时，若数据类型为寄存器，则最大读取长度400
#define     XNETWRITE_MAXLEN                 5000    // XNETWrite函数时，若数据类型为寄存器，则最大写入长度5000
#define     XNET_RESULT_0                    0       // XNET通讯成功
#define     XNET_RESULT_1                    1       // XNET通讯错误1
#define     XNET_RESULT_2                    2       // XNET通讯错误2
#define     XNET_RESULT_3                    3       // XNET通讯错误3
#define     XNET_RESULT_100                  100     // XNET通讯不成功100
#define     XNet_RESULT_PARA_ERROR           -1      // XNET通讯传入参数错误

#pragma once
//XNet设备类型
typedef enum
{
    PLC_XD = 0,
    PLC_XE,
    TouchWin,
    WBox,
    _4GBox,
    COBox,
    ABox
}XNetDevice;

//使用WriteCoils、ReadCoils函数时的线圈类型
typedef enum
{
    XNet_X = 1,
    XNet_Y = 2,
    XNet_M = 3,
    XNet_S = 4,
    XNet_T = 5,
    XNet_C = 6,
    XNet_ET = 7,
    XNet_HM = 8,
    XNet_HS = 9,
    XNet_HT = 10,
    XNet_HC = 11,
    XNet_HSC = 12,
    XNet_SM = 13,
    XNet_SSM = 14,
    XNet_TG = 15,
    XNet_HTG = 16,
    XNet_PF = 17,
    XNet_SEM = 18,
    XNet_STG = 19,
    XNet_HSTG = 20,
}CoilType;

//使用WriteRegs、ReadRegs函数时的寄存器类型
typedef enum
{
    XNet_D = 128,
    XNet_TD = 129,
    XNet_CD = 130,
    XNet_SD = 131,
    XNet_ED = 132,
    XNet_ETD = 133,
    XNet_ID = 134,
    XNet_QD = 135,
    XNet_HD = 136,
    XNet_HTD = 137,
    XNet_HCD = 138,
    XNet_HSCD = 139,
    XNet_HSD = 140,
    XNet_FD = 141,
    XNet_SFD = 142,
    XNet_SSFD = 143,
    XNet_SSD = 144,
    XNet_FS = 145,
}RegType;

//使用XNETRead、XNETWrite函数时的线圈类型
typedef enum
{
    XNET_X = XNET_COMBINE(1, 1, 0x0000),
    XNET_Y = XNET_COMBINE(1, 2, 0x0000),
    XNET_M = XNET_COMBINE(1, 3, 0x0000),
    XNET_S = XNET_COMBINE(1, 4, 0x0000),
    XNET_T = XNET_COMBINE(1, 5, 0x0000),
    XNET_C = XNET_COMBINE(1, 6, 0x0000),
    XNET_ET = XNET_COMBINE(1, 7, 0x0000),
    XNET_HM = XNET_COMBINE(1, 8, 0x0000),
    XNET_HS = XNET_COMBINE(1, 9, 0x0000),
    XNET_HT = XNET_COMBINE(1, 10, 0x0000),
    XNET_HC = XNET_COMBINE(1, 11, 0x0000),
    XNET_HSC = XNET_COMBINE(1, 12, 0x0000),
    XNET_SM = XNET_COMBINE(1, 13, 0x0000),
    XNET_SSM = XNET_COMBINE(1, 14, 0x0000),
    XNET_TG = XNET_COMBINE(1, 15, 0x0000),
    XNET_HTG = XNET_COMBINE(1, 16, 0x0000),
    XNET_PF = XNET_COMBINE(1, 17, 0x0000),
    XNET_SEM = XNET_COMBINE(1, 18, 0x0000),
}Custom_CoilsType;

//使用XNETRead、XNETWrite函数时的寄存器类型
typedef enum
{
    XNET_D = XNET_COMBINE(16, 0x80, 0x0000),
    XNET_TD = XNET_COMBINE(16, 0x81, 0x0000),
    XNET_CD = XNET_COMBINE(16, 0x82, 0x0000),
    XNET_SD = XNET_COMBINE(16, 0x83, 0x0000),
    XNET_ED = XNET_COMBINE(16, 0x84, 0x0000),
    XNET_ETD = XNET_COMBINE(16, 0x85, 0x0000),
    XNET_ID = XNET_COMBINE(16, 0x86, 0x0000),
    XNET_QD = XNET_COMBINE(16, 0x87, 0x0000),
    XNET_HD = XNET_COMBINE(16, 0x88, 0x0000),
    XNET_HTD = XNET_COMBINE(16, 0x89, 0x0000),
    XNET_HCD = XNET_COMBINE(16, 0x8A, 0x0000),
    XNET_HSCD = XNET_COMBINE(16, 0x8B, 0x0000),
    XNET_HSD = XNET_COMBINE(16, 0x8C, 0x0000),
    XNET_FD = XNET_COMBINE(16, 0x8D, 0x0000),
    XNET_SFD = XNET_COMBINE(16, 0x8E, 0x0000),
    XNET_SSFD = XNET_COMBINE(16, 0x8F, 0x0000),
    XNET_SSD = XNET_COMBINE(16, 0x90, 0x0000),
    XNET_FS = XNET_COMBINE(16, 0x91, 0x0000),
}Custom_RegsType;

// 使用XNetRead、XNetWrite组帧所需数据类型
typedef X_NETSHARED_EXPORT struct
{
    int             nReadWrite;          // 数据的读写类型：读/写
    bool            bHighPrio;           // 是否是高优先级
    bool            bSkip;               // 是否跳过，默认为false
    uchar           uchStation;          // 站号，为默认值
    int             nNet;                // 网口号，为默认值
    int             nID;                 // ID号，为默认值
    int             nObjectCombine;      // 数据类型：M、D、HD等
    int             nObjectNo;           // 首地址：如HD100的100
    int             nObjectLen;          // 数据长度2：如HD100:2 的2
    void            *Buffer;             // 读取、写入数据的指针数据：例short[2]
    int             nResult;             // 读取成功标志
} XNET_SERVICE;

/** 封装类名为X_Net，用户使用时，需使用类名实例化一个对象，通过对象调用类中的函数；例：
X_Net service；
service.函数（）；**/

class X_NETSHARED_EXPORT X_Net
{
public:
    X_Net();
    ~X_Net();

    /** 设置通讯IP地址，进行网口通讯
    Function    :
    Calls       : SetRetryTimes(1)，TestXNetService()，AdderLink(ttyNo)，TestCommunication()
    Called By   : 外部程序调用
    Input       : char *ttyNo
    Output      : char *ttyNo
    Return      : int，返回操作是否成功的标志
    Note        : 外部程序必须调用此函数，且返回值为0时，才可以使用其他函数
    *************************************************/
    XNetCommResult SetCommunication(char *ttyNo);


    /** 重新连接通讯服务
    Function    :
    Calls       : TestXNetService()，TestCommunication()
    Called By   : 外部程序调用
    Input       :
    Output      :
    Return      : int，返回操作是否成功的标志
    Note        : 必须在通讯意外断开的情况下调用
    *************************************************/
    XNetCommResult ReCommunication();


    /** 关闭XNet通讯服务
    Function    :
    Calls       : TestXNetService()，TestCommunication()
    Called By   : 外部程序调用，或者TestCommunication()
    Input       :
    Output      :
    Return      : int，返回操作是否成功的标志
    Note        : 当通讯不成功时，关闭端口服务，防止程序陷入死循环，导致软件死机
    *************************************************/
    XNetCommResult CloseXNetWindows();


    /** 写寄存器
    Function    :
    Calls       : SetSendBuf()，LibXNetRegsToBuf()，CloseXNetSocket()
    Called By   : 外部程序调用
    Input       : XNetByte _regType, int _offSet, int _num, short* _writeRegs
    Output      : short* _writeRegs
    Return      : int，返回操作是否成功的标志
    Note        : 用户提供一个short数组缓存，用于传入XNet16位寄存器的值（需要将参数类型设置正确）
                  寄存器的类型数据（已在上文给出）、寄存器的首地址、寄存器的数量、保存寄存器数据的指针
    *************************************************/
    XNetCommResult WriteRegs(XNetByte _regType, int _offSet, int _num, short* _writeRegs);


    /** 读寄存器
    Function    :
    Calls       : SetSendBuf()，LibXNetRegsToBuf()，CloseXNetSocket()
    Called By   : 外部程序调用
    Input       : XNetByte _regType, int _offSet, int _num, short* _readRegs
    Output      : short* _readRegs
    Return      : int，返回操作是否成功的标志
    Note        : 用户提供一个short数组缓存，用于获取XNet16位寄存器的值（需要将参数类型设置正确）
                  寄存器的类型数据（已在上文给出）、寄存器的首地址、寄存器的数量、保存寄存器数据的指针
    *************************************************/
    XNetCommResult ReadRegs(XNetByte _regType, int _offSet, int _num, short* _readRegs);


    /** 写线圈
    Function    :
    Calls       : SetSendBuf()，LibXNetCoilsToBuf()，CloseXNetSocket()
    Called By   : 外部程序调用
    Input       : XNetByte _coilType, int _offSet, int _num, char* _writeCoils
    Output      : short* _writeCoils
    Return      : int，返回操作是否成功的标志
    Note        : 用户提供一个char数组缓存，用于传入XNet线圈的值，0为off，1为on（需要将参数类型设置正确）
                  线圈的类型数据（已在上文给出）、线圈的首地址、线圈的数量、保存线圈数据的指针
    *************************************************/
    XNetCommResult WriteCoils(XNetByte _coilType, int _offSet, int _num, char* _writeCoils);


    /** 读线圈
    Function    :
    Calls       : SetSendBuf()，LibXNetCoilsToBuf()，CloseXNetSocket()
    Called By   : 外部程序调用
    Input       : XNetByte _coilType, int _offSet, int _num, XNetByte* _readCoil
    Output      : short* _readCoil
    Return      : int，返回操作是否成功的标志
    Note        : 用户提供一个short数组缓存，用于传入XNet线圈的值，0为off，1为on（需要将参数类型设置正确）
                  线圈的类型数据（已在上文给出）、线圈的首地址、线圈的数量、保存线圈数据的指针
    *************************************************/
    XNetCommResult ReadCoils(XNetByte _coilType, int _offSet, int _num, XNetByte* _readCoil);


    /** 读取寄存器或线圈
    Function    :
    Calls       : SetSendBuf()，LibXNetRegsToBuf()，LibXNetCoilsToBuf()，CloseXNetSocket()
    Called By   : 外部程序调用
    Input       : int nObjectCombine, int nObjectNo, int nObjectLen, void *Buffer, int &Result
    Output      : void *Buffer, int &Result
    Return      :
    Note        : 用户提供一个void型数组缓存，用于获取XNet数据（需要将参数类型设置正确）
                  类型数据（已在上文给出）、首地址、长度、保存数据的指针、读取成功状态
    *************************************************/
    void XNETRead(int nObjectCombine, int nObjectNo, int nObjectLen, void *Buffer, int &Result);


    /** 写寄存器或线圈
    Function    :
    Calls       : SetSendBuf()，LibXNetRegsToBuf()，LibXNetCoilsToBuf()，CloseXNetSocket()
    Called By   : 外部程序调用
    Input       : int nObjectCombine, int nObjectNo, int nObjectLen, void *Buffer, int &Result
    Output      : void *Buffer, int &Result
    Return      :
    Note        : 用户提供一个void型数组缓存，用于写入XNet数据（需要将参数类型设置正确）
                  类型数据（已在上文给出）、首地址、长度、保存数据的指针、写入成功状态
    *************************************************/
    void XNETWrite(int nObjectCombine, int nObjectNo, int nObjectLen, void *Buffer, int &Result);


    /** 数据添加列表
    Function    :
    Calls       :
    Called By   : 外部程序调用
    Input       : XNET_SERVICE Req
    Output      :
    Return      : int，返回操作是否成功的标志
    Note        : 添加列表,将需要读取的XNET_SERVICE型数据插入到列表reqlist中，加快读取方式
                  调用GroupReqs()函数前，必须调用此函数
    *************************************************/
    XNetCommResult AddRequest(XNET_SERVICE Req);


    /** 数据组帧
    Function    :
    Calls       : XNETRead()
    Called By   : 外部程序调用
    Input       :
    Output      :
    Return      : int，返回操作是否成功的标志
    Note        : 将列表中的数据进行组帧，将组帧后、并读取下位机内容的数据放入GroupReqslist中，
                  调用此函数前需保证reqlist列表中存在数据
    *************************************************/
    XNetCommResult GroupReqs();


    /** 清除GroupReqslist的内容
    Function    :
    Calls       :
    Called By   : 外部程序调用
    Input       :
    Output      :
    Return      : int，返回操作是否成功的标志
    Note        : 由于列表中存在指针数据，需要将指针数据内存释放，之后将Vector数组内存释放
    *************************************************/
    XNetCommResult ClearGroupList();


    /** 清除reqlist的内容,以便重新插入数据
    Function    :
    Calls       :
    Called By   : 外部程序调用
    Input       :
    Output      :
    Return      : int，返回操作是否成功的标志
    Note        : 由于列表中存在指针数据，需要将指针数据内存释放，之后将Vector数组内存释放
    *************************************************/
    XNetCommResult ClearReqList();


    /** 返回reqlist内的数据
    Called By   : 外部程序调用
    Return      : vector<XNET_SERVICE *>，返回vecReqList的数据
    Note        : 返回列表数据
    *************************************************/
    vector<XNET_SERVICE *> ReturnReqList();


    /** 返回GroupReqslist内的数据
    Called By   : 外部程序调用
    Return      : vector<XNET_SERVICE *>，返回vecGroupReqsList的数据
    Note        : 返回列表数据
    *************************************************/
    vector<XNET_SERVICE *> ReturnGroupList();


    /** 返回版本号（调用此函数可查看版本号）
    Called By   : 外部程序调用
    Return      : string，返回版本号
    Note        : 用户可查看当前版本号
    *************************************************/
    string ReturnVersion();

private:
    /**  下列函数基本无需外部调用，已被SetCommunication（）函数自己调用 **/


    /** 设置尝试次数
    Called By   : SetCommunication()
    Input       : int times
    Note        : 设置通讯重试的次数
    *************************************************/
    void SetRetryTimes(int times);


    /** 启动XNet服务
    Called By   : TestXNetService()
    Return      : int，返回操作是否成功的标志
    Note        : 此静态方法整个程序只需要调用一次，若失败，向外部返回-3
    *************************************************/
    static XNetCommResult StartXNetWindows();


    /** 初始化WindowsSocket
    Called By   : TestXNetService()
    Return      : int，返回操作是否成功的标志
    Note        : 若失败，向外部返回-4
    *************************************************/
    static XNetCommResult WinSockInit();


    /** 和XNet服务建立连接
    Called By   : TestXNetService()
    Return      : int，返回操作是否成功的标志
    Note        : 若失败，向外部返回-5
    *************************************************/
    XNetCommResult XNetCommunication();


    /** 打开串口
    Called By   : TestXNetService()
    Input       : XNetByte _com
    Return      : int，返回操作是否成功的标志
    Note        : （网口也需配置一次，网口_com填129），若失败，向外部返回-6
    *************************************************/
    XNetCommResult SetComPort(XNetByte _com);


    /** 直接指定设备地址
    Called By   : SetCommunication()
    Input       : const char* _ipStr
    Output      : const char* _ipStr
    Note        : 将IP地址的后两位赋给usNet，usStation，准备在通讯过程中的使用
    *************************************************/
    void AdderLink(const char* _ipStr);


    /** 测试XNet服务是否启动成功
    Function    :
    Calls       : StartXNetWindows()，WinSockInit()，XNetCommunication()，SetComPort(129)
    Called By   : SetCommunication()，ReCommunication()
    Return      : int，返回操作是否成功的标志
    Note        : 将重复代码整合到一个函数内
    *************************************************/
    XNetCommResult TestXNetService();


    /** 测试XNet是否成功通讯
    Function    :
    Calls       : ReadCoils()
    Called By   : SetCommunication()，ReCommunication()
    Return      : int，返回操作是否成功的标志
    Note        : 将重复代码整合到一个函数内
    *************************************************/
    XNetCommResult TestCommunication();


    /** 关闭Socket服务
    Return      : int，返回操作是否成功的标志
    Note        : 将重复代码整合到一个函数内
    *************************************************/
    void CloseXNetSocket();


    /** 对读写函数中的数组SendBuf进行处理,当读写函数中的缓冲区指针为unsigned char数组时，将其强制转换为char*
    Called By   : WriteRegs,ReadRegs,WriteCoils,ReadCoils,XNETRead,XNETWrite
    Input       : int len, XNetByte _regType, int _offSet, int _num, int ReadWrite, int reg_coils, char *sendBuf
    Output      : char *sendBuf
    Return      : int，返回操作是否成功的标志
    Note        : 将重复代码整合到一个函数内
    *************************************************/
    void SetSendBuf(int len, XNetByte _regType, int _offSet, int _num, int ReadWrite, int reg_coils, char *sendBuf);


private:

    /** 所有添加数据的列表 **/
    vector<XNET_SERVICE *>vecReqList;

    /** 组帧后的数据列表
        组帧后，通过通讯从PLC读取数据内容，将读取内容后XNET_SERVICE型数据存放的列表，用户可直接提取此列表中的数据**/
    vector<XNET_SERVICE *>vecGroupReqsList;

    /** XNet通讯过程中所需变量 **/
    int nSock;
    int nRetry;
    uchar uchGroupBuffer[131072];
    XNetByte uchType;
    XNetByte uchSerial;
    XNetByte uchModel;
    unsigned char  uchCom;
    unsigned short usNet;
    unsigned short usStation;


    /** XNet服务固定函数，无需改动 **/
    void LibXNetRegsToBuf(short* _regs, unsigned char* _buf, int _num);
    void LibXNetBufToRegs(unsigned char* _buf, unsigned short* _regs, int _num);
    void LibXNetCoilsToBuf(char* _coils, unsigned char* _buf, int _num);
    static void LibXNetBufToCoils(char* _buf, XNetByte *_coils, XNetByte _startBit, int _num);
    unsigned short XNetBytetoWord(XNetByte high, XNetByte low);
    void CopyBits(int src_bitoffset, char *src, int dst_bitoffset, char *dst, int num);
    void AdderLink(UInt16 _net, UInt16 _station);
    XNetCommResult FindDevice(XNetByte _comPort, XNetByte _type, XNetByte _serial, XNetByte _model);
    XNetCommResult FindDevice(XNetByte _comPort, XNetDevice _dev);
    XNetCommResult FindDevice(XNetByte _comPort, const char* _id);
    XNetCommResult ReleaseComPort(XNetByte _com);

};




#endif // X_NET_H
