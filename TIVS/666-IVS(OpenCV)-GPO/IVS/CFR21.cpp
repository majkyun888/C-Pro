
#include "stdafx.h"

#include "CFR21.h"
#include "VSQK.h"
#include "Input_string.h"
#include "File_crypt.h"
#include "ChildView.h"

Tipo_timer timer_log;
char User_str[MAX_LOG_STRING_LENGTH] = ""; 


CCFR21::CCFR21(CWnd* pParent)	: CDialog(CRecipe_manager::IDD, pParent)
{
  TRACE ("<Apertura file>\n");

  ASSERT( AfxIsValidString( CFR21_NOME_FILE ) );

  dim_data = CFR21_NUM_USER * ( CFR21_MAXLUNGH_USER+1 + CFR21_MAXLUNGH_PW+1 + sizeof(int) );

  num_users      =  0;
  
  for (int i=0; i<NUM_LIV; i++)
    RadioBtn[i] = NULL;
  RichEdit = NULL;
  for (int i=0; i<CFR21_NBUTT; i++)
    button[i] = NULL;

  current_user   = -1;
  Create ( IDD_CFR21_DIALOG );

  ZeroMemory( User_DB, dim_data );

// Utente base
  strncpy( User_DB[num_users].name,  "ulow", CFR21_MAXLUNGH_USER );
  strncpy( User_DB[num_users].passw, "low",   CFR21_MAXLUNGH_PW );
  User_DB[num_users].level = CFR21_LOW_LEVEL;
  num_users++;

// Utente intermedio
  strncpy( User_DB[num_users].name,  "umedium", CFR21_MAXLUNGH_USER );
  strncpy( User_DB[num_users].passw, "medium",   CFR21_MAXLUNGH_PW );
  User_DB[num_users].level = CFR21_MEDIUM_LEVEL;
  num_users++;

// Utente avanzato
  strncpy( User_DB[num_users].name,  "uhigh", CFR21_MAXLUNGH_USER );
  strncpy( User_DB[num_users].passw, "high",   CFR21_MAXLUNGH_PW );
  User_DB[num_users].level = CFR21_HIGH_LEVEL;
  num_users++;

// Utente admin
  strncpy( User_DB[num_users].name,  "phm", CFR21_MAXLUNGH_USER );
  strncpy( User_DB[num_users].passw, "phm", CFR21_MAXLUNGH_PW );
  User_DB[num_users].level = CFR21_ADMIN_LEVEL;
  num_users++;

  Load_data();
}


CCFR21::~CCFR21()
{
  //Salvo e distruggo
  Save_data();

  for (int i=0; i<CFR21_NBUTT; i++)
    if ( button[i] ) 
      delete button[i];

    for (int i=0; i<NUM_LIV; i++)
      if ( RadioBtn[i] )
        delete RadioBtn[i];

  if (RichEdit) delete RichEdit;
  
  if ( m_hWnd ) 
    DestroyWindow();

TRACE("Distruzione CChildView completata\n");
}

//Definizione degli eventi
BEGIN_MESSAGE_MAP(CCFR21, CDialog)
  ON_WM_CREATE()
  ON_BN_CLICKED( ID_LOGIN_ENTRY,  &CCFR21::OnLoginFunction)
  ON_BN_CLICKED( ID_LOGIN_EXIT,   &CCFR21::OnLoginExit)
  ON_BN_CLICKED( ID_LOGIN_MODIFY, &CCFR21::OnLoginModify)
  ON_BN_CLICKED( ID_LOGIN_DELETE, &CCFR21::OnLoginDelete)
  ON_BN_CLICKED( ID_LOGIN_LOGOUT, &CCFR21::OnLoginLogout)
  ON_BN_CLICKED( ID_EXPORT_LOG,   &CCFR21::OnExportLog)
END_MESSAGE_MAP()


void CCFR21::OnLoginFunction()
{
TRACE ("<Login Button pressed>\n");

RegolaAccessi Utente;

strcpy(Utente.name,helpvar[0]);
strcpy(Utente.passw,helpvar[1]);

SearchUser2Log(Utente);

}

