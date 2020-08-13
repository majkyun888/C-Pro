// User_manager.cpp : implementation file
//

#include "stdafx.h"
#include "IVS.h"
#include "User_manager.h"
#include "afxdialogex.h"
#include "Def_globali.h"
#include "strings.h"
#include "Globals.h"
//Pour added 20171115
#include"Password_manager.h"
#include"Logger.h"
#include"afxwin.h"
extern CLogger*         logger;
//extern CLogger*       alarm_logger;
//Pour added 20171115

//#include "Password_manager_v2.h"

UCHAR max_num_users[MAX_LEVELS] = {MAX_NUM_USERS,MAX_NUM_OPERATOR,MAX_NUM_MAINTENANCE,MAX_NUM_SUPERVISOR};
//extern CPassword_manager_v2*  password_mngr;


#ifdef NEW_PASSW_MNGR  // New users/passwords manager
  extern CPassword_manager_v2*  password_mngr;
//#else
//  extern CPassword_manager*  password_mngr;
//#endif


// CUser_manager dialog

IMPLEMENT_DYNAMIC(CUser_manager, CDialog)

CUser_manager::CUser_manager(CWnd* pParent /*=NULL*/ ) : CDialog(CUser_manager::IDD, pParent)
{

  sel_index = 0;
  
  hide_password = TRUE;
  //hide_password = FALSE;//Pour marked for testing on 20200430

  // criteria for acceptance of password
  min_lenght = PWD_MINLUNGH_TXT; //6;   // minimun lenght
  max_lenght = PWD_MAXLUNGH_TXT;
  min_lower  = 1;   // minimun number of lower case characters
  min_upper  = 1;   // minimun number of upper case characters
  min_digit  = 0;   // minimun number of digits


  #if SERIAL_N==201400012 // TOT Biopharma requirements for passwords
    min_lenght = 8;   // minimun lenght
	max_lenght = 15;
    min_lower  = 1;   // minimun number of lower case characters
    min_upper  = 1;   // minimun number of upper case characters
    min_digit  = 1;   // minimun number of digits
  #endif


  //sel_index = password_mngr->usrdb.num_users;

}

CUser_manager::~CUser_manager()
{
}

