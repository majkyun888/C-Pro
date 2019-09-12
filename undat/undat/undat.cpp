#include "pch.h"
#include <list>
#include <iostream>
#include <string>
using namespace std;

struct TDBDefine_Transaction
{
	char    chWindCode[32];     //万得代码(AG1312.SHF)
	char    chCode[32];         //交易所代码(ag1312)		64
	int     nDate;              //日期（自然日）格式:YYMMDD	64-68
	int     nTime;              //成交时间(精确到毫秒HHMMSSmmm)	68-72
	int     nIndex;             //成交编号(从1开始，递增1)	72-76
	char    chFunctionCode;     //成交代码: 'C', 0	76-77
	char    chOrderKind;        //委托类别	77-78
	char    chBSFlag;           //BS标志	78-79
	int     nTradePrice;        //成交价格((a double number + 0.00005) *10000)	80-84
	int     nTradeVolume;       //成交数量	84-88
	int     nAskOrder;          //叫卖序号	88-92
	int     nBidOrder;          //叫买序号	92-96
};
struct TDBDefine_TickAB
{
	char chWindCode[32];                //万得代码(AG1312.SHF)
	char chCode[32];                    //交易所代码(ag1312)
	int nDate;                          //日期（自然日）格式YYMMDD
	int nTime;                          //时间（HHMMSSmmm）例如94500000 表示 9点45分00秒000毫秒
	int nPrice;                         //成交价((a double number + 0.00005) *10000)
	__int64 iVolume;                    //成交量
	__int64    iTurover;                //成交额(元)
	int nMatchItems;                    //成交笔数
	int nInterest;                      //IOPV(基金)、利息(债券)
	char chTradeFlag;                   //成交标志
	char chBSFlag;                      //BS标志
	__int64 iAccVolume;                 //当日累计成交量
	__int64    iAccTurover;             //当日成交额(元)
	int nHigh;                          //最高((a double number + 0.00005) *10000)
	int nLow;                           //最低((a double number + 0.00005) *10000)
	int    nOpen;                       //开盘((a double number + 0.00005) *10000)
	int    nPreClose;                   //前收盘((a double number + 0.00005) *10000)
	int    nAskPrice[10];               //叫卖价((a double number + 0.00005) *10000)
	unsigned nAskVolume[10];            //叫卖量
	int    nBidPrice[10];               //叫买价((a double number + 0.00005) *10000)
	unsigned nBidVolume[10];            //叫买量
	int    nAskAvPrice;                 //加权平均叫卖价(上海L2)((a double number + 0.00005) *10000)
	int    nBidAvPrice;                 //加权平均叫买价(上海L2)((a double number + 0.00005) *10000)
	__int64  iTotalAskVolume;           //叫卖总量(上海L2)
	__int64  iTotalBidVolume;           //叫买总量(上海L2)

	//下面的字段指数使用
	int        nIndex;                  //不加权指数
	int        nStocks;                 //品种总数
	int        nUps;                    //上涨品种数
	int        nDowns;                  //下跌品种数
	int        nHoldLines;              //持平品种数
};
class Data_File
{
public:
	list<void *> data_list;
	string Head_name;
	string data_format;
	virtual bool Open_Source(string filename, string mode) = 0;
	virtual void Load_data() = 0;
	virtual void write_file(FILE * op) = 0;
	//virtual void write_head() = 0;
	virtual ~Data_File() {
		list<void *>::iterator ite = data_list.begin();
		while (ite != data_list.end())
		{
			if (*ite)
				delete *ite;
			ite++;
		}
		data_list.clear();
		if (_file)
			fclose(_file);
	};
protected:
	FILE * _file;
};


class Transaction_data : public Data_File
{
public:
	
