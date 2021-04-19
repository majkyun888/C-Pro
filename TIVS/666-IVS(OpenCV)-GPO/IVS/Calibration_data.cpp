// Calibration_data.cpp : file di implementazione
//

#include "stdafx.h"
#include "Calibration_data.h"
#include "afxdialogex.h"


// finestra di dialogo Calibration_data

IMPLEMENT_DYNAMIC(Calibration_data, CDialog)

Calibration_data::Calibration_data(CWnd* pParent /*=NULL*/) : CDialog(Calibration_data::IDD, pParent)
{
  previous.gage_diam_1     =  0.0;
  previous.gage_diam_2     =  0.0;
  previous.gage_diam_3     =  0.0;

  previous.master_height_1 =  0.0;
  previous.master_height_2 =  0.0;
  //previous.master_height_3 =  0.0;

  previous.master_diam_1   =  0.0;
  previous.master_diam_2   =  0.0;
  previous.master_diam_3   =  0.0;

  current.gage_diam_1     =   0.0;
  current.gage_diam_2     =   0.0;
  current.gage_diam_3     =   0.0;
  
  current.master_height_1 =   0.0;
  current.master_height_2 =   0.0;
  //current.master_height_3 =   0.0;

  current.master_diam_1   =   0.0;
  current.master_diam_2   =   0.0;
  current.master_diam_3   =   0.0;


}

Calibration_data::~Calibration_data()
{
}



void Calibration_data::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Calibration_data, CDialog)
END_MESSAGE_MAP()


// gestori di messaggi Calibration_data


BOOL Calibration_data::OnInitDialog()
{
  CDialog::OnInitDialog();

  /*
  SetWindowTextA( "Calibration data" );
  GetDlgItem( IDC_CALIB_DESC0 )->SetWindowTextA( ""             );
  GetDlgItem( IDC_CALIB_DESC1 )->SetWindowTextA( "Gage diam 1"  );
  GetDlgItem( IDC_CALIB_DESC2 )->SetWindowTextA( "Gage diam 2"  );
  GetDlgItem( IDC_CALIB_DESC3 )->SetWindowTextA( "Gage diam 3"  );
  GetDlgItem( IDC_CALIB_DESC4 )->SetWindowTextA( "Height 1"     );
  GetDlgItem( IDC_CALIB_DESC5 )->SetWindowTextA( "Height 2"     );
  GetDlgItem( IDC_CALIB_DESC6 )->SetWindowTextA( "Height 3"     );
  GetDlgItem( IDC_CALIB_DESC7 )->SetWindowTextA( "Diameter 1"   );
  GetDlgItem( IDC_CALIB_DESC8 )->SetWindowTextA( "Diameter 2"   );
  GetDlgItem( IDC_CALIB_DESC9 )->SetWindowTextA( "Diameter 3"   );
  GetDlgItem( IDC_CALIB_G0A   )->SetWindowTextA( "Previous"     );
  GetDlgItem( IDC_CALIB_G0B   )->SetWindowTextA( "Current"      );
  GetDlgItem( IDC_CALIB_G0D   )->SetWindowTextA( "Difference"   );
  */

  SetWindowText( L"Calibration data" );
  GetDlgItem( IDC_CALIB_DESC0 )->SetWindowText( L""             );
  GetDlgItem( IDC_CALIB_DESC1 )->SetWindowText( L"Gage diam 1"  );
  GetDlgItem( IDC_CALIB_DESC2 )->SetWindowText( L"Gage diam 2"  );
  GetDlgItem( IDC_CALIB_DESC3 )->SetWindowText( L"Gage diam 3"  );
  GetDlgItem( IDC_CALIB_DESC4 )->SetWindowText( L"Height 1"     );
  GetDlgItem( IDC_CALIB_DESC5 )->SetWindowText( L"Height 2"     );
  GetDlgItem( IDC_CALIB_DESC6 )->SetWindowText( L"Height 3"     );
  GetDlgItem( IDC_CALIB_DESC7 )->SetWindowText( L"Diameter 1"   );
  GetDlgItem( IDC_CALIB_DESC8 )->SetWindowText( L"Diameter 2"   );
  GetDlgItem( IDC_CALIB_DESC9 )->SetWindowText( L"Diameter 3"   );
  GetDlgItem( IDC_CALIB_G0A   )->SetWindowText( L"Previous"     );
  GetDlgItem( IDC_CALIB_G0B   )->SetWindowText( L"Current"      );
  GetDlgItem( IDC_CALIB_G0D   )->SetWindowText( L"Difference"   );


  for ( int i=1 ; i<=9 ; i++ )
  {
    //GetDlgItem( IDC_CALIB_G0A+i )->SetWindowTextA( "-" );
    //GetDlgItem( IDC_CALIB_G0B+i )->SetWindowTextA( "-" );
    //GetDlgItem( IDC_CALIB_G0D+i )->SetWindowTextA( "-" );
    GetDlgItem( IDC_CALIB_G0A+i )->SetWindowText( L"-" );
    GetDlgItem( IDC_CALIB_G0B+i )->SetWindowText( L"-" );
    GetDlgItem( IDC_CALIB_G0D+i )->SetWindowText( L"-" );
  }


 // Read_ini();

  //Load_data();

  Show_data();

  //Save_data();


/*  CTime time = CTime::GetCurrentTime();   

  sprintf_s( buff, LOG_BUFF_SIZE, "%02d/%02d/%04d %02d:%02d:%02d: %s\r\n", time.GetDay(), time.GetMonth(), time.GetYear(), time.GetHour(), time.GetMinute(), time.GetSecond(), string_in );

  fp.SeekToEnd();
  fp.Write( buff, strlen(buff) ); */


 // return 0;




  // TODO:  Aggiungere qui ulteriori informazioni per l'inizializzazione

  return TRUE;  // return TRUE unless you set the focus to a control
  // ECCEZIONE: le pagine delle proprietà OCX devono restituire FALSE
}