void CUser_manager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUser_manager, CDialog)
  ON_WM_SHOWWINDOW()
  ON_NOTIFY(UDN_DELTAPOS, IDC_USER_SELECT, &CUser_manager::OnDeltaposUserSelect)
  ON_BN_CLICKED(IDC_SUPERVISOR, &CUser_manager::OnBnClickedSupervisor)
  ON_BN_CLICKED(IDC_MAINTENANCE, &CUser_manager::OnBnClickedMaintenance)
  ON_BN_CLICKED(IDC_OPERATOR, &CUser_manager::OnBnClickedOperator)
  ON_BN_CLICKED(IDC_NOLEVEL, &CUser_manager::OnBnClickedNolevel)
  ON_BN_CLICKED(ID_UPDATE, &CUser_manager::OnBnClickedUpdate)
  ON_BN_CLICKED(ID_ADD, &CUser_manager::OnBnClickedAdd)
  ON_BN_CLICKED(ID_DELETE, &CUser_manager::OnBnClickedDelete)
  ON_STN_CLICKED(IDC_USERNAME, &CUser_manager::OnStnClickedUsername)
  ON_STN_CLICKED(IDC_PASSWORD, &CUser_manager::OnStnClickedPassword)
  ON_BN_CLICKED(IDCANCEL, &CUser_manager::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUser_manager message handlers

/*
void CUser_manager::OnBnClickedOk2()
{
  TRACE("CUser_manager: OnOk\n");
  CDialog::OnOK();
  // TODO: Add your control notification handler code here
}
*/

void CUser_manager::OnDeltaposUserSelect(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  // TODO: Add your control notification handler code here

  //TRACE( "spin %d / %d\n", (pNMUpDown->iDelta), (pNMUpDown->iPos) );

//  sel_index = ( sel_index - pNMUpDown->iDelta ) % password_mngr->usrdb.num_users;
  int itemp = sel_index - pNMUpDown->iDelta;
//  sel_index -= pNMUpDown->iDelta;

  if ( itemp<0 ) itemp=password_mngr->usrdb.num_users[0]-1;
  if ( itemp>=int(password_mngr->usrdb.num_users[0]) ) itemp = 0;

  sel_index=itemp;

  Refresh();

  *pResult = 0;
}

void CUser_manager::Refresh( void )
{
  //ASSERT( sel_index>=0 && sel_index<password_mngr->usrdb.num_users );

  CString txt;
    if ( sel_index>=0 && sel_index<password_mngr->usrdb.num_users[0] )
  {

  txt.Format( string_3820[lng], sel_index+1, password_mngr->usrdb.num_users[0] );
  GetDlgItem( IDC_INDEX    )->SetWindowText( txt );

  GetDlgItem( IDC_TXTUSERNAME )->SetWindowText( string_3830[lng] );
  GetDlgItem( IDC_USERNAME    )->SetWindowText( password_mngr->usrdb.usr[sel_index].usr );

  GetDlgItem( IDC_TXTPASSWORD )->SetWindowText( string_3832[lng] );
  if ( hide_password )
  {
		// check if username is current user
//		if ((password_mngr->current_user == sel_index && sel_index != 3/*SUPERUSER_LEVEL*/)  /*|| (password_mngr->current_level == SUPERVISOR_LEVEL && sel_index != 3 )*/)
//	{
//	  GetDlgItem( IDC_PASSWORD )->SetWindowText( password_mngr->usrdb.usr[sel_index].pwd );
//	}
//	  else
//	  {
	  GetDlgItem( IDC_PASSWORD )->SetWindowText( L"**********" );
//	  }
  }
//  else
//  {
//    GetDlgItem( IDC_PASSWORD )->SetWindowText( password_mngr->usrdb.usr[sel_index].pwd );
//  }

    //Pour added 20180507
	//#ifdef CIRON_BATCH
	//GetDlgItem( IDC_SUPERVISOR  )->SetWindowText( L"ADMIN"  );
	//#endif
	//Pour added 20180507

/*  GetDlgItem( IDC_SUPERVISOR  )->SetWindowText( L"Supervisor level"  );
  GetDlgItem( IDC_MAINTENANCE )->SetWindowText( L"Maintenance level" );
  GetDlgItem( IDC_OPERATOR    )->SetWindowText( L"Operator level"    );
  GetDlgItem( IDC_NOLEVEL     )->SetWindowText( L"No access level"   ); */

  //current_level = password_mngr->usrdb.usr[sel_index].lev;
  sel_level = password_mngr->usrdb.usr[sel_index].lev;
  UpdateLevels();
	}
}

void CUser_manager::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CDialog::OnShowWindow(bShow, nStatus);

  //Pour added on 20200429
    if ( password_mngr && password_mngr->current_level>MAINTENANCE_LEVEL )
  {
    //if ( GetDlgItem(ID_ADD) && GetDlgItem(ID_ADD)->m_hWnd )
    //{
      //GetDlgItem(ID_ADD)-ShowWindow( SW_HIDE );
	  GetDlgItem(IDC_USERNAME)->EnableWindow(TRUE);
	  GetDlgItem(IDC_USERNAME)->RedrawWindow();
	  GetDlgItem( IDC_PASSWORD)->EnableWindow(TRUE);
	  GetDlgItem( IDC_PASSWORD)->RedrawWindow();
      GetDlgItem(ID_ADD)->EnableWindow(TRUE);
      GetDlgItem(ID_ADD)->RedrawWindow();
	  GetDlgItem(ID_DELETE)->EnableWindow(TRUE);
      GetDlgItem(ID_DELETE)->RedrawWindow();
	  GetDlgItem(IDC_SUPERVISOR)->EnableWindow(FALSE);//FALSE
      GetDlgItem(IDC_SUPERVISOR)->RedrawWindow();
	  GetDlgItem(IDC_MAINTENANCE)->EnableWindow(FALSE);//FALSE
      GetDlgItem(IDC_MAINTENANCE)->RedrawWindow();
	  GetDlgItem(IDC_OPERATOR)->EnableWindow(FALSE);//FALSE
      GetDlgItem(IDC_OPERATOR)->RedrawWindow();
	  GetDlgItem(IDC_NOLEVEL)->EnableWindow(FALSE);//FALSE
      GetDlgItem(IDC_NOLEVEL)->RedrawWindow();

    //}
  }
  else
  {
    //if ( GetDlgItem(ID_ADD) && GetDlgItem(ID_ADD)->m_hWnd )
    //{
      //GetDlgItem(ID_ADD)-ShowWindow( SW_HIDE );
	  GetDlgItem(IDC_USERNAME)->EnableWindow(FALSE);
	  GetDlgItem(IDC_USERNAME)->RedrawWindow();
	  GetDlgItem( IDC_PASSWORD)->EnableWindow(TRUE);
	  GetDlgItem( IDC_PASSWORD)->RedrawWindow();
      GetDlgItem(ID_ADD)->EnableWindow(FALSE);
      GetDlgItem(ID_ADD)->RedrawWindow();
	  GetDlgItem(ID_DELETE)->EnableWindow(FALSE);
      GetDlgItem(ID_DELETE)->RedrawWindow();
	  GetDlgItem(IDC_SUPERVISOR)->EnableWindow(FALSE);
      GetDlgItem(IDC_SUPERVISOR)->RedrawWindow();
	  GetDlgItem(IDC_MAINTENANCE)->EnableWindow(FALSE);
      GetDlgItem(IDC_MAINTENANCE)->RedrawWindow();
	  GetDlgItem(IDC_OPERATOR)->EnableWindow(FALSE);
      GetDlgItem(IDC_OPERATOR)->RedrawWindow();
	  GetDlgItem(IDC_NOLEVEL)->EnableWindow(FALSE);
      GetDlgItem(IDC_NOLEVEL)->RedrawWindow();

     //}
  }
  //Pour added on 20200429

  Refresh();

  // TODO: Add your message handler code here
}


