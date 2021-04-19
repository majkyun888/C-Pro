#include "StdAfx.h"
#include "Password_manager_v2.h"
#include "Input_string.h"
#include "User_manager.h"
//#include "Globals.h"

extern wchar_t          PC_name[30]; // Computer name read from system configuration

/*
#define OPERATOR_LEVEL      0
#define MAINTENANCE_LEVEL   1
#define SUPERVISOR_LEVEL    2
#define SUPERUSER_LEVEL     3
*/

extern CChildView*       pView;
//extern CMenu_button*     user_btn;
extern CMenu_button*     datarec_btn;
extern CMenu_button*     inpdatarec_btn;
extern CMenu_button*     hvdatarec_btn;
extern CMenu_button*     hgadatarec_btn;

//Pour added for testing 20171114
extern CMenu_button*     idpassword_btn;
//Pour added for testing 20171114

extern UINT              lng;
extern CLogger*          logger;

#ifdef ALARMLOG_SEPARATE // log alarms separately and add button to show them
  extern CLogger*       alarm_logger;
#endif

//extern char output_folder[MAX_LENGHT_PATH];
//extern char input_folder [MAX_LENGHT_PATH];

//unsigned char* key = "e1n2c3d4e5c6k7e8y9";


static const char* key = "e1n2c3d4e5c6k7e8y9";
wchar_t* titles[MAX_LEVELS]  = { L"OP",L"MA",L"SV",L"SU" };


CPassword_manager_v2::CPassword_manager_v2(void)
{
  current_level =  0;
  current_user  = -1; // undefined / not initialized
 // usrdb.num_users  =  4; // number of users into db
  hminel        =  0; // n of hmi elements to be updated

  elapsed.reset();
  //logout_timeout = 10;//Pour marked on 20200629
  logout_timeout = 3;
  chgpwd_days    = 90;
  annotate_on    = FALSE;


  //ZeroMemory( input_usr, PWD_MAXLUNGH_USR+1 );
  //ZeroMemory( input_pwd, PWD_MAXLUNGH_PWD+1 );

  ZeroMemory(  hmidb, sizeof(hmidb) ); // clear all pointers and set default access level to 0 
  ZeroMemory( &usrdb, sizeof(usrdb) ); // clear all pointers and set default access level to 0 

 // current_level = 123;

//  input_usr = (char*) malloc( 100 );
  input_usr = (wchar_t*) malloc( 100 );
  ZeroMemory( input_usr, 100 );

//  input_pwd = (char*) malloc( 100 );
  input_pwd = (wchar_t*) malloc( 100 );
  ZeroMemory( input_pwd, 100 );

  //ZeroMemory(  input_folder, MAX_LENGHT_PATH );
  //ZeroMemory( output_folder, MAX_LENGHT_PATH );
  for (int i = 0; i<MAX_LEVELS;i++)
  usrdb.num_users[i] = 0;

  //usrdb.num_users = 0;

  // prepare default users
  //wcscpy( usrdb.usr[0].txt, L"OPERATOR" );
  wcscpy( usrdb.usr[0].usr, L"op" );
  wcscpy( usrdb.usr[0].pwd, L"123456Aa" );
  usrdb.usr[0].lev = OPERATOR_LEVEL; // 1
  usrdb.usr[0].locked = FALSE;
  usrdb.num_users[0]++;
  usrdb.num_users[usrdb.usr[0].lev]++;
  //usrdb.num_users++;

  //wcscpy( usrdb.usr[1].txt, L"QA");//QA for HZB //changed 20190107 and need to be recovered.
  //wcscpy( usrdb.usr[1].txt, L"MAINTENANCE" );//SUPERVISOR//MAINTENANCE
  wcscpy( usrdb.usr[1].usr, L"ma" );
  wcscpy( usrdb.usr[1].pwd, L"123456Ab" ); 
  usrdb.usr[1].lev = MAINTENANCE_LEVEL; // 2
  usrdb.usr[1].locked = FALSE;
  usrdb.num_users[0]++;
  usrdb.num_users[usrdb.usr[1].lev]++;
  //usrdb.num_users++;
  
  //wcscpy( usrdb.usr[2].txt, L"SUPERVISOR" );
  wcscpy( usrdb.usr[2].usr, L"sv" );
  wcscpy( usrdb.usr[2].pwd, L"123456Ac" );
  usrdb.usr[2].lev = SUPERVISOR_LEVEL; // 3
  usrdb.usr[2].locked = FALSE;
  usrdb.num_users[0]++;
  usrdb.num_users[usrdb.usr[2].lev]++;
  //usrdb.num_users++;
  //Pour removed for FMC 20171115
  
  //wcscpy( usrdb.usr[3].txt, L"ADMIN" );//admin
  wcscpy( usrdb.usr[3].usr, L"su" );
  wcscpy( usrdb.usr[3].pwd, L"Tofflon139" );
  usrdb.usr[3].lev = SUPERUSER_LEVEL; // 4
  usrdb.usr[3].locked = FALSE;
  usrdb.num_users[0]++;
  usrdb.num_users[usrdb.usr[3].lev]++;
  //usrdb.num_users++;
  
  //Pour removed for FMC 20171115
  /*
  #ifdef NEW_SVPWD // if defined set new supervisor password to TFLSUPWD
    wcscpy( usrdb.usr[3].usr, L"tflsupwd" );
    wcscpy( usrdb.usr[3].pwd, L"tflsupwd" );
  #endif

  #if SERIAL_N==201400007 // if yoko pharma disable auto-logout
    #undef  LOGOUT_TIMEOUT
    #define LOGOUT_TIMEOUT 6000000
  #endif
*/

  dimdata = sizeof(usrdb); //+2*MAX_LENGHT_PATH;
  //TRACE("Dimdata=%d\n",dimdata);
  //TRACE( L"password_manager dimdata=%d\n", dimdata );

 //  Save_data();
 //  ZeroMemory( &usrdb, sizeof(usrdb) ); // clear all pointers and set default access level to 0 
 //  Load_data();




  if ( Load_data() ) Save_data(); // if cannot load, save default

//  TRACE("Password_mngr created - current level=%d - current user=%d - users number=%d\n", current_level, current_user, usrdb.num_users );
  TRACE( L"Password_mngr created - dimdata=%d - current level=%d - current user=%d - users number=%d\n", dimdata, current_level, current_user, usrdb.num_users );

  TRACE("USERDATABASE IN USE:\n");
  for ( UINT i=0 ; i<usrdb.num_users[0] ; i++ ) TRACE( L"usrdb.usr[%d]: <%s><%s> Level=%d\n", i, usrdb.usr[i].usr, usrdb.usr[i].pwd, usrdb.usr[i].lev );

}


