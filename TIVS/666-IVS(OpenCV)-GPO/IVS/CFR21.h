/* ------------------------------------ CCFR21 Class -------------------------------------- 

  Function executed on Botton Pressed:
  
  OnLoginFunction - Compare strings added (Username and Password) with with database
  OnLoginExit - Logout function is possibile when user is logged
  OnLoginModify - Administrator can modify users and passwords. Compare a template for changes
  OnLoginPressed - Create a template for login
  OnLoginDelete - Administrator can delete users and password
  OnExportLog - Copy text Selected into a file, called Export.dat
                If area selected is null, it copies all text contained in RichEdit into the file

  RegolaAccessi - struct that contains username password and level for each user
  InsertString - Screen for characters input
  CheckAccess - function called before logging 

  Add_log - Add to file_log the action clicked
  Show_log - Show in a RichEdit the log
*/


#pragma once

//#include "Menu_button.h"
//#include "Parameter.h"

#define CFR21_NOME_FILE     "database.dat"    //file for Account
#define CFR21_NOME_FILE_LOG "logFile.dat"     //log file
#define CFR21_FILE_EXPORT   "export.txt"      //file exported from RichEdit Log

//lenght of string variables
#define CFR21_MAXLUNGH_USER  20
#define CFR21_MAXLUNGH_PW    20

#define CFR21_NBUTT     4
#define CFR21_TEXT_BTN  2

//ID for CFR21 login windows
#define NID_CFR21       8000
#define CFR21_NUM_USER  100           //max num user
#define MAX_LOG_STRING_LENGTH 100     //max lenght for log text
#define CFR21_MAX_FILE_LENGTH 200     //max lenght for log file

//log levels
#define CFR21_NO_LEVEL                0
#define CFR21_LOW_LEVEL               1
#define CFR21_MEDIUM_LEVEL            2
#define CFR21_HIGH_LEVEL              3
#define CFR21_ADMIN_LEVEL             4


#define LEVEL_RECIPE    2
#define LEVEL_EXIT      0
#define LEVEL_UTILS     1
#define LEVEL_MACH_PAR  2
#define LEVEL_INSP_PAR  1
#define LEVEL_START     1


#define NUM_LIV         4

#define NO_USER         "No user logged"

#define CFR21_AUTO_LOGOUT_TIME        1000


 struct RegolaAccessi{
    char name[CFR21_MAXLUNGH_USER+1];
    char passw[CFR21_MAXLUNGH_PW+1];
    UINT level;
  };


class CCFR21  : public CDialog
{  

public:
 
  CCFR21(CWnd* pParent = NULL);
  virtual ~CCFR21();
  CFileException ex;

  CButton *button[CFR21_NBUTT+CFR21_TEXT_BTN];
  RegolaAccessi  User_DB[CFR21_NUM_USER];
  char helpvar[CFR21_NBUTT+CFR21_TEXT_BTN][CFR21_MAXLUNGH_USER];

  UINT      num_users;
  UINT      dim_data;
  UINT      val;

  char      msg_err[400];
  int       current_user;   //-1 nessun utente loggato

  CStatic *spin_text0 ;
  CSpinButtonCtrl *spin ; 
  CButton  *RadioBtn[NUM_LIV];
  CRichEditCtrl* RichEdit;


protected:
	DECLARE_MESSAGE_MAP()

public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg int OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult  );
  
  int Load_data(void);
  int Save_data(void);

  int SearchUser2Log(RegolaAccessi User2Search);

  void OnLoginFunction();
  void OnLoginExit();
  void OnLoginModify();
  void OnLoginPressed();
  void OnLoginDelete();
  void OnExportLog();
  void OnLoginLogout();

  void InsertString( int ind );

  BOOL CheckAccess( int LevelReq ); //questa sarà chiamata prima di ogni operazione per controllare il log dell'utente

protected:
  virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);  

public:
  int Add_log( char* string);
  int AddToDay( char* string);
  int Show_log( void );

  int PreOperation( int level, char* string);
};