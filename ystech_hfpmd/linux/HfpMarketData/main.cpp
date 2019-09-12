#include"../logger_instance/logger_instance.h"
#include<iostream>
#include"../api/HFPTradingClient.h"
#include"../misc/common.h"
#include"../misc/timer.h"
#include<time.h>
#include"../misc/nano_executor.h"
#include"../tars/util.h"
#include <stdarg.h>
#include<math.h>
#include"../misc/auto_file.h"
#include"../misc/options.h"

#define INT_TO_FLOAT(TYPE,VALUE) ((TYPE) (VALUE));
#define  PRICE_UNIT 100.0;
#define TIME_BUFFER 256
#define FILE_PATH_LENGTH 1024
#define MEM_BUFFER 1024
#define PRICE_EPS 0.0000000001
#define HFP_FILE_NAME 256



using misc::options;
using misc::cstr_t;
using misc::options;

RegisterOption(HFP_ID, "", "HFP_ID")
RegisterOption(HFP_LICENSE, "", "HFP_LICENSE")
RegisterOption(HFP_FRONT_IP, "", "HFP_FRONT_IP")
RegisterOption(HFP_FRONT_PORT, "", "HFP_FRONT_PORT")


using namespace std;
using namespace hfp;
using namespace std::placeholders;
using namespace nano;

char Delim = '\n';
char path[FILE_PATH_LENGTH] = {0};
char opet[MEM_BUFFER] = {0};


//---------------------------------------------------------------------------登录配置信息-------------------------------------------------------------------------------------------------------------------------------------
#define DEVE_ENV
#ifdef DEVE_ENV
const char ID[] = "A12BDE805AB8C68C10F99DCF48F5B1BD";
const char LICENSE[] = "EJOrD6LZPZCjCMHcOIqqCJKoBM9bE68jCJ4rD31cOJCsOZXbwB2YvRI7vfsmHqT6Kp8mQJLjRMbbQaLFE7TEPt5XKJPlHb0uP6mmUNTlKZStJJOsE3PgJNH9TdKmDs9KCMjqCL1nIaHnRJSnTainDMLkDdTrDsCsUMLOJKTKDMetPt1NC4WmR6LOQ7CuIN0tH4bPRKnrC6z9TJT2L35GOsSpQ4CpGqbhKNGoD31cJsHDDaaoKd1rCK99CdWtLJCmCpSmGMTLD55vCJ8nOZT1KZLBI5X8IJ5ODc4mOMrEMJ5gD6TPDpSn";
const char FRONT_IP[] = "58.215.39.211";
const int FRONT_PORT = 59000;
#endif
//---------------------------------------------------------------------------登录配置信息-------------------------------------------------------------------------------------------------------------------------------------