CPassword_manager_v2::~CPassword_manager_v2(void)
{

  //Save_data();

  if ( input_usr )
  {
    free( input_usr );
    input_usr = NULL;
  }

  if ( input_pwd )
  {
    free( input_pwd );
    input_pwd = NULL;
  }

/*  for ( int i=0 ; i<usrdb.num_users ; i++ )
  {
    if ( usrdb.usr[i].txt ) free( usrdb.usr[i].txt );
    if ( usrdb.usr[i].usr ) free( usrdb.usr[i].usr );
    if ( usrdb.usr[i].pwd ) free( usrdb.usr[i].pwd );
  }*/



}






void CPassword_manager_v2::set_new_level( void ) 
{

  //user_btn->SetWindowTextA( _strupr(usrdb.usr[current_level].txt) );
//  user_btn->SetWindowText( _wcsupr( usrdb.usr[current_level].txt ) );
//  user_btn->SetWindowTextA( usrdb.usr[current_level].txt.MakeUpper() );

  ASSERT( current_user>=-1 && current_user<int(usrdb.num_users) );

  if ( current_user<0 ) current_level = NOACCESS_LEVEL;
  else                  current_level = usrdb.usr[current_user].lev;

  if ( pView && pView->container_main && pView->container_main->User_st )
  {
    pView->Close_all_menus();

    //if ( current_user>=0 ) pView->container_main->User_st->SetWindowTextW( usrdb.usr[current_user].txt );
		CString csUser;
	    if ( current_user>=0 )
	{
			csUser.Format(L"%s %s", usrdb.usr[current_user].usr,titles[usrdb.usr[current_user].lev-1]);//Pour marked on 20200512
			pView->container_main->User_st->SetWindowTextW( csUser );
	}
    else                   pView->container_main->User_st->SetWindowTextW( L"no user logged" );
  }

  TRACE( L"Change of access level: new user <%s> - level=%d\n", usrdb.usr[current_user].usr, usrdb.usr[current_user].lev );


  // update all enabled/disabled items
  ASSERT( hminel>=0 && hminel<=MAX_HMINEL );

  for ( int i=0 ; i<hminel ; i++ )
  {
    if ( hmidb[i].btn ) // Button
    {
      if ( current_level>=hmidb[i].lev ) hmidb[i].btn->enabled =  TRUE;
      else                               hmidb[i].btn->enabled = FALSE;
      hmidb[i].btn->RedrawWindow();
    }

    if ( hmidb[i].par ) // Parameter
    {
      if ( current_level>=hmidb[i].lev )
      {
        hmidb[i].par->enabled = TRUE;
        hmidb[i].par->pt_desc->ModifyStyleEx( WS_EX_STATICEDGE, WS_EX_TOPMOST|WS_EX_CLIENTEDGE );
        hmidb[i].par->pt_desc->EnableWindow( TRUE );
   //     hmidb[i].par->pt_val->EnableWindow ( TRUE );
        hmidb[i].par->pt_udm->EnableWindow ( TRUE ); 
      }
      else
      {
        hmidb[i].par->enabled = FALSE;
        hmidb[i].par->pt_desc->ModifyStyleEx( WS_EX_TOPMOST|WS_EX_CLIENTEDGE, WS_EX_STATICEDGE );
        hmidb[i].par->pt_desc->EnableWindow( FALSE );
//        hmidb[i].par->pt_val->EnableWindow ( FALSE );
        hmidb[i].par->pt_udm->EnableWindow ( FALSE ); 

      }

      hmidb[i].par->pt_desc->RedrawWindow();
      hmidb[i].par->pt_val->RedrawWindow();
      hmidb[i].par->pt_udm->RedrawWindow();
    }
  }

  // oggetti custom

/*  if ( current_level >= SUPERUSER_LEVEL ) 
  {
    datarec_btn->ShowWindow( SW_SHOW );
  }
  else
  {
    datarec_btn->ShowWindow( SW_HIDE );
  }  */

  //Pour added 20171114
  /*
    if ( current_level <= OPERATOR_LEVEL ) 
  {
    idpassword_btn->ShowWindow( SW_SHOW );
  }
  else
  {
    idpassword_btn->ShowWindow( SW_HIDE );
  } 
  */
  //Pour added 20171114

  TRACE("HMI permissions updated (%d elements)\n", hminel );
}



