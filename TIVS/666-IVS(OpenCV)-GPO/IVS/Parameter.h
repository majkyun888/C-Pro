#pragma once

// CParameter

#include "stdafx.h"
#include "Static_mod.h"
#include "Button_mod.h"
#include "Input_figures.h"
#include "Input_string.h"
#include "Globals.h"
#include "Container_wnd.h"


class CFile_mngmt;
//extern BOOL          f_run_automatic;//20200702

class CParameter : public CWnd
{
	DECLARE_DYNAMIC(CParameter)

public:

// ---------------------------------------------
// PARAMETRI RESIDENTI SU BECKHOFF
// ---------------------------------------------

	CParameter( LPVOID var_in, int type_in, int dim_in, CContainer_wnd* parent, wchar_t* desc, wchar_t* udm=NULL, double def_in=0, double min_in=0, double max_in=0, CFile_mngmt* file_manager=NULL, int (*funct_in)(int)=NULL, int index_in=0 );

	// Codice di inizializzazione per booleani
  void Construct_bool( void );

  void Scaling_parameters( double p1_usr, int p1_int, double p2_usr, int p2_int );

	// Codice di inizializzazione per interi
	void Construct_int( void );

  virtual ~CParameter();

	void Update_val_text( void );

  int  Read_PLC_val ( void ); // Funzione che legge il valore attuale da PLC in val
  int  Write_PLC_val( void ); // Funzione che scrive il valore attuale di val nel PLC
  //Pour add on 20200702
  //void  Read_PLC_bool (ULONG handle, bool *input ); //

  // Dati membro
	CContainer_wnd* parent;
	CStatic_mod*    pt_val;   // Puntatore al campo di valore modificabile
	CStatic*        pt_desc;  // Puntatore al campo di descrizione
	CStatic*        pt_udm;   // Puntatore al campo dell'unità di misura
	CButton_mod*    pt_flag;  // Puntatore al controllo checkbox
	double          val_def;  // Valore di default del parametro
  double          val_min;  // Valore minimo ammesso del parametro
	double          val_max;  // Valore massimo ammesso del parametro

	int (*function)(int);     // function to call in case of button push


  wchar_t*        parcat_1;  // main category description for cparameter logging (camera/PLC)
  wchar_t*        parcat_2;  // sub  category description for cparameter logging (control)


  CFont           font;

	int             type;     // Tipo del dato/valore
	void*           val;      // Valore corrente del parametro
	UINT            dim;      // Dimensione parametro in bytes (load/save)
	UINT            index;    // Indice per il riscontro load/save

	BOOL            enabled;

	wchar_t*        desc;     // Nome da visualizzare
	wchar_t*        udm;      // Unità di misura da visualizzare

	// Dati membro specifici BECKHOFF
	ULONG           handle;
	char*           var_name;

  double          usrp[2];   // coordinate reali x conversione coordinate user-PLC
  int             plcp[2];   // coordinate intere
  double          usr2plc;   // coefficient usr=>plc
  double          plc2usr;   // coefficient plc=>usr

  wchar_t*        format_spc; // format specifier string


// Membri statici comuni a tutti gli oggetti
	static int      nID_prog;
	static RECT     area_tot; 
	static SIZE     dim_ctrl;
	static int      amp_desc; // Ampiezza del campo di descrizione
	static int      amp_val;  // Ampiezza del campo del valore
	static int      amp_udm;  // Ampiezza del campo dell'unità di misura
	static int      spcx;     // Spaziatura orizzontale tra i campi
	static DWORD    stile;    // Stile utilizzato per la costruzione


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT On_modified(WPARAM wParam, LPARAM lParam);

public:
  //afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
  //virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};



