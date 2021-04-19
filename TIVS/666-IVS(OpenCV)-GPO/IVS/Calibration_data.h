#pragma once

// finestra di dialogo Calibration_data
#include "stdafx.h"
#include "Resource.h"
#include "IniFile.h"

//#define CALIBRATION_FILE_NAME "D:\\IVS\\Calibration_data.bin"
//#define CALIBRATION_INI_FILE  "D:\\IVS\\Calibration_data.ini"
#define CALIBRATION_FILE_NAME L"D:\\IVS\\Calibration_data.bin"
#define CALIBRATION_INI_FILE  L"D:\\IVS\\Calibration_data.ini"
#define CALDATADIM            sizeof(calibration_data_type)


class Calibration_data : public CDialog
{
	DECLARE_DYNAMIC(Calibration_data)

public:
	Calibration_data(CWnd* pParent = NULL);   // costruttore standard
	virtual ~Calibration_data();

// Dati della finestra di dialogo
	enum { IDD = IDD_CALIBRATION };

  // calibration data structures
  struct calibration_data_type
  {
    // master inner diameters measures (gage)
    double gage_diam_1;  // primo diametro interno master   40 mm
    double gage_diam_2;  // secondo diametro interno master 29 mm
    double gage_diam_3;  // terzo diametro interno master   13 mm

    double master_diam_1; // diametro misurato tlc collo   50 mm
    double master_diam_2; // diametro superiore tlc corpo 170 mm
    double master_diam_3; // diametro inferiore tlc corpo 190 mm

    // master outer measures (vision station)
    double master_height_1; // altezza totale master misurata da tcl collo 225 mm
    double master_height_2; // altezza intermedia master misurata tlc corpo 90 mm

  };

  calibration_data_type previous, current;





protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Supporto DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnInitDialog();

  int Load_data(void);
  int Save_data(void);
  int Show_data(void);
  int Create_ini(void);
  int Read_ini(void);
};