int CPassword_manager_v2::insert_hmi( CMenu_button* button, int level ) 
{
  ASSERT( button );
  ASSERT( level>=0 && level<=SUPERUSER_LEVEL );

  if ( hminel >= MAX_HMINEL-1 ) // max num reached!
  {
    ASSERT("Max number of elements reached!\n");
    return( 99 ); 
  }

  hmidb[hminel].lev = level;
  hmidb[hminel].par = 0;
  hmidb[hminel].btn = button;
  hminel++;

  return 0; 
}



int CPassword_manager_v2::insert_hmi( CParameter* parameter, int level ) 
{
  ASSERT( parameter );
  ASSERT( level>=0 && level<=SUPERUSER_LEVEL );

  if ( hminel >= MAX_HMINEL ) // max num reached!
  {
    ASSERT("Max number of elements reached!\n");
    return( 99 ); 
  }

  hmidb[hminel].btn = 0;
  hmidb[hminel].par = parameter;
  hmidb[hminel].lev = level;
  hminel++;

  return 0; 
}




void CPassword_manager_v2::logout( int mode ) // ==0 timeout / ==1 forced by user
{

  if ( logger && current_user>=0 )
  {
    CString msg;
    if ( mode==0 ) msg.Format( L"User <%s> logged out because of timeout", usrdb.usr[current_user].usr );
    else           msg.Format( L"User <%s> manual logout", usrdb.usr[current_user].usr );
    logger->Add_string( &msg );

    // reset current status
    current_level =  0;
    current_user  = -1; // undefined / not initialized
    set_new_level();
  }

}