int Calibration_data::Show_data(void)
{
  /*
  char txt[100];
  char frmt[] = "%.3lf";

  GetDlgItem( IDC_CALIB_DESC0 )->SetWindowTextA( ""                );
  GetDlgItem( IDC_CALIB_DESC1 )->SetWindowTextA( "Gage diam 1"     );
  GetDlgItem( IDC_CALIB_DESC2 )->SetWindowTextA( "Gage diam 2"     );
  GetDlgItem( IDC_CALIB_DESC3 )->SetWindowTextA( "Gage diam 3"     );
  GetDlgItem( IDC_CALIB_DESC4 )->SetWindowTextA( "Master diam 1"   );
  GetDlgItem( IDC_CALIB_DESC5 )->SetWindowTextA( "Master diam 2"   );
  GetDlgItem( IDC_CALIB_DESC6 )->SetWindowTextA( "Master diam 3"   );
  GetDlgItem( IDC_CALIB_DESC7 )->SetWindowTextA( "Master height 1" );
  GetDlgItem( IDC_CALIB_DESC8 )->SetWindowTextA( "Master height 2" );
  GetDlgItem( IDC_CALIB_DESC9 )->ShowWindow( SW_HIDE );


  // PREVIOUS DATA 

  // Gage diam 1-2-3
  sprintf( txt, frmt, previous.gage_diam_1 );
  GetDlgItem( IDC_CALIB_G1A )->SetWindowTextA( txt );
  sprintf( txt, frmt, previous.gage_diam_2 );
  GetDlgItem( IDC_CALIB_G2A )->SetWindowTextA( txt );
  sprintf( txt, frmt, previous.gage_diam_3 );
  GetDlgItem( IDC_CALIB_G3A )->SetWindowTextA( txt );

  // Master diameter 1-2-3
  sprintf( txt, frmt, previous.master_diam_1 );
  GetDlgItem( IDC_CALIB_G4A )->SetWindowTextA( txt );
  sprintf( txt, frmt, previous.master_diam_2 );
  GetDlgItem( IDC_CALIB_G5A )->SetWindowTextA( txt );
  sprintf( txt, frmt, previous.master_diam_3 );
  GetDlgItem( IDC_CALIB_G6A )->SetWindowTextA( txt );

  // Master height 1-2
  sprintf( txt, frmt, previous.master_height_1 );
  GetDlgItem( IDC_CALIB_G7A )->SetWindowTextA( txt );
  sprintf( txt, frmt, previous.master_height_2 );
  GetDlgItem( IDC_CALIB_G8A )->SetWindowTextA( txt );
//  sprintf( txt, frmt, previous.master_height_3 );
//  GetDlgItem( IDC_CALIB_G6A )->SetWindowTextA( txt );
  GetDlgItem( IDC_CALIB_G9A )->ShowWindow( SW_HIDE );


  // CURRENT DATA 

  // Gage diam 1-2-3
  sprintf( txt, frmt, current.gage_diam_1 );
  GetDlgItem( IDC_CALIB_G1B )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.gage_diam_2 );
  GetDlgItem( IDC_CALIB_G2B )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.gage_diam_3 );
  GetDlgItem( IDC_CALIB_G3B )->SetWindowTextA( txt );

  // Master diameter 1-2-3
  sprintf( txt, frmt, current.master_diam_1 );
  GetDlgItem( IDC_CALIB_G4B )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.master_diam_2 );
  GetDlgItem( IDC_CALIB_G5B )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.master_diam_3 );
  GetDlgItem( IDC_CALIB_G6B )->SetWindowTextA( txt );

  // Master height 1-2
  sprintf( txt, frmt, current.master_height_1 );
  GetDlgItem( IDC_CALIB_G7B )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.master_height_2 );
  GetDlgItem( IDC_CALIB_G8B )->SetWindowTextA( txt );
  //sprintf( txt, frmt, current.master_height_3 );
  //GetDlgItem( IDC_CALIB_G6B )->SetWindowTextA( txt );
  GetDlgItem( IDC_CALIB_G9B )->ShowWindow( SW_HIDE );
  
  // DIFFERENCES

  // Gage diam 1-2-3
  sprintf( txt, frmt, current.gage_diam_1-previous.gage_diam_1 );
  GetDlgItem( IDC_CALIB_G1D )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.gage_diam_2-previous.gage_diam_2 );
  GetDlgItem( IDC_CALIB_G2D )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.gage_diam_3-previous.gage_diam_3 );
  GetDlgItem( IDC_CALIB_G3D )->SetWindowTextA( txt );

  // Master diameter 1-2-3
  sprintf( txt, frmt, current.master_diam_1-previous.master_diam_1 );
  GetDlgItem( IDC_CALIB_G4D )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.master_diam_2-previous.master_diam_2 );
  GetDlgItem( IDC_CALIB_G5D )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.master_diam_3-previous.master_diam_3 );
  GetDlgItem( IDC_CALIB_G6D )->SetWindowTextA( txt );

  // Master height 1-2-3
  sprintf( txt, frmt, current.master_height_1-previous.master_height_1 );
  GetDlgItem( IDC_CALIB_G7D )->SetWindowTextA( txt );
  sprintf( txt, frmt, current.master_height_2-previous.master_height_2 );
  GetDlgItem( IDC_CALIB_G8D )->SetWindowTextA( txt );
//  sprintf( txt, frmt, current.master_height_3-previous.master_height_3 );
//  GetDlgItem( IDC_CALIB_G6D )->SetWindowTextA( txt );
  GetDlgItem( IDC_CALIB_G9D )->ShowWindow( SW_HIDE );

  */
  return 0;
}