using execute_type = single_thread_executor<512, false>;
execute_type execute(20, 100, tars::util::micro_spin());
os::auto_file af;
class  HfpData
{
public:
	HfpData(quotation_data& data)
	{
		raw_time = data.datetime;
		datetime = data.datetime;
		pLocalTime = localtime(&datetime);
		sprintf(fdatatime, "%02d:%02d:%02d", pLocalTime->tm_hour, pLocalTime->tm_min, pLocalTime->tm_sec);


		buy_price[0] = (float)data.buy_price[0] / PRICE_UNIT;
		buy_price[1] = (float)data.buy_price[1] / PRICE_UNIT;
		buy_price[2] = (float)data.buy_price[2] / PRICE_UNIT;
		buy_price[3] = (float)data.buy_price[3] / PRICE_UNIT;
		buy_price[4] = (float)data.buy_price[4] / PRICE_UNIT;
		buy_volum[0] = (float)data.buy_volume[0];
		buy_volum[1] = (float)data.buy_volume[1];
		buy_volum[2] = (float)data.buy_volume[2];
		buy_volum[3] = (float)data.buy_volume[3];
		buy_volum[4] = (float)data.buy_volume[4];
		sell_price[0] = (float)data.sell_price[0] / PRICE_UNIT;
		sell_price[1] = (float)data.sell_price[1] / PRICE_UNIT;
		sell_price[2] = (float)data.sell_price[2] / PRICE_UNIT;
		sell_price[3] = (float)data.sell_price[3] / PRICE_UNIT;
		sell_price[4] = (float)data.sell_price[4] / PRICE_UNIT;
		sell_volum[0] = data.sell_volume[0]; sell_volum[1] = data.sell_volume[1]; sell_volum[2] = data.sell_volume[2];
		sell_volum[3] = data.sell_volume[3]; sell_volum[4] = data.sell_volume[4];
		seq = data.seq; 
		strcpy(contract_id, data.contract_id);
		strcpy(marketId, data.marketId); open = data.open / PRICE_UNIT;
		high = data.high / PRICE_UNIT; low = data.low / PRICE_UNIT; _new = data._new / PRICE_UNIT; last_close = data.last_close / PRICE_UNIT; average = data.average / PRICE_UNIT; total_volume = data.total_volume;
		total_amount = data.total_amount; subs_volume = data.subs_volume; cur_volume = data.cur_volume; type = data.type;
		down_limit = data.down_limit / PRICE_UNIT; up_limit = data.up_limit / PRICE_UNIT; balance_price = data.balance_price / PRICE_UNIT; updown_base = data.updown_base / PRICE_UNIT;
		last_subs_volume = data.last_subs_volume;
	}
public:
	long long seq;//流水号          
	//int datetime;//发生时间(秒)      
	//char contract_id[10];//合约编码    
	char contract_id[10];
	char marketId[10];//市场编码
	int raw_time;
	//char * marketId;
	double open;//开盘价                       
	double high;//最高价                          
	double low;//最低价                             
	double _new;//最新价                              
	double last_close;//昨日收盘价                      
	double average;//均价                                 
	long long total_volume;//总成交量                   
	long long total_amount;//总成交额                        
	long long subs_volume;//总持仓量                             
	int cur_volume;//现量                                           
	int type;//行情类型1-集合竞价申报，2-集合竞价成交，3-申报，4-成交  
	double down_limit;//最低限价                                            
	double up_limit;//最高限价                                                
	double balance_price;//盈亏计算价                                           
	double updown_base;//涨跌幅基准价                                             
	int last_subs_volume;//昨日持仓     
	double buy_price[5] = { 0 };
	double sell_price[5] = { 0 };
	int buy_volum[5] = { 0 };
	int sell_volum[5] = { 0 };
	char fdatatime[10] = { 0 };
	struct tm* pLocalTime;
	time_t datetime;
	int EventTime;

};

class HfpMarketData_Getter	//市场信息获取器
{
public:

	HfpMarketData_Getter():pfile(NULL){};
	static HfpMarketData_Getter * Instance;
public:

	void OpenFile(string path)
	{
		if (af.file_open(path, "a+"))
			pfile = af.get_fp();
	}
	void PrintHfpTitle(string title)
	{
		if (pfile);
		fprintf(pfile, "%s", title.c_str());
	}
	void operator() (HfpData &fpdata, unsigned long long &start_mill)
	{
		unsigned long long write_mill = os::nano_time();
		fprintf(pfile, "%lld, %s,%d,%s,"
			"%s, %0.4f,%0.4f, %0.4f,"
			"%0.4f, %0.4f, %0.4f, %lld,"
			"%lld, %lld, %d,%d,"
			"%0.4f,%0.4f,%0.4f,%0.4f,"
			"%d,%0.4f,%0.4f,%0.4f,"
			"%0.4f,%0.4f,%d,%d,"
			"%d,%d,%d,%0.4f,"
			"%0.4f,%0.4f,%0.4f,%0.4f,"
			"%d,%d,%d,%d,"
			"%d,%llu,%llu,%0.4f\n",
			fpdata.seq, fpdata.fdatatime, fpdata.raw_time, fpdata.contract_id,
			fpdata.marketId, fpdata.open, fpdata.high, fpdata.low,
			fpdata._new, fpdata.last_close, fpdata.average, fpdata.total_volume,
			fpdata.total_amount, fpdata.subs_volume, fpdata.cur_volume, fpdata.type,
			fpdata.down_limit, fpdata.up_limit, fpdata.balance_price, fpdata.updown_base,
			fpdata.last_subs_volume, fpdata.buy_price[0], fpdata.buy_price[1], fpdata.buy_price[2],
			fpdata.buy_price[3], fpdata.buy_price[4], fpdata.buy_volum[0], fpdata.buy_volum[1],
			fpdata.buy_volum[2], fpdata.buy_volum[3], fpdata.buy_volum[4], fpdata.sell_price[0],
			fpdata.sell_price[1], fpdata.sell_price[2], fpdata.sell_price[3], fpdata.sell_price[4],
			fpdata.sell_volum[0], fpdata.sell_volum[1], fpdata.sell_volum[2], fpdata.sell_volum[3],
			fpdata.sell_volum[4], start_mill, write_mill, ((write_mill - start_mill) / 1000.0));
		fflush(pfile);
	};
private:
	string path;
	FILE *pfile;
};