void CPassword_manager_v2::authenticate_user( void ) 
{

  pView->Close_all_menus();
  
/*  if ( current_level>OPERATOR_LEVEL ) 
  {
    current_level = OPERATOR_LEVEL;
    set_new_level();
    return;
  }*/
  int auth_user;
  CString msg_err;
  CString login_msg_err;
  CString failure_msg_err;
  int i;

  if ( read_string( 0 ) ) return; //input the user name
  // check username, find the index
  for ( i=0; i<usrdb.num_users[0];i++ )
  {
	if (!wcscmp(_wcslwr(input_usr), _wcslwr(usrdb.usr[i].usr)) ) //find the usernamed
	//if (!wcscmp(input_usr, usrdb.usr[i].usr) ) //find the usernamed
	  {
		  if (i < MAX_LEVELS) //if user is the default count, no other regulation
		  {
			  auth_user = i;
			  break;
		  }
		  else if ( !(usrdb.usr[i].locked) && difftime(time(NULL),usrdb.usr[i].opentime) <= chgpwd_days*3600*24 ) //if not, check whether user count is locked or password expired
		  {
			  auth_user = i; //find the username
			  if ( difftime((NULL),usrdb.usr[auth_user].opentime) > (chgpwd_days-7)*3600*24 )
				  Message_box( string_3872a[lng],MBOX_ERROR);
			  break;
		  }
		  else
		  {
			  msg_err.Format ( string_3872[lng],usrdb.usr[i].usr,usrdb.usr[i].lev);
			  Message_box(msg_err,MBOX_ERROR);
			  return;
		  }
	  }
  }
	
	//Pour removed on 20200430
    if( i == usrdb.num_users[0]  || usrdb.usr[auth_user].lev == 0 )  // username is inexistent or locked 
  {	
	  Message_box( string_3869[lng], MBOX_ERROR );
	  return;
  }
	//Pour removed on 20200430

	//Pour 20200728
	if ((usrdb.usr[i].locked) && difftime(time(NULL),usrdb.usr[auth_user].lockedtime) < 10*60) //current user will be locked for 10 minutes
	{
		login_msg_err.Format ( string_3872b[lng],usrdb.usr[i].usr);
		Message_box( login_msg_err,MBOX_ERROR);
		return;
	}
	//Pour 20200728
	else
	{
		  i = 5; //5 times to try the password
		  while ( i > 0 )
		  {
			if ( read_string(1) ) return; //input the password

		  if (!wcscmp(input_pwd, usrdb.usr[auth_user].pwd) )
		  {
			  current_level = usrdb.usr[auth_user].lev;
			  current_user  = auth_user;
			  elapsed.reset();
			  set_new_level();
			  if ( logger )
			  {
				  msg_err.Format( string_3870[lng], usrdb.usr[current_user].usr,usrdb.usr[current_user].lev );
				  logger->Add_string (&msg_err);
			  }
			  break;
		  }
		  else
		  {
			  failure_msg_err.Format ( string_3875[lng],i-1);
			  Message_box(failure_msg_err,MBOX_ERROR);
		  }
		  i--;
	  }
	}
	  //if ( i<=0 && auth_user >= MAX_LEVELS )//Pour removed on 20200728
		  if ( i<=0 )
	  {
		  usrdb.usr[auth_user].locked = TRUE;
		  usrdb.usr[auth_user].lockedtime = time(NULL);//Pour 20200728
		  msg_err.Format( string_3871[lng], usrdb.usr[auth_user].usr,usrdb.usr[auth_user].lev);
		  if ( logger )
		  {
			  logger-> Add_string(&msg_err);
		  }
		  Message_box(msg_err, MBOX_ERROR);
	  }
	  
}



void CPassword_manager_v2::check_timeout( void ) // function to automatically logout after timeout (never if LOGOUT_TIMEOUT==0)
{

	LASTINPUTINFO lpi;
  lpi.cbSize = sizeof(lpi);
  GetLastInputInfo(&lpi);
  //if ( LOGOUT_TIMEOUT>0 && elapsed.elapsed_sec()>LOGOUT_TIMEOUT && current_user>=0 )
	//if ( logout_timeout>0 && elapsed.elapsed_sec()>logout_timeout*60 && current_user>=0 && (GetTickCount()-lpi.dwTime)/1000> logout_timeout*60  )
  if ( logout_timeout>0 && elapsed.elapsed_sec()>logout_timeout*60 && current_user>=0 && lpi.dwTime/1000> logout_timeout*60  )
  {
 /*   if ( logger ) 
    {
      CString msg; //wchar_t msg[100];
      msg.Format( L"user <%s> logged out because of timeout", usrdb.usr[current_user].usr );
      logger->Add_string( &msg );
    } */
    logout();
  }
}



int CPassword_manager_v2::read_string(int ind) // ind=0->Username / ind=1->Password
{

  ASSERT( ind==0 || ind==1 );

  if (ind) wcscpy(input_pwd,L"");
  else     wcscpy(input_usr,L"");

/*
  CInput_string stringpad( pView, ind?input_pwd:input_usr, ind?PWD_MAXLUNGH_PWD:PWD_MAXLUNGH_USR, ind?CINPSTR_PASSWORD:0 ); // parent, stringa iniziale, massima lunghezza
//  stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowTextA( ind?string_1042[lng]:string_1041[lng] );
  stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( ind?string_1042[lng]:string_1041[lng] );
 */

  CInput_string stringpad;
  stringpad.password = ind==1;
  stringpad.Create( IDD_INPUT_STRING, AfxGetMainWnd() /*this*/ );
  stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( ind?string_1042[lng]:string_1041[lng] );

//  if ( stringpad.RunModalLoop() == IDOK )
  if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>0 )
	{
		ASSERT( AfxIsValidString(stringpad.buffer) );
		ASSERT( wcslen(stringpad.buffer)<=PWD_MAXLUNGH_TXT );
		wcsncpy( ind?input_pwd:input_usr, stringpad.buffer, PWD_MAXLUNGH_TXT+1 );
    return 0;
	}

  return 99;
}


