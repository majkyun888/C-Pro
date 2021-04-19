#pragma once




//#define LOG_FILE_NAME   "D:\\IVS\\alarmlog.txt"
//#define LOG_FILE_NAME   L"C:\\IVS\\LOG\log.txt"
#define LOG_FILE_NAME         L"log.txt"
#define NEWLOG_FILE_NAME      L"log.lgf"                     // new file name/extension for encrypted logs

#define ALARMLOG_FILE_NAME    L"alarmlog.txt" 
#define NEWALARMLOG_FILE_NAME      L"alarmlog.lgf"                     // new file name/extension for encrypted logs

#define LOG_BUFF_SIZE         1024                           // max size for each row
#define LOG_NUM_LINES         20000                           // max number of lines/rows to keep//Pour changed on 20200708 from 2000 to 20000

//#define NEWLOG_FILE_NAME      L"log.lgf"                     // new file name/extension for encrypted logs//Pour removed for testing on 20200507
//#define NEWLOG_FILE_NAME      L"D:\\LOG REPORT\\log.lgf"//Pour enabled for testing on 20200507

#define NEWLOG_MAXFILESZ      200*1024                      // new logger max size for trimming at beginning

#define BLPARAM_FILE_NAME     L"blparam.txt"       // logger used to record params changes during batch
#define BLALARM_FILE_NAME     L"blalarm.txt"       // logger used to record alarms changes during batch

//added by Pour 20190328
#define BATCH_LOG_TXT         L"logcopy_current_batch.txt"   //for xinchang, logcopy file recording to the end of current batch


class CLogger
{
public:
  CLogger( wchar_t* name=LOG_FILE_NAME );  //void);
  ~CLogger(void);
  //int Add_string(char* string_in);
  int Add_string(wchar_t* string_in);
  int Add_string(CString* string_in);

  int Add_string2(wchar_t* string_in);
  int Add_string2(CString* string_in);

  int Trim_numlines(void);
  int Check_filesize(void);

  wchar_t* file_name;

  //added by yang 20180903
  int Save_txt_archive();//save the log file when batch ends
  //added by Pour 20190220
  int Save_alarm_txt_archive();//save the log file when batch ends

};

