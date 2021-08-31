# XNet服务说明文档（c++） #
# 一、宏定义 #
	(一)、XNet服务端口
		#define 	ServicePort	2323
		#define     ServiceIp "127.0.0.1"
	
	(二)、操作结果（仅告知操作是否成功）
		#define		XNetCommResult		   int
		#define		XNetCommResult_Err		-1
		#define		XNetCommResult_Success	 0
	
	(三)、XNet数据类型为读，或写
		#define     XNET_READ_OP    1
		#define     XNET_WRITE_OP   2
		#define     XNET_AUTO_FIND  3
	
	(四)、XNet数据类型为寄存器，或线圈
		#define     XNET_AUTO_OP    0
		#define     XNET_REGS_OP    1
		#define     XNET_COILS_OP   2
	
	(五)、使用组帧时，XNet数据类型的表示
		#define     XNET_COMBINE(bitlen, type, addr)      ((((int)(bitlen)) << 24) | (((int)(type)) << 16) | (addr))
		#define     XNET_BITLEN(combine)                  (((combine) >> 24) & 0xFF)
		#define     XNET_TYPE(combine)                    (((combine) >> 16) & 0xFF)
		#define     XNET_ADDR(combine)                    ((combine) & 0xFFFF)

	(六)、版本号
		#define PLATFORM_CFG_VERSION	"02.00.02.01" //20201106-将重复代码提取并优化 

	(七)、数据的宏定义
		#define     REGS_BUFFER_LENGTH      20000   // 寄存器缓冲区数组大小
		#define     REGS_NUM_SIZE           9200    // 寄存器长度
		#define     COILS_BUFFER_LENGTH     9000    // 线圈缓冲区数组大小
		#define     COILS_NUM_SIZE          65535   // 线圈长度
		#define     XNET_BITLEN_M           1       // 数据类型为线圈
		#define     XNET_BITLEN_MAXLEN_M    640     // 类型为线圈时，组帧最大长度为640
		#define     XNET_BITLEN_D           16      // 数据类型为寄存器
		#define     XNET_BITLEN_MAXLEN_D    40      // 类型为寄存器时，组帧最大长度为40
		#define     XNETREAD_MAXLEN         400     // XNETRead函数时，若数据类型为寄存器，则最大读取长度400
		#define     XNETWRITE_MAXLEN        5000    // XNETWrite函数时，若数据类型为寄存器，则最大写入长度5000
		#define     XNET_RESULT_0           0       // XNET通讯成功
		#define     XNET_RESULT_1           1       // XNET通讯错误1
		#define     XNET_RESULT_2           2       // XNET通讯错误2
		#define     XNET_RESULT_3           3       // XNET通讯错误3


# 二、类型定义 
	(一)、程序中使用的数据类型
		typedef unsigned char  XNetByte;   
		typedef unsigned char  uchar;
		typedef unsigned short UInt16; 

	(二)、XNet设备类型
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

	(三)、线圈类型
		1、使用WriteCoils、ReadCoils函数时的线圈类型
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
		2、使用XNETRead、XNETWrite函数时的线圈类型
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

	(四)、寄存器类型
		1、使用WriteRegs、ReadRegs函数时的寄存器类型
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
		2、使用XNETRead、XNETWrite函数时的寄存器类型
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

	(五)、组帧时所需的数据类型
		typedef X_NETSHARED_EXPORT struct
		{
		    int             rd_wr;          // 数据的读写类型：读/写
		    bool            high_prio;      // 是否是高优先级
		    bool            skip;           // 是否跳过，默认为false
		    uchar           station;        // 站号，为默认值
		    int             net;            // 网口号，为默认值
		    int             id;             // ID号，为默认值
		    int             object_combine; // 数据类型：M、D、HD等
		    int             object_no;      // 首地址：如HD100的100
		    int             object_len;     // 数据长度2：如HD100:2 的2
		    void            *buffer;        // 读取、写入数据的指针数据：例short[2]
		    int             result;         // 读取成功标志
		} XNET_SERVICE;