void CUser_manager::UpdateLevels( void )
{
  CString txt;

  txt.Format( L">>> %s <<<", string_3810[lng] );
  ((CButton*) GetDlgItem( IDC_SUPERVISOR  ))->SetWindowText( sel_level==SUPERVISOR_LEVEL  ? txt:string_3810[lng] ); // >SetWindowText( L"Operator level"    );
  txt.Format( L">>> %s <<<", string_3812[lng] );
  ((CButton*) GetDlgItem( IDC_MAINTENANCE ))->SetWindowText( sel_level==MAINTENANCE_LEVEL ? txt:string_3812[lng] ); // >SetWindowText( L"Operator level"    );
  txt.Format( L">>> %s <<<", string_3814[lng] );
  ((CButton*) GetDlgItem( IDC_OPERATOR    ))->SetWindowText( sel_level==OPERATOR_LEVEL    ? txt:string_3814[lng] ); // >SetWindowText( L"Operator level"    );
  txt.Format( L">>> %s <<<", string_3816[lng] );
  ((CButton*) GetDlgItem( IDC_NOLEVEL     ))->SetWindowText( sel_level==NOACCESS_LEVEL    ? txt:string_3816[lng] ); // >SetWindowText( L"Operator level"    );*/

}


void CUser_manager::OnBnClickedSupervisor()
{
  sel_level = SUPERVISOR_LEVEL;
  TRACE("Selected SUPERVISOR level\n");
  UpdateLevels();
}


void CUser_manager::OnBnClickedMaintenance()
{
  sel_level = MAINTENANCE_LEVEL;
  TRACE("Selected MAINTENANCE level\n");
  UpdateLevels();
}


void CUser_manager::OnBnClickedOperator()
{
  sel_level = OPERATOR_LEVEL;
  TRACE("Selected OPERATOR level\n");
  UpdateLevels();
}


void CUser_manager::OnBnClickedNolevel()
{
  sel_level = NOACCESS_LEVEL;
  TRACE("Selected NOACCESS level\n");
  UpdateLevels();
}



BOOL CUser_manager::GetFormData()
{
  ((CEdit*) GetDlgItem( IDC_USERNAME ))->GetWindowText( usr, PWD_MAXLUNGH_TXT+1 );

  if ( hide_password ) 
  {
    // keep pwd, not *** shown
	((CEdit*) GetDlgItem( IDC_PASSWORD ))->GetWindowText( pwd, PWD_MAXLUNGH_TXT+1 );
  }
  else // can be read from form control
  {
    ((CEdit*) GetDlgItem( IDC_PASSWORD ))->GetWindowText( pwd, PWD_MAXLUNGH_TXT+1 );
  }
  return TRUE;
}