void CCFR21::OnLoginExit()
{
  for (int i=0; i<NUM_LIV; i++)
    if ( RadioBtn[i] )
    {
      delete RadioBtn[i];  
      RadioBtn[i] = NULL;
    }

  if ( m_hWnd ) DestroyWindow();

} 


void CCFR21::OnLoginModify()
{
if ( PreOperation( CFR21_ADMIN_LEVEL, "MODIFY" )) return; 

TRACE ("<Modify button pressed>\n");

//Prendo i valori dei tre bottoni e modifico il DB

if (strcmp(helpvar[2],""))
    strcpy(User_DB[val].name, helpvar[2]);
if (strcmp(helpvar[3],""))
    strcpy(User_DB[val].passw,helpvar[3]);

for (int i=1;i<NUM_LIV;i++)
  if (RadioBtn[i-1]->GetState())
    User_DB[val].level=i;

TRACE("Utente %d modificato\n", val);
Message_box("User modified correctly.", MBOX_INFO);

}

int CCFR21::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)	return -1;
  
  CRect login_re; 
	GetWindowRect( &login_re );        // leggo le coordinate
	login_re.OffsetRect( 0, 0 );
	MoveWindow( &login_re );

  ShowWindow( SW_SHOW );

  if (current_user == -1) //creazione della dialog principale
  {
	  RECT cll_area, area;
	  CString capt;

      GetClientRect( &cll_area );

      area.left   = cll_area.left  + 10;
		  area.right  = cll_area.right - 10;
		  area.top    = + 10;
		  area.bottom = area.top      + 35;
 
		  strcpy(helpvar[0],"Username");

      button[0] = new CButton;
      button[0]->Create( helpvar[0], WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, area, this, NID_CFR21); //ident

      area.left   = cll_area.left  + 10;
		  area.right  = cll_area.right - 10;
		  area.top    = 55;
		  area.bottom = area.top      + 35;

      strcpy(helpvar[1],"Password");

      button[1] = new CButton;
      button[1]->Create( helpvar[1], WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, area, this, NID_CFR21+1);
  }
  
  return 0;
}

BOOL CCFR21::OnCommand(WPARAM wParam, LPARAM lParam)
{
  //TRACE( "OnCommand WPARAM=%d - LPARAM=%d\n", wParam, lParam );

if (LOWORD(wParam)<NID_CFR21+CFR21_NBUTT+CFR21_TEXT_BTN)
{
  for (int i=0; i< CFR21_NBUTT+2; i++) 
  {
    //TRACE ("LOWORD(wParam) %d -- NID_CFR21+i %d\n", LOWORD(wParam), NID_CFR21+i);
    if (LOWORD(wParam)==NID_CFR21+i)
    {
      strcpy(helpvar[i],""); //parto con la stringa vuota
      InsertString(i);
      return 0;
    }
  }
  //ShowWindow( SW_SHOW );
}
 else
{
  for (int i=0; i<4; i++)
  {
    if (LOWORD(wParam)==NID_CFR21+4+i)
      RadioBtn[i]->SetState(TRUE);
    else RadioBtn[i]->SetState(FALSE);
    }
}

return CDialog::OnCommand(wParam, lParam);
}

int CCFR21::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult  )
{
  //TRACE( "OnNotify WPARAM=%d - LPARAM=%d\n", wParam, lParam );
  if ( spin && spin->m_hWnd )
  {
    spin = (CSpinButtonCtrl*) GetDlgItem( IDC_SPIN1 );
    spin_text0 = (CStatic*) GetDlgItem( IDC_SPIN_TEXT );

    val = spin->GetPos32();
    strcpy(helpvar[2],"");
    strcpy(helpvar[3],"");

    spin_text0 = (CStatic*) GetDlgItem( IDC_SPIN_TEXT );
    CString text0;
    text0.Format( "%d/%d", val+1, CFR21_NUM_USER );
    spin_text0->SetWindowText( text0 );
    button[2]->SetWindowTextA( User_DB[val].name );
    button[3]->SetWindowTextA( User_DB[val].passw );

    for (int i=1; i<NUM_LIV+1; i++)
      if (User_DB[val].level==i)
        RadioBtn[i-1]->SetState(TRUE);
      else RadioBtn[i-1]->SetState(FALSE);
  }

  if ( spin_text0 ) delete spin_text0;

  return( CWnd::OnNotify(wParam, lParam, pResult ) );
}