int CPassword_manager_v2::read_string( wchar_t* buffer, wchar_t* caption, BOOL pwd ) // ind=0->Username / ind=1->Password
{
  ASSERT( buffer  && AfxIsValidAddress(buffer, PWD_MAXLUNGH_TXT+1) );
  ASSERT( caption && AfxIsValidAddress(caption,100) );

  wcscpy( buffer ,L"" );

  CInput_string stringpad;
  stringpad.password = pwd;
  stringpad.Create( IDD_INPUT_STRING, AfxGetMainWnd() );
  stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( caption );

//  if ( stringpad.RunModalLoop() == IDOK )
  if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>0 )
	{
		ASSERT( AfxIsValidString(stringpad.buffer) );
		ASSERT( wcslen(stringpad.buffer)<=PWD_MAXLUNGH_TXT );
		wcsncpy( buffer, stringpad.buffer, PWD_MAXLUNGH_TXT+1 );
    return 0;
	}

  return 99;
}







int CPassword_manager_v2::Select_folders(void)
{

//  if ( Browse_folders( string_1032[lng], input_folder  ) ) return 99;
//  if ( Browse_folders( string_1034[lng], output_folder ) ) return 99;
//  Save_data();
  return 0;
}



// return values:
//   0 = ok, updated
//  99 = cancelled during input by cancel button
// 199 = mismatch during confirmation of one field