BOOL CUser_manager::CopyUserData( UINT from, UINT to )
{
  if ( from<0 || from>=password_mngr->usrdb.num_users[0] ) 
  {
    TRACE( "CopyUserData: ERROR <from> value <%d> unacceptable (0,%d)\n", from, password_mngr->usrdb.num_users );
    return FALSE; 
  }

  if ( to<0 || to>=password_mngr->usrdb.num_users[0] ) 
  {
    TRACE( "CopyUserData: ERROR <to> value <%d> unacceptable (0,%d)\n", to, password_mngr->usrdb.num_users );
    return FALSE; 
  }

  if ( from==to ) 
  {
    TRACE( "CopyUserData: ERROR <to> and <from> values cannot be equal <%d>\n", to );
    return FALSE; 
  }

  // indexes Ok, copy
  wcscpy_s( password_mngr->usrdb.usr[to].usr, PWD_MAXLUNGH_TXT+1, password_mngr->usrdb.usr[from].usr );
  wcscpy_s( password_mngr->usrdb.usr[to].pwd, PWD_MAXLUNGH_TXT+1, password_mngr->usrdb.usr[from].pwd );
  password_mngr->usrdb.usr[to].opentime = password_mngr->usrdb.usr[from].opentime;
  password_mngr->usrdb.usr[to].lev = password_mngr->usrdb.usr[from].lev;
  password_mngr->usrdb.usr[to].locked = password_mngr->usrdb.usr[from].locked;

  TRACE( "Copied user <%d> to <%d>\n", from, to );

  return TRUE;
}

// UPDATE CURRENT USER WITH DATA PRESENT IN THE FORM
void CUser_manager::OnBnClickedUpdate()
{
  // TODO: Add your control notification handler code here
  GetFormData();

    CString txt;
    if ( wcscmp( _wcslwr(usr), _wcslwr(password_mngr->usrdb.usr[sel_index].usr) ) )
    {
      txt.Format( string_3874[lng], usr, sel_index+1 );
      Message_box( txt, MBOX_ERROR );
      return;
    }

  	//Pour added for Aohong on 20191101
  if(password_mngr->current_level < SUPERVISOR_LEVEL)
  {
	if (password_mngr->current_user != sel_index )
	{
	   CString warning_txt;
	   warning_txt.Format( string_3846[lng]);
       Message_box( warning_txt, MBOX_ERROR );
	   return;
	}
	//Pour added for Aohong on 20191101

  //Pour marked on 20200429
	//if ( !wcscmp( _wcslwr(pwd), _wcslwr(password_mngr->usrdb.usr[sel_index].pwd) ) ) //neglect Caps of letters
	if ( !wcscmp( pwd, password_mngr->usrdb.usr[sel_index].pwd)  )
	{
		CString warning_txt;
	    warning_txt.Format( string_3844a[lng]);
        Message_box( warning_txt, MBOX_ERROR );
		return;
	}
	else
	{
	CString msg;
	msg.Format( string_3849[lng], password_mngr->usrdb.usr[sel_index].usr );
	if ( Message_box( msg, MBOX_YESNO )==IDYES )
	{
		wcscpy_s( password_mngr->usrdb.usr[sel_index].pwd, PWD_MAXLUNGH_TXT+1, pwd );
	}
	if ( logger ) 
	{
	CString msg; //wchar_t msg[100];
	msg.Format( L"User <%s> <%d> password updated", password_mngr->usrdb.usr[sel_index].usr,password_mngr->usrdb.usr[sel_index].lev );
	logger->Add_string( &msg );
	}
	}
   }

   //if(password_mngr->current_level >= SUPERVISOR_LEVEL)
   else
   {
	//if ( !wcscmp( _wcslwr(pwd), _wcslwr(password_mngr->usrdb.usr[sel_index].pwd) ) )
	if ( !wcscmp( pwd, password_mngr->usrdb.usr[sel_index].pwd ) )
	{
		CString warning_txt;
	    warning_txt.Format( string_3844a[lng]);
        Message_box( warning_txt, MBOX_ERROR );
		return;
	}
	else
	{
	CString msg;
	msg.Format( string_3849[lng], password_mngr->usrdb.usr[sel_index].usr );
	if ( Message_box( msg, MBOX_YESNO )==IDYES )
	{
	//wcscpy_s( password_mngr->usrdb.usr[sel_index].usr, PWD_MAXLUNGH_TXT, usr );
	wcscpy_s( password_mngr->usrdb.usr[sel_index].pwd, PWD_MAXLUNGH_TXT+1, pwd );

	//Pour added 20200426
	if ( logger ) 
	{
	CString msg; //wchar_t msg[100];
	//msg.Format( L"User <%s> password updated", password_mngr->usrdb.usr[sel_index].usr );
	msg.Format( L"User <%s> <%d> password updated", password_mngr->usrdb.usr[sel_index].usr,password_mngr->usrdb.usr[sel_index].lev );
	logger->Add_string( &msg );
	}
	//Pour added 20200426
	//Update also the expiry time for new password
	password_mngr->usrdb.usr[sel_index].opentime = time(NULL);
	}
	else
		return;

	TRACE(L"Update user[%d]: <%s><%s><%d>\n", sel_index, 
    password_mngr->usrdb.usr[sel_index].usr, 
    password_mngr->usrdb.usr[sel_index].pwd, 
    password_mngr->usrdb.usr[sel_index].lev );
	}
		//Pour marked on 20200429
  }
}

