// Manual_jog.cpp : file di implementazione
//

#include "stdafx.h"
#include "Manual_jog.h"
#include "afxdialogex.h"


// finestra di dialogo CManual_jog
extern UINT  lng;  // Lingua attuale

UINT __cdecl manualjog_3d( LPVOID pParam );

int            run3d  = FALSE;
CWnd*          wndpos =  NULL;
ADS_data_type* actpos =  NULL;

IMPLEMENT_DYNAMIC(CManual_jog, CDialog)

CManual_jog::CManual_jog(CWnd* pParent /*=NULL*/) : CDialog(CManual_jog::IDD, pParent)
{

  run3d  = FALSE;
  wndpos =  NULL;
  actpos =  NULL;

  selected_axis   =     0; // no axis selected initially
  jogplus_pushed  = FALSE;
  jogminus_pushed = FALSE;

}

CManual_jog::~CManual_jog()
{

  ADS_write_byte( &ADS_manual_controls, FALSE );
  ADS_write_byte( &ADS_jog_fwd,         FALSE );
  ADS_write_byte( &ADS_jog_rev,         FALSE );
  ADS_write_byte( &ADS_homing,          FALSE );
  ADS_write_byte( &ADS_jog_rev,         FALSE );

  // Clean ADS part
  ADS_clean_parameter( &ADS_axis_nr         );
  ADS_clean_parameter( &ADS_jog_fwd         );
  ADS_clean_parameter( &ADS_jog_rev         );
  ADS_clean_parameter( &ADS_homing          );
  ADS_clean_parameter( &ADS_stop            );
  ADS_clean_parameter( &ADS_homing_pending  );
  ADS_clean_parameter( &ADS_axis_pos        );
  ADS_clean_parameter( &ADS_jog_possible    );
  ADS_clean_parameter( &ADS_manual_controls );


}

void CManual_jog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CManual_jog, CDialog)
  ON_BN_CLICKED(IDC_AXIS, &CManual_jog::OnBnClickedAxis)
  ON_BN_CLICKED(IDC_STOP, &CManual_jog::OnBnClickedStop)
  ON_BN_CLICKED(IDC_HOMING, &CManual_jog::OnBnClickedHoming)
END_MESSAGE_MAP()


// gestori di messaggi CManual_jog




BOOL CManual_jog::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_LBUTTONDOWN )
	{
    //TRACE("WM_LBUTTONDOWN in (%d,%d)\n", pMsg->pt.x, pMsg->pt.y );

    CRect area;

    GetDlgItem(IDC_JOGPLUS)->GetWindowRect( &area );
    if ( area.PtInRect(pMsg->pt) )
    {
      ADS_write_byte( &ADS_jog_fwd,  TRUE );
      ADS_write_byte( &ADS_jog_rev, FALSE );
      jogplus_pushed = TRUE;
      TRACE("PREMUTO JOGPLUS\n" );
    }

    GetDlgItem(IDC_JOGMINUS)->GetWindowRect( &area );
    if ( area.PtInRect(pMsg->pt) )
    {
      ADS_write_byte( &ADS_jog_fwd, FALSE );
      ADS_write_byte( &ADS_jog_rev,  TRUE );
      jogminus_pushed = TRUE;
      TRACE("PREMUTO JOGMINUS\n" );
    }

  }

	if ( pMsg->message==WM_LBUTTONUP )
	{
//    TRACE("WM_LBUTTONUP in (%d,%d)\n", pMsg->pt.x, pMsg->pt.y );
//    TRACE("RILASCIATO\n", pMsg->pt.x, pMsg->pt.y );

    if ( jogplus_pushed )
    {
      ADS_write_byte( &ADS_jog_fwd, FALSE );
      ADS_write_byte( &ADS_jog_rev, FALSE );
      jogplus_pushed = FALSE;
      TRACE("RILASCIATO JOGPLUS\n" );
    }

    if ( jogminus_pushed )
    {
      ADS_write_byte( &ADS_jog_fwd, FALSE );
      ADS_write_byte( &ADS_jog_rev, FALSE );
      jogminus_pushed = FALSE;
      TRACE("RILASCIATO JOGMINUS\n" );
    }

  }

  return CDialog::PreTranslateMessage(pMsg);

}


