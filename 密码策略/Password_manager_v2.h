#pragma once

#include "Globals.h"
#include "Timer.h" //for password expiry

#define NOACCESS_LEVEL       0  // basic/no access
#define OPERATOR_LEVEL       1  // operator    - lowest level
#define MAINTENANCE_LEVEL    2  // maintenance - medium level
#define SUPERVISOR_LEVEL     3  // supervisor  - high level
#define SUPERUSER_LEVEL      4  // SUPERUSER/DEVELOPER LEVEL

#define MAX_NUM_SUPERVISOR   10
#define MAX_NUM_MAINTENANCE  10
#define MAX_NUM_OPERATOR     40
#define MAX_NUM_USERS        70 //maximum allowed number of user

#define LOGOUT_TIMEOUT     600  // seconds for automatic logout

#define PWD_MAXLUNGH_TXT    20U // max allowed length for username & password
/*#define PWD_MAXLUNGH_USR    20U
#define PWD_MAXLUNGH_PWD    20U */

#define PWD_MINLUNGH_TXT    6U

#define MAX_HMINEL         200

#define MAX_LEVELS         4
#define MIN_UPPER          1

//#define MAX_NUM_USERS       50  // maximum allowed number of users

//#define CONFIG_FILE_NAME   "D:\\IVS\\Config.ini"
#define CONFIG_FILE_NAME   L"config.ini"


/*
  struct hmi_object
  {
    CMenu_button*  btn; // pointer to CMenu_button 
    CParameter*    par; // pointer to CParameter
    int            lev; // minimum required access level
  };
  */

/*
#pragma pack( 1 )
  struct access_info
  {
    wchar_t   txt[PWD_MAXLUNGH_TXT+1];
    wchar_t   usr[PWD_MAXLUNGH_TXT+1];
    wchar_t   pwd[PWD_MAXLUNGH_TXT+1]; 
    int       lev;
  };


#pragma pack()
  */


class CPassword_manager_v2
{


public:
  CPassword_manager_v2(void);
  ~CPassword_manager_v2(void);

  int  read_string( int ind ); // ind=0->Username / ind=1->Password - ret=99 for cancel
  int  read_string( wchar_t* buffer, wchar_t* caption, BOOL pwd ); // 

  void authenticate_user( void );
  void set_new_level( void );
  void logout( int mode=0 );
  void check_timeout( void ); // function to automatically logout after timeout (never if LOGOUT_TIMEOUT==0)



  int  insert_hmi( CMenu_button* button, int level );
  int  insert_hmi( CParameter*   param,  int level );

  wchar_t* input_usr; //[PWD_MAXLUNGH_USR+1];
  wchar_t* input_pwd;

  //char* input_usr; //[PWD_MAXLUNGH_USR+1];
  //char* input_pwd;
  //char input_usr[21]; //[PWD_MAXLUNGH_USR+1];
  //char input_pwd[21]; //[PWD_MAXLUNGH_PWD+1];

  int current_level;  // current authorization level
  int current_user;   // array index
 // int users_number;   // number of users into db


  struct hmi_object
  {
    CMenu_button*  btn; // pointer to CMenu_button 
    CParameter*    par; // pointer to CParameter
    int            lev; // minimum required access level
  };

  #pragma pack( 1 )
  struct access_info
  {
    //wchar_t   txt[PWD_MAXLUNGH_TXT+1];
	bool      locked;
    wchar_t   usr[PWD_MAXLUNGH_TXT+1];
    wchar_t   pwd[PWD_MAXLUNGH_TXT+1]; 
    //int       lev;
	UCHAR     lev;
	time_t    opentime;
	time_t    lockedtime;//Pour added on 20200728
  };
  #pragma pack()


  // struttura dati:
  // UINT num_users
  // access_info user[ 0]
  // access_info user[ 1]
  // access_info user[ 2]
  // ..
  // ..
  // access_info user[99]

  struct usrdb_type
  {
    //UINT        num_users;
	UINT        num_users[MAX_LEVELS];
    access_info usr[MAX_NUM_USERS];
  } usrdb;


  int encrypt( BYTE* ptr, UINT lng );
  int decrypt( BYTE* ptr, UINT lng );

  UINT  dimdata; // total n of bytes to load/save

  int hminel; // number of elements to manage
  hmi_object  hmidb[MAX_HMINEL];

  int Change_passwords(void);
  int Select_folders(void);
  int Save_data( void );
  int Load_data( void );

  int Trace_data( void );

  CTimer  elapsed; // time elapsed since last authentication
  UINT    logout_timeout;
  UINT    chgpwd_days;
  BOOL    annotate_on;

  //int Browse_folders( char* title, char* path );
  int Browse_folders( wchar_t* title, wchar_t* path );

};