// ADD NEW USER AT THE END OF LIST WITH DATA PRESENT IN THE FORM
void CUser_manager::OnBnClickedAdd()
{
  CString txt;
  UINT i;
    //Pour added for Aohong on 20191101
	if ( password_mngr->current_level<=MAINTENANCE_LEVEL )
	{
	CString warning_txt;
	warning_txt.Format( string_3847[lng]);
    Message_box( warning_txt, MBOX_ERROR );
	return;
	}
	//Pour added for Aohong on 20191101

  if ( password_mngr->usrdb.num_users[0]>=MAX_NUM_USERS )
  {
    txt.Format( string_3840[lng], MAX_NUM_USERS );
    Message_box( txt, MBOX_ERROR );
    return;
  }

      if(password_mngr->usrdb.num_users[sel_level] >= max_num_users[sel_level])
  {
       Message_box( string_3840a[lng], MBOX_ERROR );
	   return;
  }
  
  GetFormData();

  // check if username already exists
  for ( i=0 ; i<password_mngr->usrdb.num_users[0] ; i++ )
  {
    if ( !wcscmp( _wcslwr(usr), _wcslwr(password_mngr->usrdb.usr[i].usr) ) )
    {
      txt.Format( string_3844[lng], usr, i+1 );
      Message_box( txt, MBOX_ERROR );
      return;
    }
  }

  /*
  // Ok, insert new user at current position
  password_mngr->usrdb.num_users++;

  // first move all current users one position further
  for ( i=password_mngr->usrdb.num_users-2 ; i>=sel_index && i>0 ; i-- )
  {
    CopyUserData( i, i+1 );
  }

  // and copy current data at sel_index
  GetFormData();
  wcscpy_s( password_mngr->usrdb.usr[sel_index].usr, PWD_MAXLUNGH_TXT, usr );
  wcscpy_s( password_mngr->usrdb.usr[sel_index].pwd, PWD_MAXLUNGH_TXT, pwd );
  password_mngr->usrdb.usr[sel_index].lev = current_level;
  */

  // insert at last position
  sel_index =  password_mngr->usrdb.num_users[0];

//  GetFormData();
  wcscpy_s( password_mngr->usrdb.usr[sel_index].usr, PWD_MAXLUNGH_TXT+1, usr );
  wcscpy_s( password_mngr->usrdb.usr[sel_index].pwd, PWD_MAXLUNGH_TXT+1, pwd );
  password_mngr->usrdb.usr[sel_index].lev = sel_level;
  password_mngr->usrdb.usr[sel_index].locked = FALSE;
  password_mngr->usrdb.num_users[0]++;
  password_mngr->usrdb.num_users[sel_level]++;
  //record the opening time
  password_mngr->usrdb.usr[sel_index].opentime = time(NULL);
  password_mngr->usrdb.usr[sel_index].lockedtime = 0;//Pour added on 20200728

  //Pour added 20171115
  if ( logger ) 
  {
   CString msg; //wchar_t msg[100];
   msg.Format( L"User <%s> <%d> created", password_mngr->usrdb.usr[sel_index].usr,password_mngr->usrdb.usr[sel_index].lev );
	//msg.Format( string_3818a[NLANGS], password_mngr->usrdb.usr[sel_index].usr );
   logger->Add_string( &msg );
   }
  //Pour added 20171115


  Refresh();

  // TODO: Add your control notification handler code here
}