int CPassword_manager_v2::Change_passwords(void)
{

  int ind = 0;

  #ifdef NEW_PASSW_MNGR  // New users/passwords manager

  CUser_manager user_mngr;
  user_mngr.DoModal();

  TRACE("USERDATABASE UPDATED - going to save it\n");
//  for ( UINT i=0 ; i<usrdb.num_users ; i++ ) TRACE( L"usrdb.usr[%d]: <%s><%s><%s> L=%d\n", i, usrdb.usr[i].usr, usrdb.usr[i].pwd, usrdb.usr[i].txt, usrdb.usr[i].lev );
//  for ( UINT i=0 ; i<usrdb.num_users ; i++ ) TRACE( L"usr[%d] <%s,%s,%d> ", i, usrdb.usr[i].usr, usrdb.usr[i].pwd, usrdb.usr[i].lev );

  Save_data();


  #endif //  #ifdef NEW_PASSW_MNGR  // New users/passwords manager

  return 0;


  // OPERATOR 

  //char msg[200];
  //char ins[200];
  //char cnf[4][200];

/*  wchar_t msg[200];
  wchar_t ins[200];
  wchar_t cnf[4][200];
  ZeroMemory( cnf, sizeof(cnf) ); */

  //char* names[2]  ={ "MAINTENANCE", "SUPERVISOR" };
  //char* strings[4]={ string_1046[lng], string_1047[lng], string_1048[lng], string_1049[lng] };
  //wchar_t* names[3]   = { L"OPERATOR", L"MAINTENANCE", L"SUPERVISOR" };
  wchar_t* strings[4] = { string_1046[lng], string_1047[lng], string_1048[lng], string_1049[lng] };


//  access_info temp[4];
  struct usrdb_type temp;


//  ZeroMemory( temp, sizeof(temp) );
  CopyMemory( &temp, &usrdb, sizeof(usrdb) );

  for ( int usr=0 ; usr<3 ; usr++ ) // user scan - 0=operator - 1=maintenance -2=supervisor ( 3=superuser NO! )
  {
    
    wchar_t txt1[PWD_MAXLUNGH_TXT+1];
    wchar_t txt2[PWD_MAXLUNGH_TXT+1];
    wchar_t caption[200+1];

    // read new username
    swprintf( caption, 200, strings[0], titles[usr] ); // prepare caption
    if ( read_string( txt1, caption, FALSE ) ) return 99; 

    swprintf( caption, 200, strings[1], titles[usr] ); // prepare caption
    if ( read_string( txt2, caption, FALSE ) ) return 99;

    if ( wcslen(txt1)>0 && wcslen(txt2)>0 && !wcscmp( txt1, txt2 ) ) // equal
    {
      wcscpy( temp.usr[usr].usr, txt1 );
      TRACE( L"confirmed username[%d]=<%s> l=%d\n", usr, txt1, wcslen(txt1) );
    }
    else return 199; // mismatch!

    // read new password
    swprintf( caption, 200, strings[2], titles[usr] ); // prepare caption
    if ( read_string( txt1, caption, TRUE ) ) return 99;

    swprintf( caption, 200, strings[3], titles[usr] ); // prepare caption
    if ( read_string( txt2, caption, TRUE ) ) return 99;

    if ( wcslen(txt1)>0 && wcslen(txt2)>0 && !wcscmp( txt1, txt2 ) ) // equal
    {
      wcscpy( temp.usr[usr].pwd, txt1 );
      TRACE( L"confirmed password[%d]=<%s> l=%d\n", usr, txt1, wcslen(txt1) );
    }
    else return 199; // mismatch!


    /*for ( ind=0 ; ind<2 ; ind++ ) // 0=userid - 1=pwd
    {
      CInput_string stringpad( pView, L"", PWD_MAXLUNGH_TXT, CINPSTR_PASSWORD );
      //sprintf( msg, strings[ind*2], names[usr] );
      swprintf( msg, strings[ind*2], names[usr] );
      //stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowTextA( msg );
      stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( msg );
      if ( stringpad.RunModalLoop() == IDOK )
      {
        //strcpy( ins, stringpad.buffer );
        wcscpy( ins, stringpad.buffer );
        //TRACE( "Inserito 1o <%s>\n", ins );
        //CInput_string stringpad( pView, "", PWD_MAXLUNGH_PWD, CINPSTR_PASSWORD );
        CInput_string stringpad( pView, L"", PWD_MAXLUNGH_TXT, CINPSTR_PASSWORD );
        //sprintf( msg, strings[ind*2+1], names[usr] );
        swprintf( msg, strings[ind*2+1], names[usr] );
        //stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowTextA( msg );
        stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( msg );
        if ( stringpad.RunModalLoop() == IDOK )
        {
          //TRACE( "Inserito 2o <%s>\n", ins );
          //if ( strlen(ins)>=1 && !strcmp( ins, stringpad.buffer ) ) // uguali
          if ( wcslen(ins)>=1 && !wcscmp( ins, stringpad.buffer ) ) // uguali
          {
            //strcpy( cnf[usr*2+ind], ins );
            wcscpy( cnf[usr*2+ind], ins );
            //TRACE("Confermato[%d] <%s>\n", usr*2+ind, ins );
            TRACE( L"Confirmed[%d] <%s>\n", usr*2+ind, ins );
          }
          //strncpy( ind?input_pwd:input_usr, stringpad.buffer, ind?PWD_MAXLUNGH_USR:PWD_MAXLUNGH_PWD );
        }
        else return 199; // annullato
      }
      else return 99; // annullato
    } // fine scansione su ind  */
  }

  // OK, copy new input data for first 3 users
  CopyMemory( &(usrdb.usr[0]), &(temp.usr[0]), sizeof(access_info) );
  CopyMemory( &(usrdb.usr[1]), &(temp.usr[1]), sizeof(access_info) );
  CopyMemory( &(usrdb.usr[2]), &(temp.usr[2]), sizeof(access_info) );

/*  if ( wcslen(cnf[0])>=1 ) wcscpy( usrdb.usr[MAINTENANCE_LEVEL].usr, cnf[0] );
  if ( wcslen(cnf[1])>=1 ) wcscpy( usrdb.usr[MAINTENANCE_LEVEL].pwd, cnf[1] );
  if ( wcslen(cnf[2])>=1 ) wcscpy( usrdb.usr[SUPERVISOR_LEVEL ].usr, cnf[2] );
  if ( wcslen(cnf[3])>=1 ) wcscpy( usrdb.usr[SUPERVISOR_LEVEL ].pwd, cnf[3] ); */

  TRACE("USERDATABASE UPDATED:\n");
  Trace_data();

  //for ( UINT i=0 ; i<usrdb.num_users ; i++ ) TRACE( L"usrdb.usr[%d]: <%s><%s><%s> L=%d\n", i, usrdb.usr[i].usr, usrdb.usr[i].pwd, usrdb.usr[i].txt, usrdb.usr[i].lev );

  Save_data();
  return 0;
}