void CManual_jog::OnBnClickedAxis()
{
  //TRACE("PREMUTO Axis\n" );

  if ( selected_axis==12 ) selected_axis=1;
  else                     selected_axis++;

  ADS_write_byte( &ADS_axis_nr, selected_axis );

  GetDlgItem(IDC_AXIS)->SetWindowTextA(axis_name[selected_axis*3+lng]);

}

void CManual_jog::OnBnClickedStop()
{
//  TRACE("PREMUTO Stop\n" );

  ADS_write_byte( &ADS_stop,     TRUE );
  ADS_write_byte( &ADS_homing,  FALSE );
  ADS_write_byte( &ADS_jog_fwd, FALSE );
  ADS_write_byte( &ADS_jog_rev, FALSE );

}



void CManual_jog::OnBnClickedHoming()
{
//  TRACE("PREMUTO Homing\n" );

  ADS_write_byte( &ADS_stop,    FALSE );
  ADS_write_byte( &ADS_homing,   TRUE );
  ADS_write_byte( &ADS_jog_fwd, FALSE );
  ADS_write_byte( &ADS_jog_rev, FALSE );
}



BOOL CManual_jog::OnInitDialog()
{
  CDialog::OnInitDialog();

  GetDlgItem(IDC_AXIS)->SetWindowTextA(axis_name[selected_axis*3+lng]);

  GetDlgItem(IDOK)->SetWindowTextA(string_432[lng]);

  GetDlgItem(IDC_JOGPLUS )->SetWindowTextA(string_1060[lng]);
  GetDlgItem(IDC_JOGMINUS)->SetWindowTextA(string_1062[lng]);
  GetDlgItem(IDC_HOMING  )->SetWindowTextA(string_1064[lng]);
  GetDlgItem(IDC_STOP    )->SetWindowTextA(string_1066[lng]);


  ADS_init_parameter( ".M_AXIS_NR",           &ADS_axis_nr,          1 );
  ADS_init_parameter( ".M_JOG_FWD",           &ADS_jog_fwd,          1 );
  ADS_init_parameter( ".M_JOG_REV",           &ADS_jog_rev,          1 );
  ADS_init_parameter( ".M_HOMING",            &ADS_homing,           1 );
  ADS_init_parameter( ".M_STOP",              &ADS_stop,             1 );
  ADS_init_parameter( ".M_HOME_SEARCH",       &ADS_homing_pending,   1 );
  ADS_init_parameter( ".M_AXIS_POS",          &ADS_axis_pos,         4 );
  ADS_init_parameter( ".M_MAINTENANCE_READY", &ADS_jog_possible,     1 );
  ADS_init_parameter( ".M_POPUP_ACTIVE",      &ADS_manual_controls,  1 );

  if ( ADS_status && *((BOOL*)(ADS_jog_possible.val)) == FALSE ) // condizione di uscita
  {
    run3d = FALSE;
    Message_box( string_1052[lng], MBOX_ERROR );
    EndDialog(IDCANCEL);
    return TRUE;
  } 

  ADS_write_byte( &ADS_manual_controls, TRUE );

  wndpos = GetDlgItem(IDC_AXIS_TEXT);
  wndpos->SetWindowTextA("");

  actpos = &ADS_axis_pos;

  AfxBeginThread( manualjog_3d, 0 );
  return TRUE;  // return TRUE unless you set the focus to a control
  // ECCEZIONE: le pagine delle proprietà OCX devono restituire FALSE
}





BOOL CManual_jog::DestroyWindow()
{
  // TODO: aggiungere qui il codice specifico e/o chiamare la classe base
  TRACE("DestroyWindow\n");
  run3d = FALSE;
  Sleep(500);

  return CDialog::DestroyWindow();
}



UINT __cdecl manualjog_3d( LPVOID pParam )
{
  char txt[200];
  UINT cycles = 0;
  run3d = TRUE;

  TRACE("manualjog_3d avviato\n" );

  while ( run3d )
  {
    Sleep( 300 );
    sprintf( txt, string_1054[lng], ADS_read_int( actpos ) );
    wndpos->SetWindowTextA( txt );
  }


  TRACE("manualjog_3d terminato\n" );
  return 0;
}