// DELETE AND REMOVE CURRENT USER SHOWN IN THE FORM
void CUser_manager::OnBnClickedDelete()
{
  CString txt;
  UINT i;

      	//Pour added for Aohong on 20191101
	if ( password_mngr->current_level<=2 )
	{
	CString warning_txt;
	warning_txt.Format( string_3848[lng]);
    Message_box( warning_txt, MBOX_ERROR );
	return;
	}
	//Pour added for Aohong on 20191101

  if ( sel_index>=0 && sel_index<MAX_LEVELS )
  {
    txt.Format( string_3842[lng] );
    Message_box( txt, MBOX_ERROR );
    return;
  }
  //Pour on 20200629

    if ( password_mngr->current_level==3 )
  {
	if ( Message_box( string_1341[lng], MBOX_YESNO ) == IDYES )
	{

  // first move all current users one position back
  for ( i=sel_index ; i<password_mngr->usrdb.num_users[0] ; i++ )
  {
    CopyUserData( i+1, i );
  }
  //Pour added 20171115
  if ( logger ) 
  {
   CString msg; //wchar_t msg[100];
   msg.Format( L"User <%s> <%d> deleted", password_mngr->usrdb.usr[sel_index].usr,password_mngr->usrdb.usr[sel_index].lev );
	//msg.Format( string_3818b[NLANGS], password_mngr->usrdb.usr[sel_index].usr );
  logger->Add_string( &msg );
  }
  //Pour added 20171115

  //password_mngr->usrdb.num_users--;

      // first move all current users one position back?overwrite the user who will be removed
  for (i=sel_index ; i< password_mngr->usrdb.num_users[0]; i++)
  {
    CopyUserData( i+1, i );
  }
  // renew the count of db 
  password_mngr->usrdb.num_users[0]--;
  if( password_mngr->usrdb.usr[sel_index].lev )
	password_mngr->usrdb.num_users[password_mngr->usrdb.usr[sel_index].lev]--;
  if( sel_index >=  password_mngr->usrdb.num_users[0]) 
	  sel_index = password_mngr->usrdb.num_users[0]-1;
  // delete the last usr information
  wcscpy_s( password_mngr->usrdb.usr[i-1].usr, PWD_MAXLUNGH_TXT+1, L"" );
  wcscpy_s( password_mngr->usrdb.usr[i-1].pwd, PWD_MAXLUNGH_TXT+1, L"" );
  password_mngr->usrdb.usr[i-1].locked = FALSE;
  password_mngr->usrdb.usr[i-1].opentime = 0;
  password_mngr->usrdb.usr[i-1].lev = 0;


  Refresh();
  	}
	else
    return;
  }

}


void CUser_manager::OnStnClickedUsername()
{
  CInput_string stringpad;
  stringpad.password = 0; // ind==1;
  stringpad.Create( IDD_INPUT_STRING, AfxGetMainWnd() /*this*/ );
  stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( string_3850[lng] );

  if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>0 && wcslen(stringpad.buffer)<PWD_MAXLUNGH_TXT )
	{
		ASSERT( AfxIsValidString(stringpad.buffer) );
		ASSERT( wcslen(stringpad.buffer)<=PWD_MAXLUNGH_TXT+1 );//Pour marked on 20200430
		if(wcslen(stringpad.buffer)>PWD_MAXLUNGH_TXT+1) // confirm username length not too long
		{
			Message_box( string_3860a[lng], MBOX_ERROR);
			return;
		}

    ((CEdit*) GetDlgItem( IDC_USERNAME ))->SetWindowText( stringpad.buffer );
	}
}