//int CPassword_manager_v2::Browse_folders( char* title, char* path )
int CPassword_manager_v2::Browse_folders( wchar_t* title, wchar_t* path )
{

  /*
  BROWSEINFO bi = { 0 };
  TCHAR path[MAX_PATH];
  bi.lpszTitle = _T("Pick a Directory");
  bi.pszDisplayName = path;
  LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
  if ( pidl != 0 )
  {
    // get the name of the folder
    _tprintf ( _T("Selected Item: %s\n"), path );
    // free memory used
    IMalloc * imalloc = 0;
    if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
    {
      imalloc->Free ( pidl );
      imalloc->Release ( );
    }
  }
  */

  /*
  TCHAR path[MAX_PATH];
  BROWSEINFO bi = { 0 };
  bi.lpszTitle = ("All Folders Automatically Recursed.");
  LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );

  if ( pidl != 0 )
  {
    // get the name of the folder and put it in path
    SHGetPathFromIDList( pidl, path );

    //Set the current directory to path
    SetCurrentDirectory ( path );

    TRACE("Inserito percorso <%s>\n", path);

    //Begin the search
    //SearchFolder( path );

    // free memory used
    IMalloc * imalloc = 0;
    if ( SUCCEEDED( SHGetMalloc ( &imalloc )) )
    {
      imalloc->Free ( pidl );
      imalloc->Release ( );
    }
  }
  */





  
  BROWSEINFO bi;
	CString temppath("");
	
	::ZeroMemory(&bi,sizeof(BROWSEINFO));
	
	LPITEMIDLIST pidlRoot = NULL;
	LPITEMIDLIST lpidl = NULL;
  LPMALLOC pMalloc = NULL;
  SHGetMalloc(&pMalloc);

	//prepare to show folder-selecting dialog
	TCHAR szDisplayName[MAX_PATH];
	bi.pszDisplayName = szDisplayName;
	bi.pidlRoot = pidlRoot;
	bi.hwndOwner = ::GetActiveWindow();
//	bi.lpszTitle = _T( title );
	bi.lpszTitle = title;
	bi.ulFlags = 0;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_BROWSEFORCOMPUTER;
	bi.lpfn = NULL;
	bi.lParam = 0;

	
	lpidl = SHBrowseForFolder(&bi); //show the dialog
	
  if (lpidl== NULL) // annullato dall'utente
	{
    return 99; 
    //TRACE("Cancellato dall'utente\n");
    TRACE( L"Cancelled by user\n");
  }

  //get the selected directory
  SHGetPathFromIDList(lpidl,temppath.GetBuffer(_MAX_PATH));
  temppath.ReleaseBuffer();

  if (pidlRoot) pMalloc->Free(pidlRoot);

  pMalloc->Free(lpidl);
  pMalloc->Release();
  //return temppath;

  ZeroMemory( path, 256 );
  //strcpy( path, temppath );
  wcscpy( path, temppath );

//  TRACE("Inserito percorso <%s> come <%s>\n", temppath, title );
  TRACE( L"Selected path <%s> come <%s>\n", temppath, title );
  return 0;
  
}




int CPassword_manager_v2::encrypt( BYTE* ptr, UINT lng )
{
  ASSERT( AfxIsValidAddress(ptr,lng) );
  int lk=strlen(key);
  for ( UINT i=0 ; i<lng ; i++ ) ptr[i]=(ptr[i]^key[i%lk])+3*key[i%lk];
  return 0;
}

int CPassword_manager_v2::decrypt( BYTE* ptr, UINT lng )
{
  ASSERT( AfxIsValidAddress(ptr,lng) );
  int lk=strlen(key);
  for ( UINT i=0 ; i<lng ; i++ ) ptr[i]=(ptr[i]-3*key[i%lk])^key[i%lk];
  return 0;
}


int CPassword_manager_v2::Trace_data(void)
{
  TRACE( "password database - %d users - ", usrdb.num_users );
  for ( UINT i=0 ; i<usrdb.num_users[0] ; i++ ) TRACE( L"usr[%d] <%s,%s,%d> ", i, usrdb.usr[i].usr, usrdb.usr[i].pwd, usrdb.usr[i].lev );
  TRACE( "\r\n" );
  return 0;
}