# 三、公共接口函数 
	封装类名为X_Net，用户使用时，需使用类名实例化一个对象，通过对象调用类中的函数；例：
	X_Net service；
	service.接口函数（）
	
	(一)、设置XNet通讯服务 
		1、设置通讯IP地址,将IP地址传入（传递参数类型char*）：若通讯成功，返回0；若失败，返回-1
			XNetCommResult SetCommunication(char *ttyNo);
		示例：
		{
			int i = service.SetCommunication（“192.168.60.16”）；
			if （ i == 0 )
				qDebug() << "通讯成功";
			else
				qDebug() << "通讯失败";
		}
		
		2、重连XNet服务:若通讯成功，则返回0；否则返回-1
			XNetCommResult ReCommunication();
		示例：
		{
			int i = service.ReCommunication()
			if （ i == 0 )
				qDebug() << "重连成功";
			else
				qDebug() << "重连失败";
		}
		
		3、关闭XNet服务，成功返回0（调用此函数，关闭XNet服务）
		  	XNetCommResult CloseXNetWindows();

	（二）、读写寄存器 
	用户提供一个short数组缓存，用于传入或获取XNet16位寄存器的值；
    参数设置分别为：寄存器的类型数据（已在上文给出）、寄存器的首地址、寄存器的数量、保存寄存器数据的指针
		1、写寄存器：	
		    XNetCommResult WriteRegs(XNetByte _regType, int _offSet, int _num, short* _writeRegs);
		示例：将数组中的值1、2、3、4写入HD0:4中
			{
				short a[4] = {1,2,3,4};
				int i = service.WriteRegs((XNetByte)XNet_HD ,0,4,a);
			其中：(XNetByte)XNet_HD：为寄存器HD的类型，在第二章第三节已定义
								 0：为寄存器数组HD0:4的首地址
								 4：为寄存器数组HD0:4的长度
								 a: 为写入寄存器数据的指针
								 i: 0为读取成功
			} 
		2、读寄存器
		    XNetCommResult ReadRegs(XNetByte _regType, int _offSet, int _num, short* _readRegs);
		示例：将从HD0:4中读取数据到数组中
			{
				short a[4] = {0};
				int i = service.ReadRegs((XNetByte)XNet_HD ,0,4,a);
			其中：(XNetByte)XNet_HD：为寄存器HD的类型，在第二章第三节已定义
								 0：为寄存器数组HD0:4的首地址
								 4：为寄存器数组HD0:4的长度
								 a: 为保存寄存器数据的指针
								 i: 0为读取成功
			} 

	(三)、读写线圈
	用户提供一个char数组缓存，用于传入或获取XNet线圈状态，0为off，1为on；
    参数设置分别为：寄存器的类型数据（已在上文给出）、寄存器的首地址、寄存器的数量、保存寄存器数据的指针
		1、写线圈：	
		    XNetCommResult WriteCoils(XNetByte _coilType, int _offSet, int _num, char* _writeCoils);
		示例：将数组中的值写入M0:4中
			{
				char a[4] = {1,1,1,1};
				int i = service.WriteCoils((XNetByte)XNet_M,0,4,a);
			其中：(XNetByte)XNet_M ：为寄存器M 的类型，在第二章第三节已定义
								 0：为寄存器数组M0:4的首地址
								 4：为寄存器数组M0:4的长度
								 a: 为写入寄存器数据的指针
								 i: 0为读取成功
			} 
		2、读线圈
		    XNetCommResult ReadCoils(XNetByte _coilType, int _offSet, int _num, XNetByte* _readCoil);
		示例：将从M0:4中读取数据到数组中
			{
				XNetByte a[4] = {0};
				int i = service.ReadCoils((XNetByte)XNet_M,0,4,a);
			其中：(XNetByte)XNet_M ：为寄存器M的类型，在第二章第三节已定义
								 0：为寄存器数组M0:4的首地址
								 4：为寄存器数组M0:4的长度
								 a: 为保存寄存器数据的指针
								 i: 0为读取成功
			} 

	(四)、使用组帧读取寄存器、线圈 
	需要提供寄存器的类型object_combine，首地址object_no，寄存器长度object_len，保存数据的缓冲区*buffer，读取成功状态result

		1、数据写入	
		    void XNETWrite(int object_combine, int object_no, int object_len, void *Buffer, int &result);
		示例：将数据写入HD0:2中
			{
				int i= 100；
				int result = 0；
				service.XNETWrite( XNET_HD,0,2,&i,result);
			其中：		XNET_HD ：为寄存器HD 的类型，在第二章第四节已定义
							   0：为寄存器数组HD0:2的首地址
							   2：为寄存器数组HD0:2的长度
							   i: 为写入寄存器数据的指针
						  result: 0为读取成功
			} 
		2、数据读取
		    void XNETRead(int object_combine, int object_no, int object_len, void *Buffer, int &result);
		示例：从HD0:2中读取数据
			{
				int i= 0；
				int result = 0；
				service.XNETWrite( XNET_HD,0,2,&i,result);
			其中：		XNET_HD ：为寄存器HD 的类型，在第二章第四节已定义
							   0：为寄存器数组HD0:2的首地址
							   2：为寄存器数组HD0:2的长度
							   i: 为保存寄存器数据的指针
						  result: 0为读取成功
			} 

	(五)、组帧所需函数（当通讯数据过多时，可将数据组帧后读取 ）
		1、添加列表,将需要读取的XNET_SERVICE型数据插入到列表req_list中，开始排列 
			XNetCommResult AddRequest(XNET_SERVICE req);
		将XNET_SERVICE 型数据传入此函数，在req_list中一次排序，若成功，返回0，若失败，返回-1
	
		2、将列表中的数据进行组帧，将组帧后、并读取下位机内容的数据放入GroupReqs_list中
			XNetCommResult GroupReqs();
		完成向req_list列表插入数据的操作后，调用GroupReqs()函数进行组帧，若成功，返回0，若失败，返回-1
	
		3、清除GroupReqs_list的内容
	    	XNetCommResult ClearGroupList()，若成功，返回0，若失败，返回-1
	
	    4、清除req_list的内容,以便重新插入数据 
	    	XNetCommResult ClearReqList()，若成功，返回0，若失败，返回-1
	
		5、返回req_list内的数据，返回值类型为vector<XNET_SERVICE *>
	    	vector<XNET_SERVICE *> ReturnReqList();
	
	    6、返回GroupReqs_list内的数据，返回值类型为vector<XNET_SERVICE *>
	    	vector<XNET_SERVICE *> ReturnGroupList();
	
	    7、返回版本号（调用此函数可查看版本号）
	    	string ReturnVersion();


			