// Check if password complies with requirements
int CUser_manager::Check_password( wchar_t* pwd )
{
  ASSERT( pwd && AfxIsValidString(pwd) );

  UINT len = wcslen(pwd);

  // check lenght first
  if ( len<min_lenght   || len> max_lenght) // password too short
  {
    msg_err.Format( string_3860[lng], len, min_lenght );
    return 70;
  }

  UINT i;
  UINT num_lower = 0;
  UINT num_upper = 0;
  UINT num_digit = 0;

  for ( i=0 ; i<len ; i++ )
  {
    if ( pwd[i]>='a' && pwd[i]<='z' ) num_lower++;
    else if ( pwd[i]>='A' && pwd[i]<='Z' ) num_upper++;
    else if ( pwd[i]>='0' && pwd[i]<='9' ) num_digit++;
	else
	{
		msg_err.Format(string_3868[lng]);
		return 80;
	}
  }

  TRACE( L"Password <%s> is composed by %d lower / %d upper / %d digits\n", pwd, num_lower, num_upper, num_digit );

  // check upper case characters
  if ( num_upper<min_upper ) // too few upper case characters
  {
    msg_err.Format( string_3862[lng], num_upper, min_upper );
    return 80;
  }

  // check lower case characters
  if ( num_lower<min_lower ) // too few lower case characters
  {
    msg_err.Format( string_3864[lng], num_lower, min_lower );
    return 90;
  }

  // check digits case characters
  if ( num_digit<min_digit ) // too few digits
  {
    msg_err.Format( string_3866[lng], num_digit, min_digit );
    return 100;
  }
  if ( !wcscmp(pwd, password_mngr->usrdb.usr[sel_index].pwd))
  {
    msg_err.Format(string_3844a[lng]);
    return 110;
  }


  return 0;
}


void CUser_manager::OnStnClickedPassword()
{
  CInput_string stringpad;

  if ( hide_password )
  {
    wchar_t txt1[PWD_MAXLUNGH_TXT+1];
    wchar_t txt2[PWD_MAXLUNGH_TXT+1];
   // wchar_t pwd [PWD_MAXLUNGH_TXT+1];
    wchar_t caption[200+1];
    wchar_t curr_usr[PWD_MAXLUNGH_TXT+1];

    ((CEdit*) GetDlgItem( IDC_USERNAME ))->GetWindowText( curr_usr, PWD_MAXLUNGH_TXT+1 );


    // read new password
    swprintf( caption, 200, string_1048[lng], curr_usr ); // prepare caption
    if ( password_mngr->read_string( txt1, caption, TRUE ) ) return;

    if ( Check_password( txt1 ) ) 
    {
      Message_box( msg_err, MBOX_ERROR );
      return;
    }

    swprintf( caption, 200, string_1049[lng], curr_usr ); // prepare caption
    if ( password_mngr->read_string( txt2, caption, TRUE ) ) return;

    if ( Check_password( txt2 ) ) 
    {
      Message_box( msg_err, MBOX_ERROR );
      return;
    }


    if ( wcslen(txt1)>0 && wcslen(txt2)>0 && !wcscmp( txt1, txt2 ) ) // equal
    {
	  ((CEdit*) GetDlgItem( IDC_PASSWORD ))->SetWindowText( txt1 );
      //wcscpy( pwd, txt1 );
      TRACE( L"confirmed password[%d]=<%s> lng=%d\n", sel_index, txt1, wcslen(txt1) );
    }
    else
    {
      Message_box( string_3854[lng], MBOX_ERROR );
      //return; // mismatch!
    }

  }
  else // show password
  {
    stringpad.password = 0; // ind==1;
    stringpad.Create( IDD_INPUT_STRING, AfxGetMainWnd() /*this*/ );
    stringpad.GetDlgItem( IDC_CURRENT_DESC )->SetWindowText( string_3852[lng] );

    if ( (stringpad.RunModalLoop()==IDOK) && wcslen(stringpad.buffer)>0 )
    {
      ASSERT( AfxIsValidString(stringpad.buffer) );
      ASSERT( wcslen(stringpad.buffer)<=PWD_MAXLUNGH_TXT );
      ((CEdit*) GetDlgItem( IDC_PASSWORD ))->SetWindowText( stringpad.buffer );
    }
  }
}




void CUser_manager::OnBnClickedCancel()
{
  // TODO: Add your control notification handler code here
//  if ( lng==ENGLISH && PathFileExists( AtoW(full_name) ) ) ShellExecute( NULL, L"open", AtoW(full_name), NULL, L"", SW_SHOWDEFAULT );

/*
  CString source, dest;
  source.Format( L"%s\\%s", MAIN_DIRECTORY, CONFIG_FILE_NAME );
  dest.Format( L"%s\\%s", L"\\\\N56\\c$\\IVS", CONFIG_FILE_NAME );

  if ( !CopyFile( source, dest, false) )
  {
    CString txt;
    txt.Format( L"Unable to copy config.ini file to PC2\r\nError n.%d", GetLastError() );
    Message_box( txt, MBOX_ERROR );
    return;
  }
*/

  CDialog::OnCancel();
}

#endif // #ifdef NEW_PASSW_MNGR  // New users/passwords manager