int Calibration_data::Load_data(void)
{
  // provo a caricare i dati dell'ultima calibrazione
  CFile fp; 
  CFileException ex;

  if ( !fp.Open( CALIBRATION_FILE_NAME , CFile::modeRead, &ex ) )
  {
    ex.ReportError();
    return 99;

    // Try to get data from ini_file
    //Read_ini();
  }

  if ( fp.GetLength() != CALDATADIM ) // wrong file size
  {
    return 109;
  }

  fp.Read( &previous, CALDATADIM );

  fp.Close();
  return 0;

}

int Calibration_data::Save_data(void)
{
  // provo a caricare i dati dell'ultima calibrazione
  CFile fp; 
  CFileException ex;

  if ( !fp.Open( CALIBRATION_FILE_NAME , CFile::modeWrite|CFile::modeCreate, &ex ) )
  {
    ex.ReportError();
    return 199;
  }

  //TRACE( "%d bytes\n", CALDATADIM );
  TRACE( L"%d bytes\n", CALDATADIM );

  fp.Write( &current, CALDATADIM );

  fp.Close();
  return 0;

}




/*

   - nel secondo foglio ho invece provato a correggere l'errore di posizionamento con 2 semplici offset fissi e ho poi graficato l'errore residuo che grazie a                 questa semplice correzione è accettabile. 
        La correzione è calcolata così:   
        Nell'intervallo 0 - 132 mm non ho applicato nessun offset 
        
        Nell'intervallo 147,69 - 293,8 mm ho applicato  un offset di 0,1 -> Correzione= Altezza Teorica - 0,1 
        
        Nell'intervallo  301,84 mm fino alla fine ho applicato un offset di 0,25 -> Correzione= Altezza Teorica - 0,25. 


                                        */