void CCFR21::InsertString(int ind)
{
  //ind=0 --> Username   ind=1-->Password
  strcpy(helpvar[ind],"");
  
  CInput_string stringpad( this, helpvar[ind], CFR21_MAXLUNGH_USER, ind==1?CINPSTR_PASSWORD:0 ); // parent, stringa iniziale, massima lunghezza
 
  if ( stringpad.RunModalLoop() == IDOK )
	{
		ASSERT( AfxIsValidString(stringpad.buffer) );
		ASSERT( strlen(stringpad.buffer)<=CFR21_MAXLUNGH_USER );
		strncpy( helpvar[ind], stringpad.buffer, CFR21_MAXLUNGH_USER );
	}

  if ( ind==1)
  {
    CString txt( '*', strlen(helpvar[ind]) );
    button[ind]->SetWindowText( txt ); 
  }
  else button[ind]->SetWindowText( helpvar[ind] ); 
}

int CCFR21::Load_data(void)
{
  CFile_crypt fp;

  if ( !fp.Open(  CFR21_NOME_FILE, CFile::modeRead , &ex ) )
	{
		ex.GetErrorMessage(msg_err,1024);
    return 99;
	}

  if ( fp.Read() )
	{
    CString err;
    err.Format("Unable to read file <%s>:\r\n<%s>", CFR21_NOME_FILE, fp.msg_error );
		Message_box( err, MBOX_ERROR);
    return (99);
	}

  if (fp.GetLength()!=0)
  {
    ZeroMemory( User_DB, dim_data);
    fp.Get_data( User_DB, dim_data );
  }

  fp.Close();

  return 0;
}

int CCFR21::Save_data(void)
{
  //riscrivo l'intero array nel file
  CFile_crypt fp;

  if ( !fp.Open( CFR21_NOME_FILE, CFile::modeCreate|CFile::modeWrite, &ex ) )
	{
		Message_box ("Error opening file", MBOX_ERROR);
    return 99;
	}  
  
  fp.Add_data( User_DB, dim_data );

  fp.Write();

  fp.Flush();
  fp.Close();

  return 0;
}

int CCFR21::SearchUser2Log(RegolaAccessi User2Search)
{
  BOOL trovato=false;
  UINT i;
  //scorro l'array
  for ( i=0; i<num_users; i++)
  {
    if ( strcmp(User_DB[i].name,User2Search.name)==0 && strcmp(User_DB[i].passw,User2Search.passw)==0 )
    {
      trovato=true;
      break;
    }
  }

  if ( trovato )
  {
   // if (User_DB[i].level>=(UINT)LevelAsked)
    //{
      Message_box("User found. Login performed.",MBOX_INFO);
      strcpy(User_str,User_DB[i].name);
      current_user = i;
      timer_log.reset(); //tempo 0 al momento del log
      if ( m_hWnd ) DestroyWindow();
    //}
   // else Message_box("User level to low.", MBOX_ERROR);
  }
  else Message_box("User not found.", MBOX_ERROR);


return 0;
}


BOOL CCFR21::CheckAccess( int LevelReq )
{
    
  //if ( timer_log.elapsed()>= CFR21_AUTO_LOGOUT_TIME || current_user<0 ) //serve per l'inzio
  if ( current_user < 0 )
  {
    //current_user = -1 ; //slogga l'utente eventual looggato 
    Message_box("No user logged. Please Log in!", MBOX_INFO);  
    if ( m_hWnd ) DestroyWindow();
    Create ( IDD_CFR21_DIALOG );
  }
  else
  {
    //TRACE("Current , Req %d, %d\n",User_DB[current_user].level,LevelReq);
    if ( (int)User_DB[current_user].level < LevelReq )  
    {
      Message_box("Log level to low", MBOX_ERROR);
      if ( m_hWnd ) DestroyWindow();
    }
    else return TRUE;
  }

  return false;
}