	Transaction_data()
	{
		Head_name = "万得代码, 交易所代码, 日期, 成交时间, 成交编号, 成交代码, 委托类别, BS标志, 成交价格, 成交数量, 叫卖序号, 叫买序号\n";
		data_format = "%s, %s, %d, %d, %d,%c, %c, %c, %d, %d, %d, %d\n";
	}
	~Transaction_data()
	{
	}
	bool Open_Source(string filename, string mode = "a+")
	{
		_file = fopen(filename.c_str(), mode.c_str());
		return _file;
	}
	void Load_data()
	{
		if (_file)
		{
			fseek(_file, 0, SEEK_SET);
			while (!feof(_file))
			{
				void * buffer = operator new(sizeof(TDBDefine_Transaction));
				fread(buffer, sizeof(TDBDefine_Transaction), 1, _file);
				data_list.push_back(buffer);
			}
		}
		fclose(_file);
	}
	void write_file(FILE * op)
	{
		if (op)
		{
			list<void *>::iterator ite = data_list.begin();
			while (ite != data_list.end())
			{
				TDBDefine_Transaction *  bufferStart = (TDBDefine_Transaction *)(*ite);
				fprintf(op, data_format.c_str(),
					bufferStart->chWindCode, bufferStart->chCode, bufferStart->nDate, bufferStart->nTime, bufferStart->nIndex, bufferStart->chFunctionCode,
					bufferStart->chOrderKind, bufferStart->chBSFlag, bufferStart->nTradePrice, bufferStart->nTradeVolume, bufferStart->nAskOrder, bufferStart->nBidOrder);
				fflush(op);
				ite++;
			}
		}
	}
private:
	TDBDefine_Transaction * bufferStart;
};


class TickAB_data : public Data_File
{
public:

	TickAB_data()
	{
		Head_name = "万得代码, 交易所代码, 日期, 时间, 成交价, 成交量,"
				"成交额, 成交笔数, IOPV(基金)、利息(债券), 成交标志, BS标志, 当日累计成交量,"
				"当日成交额(元), 最高, 最低, 开盘, 前收盘, 叫卖价一,"
				"叫卖价二, 叫卖价三, 叫卖价四, 叫卖价五, 叫卖价六, 叫卖价七,"
				"叫卖价八, 叫卖价九, 叫卖价十, 叫卖量一, 叫卖量二, 叫卖量三,"
				"叫卖量四, 叫卖量五,  叫卖量六, 叫卖量七,  叫卖量八, 叫卖量九, "
				"叫卖量十, 叫买价一,  叫买价二, 叫买价三, 叫买价四, 叫买价五, "
				"叫买价六, 叫买价七, 叫买价八,  叫买价九, 叫买价十,"
				"叫买量一, 叫买量二, 叫买量三, 叫买量四, 叫买量五, 叫买量六, "
				"叫买量七, 叫买量八, 叫买量九, 叫买量十, 加权平均叫卖价(上海L2), 加权平均叫买价(上海L2),"
				"叫卖总量(上海L2), 叫买总量(上海L2), 不加权指数, 品种总数, 上涨品种数, 下跌品种数, 持平品种数\n";
		data_format = "%s, %s, %d, %d, %d, %I64d, "
					"%I64d, %d, %d, %c, %c, %I64d,"
					"%I64d, %d, %d, %d, %d, %d,"
					"%d, %d, %d, %d, %d, %d,"
					"%d, %d, %d, %u, %u, %u,"
					"%u, %u, %u, %u, %u, %u,"
					"%u, %d, %d, %d, %d, %d,"
					"%d, %d, %d, %d, %d, %u,"
					"%u, %u, %u, %u, %u, %u,"
					"%u, %u, %u, %d, %d, %I64d,"
					"%I64d, %d, %d, %d, %d, %d\n";
	}
	~TickAB_data()
	{

	}
	bool Open_Source(string filename, string mode = "a+")
	{
		_file = fopen(filename.c_str(), mode.c_str());
		return _file;
	}
	void Load_data()
	{
		if (_file)
		{
			fseek(_file, 0, SEEK_SET);
			while (!feof(_file))
			{
				void * buffer = operator new(sizeof(TDBDefine_TickAB));
				fread(buffer, sizeof(TDBDefine_TickAB), 1, _file);
				data_list.push_back(buffer);
			}
		}
		fclose(_file);
	}
	void write_file(FILE * op)
	{
		if (op)
		{
			list<void *>::iterator ite = data_list.begin();
			while (ite != data_list.end())
			{
				TDBDefine_TickAB *  bufferStart = (TDBDefine_TickAB *)(*ite);
				fprintf(op, data_format.c_str(),
					bufferStart->chWindCode, bufferStart->chCode, bufferStart->nDate, bufferStart->nTime, bufferStart->nPrice, bufferStart->iVolume,
					bufferStart->iTurover, bufferStart->nMatchItems, bufferStart->nInterest, bufferStart->chTradeFlag, bufferStart->chBSFlag, bufferStart->iAccVolume,
					bufferStart->iAccTurover, bufferStart->nHigh, bufferStart->nLow, bufferStart->nOpen, bufferStart->nPreClose, bufferStart->nAskPrice[0],
					bufferStart->nAskPrice[1], bufferStart->nAskPrice[2], bufferStart->nAskPrice[3], bufferStart->nAskPrice[4], bufferStart->nAskPrice[5], bufferStart->nAskPrice[6],
					bufferStart->nAskPrice[7], bufferStart->nAskPrice[8], bufferStart->nAskPrice[9], bufferStart->nAskVolume[0], bufferStart->nAskVolume[1], bufferStart->nAskVolume[2],
					bufferStart->nAskVolume[3], bufferStart->nAskVolume[4], bufferStart->nAskVolume[5], bufferStart->nAskVolume[6], bufferStart->nAskVolume[7], bufferStart->nAskVolume[8],
					bufferStart->nAskVolume[9], bufferStart->nBidPrice[0], bufferStart->nBidPrice[1], bufferStart->nBidPrice[2], bufferStart->nBidPrice[3], bufferStart->nBidPrice[4],
					bufferStart->nBidPrice[5], bufferStart->nBidPrice[6], bufferStart->nBidPrice[7], bufferStart->nBidPrice[8], bufferStart->nBidPrice[9], bufferStart->nBidVolume[0],
					bufferStart->nBidVolume[1], bufferStart->nBidVolume[2], bufferStart->nBidVolume[3], bufferStart->nBidVolume[4], bufferStart->nBidVolume[5], bufferStart->nBidVolume[6],
					bufferStart->nBidVolume[7], bufferStart->nBidVolume[8], bufferStart->nBidVolume[9], bufferStart->nAskAvPrice, bufferStart->nBidAvPrice, bufferStart->iTotalAskVolume,
					bufferStart->iTotalBidVolume, bufferStart->nIndex, bufferStart->nStocks, bufferStart->nUps, bufferStart->nDowns, bufferStart->nHoldLines);
				fflush(op);
				ite++;
			}
		}
	}
private:
	TDBDefine_TickAB * bufferStart;
};