HfpMarketData_Getter * hfp_getter = new HfpMarketData_Getter;
namespace hfp_data
{

	void Re_Connect(const char * front_ip, const int front_port);

	void PrintQuoteHeader() {
		std::string CSVTitle("Seq,DateTime,RawTime,Contract_Id,MarketId,Open_Price,Highest_Price,Lowest_Price,"
			"New_Price,Last_Close,Average_Price,Total_Volume,Total_Amount,Subs_Volume,Cur_Volume,Type,Down_Limit,Up_Limit,Balance_Price,Updown_Base,Last_Subs_Volume,"
			"BP1,BP2,BP3,BP4,BP5,BV1,BV2,BV3,BV4,BV5,"
			"SP1,SP2,SP3,SP4,SP5,SV1,SV2,SV3,SV4,SV5,Start_nano,Write_nano,Offset_μs\n");
		hfp_getter->PrintHfpTitle(CSVTitle);
	}
	template<typename T>
	void ClientConnected(T client)//连接成功
	{
		printf("Response:Connect success!\n");
	};
	template<typename T>
	void ClientDisConnected(T client, int code)//连接失败
	{
		printf("Response:Connect fail! code=%d\n", code);//此处错误码为socket码
		cout << "Re-Connect,Hfp-Server." << endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		::connect(client, FRONT_IP, FRONT_PORT);
	};
	template<typename T>
	void ClientClosed(T client, hfp::close_type type) //客户端关闭函数
	{
		switch (type)//type枚举关闭类型
		{
		case 0:
		{
			printf("Response:Connect Closed!close_type none;\n");
			cout << "Re-Connect,Hfp-Server." << endl;
			Re_Connect(FRONT_IP, FRONT_PORT);
			break;
		}
		case 1:
		{
			printf("Response:Connect Closed!close_type server_force;\n");
			cout << "Re-Connect,Hfp-Server." << endl;
			Re_Connect(FRONT_IP, FRONT_PORT);
			break;
		}
		case 2:
		{
			printf("Response:Connect Closed!close_type client _close;\n");
			cout << "Re-Connect,Hfp-Server." << endl;
			Re_Connect(FRONT_IP, FRONT_PORT);
			break;
		}
		case 3:
		{
			printf("Response:Connect Closed!close_type timeout;\n");
			cout << "Re-Connect,Hfp-Server." << endl;
			Re_Connect(FRONT_IP, FRONT_PORT);
			break;
		}
		}
	};
	template<class T>
	void Clienthandshaked(T client, bool IsSuccess, int index, const char *code)//与服务器握手函数
	{
		if (IsSuccess)
		{//如果握手成功
			printf("Response:HandShaked Success! errCode=%d, errMsg=%s\n", index, code);//打印错误代码和错误信息
			int QUOTA_LEVEL = getquotationlevel(client);	//获取行情档位
			printf("Response:getquotationlevel=%d\n", QUOTA_LEVEL);	//打印行情档数
			PrintQuoteHeader();	//	获得的信息栏目
		}
		else
		{
			printf("Error:HandShaked Fail! errCode = %d, errMsg = %s\n", index, code);	//握手失败
		}
	};
	void ClientQuotationInfo(CLIENT client, quotation_data& data)//获取行情信息
	{
		unsigned long long start_mill = os::nano_time();

		execute.submit(*hfp_getter, HfpData(data), start_mill);
	};
	template<class T>
	class Hfp_Market_Executor
	{
	public:
		HfpMarketData_Getter * hfp_getter;
	public:
		void Create_Client()
		{
			client = ::client(hfp_id, hfp_license, hfp_encryption, hfp_type);
		}
		bool  OpenClient(T const&ClientSeq)
		{
			return this->client = ClientSeq;
		}
		bool SetCallBackFunc()
		{
			if (client)
			{
				::setonconnected(client, hfp_data::ClientConnected);
				::setonconnectfail(client, hfp_data::ClientDisConnected);
				::setonclosed(client, hfp_data::ClientClosed);
				::setonhandshaked(client, hfp_data::Clienthandshaked);
				::setonquotation(client, hfp_data::ClientQuotationInfo);
				return true;
			}
			else
			{
				return false;
			}
		}
		void Hfp_Connect(const char * front_ip, const int front_port)
		{
			::connect(client, front_ip, front_port);
		}
		void Close_Connect(const T& ClientSeq) {};
		bool Re_start(const char * front_ip, const int front_port)
		{
			client = ::client(hfp_id, hfp_license, hfp_encryption, hfp_type);
			SetCallBackFunc();
			::connect(client, front_ip, front_port);
			return true;
		}
	public:
		Hfp_Market_Executor() = delete;
		Hfp_Market_Executor(const char * id, const char * license, bool encryption, client_type type) :
			hfp_id(id), hfp_license(license), hfp_encryption(encryption), hfp_type(type)
		{

		};
		~Hfp_Market_Executor(){};
	private:
		T client;
		const char * hfp_id;
		const char * hfp_license;
		bool hfp_encryption;
		client_type hfp_type;
	};
	void Re_Connect(const char * front_ip, const int front_port)
	{
		Hfp_Market_Executor<CLIENT> re_excutor(ID, LICENSE, true, hfp::client_type::quotation);
		re_excutor.Re_start(front_ip, front_port);

		while (true)
		{
			char quit = getchar();
			if (quit == 'q')
			{
				return;
			}
		}
	}
}
using namespace hfp_data;
//-----------------------------------------------------------------------------设置回调函数---------------------------------------------------------------------------------------------------------------
inline void getFileName(const char *path, char(*fptr)[HFP_FILE_NAME])	//获得本地时间
{
	char ftime[TIME_BUFFER] = { 0 };
	char fpath[TIME_BUFFER] = { 0 };
	time_t t = time(0);
	time(&t);
	strcpy(fpath, path);
	strftime(ftime, sizeof(ftime), "/%Y.%m.%d %H-%M-%S.csv", localtime(&t));
	strcat(fpath, ftime);
	strcpy(*fptr, fpath);
	return;
};
int main(int argc, char* argv[])
{
	logger_instance::init_instance("HFP_MD_DEB_LOG", 0, true);//创建一个日志文件
	options::get().load_options(argc, argv);
	options::get().show_options(stdout);
	char hfp_name[HFP_FILE_NAME] = { 0 };//存储文件名
	getFileName("./data", &hfp_name);//路径+时间
	hfp_getter->OpenFile(hfp_name);
	hfp_data::Hfp_Market_Executor<CLIENT> hfp_executor(options::get().as_str("HFP_ID"), options::get().as_str("HFP_LICENSE"), true, hfp::client_type::quotation);//创建客户端
	hfp_executor.Create_Client();
	hfp_executor.SetCallBackFunc();//设置回调函数
	hfp_executor.Hfp_Connect(options::get().as_str("HFP_FRONT_IP"), atoi(options::get().as_str("HFP_FRONT_PORT")));//连接服务器
	while (true) //窗口保持
	{
		//fprintf()
		char quit = getchar();
		if (quit == 'q')
			break;

	}
	//system("pause");
	return 0;
}