void CCFR21::OnLoginPressed()
{

  if ( m_hWnd ) DestroyWindow();
  if (!CheckAccess(LEVEL_UTILS))  return;

    Create ( IDD_CFR21_NEW );  
    
    spin = (CSpinButtonCtrl*) GetDlgItem( IDC_SPIN1 );
    ASSERT( spin );

    spin->SetRange32( 0, num_users-1 );
    spin->SetPos( current_user );

    spin_text0 = (CStatic*) GetDlgItem( IDC_SPIN_TEXT );
    ASSERT (spin_text0);
    CString text0;
    text0.Format( "%d/%d", current_user+1, CFR21_NUM_USER);
    spin_text0->SetWindowText( text0 );

      RECT cll_area, area;

      GetClientRect( &cll_area );

      area.left   = cll_area.left  + 100;
		  area.right  = cll_area.right - 30;
		  area.top    = + 30;
		  area.bottom = area.top      + 30;
  
      button[2] = new CButton;
      button[2]->Create( "Username", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, area, this, NID_CFR21+2); //ident

      area.left   = cll_area.left  + 100;
		  area.right  = cll_area.right - 30;
		  area.top    = + 70;
		  area.bottom = area.top      + 30;
  
      button[3] = new CButton;
      button[3]->Create( "Password", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, area, this, NID_CFR21+3); //ident

     CString Capt;
     for (int i=0; i<NUM_LIV; i++)
     {
       area.left   = cll_area.left  + 100 + i*85;
       area.right   = area.left  + 45 ;
		   area.top    = + 150;
		   area.bottom = area.top      + 35;
       
       Capt.Format("Lev: %d", i+1 );
       RadioBtn[i]= new CButton;
       RadioBtn[i]->Create(Capt, WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, area, this, NID_CFR21+4+i);    
     }

}

void CCFR21::OnLoginLogout()
{
  if ( timer_log.elapsed()<CFR21_AUTO_LOGOUT_TIME )
    if ( Message_box( "Really Logout?" , MBOX_YESNO ) != IDYES ) return;
  
  current_user = -1;
  strcpy ( User_str, NO_USER );
  if ( timer_log.elapsed()<CFR21_AUTO_LOGOUT_TIME )
    Message_box("Logout performed.", MBOX_INFO);
  else 
    Message_box("Logout performed by Autologout, time expired.", MBOX_INFO);

  //Close Window
  DestroyWindow();
}


void CCFR21::OnLoginDelete()
{
if ( PreOperation( CFR21_ADMIN_LEVEL, "DELETE" )) return; 

TRACE ("<Cliccato Elimina>\n");

//Shifto indietro di uno da quella posizione in poi, così si cancella quel valore
if (val==current_user) 
{
  Message_box("Impossible delete selected user", MBOX_ERROR);
  return;
}

for (UINT i=val; i<num_users-1; i++)
{
  User_DB[val].level=User_DB[val+1].level;
  strcpy(User_DB[val].name,User_DB[val+1].name);
  strcpy(User_DB[val].passw,User_DB[val+1].passw);
}
 if ((UINT)current_user>val) current_user--;
 num_users--;

TRACE("User %d deleted\n", val);
Message_box("User deleted/modified correctly", MBOX_INFO);

}


int CCFR21::Add_log( char* string )
{
  CFile_crypt fp; 

  ASSERT( AfxIsValidString( string ) );

  if ( strlen( string ) > MAX_LOG_STRING_LENGTH ) 
    string[MAX_LOG_STRING_LENGTH] = 0;

  //Aggiungo anche il nome utente
  char n_string[MAX_LOG_STRING_LENGTH+CFR21_MAXLUNGH_USER] ="";;
  strcpy  (n_string, User_str );
  strcat  (n_string, " ");
  strcat  (n_string, string);

  if ( !fp.Open( CFR21_NOME_FILE_LOG , CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate, &ex ) )
	{
		ex.GetErrorMessage(msg_err,1024);
    return 99;
	}

  int dim_file = (int)fp.GetLength();
  
  if ( dim_file > CFR21_MAX_FILE_LENGTH )
  {
    fp.RollingFile( CFR21_NOME_FILE_LOG );
  }

  fp.AppendString(n_string);

  fp.Close();
  
  return 0;
}