int CPassword_manager_v2::Save_data(void)
{
  CFile fp; 
  CFileException ex;

  if ( !fp.Open( CONFIG_FILE_NAME , CFile::modeWrite|CFile::modeCreate|CFile::typeBinary, &ex ) )
  {
//    TRACE("CPassword_manager_v2::Save_data file <%s> doesnt exist - Can't save data\n", CONFIG_FILE_NAME );
    TRACE( L"CPassword_manager_v2::Save_data file <%s> doesnt exist - Can't save data\n", CONFIG_FILE_NAME );
    ex.ReportError();
    return 99;
  }

  encrypt ( (BYTE*)&usrdb, sizeof(usrdb) );
  fp.Write( (BYTE*)&usrdb, sizeof(usrdb) );
//  TRACE("cifrato e scritto <usrdb> - %d bytes\n", sizeof(usrdb) );
  TRACE( L"SAVE_DATA: encrypted and written <usrdb> - %d bytes\n", sizeof(usrdb) );
  decrypt ( (BYTE*)&usrdb, sizeof(usrdb) );
  Trace_data();

//  fp.Write( input_folder,  MAX_LENGHT_PATH );
//  fp.Write( output_folder, MAX_LENGHT_PATH );

  fp.Close();

  fp.Flush();


  #if !defined _DEBUG   // Copy to other PC

  #ifndef TIVS_S40
    if ( IS_PC1 )
    {
      CString source, dest;
      source.Format( L"%s\\%s", MAIN_DIRECTORY, CONFIG_FILE_NAME );
      //    dest.Format( L"%s\\%s", L"\\\\N56\\c$\\IVS", CONFIG_FILE_NAME );
      dest.Format( L"%s\\%s", L"\\\\192.168.2.200\\d\\IVS", CONFIG_FILE_NAME );

      if ( !CopyFile( source, dest, false) )
      {
        CString txt;
        txt.Format( L"Unable to copy config.ini file to PC2\r\nError n.%d", GetLastError() );
        Message_box( txt, MBOX_ERROR );
        return 99;
      }
      else TRACE(L"config.ini copied from PC1 to PC2\n");
    }
	#endif


	  #ifndef TIVS_S60
    if ( IS_PC1 )
    {
      CString source, dest;
      source.Format( L"%s\\%s", MAIN_DIRECTORY, CONFIG_FILE_NAME );
      //    dest.Format( L"%s\\%s", L"\\\\N56\\c$\\IVS", CONFIG_FILE_NAME );
      dest.Format( L"%s\\%s", L"\\\\192.168.2.200\\d\\IVS", CONFIG_FILE_NAME );

      if ( !CopyFile( source, dest, false) )
      {
        CString txt;
        txt.Format( L"Unable to copy config.ini file to PC2\r\nError n.%d", GetLastError() );
        Message_box( txt, MBOX_ERROR );
        return 99;
      }
      else TRACE(L"config.ini copied from PC1 to PC2\n");
    }
	#endif


	#endif


  return 0;
}




int CPassword_manager_v2::Load_data(void)
{
  CFile fp; 
  CFileException ex;

  if ( !fp.Open( CONFIG_FILE_NAME , CFile::modeRead|CFile::typeBinary, &ex ) )
  {
    //TRACE("CPassword_manager_v2::Load_data file <%s> doesnt exist - Can't load data\n", CONFIG_FILE_NAME );
    TRACE( L"CPassword_manager_v2::Load_data file <%s> doesnt exist - Can't load data\n", CONFIG_FILE_NAME );
    ex.ReportError();
    return 99;
  }

  if ( fp.GetLength() != dimdata ) // wrong file size
  {
//    TRACE("CPassword_manager_v2::Load_data file <%s> wrong size - Can't load data\n", CONFIG_FILE_NAME );
    TRACE( L"CPassword_manager_v2::Load_data file <%s> wrong size - Can't load data\n", CONFIG_FILE_NAME );
    return 109;
  }


  fp.Read( (BYTE*)&usrdb, sizeof(usrdb) );
  decrypt( (BYTE*)&usrdb, sizeof(usrdb) );
//  TRACE("letto e decifrato <usrdb> - %d bytes\n", sizeof(usrdb) );
  TRACE( L"LOAD_DATA: read and decrypted <usrdb> - %d bytes\n", sizeof(usrdb) );
  Trace_data();

  //for ( UINT i=0 ; i<usrdb.num_users ; i++ ) TRACE( L"usr[%d] <%s,%s,%d> ", i, usrdb.usr[i].usr, usrdb.usr[i].pwd, usrdb.usr[i].lev );

/*  ZeroMemory( input_folder, MAX_LENGHT_PATH );
  fp.Read( input_folder, MAX_LENGHT_PATH );

  ZeroMemory( output_folder, MAX_LENGHT_PATH );
  fp.Read( output_folder, MAX_LENGHT_PATH );  */

  fp.Close();

  logout();


//  TRACE("CPassword_manager_v2::Load_data file <%s> read\n", CONFIG_FILE_NAME );
  //TRACE( L"CPassword_manager_v2::Load_data file <%s> read\n", CONFIG_FILE_NAME );

  return 0;
}