class Writer
{
public:
	static Writer * getInstance()
	{
		if (Instance)
			return Instance;
	}
	void start_writer(Data_File ** temp)
	{
		data_file = *temp;
	}
	bool Open_file(string filename,string mode)
	{
		data_file->Open_Source(filename,mode);
		data_file->Load_data();
		return data_file;
	}
	void write_head(string target_path, string mode = "a+")
	{
		op = fopen(target_path.c_str(), mode.c_str());
		if (op)
		{
			fprintf(op, data_file->Head_name.c_str());
		}
		
	}
	void write_file()
	{
		if (op)
		{
			data_file->write_file(op);
		}
	}
	~Writer()
	{
		if (data_file)
			delete data_file;
		if (_file)
			fclose(_file);
		if (op)
			fclose(op);
	}
private:
	Writer(FILE * file = 0):_file(file)
	{
	}
	Writer(const Writer &) = delete;
	Writer & operator= (const Writer&) = delete;
	FILE * _file;
	static Writer * Instance;
	FILE * op;
	Data_File * data_file;
private:
	static void Destory()
	{
		if (Instance)
		{
			delete Instance;
			Instance = NULL;
		}
	}
	struct GC
	{
		~GC()
		{
			Destory();
		}
	};
	static GC gc;
};
Writer * Writer::Instance = new Writer;
Writer::GC Writer::gc;


int main()
{
	Data_File * temp = new TickAB_data;
	Writer * opertt = Writer::getInstance();
	opertt->start_writer(&temp);
	opertt->Open_file("C:/Users/liu.bingqiang/Desktop/000001_20190301_TickAB.dat", "rb");
	opertt->write_head("./000003.csv");
	opertt->write_file();


	
		

	
	
	getchar();
	
	return 0;
}