int CCFR21::Show_log( void )
{
if ( m_hWnd ) DestroyWindow();

  Create ( IDD_CFR21_SHOW );

  RECT cll_area, area;

  GetClientRect( &cll_area );
  
  area.left   = cll_area.left  + 10;
  area.right  = cll_area.right - 10;
	area.top    = + 50;
	area.bottom = area.top      + 400;
  
  AfxInitRichEdit( );

  RichEdit = new CRichEditCtrl;

  SYSTEMTIME time;
	GetSystemTime( &time );

  char FileName[25];

  if ( FileMode==0 )
    sprintf( FileName, "DayFile_%02d_%02d_%04d.dat", time.wDay, time.wMonth, time.wYear );
  else strcpy ( FileName, CFR21_NOME_FILE_LOG );

  CPaintDC dc(this);
  
  char sample_txt[MAX_LOG_STRING_LENGTH+30];
  sprintf( sample_txt,"%02d/%02d/%04d %02d:%02d:%02d %s", time.wDay, time.wMonth, time.wYear, time.wHour, time.wMinute, time.wSecond, CString( 'X', MAX_LOG_STRING_LENGTH ) );

  CSize Size = dc.GetTextExtent( sample_txt ); 
  
  RichEdit->Create( WS_CHILD|WS_VISIBLE|WS_BORDER|WS_HSCROLL|WS_VSCROLL|ES_MULTILINE|ES_AUTOHSCROLL|ES_AUTOVSCROLL , area, this, 0);

  CFile_crypt fp;
  if ( !fp.Open( FileName, CFile::modeRead, &ex ) )
	{
		Message_box ("Error opening file", MBOX_ERROR);
    return 99;
	}  

  fp.ReadLine( RichEdit );
  fp.Close();

  return 0;
}

void CCFR21::OnExportLog()
{
  TRACE ("<Premuto tasto Export>\n");

  CString ExpStr = RichEdit->GetSelText();

  CFile fp;
  if ( !fp.Open( CFR21_FILE_EXPORT, CFile::modeWrite|CFile::modeCreate, &ex ) )
	{
		  Message_box ("Error opening file", MBOX_ERROR);
    return;
	}  

  int dim_dati = strlen (ExpStr);
  if (dim_dati == 0) 
  {
    dim_dati = RichEdit->GetTextLength() -5;
    RichEdit->GetTextRange(0, dim_dati, ExpStr);
  }
  TRACE ("Selected string: %s", ExpStr);

  BYTE* buffer = (BYTE*) malloc ( dim_dati+1 ); 

  CopyMemory ( buffer, ExpStr, dim_dati );  
  buffer[dim_dati]=0;

  fp.Write ( buffer, dim_dati+1); 
  fp.Close();

  free (buffer);

  Message_box ( "Text Exported Correctly!", MBOX_INFO );
  
  if ( RichEdit ) 
  {
    delete RichEdit;
    RichEdit = NULL;
  }
  DestroyWindow();

}

int CCFR21::AddToDay( char* string)
{
  CFile_crypt fp; 

  ASSERT( AfxIsValidString( string ) );

  if ( strlen( string ) > MAX_LOG_STRING_LENGTH ) string[MAX_LOG_STRING_LENGTH] = 0;
  
  //Creo il nome del file
  char FileName[25];

  SYSTEMTIME time;
	GetSystemTime( &time );
  sprintf( FileName, "DayFile_%02d_%02d_%04d.dat", time.wDay, time.wMonth, time.wYear );

  //TRACE ("nome_file %s\n", FileName );

  if ( !fp.Open( FileName , CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate, &ex ) )
	{
		ex.GetErrorMessage(msg_err,1024);
    return 99;
	}

  fp.AppendString(string);

  fp.Close();
  
  return 0;
}


int CCFR21::PreOperation( int level, char* string)
{
if ( CheckAccess(level)==0 ) return 1;

  if (FileMode==0) 
    AddToDay( string );
  else Add_log( string );

return 0;

}
