

#pragma once

#include "stdafx.h"
//#include "IVS.h"
//#include "Def_globali.h"
#include "Globals.h"
#include "Area_adjust.h"
#include "File_mngmt.h"

using namespace cv;


#define    TIP_INSPECTION           64  // only tip
#define    CAP_INSPECTION          128  // old cap 
#define    TOPLIGHT_INSPECTION     256  // new double cap
#define    TIPCAP_INSPECTION       512  // combined new tip and new double cap
#define    ORALCAP_INSPECTION     1024  // combined new tip and new double cap for oral bottles (LUNAN first)

#define    NPTCNT                 2400 // max number of points making contour

//#define CAPMODELSIZE     250


//Pour added on 20200320
//==========================
typedef struct _targetBox
{
	//int label;
	int area;
	cv::Rect boundryBox;
	bool operator>= (const _targetBox& res)
	{
		return area >= res.area;
	}

	bool operator< (const _targetBox& res)
	{
		return area < res.area;
	}
} targetBox;


#undef   OUTP
#define  OUTP(x,y) *(dst.data+ x + y*dimx)


#define MAX_DIM  4096 // Massimo numero di pixel contati ( 4096 )
#define SPREAD_BLOB(ox,oy)  if ( OUTP(ox,oy) == 255)   {         \
							  area++;							 \
							  pointStk.emplace(next);            \
                              if ( leftBox  >ox ) leftBox  =ox;  \
                              if ( rightBox <ox ) rightBox =ox;  \
                              if ( topBox   >oy ) topBox   =oy;  \
                              if ( bottomBox<oy ) bottomBox=oy;  \
                              OUTP(next.x, next.y) = 0;          \
                                                      }

typedef struct
{
	int insp_offset;  //裂纹检测框的高度补偿值（从瓶头下缘关键点往下距离）
	int insp_height;  //
	int th_crack_brtness;
	int th_blob_area;       
	double th_blob_ratio;
	int th_blob_width;
	int th_crack_area;
	
}xilin_head_para;

typedef struct
{
	
	int crack_area;
} xilin_head_rlt;

//Pour added on 20200320





class CTip_inspection
{

//	DECLARE_DYNAMIC(CTip_inspection)

public:

  static int Set_inspection_area( int ind );

  CTip_inspection( UINT ind, CFile_mngmt* file_manager_in=NULL, int mode_in=TIP_INSPECTION );
  ~CTip_inspection(void);

  int  Add_menu_parameters( CContainer_wnd* child );
  int  Process_image       ( BYTE* img_acq, UINT nimg=0 );
  int  Process_image_tip   ( BYTE* img_acq, UINT nimg );
  int  Process_image_cap   ( BYTE* img_acq, UINT nimg );
  int  Process_image_toplight( BYTE* img_acq, UINT nimg );

  int  Process_image_cap_v30( BYTE* img_acq, UINT nimg );

  int  Process_image_tip_v30( BYTE* img_acq, UINT nimg );
  int  Find_contour_tip_v30( void );


  int  Find_contour_cw ( void ); // Single contour search CW
  int  Find_contour_ccw( void ); // Single contour search CCW

  int  tot_time;
  int  tot_count;


  int  Oralcap_backlight( BYTE* img_acq, UINT nimg );
  int  Oralcap_toplight ( BYTE* img_acq, UINT nimg );

  int  Add_menu_parameters_v30( CContainer_wnd* child );
  int  Add_menu_parameters_tip_v30( CContainer_wnd* child );

  int  Analysis( BYTE* img );
  int  Find_line_tip( BYTE* img_acq, int x );
  int  Find_line_cap( BYTE* img_acq, int x );
  int  Allocate_profile( void );
  void Draw_height( UINT col );

  int  Find_edge_downward( int x );
  int  Find_contour( int x0 );
  int  Analyze_contour( void );

  int  Find_contour_new( int y0 );

  int  Find_contour_v30( void );
  int  Analyze_contour_v30( void );

  int  Flipoff_analysis_v30( void );
  int  Aluside_analysis_v30( void );
  int  Crimping_analysis_v30( void );
  int  Stopper_analysis_v30( void );


  int  PTN( int x, int y );


//  int  Analyze_keypoints_v30( void );

  int  Analyze_contour_new( void );
  int  Analyze_contour_fins( void ); // special version for Sanjing
  int  Analyze_contour_oral( void );

  int  Analyze_keypoints     ( void );
  int  Analyze_keypoints_v30 ( void );
  int  Analyze_keypoints_oral( void );
  int  Analyze_keypoints_fins( void );


  int  Find_contour_toplight( int y0 );

  int  Analyze_shoulder( void );

  int  Analyze_crimping( void );

  int  Analyze_segment_y( int y, int x1, int x2 );

  int  Analyze_powder_stopper( void );


  //Pour added on 20200320
//#ifdef NECK_CRACK // for carck inspection on backlight images

	xilin_head_para para_crack;
	xilin_head_rlt  result_crack;
	int vector2Mat(vector< vector<int> > src, cv::Mat &dst);//transfer of images data
	int tintPixel(cv::Mat& mask, Rect roi);//to be defined
	int detectCrack(const Mat &srcImage, Point pt1, Point pt2);

//#endif
  //Pour added on 20200320


  struct segment_type
  {
    int ind1;     // index of first  point (0..ptn-1)
    int ind2;     // index of second point (0..ptn-1)
    int lenght;   // straight lenght of segment (rounded to int)
    int ctlenght; // lenght on contour of segment (rounded to int)
    int perc;     // percent longer contour than straight
    int angle;    // angle of segment (0=down 90=left 180=up 270=right) rounded to /100 degrees
  };  

  int  Segment_data( int index1, int index2, segment_type* segm );

  CFile_mngmt*  file_manager;
  CArea_adjust* area; // inspection/search area

  #define MAXNUMMEASURES 10  // MAX number of measures on the model of CAP

  segment_type model_fnd[MAXNUMMEASURES];
  segment_type model_ref[MAXNUMMEASURES];
  //BOOL learn_model;

  // results area
  BOOL    good;  // TRUE if final inspection result is acceptable, reject otherwise
  CString txt1;  // first row of result's text
  CString txt2;  // second row of result's text
  CString txt3;  // third row of result's text (crimping result)
  UINT    col1;  // color of first row of text
  UINT    col2;  // color of second row of text
  UINT    col3;  // color of third row of text (crimping result)

  struct segm_type // horizontal segment, used for tip inspection
  {
    int x;
    int y1;
    int y2;
  };  

  segm_type *profile;
  int        prosize;           // total size of array (xmax-xmin+1)
  int        pronpt;            // number of valid segments
  
  int        yasse;          // y of symmetry
  int        xheight;        // max height of tip
  int        yheight;        // max height of tip
  int        iheight;        // index for max height of tip
  int        xheight1;       // max height1 of tip
  int        yheight1;       // max height1 of tip
  int        iheight1;       // index for max height1 of tip
  int        xheight2;       // max height2 of tip
  int        yheight2;       // max height2 of tip
  int        iheight2;       // index for max height2 of tip


  int        index;          // index of camera/image to be bound to
  int        mode;           // TIP_INSPECTION for AMPOULES - CAP_INSPECTION for VIALS

  int        ptn;            // number of points found into CAP contour 
  int        ptx[NPTCNT];    // CAP's contour x coordinates
  int        pty[NPTCNT];    // CAP's contour y coordinates

  #ifdef NEW_TIP_V30 // in tip mode search inner profile
    int        ptn2;            // number of points found into CAP contour 
    int        ptx2[NPTCNT];    // CAP's contour x coordinates
    int        pty2[NPTCNT];    // CAP's contour y coordinates
  #endif

  // PARAMETERS RELATED TO NEW TIP_V30 CONTROL
  // bulge shrink
  int      blgsrk_hmax;       // upper height for analysis of bulge shrink (0 disabled)
  int      blgsrk_hmin;       // lower height for analysis of bulge shrink (0 disabled)
  int      blgsrk_hchk;       // height to check variation of width
  int      blgsrk_maxd;       // max delta over height to check error
  int      blgsrk_mxer;       // max allowed bulge shrink error 
  int      blgsrk_err;        // bulge shrink error 
  CString  blgsrk_txt;        // text related to bulge shrink analysis

  // inner burns 
  int      innbrn_hmin;       // upper height for analysis of inner burns (0 disabled)
  int      innbrn_hmax;       // lower height for analysis of inner burns (0 disabled)
  int      innbrn_mxer;       // max allowed inner burns error 
  int      innbrn_err;        // inner burns error 
  int      innbrn_minw;       // inner burns minimum width
  int      innbrn_minb;       // inner burns minimum brightness
  int      innbrn_fltm;       // inner burns filter threshold
  CString  innbrn_txt;        // text related to inner burns analysis

  // tip top curvature
  int      tpcurv_hmin;       // upper height for analysis of tip top curvature (0 disabled)
  int      tpcurv_hmax;       // lower height for analysis of tip top curvature (0 disabled)
  int      tpcurv_mxer;       // max allowed error 
  int      tpcurv_err;        // calculated error 
  int      tpcurv_modw;       // curvature reference width  (height in buffer coords)
  int      tpcurv_modh;       // curvature reference height (width  in buffer coords)
  CString  tpcurv_txt;        // text related to inner burns analysis




  int      dxfins;         // x to skip to avoid fins

  #define PA    0
  #define PB    1
  #define PC    2
  #define PD    3
  #define PE    4
  #define PF    5
  #define LEFT  0
  #define RIGHT 1

  int    kpts[6][2];    // [ABDCE][LR] keypoints indexes (0..ptn-1)
   // if CAP_KEYPOINTS6 defined, use one more point (sixth - PF)

  int    maxerr_modtot; // maximum TOTAL error allowed (abs sum on all measures)
  int    err_modtot;    // calculated TOTAL error between model and measure

//***  RECT   cntrbbox;      // surrounding box of found contour (min/max x/y)
  int    ptmid;

  // inspection parameters
//  int    lum_thr;      // threshold brightness
  int    contour_thr;  // threshold for contour extraction
  int    min_npt;      // minimum number of points to validate found profile
  int    max_npt;      // maximum number of points to validate found profile
  CString cont_txt;    // text for contour search and check

  int    htot_max;     // maximum acceptable height
  int    htot_min;     // minimum acceptable height
  CString htot_txt;    // text for height search and check

  int    maxerr_symm;  // maximum allowed error for symmetry      ( 0=disabled )
  int    maxerr_curv;  // maximum allowed error for top curvature ( 0=disabled )
  int    maxerr_bulge; // maximum allowed error for stem bulge    ( 0=disabled )

  int    burns_border; // border allowance inside found external profile (pixel)
  int    burns_maxvar; // maximum allowed variance per line, if greater red and error accumulated
  double burns_hmin;   // TOP height (from top) to start burns inspection from
  double burns_hmax;   // BOTTOM height (from top) to start burns inspection to
  int    maxerr_burns; // maximum allowed error for stem burns/black ( 0=disabled )

  // Specific for ORAL
  int    AD_lenghtmin;
  int    AD_lenghtmax;
  int    AD_percmax;

  int    DRL_lenghtmin; //
  int    DRL_lenghtmax;

  int    BC_lenghtmin;
  int    BC_lenghtmax;
  int    BC_percmax;
  int    BC_angmin;
  int    BC_angmax;

  // BACKLIGHT standard vial cap/seal parameters
  int    CD_lenghtmin;
  int    CD_lenghtmax;
  int    CD_percmax;

  int    DE_lenghtmin;
  int    DE_lenghtmax;
  int    DE_percmax;
  int    DE_angmin;
  int    DE_angmax;

  // New for v30
  int    AB_lenghtmin;
  int    AB_lenghtmax;
  int    AB_percmax;
  int    AB_angmin;
  int    AB_angmax;

/*  int    BC_lenghtmin;
  int    BC_lenghtmax;
  int    BC_percmax;
  int    BC_angmin;
  int    BC_angmax;*/


  int    tipthick_height;   // thickness from top to search for dark spots
  int    tipthick_minlum;   // minium brightness for top thickness 
  int    tipthick_minnpt;   // minimum acceptable number of dark points 
  int    tipthick_maxprc;   // maximum acceptable dark points percentage
  int    tipthick_minprc;   // minimum acceptable dark points percentage


  double scalefct;     // mm to pixel scale factor  =mm/pix
  double tip_diameter; // nominal diameter of tip top radius

  double mm2pix;       // mm to pixel scale factor  =pix/mm
  double pix2mm;       // pixel to mm scale factor  =mm/pix

  BOOL reject_tot;
  CString FO_txt, ALU_txt, CRI_txt, STO_txt;
  CString ABL_txt, ABR_txt, BCL_txt, BCR_txt;

// new parameters for toplight inspection

  POINT  tl_al, tl_ar;

  int    tl_contour;   // brigthness value for contour search

  // FLIP-OFF SIDE ANALYSIS PARAMETERS
  int    tl_fohgtofs; // height offset for side aluminium analysis (positive going up, negative down)
  int    tl_fowidth;  // width  of area for side aluminium analysis
  int    tl_foheight; // height of area for side aluminium analysis

  int    tl_fomax;    // maximum brightness for side aluminium analysis
  int    tl_fomin;    // minimum brightness for side aluminium analysis
  int    tl_foedge;   // maximum allowed edge threshold to detect horizontal errors
  
  int    tl_folenght; // lenght for edge analysis
  int    tl_fomaxerr; // maximum error allowed before rejecting (0 to DISABLE)

  //Pour added for rejection separation 20180927
  int    tl_fomaxerr2; // maximum error allowed before rejecting (0 to DISABLE)

  // ALUMINIUM SIDE ANALYSIS PARAMETERS
  int    tl_aluhgtofs; // height offset for side aluminium analysis
  int    tl_aluwidth;  // width  of area for side aluminium analysis
  int    tl_aluheight; // height of area for side aluminium analysis

  int    tl_alumax;    // maximum brightness for side aluminium analysis
  int    tl_alumin;    // minimum brightness for side aluminium analysis
  int    tl_aluedge;   // maximum allowed edge threshold to detect horizontal errors
  
  int    tl_alulenght; // lenght for edge analysis
  int    tl_alumaxerr; // maximum error allowed before rejecting (0 to DISABLE)

  //Pour added for rejection separation 20180927
  int    tl_alumaxerr2; // maximum error allowed before rejecting (0 to DISABLE)

  // ALUMINIUM CRIMPING ANALYSIS PARAMETERS
  int    tl_crihgtofs; // height offset for side aluminium analysis
  int    tl_criwidth;  // width  of area for side aluminium analysis
  int    tl_criheight; // height of area for side aluminium analysis

  int    tl_crimax;    // maximum brightness for side aluminium analysis
  int    tl_crimin;    // minimum brightness for side aluminium analysis
  int    tl_criedge;   // maximum allowed edge threshold to detect horizontal errors
  
  int    tl_crilenght; // lenght for edge analysis
  int    tl_crimaxerr; // maximum error allowed before rejecting (0 to DISABLE)


  // STOPPER SIDE ANALYSIS PARAMETERS
  int    tl_stohgtofs; // height offset for side aluminium analysis
  int    tl_stowidth;  // width  of area for side aluminium analysis
  int    tl_stoheight; // height of area for side aluminium analysis

  int    tl_stomax;    // maximum brightness for side aluminium analysis
  int    tl_stomin;    // minimum brightness for side aluminium analysis
  int    tl_stoedge;   // maximum allowed edge threshold to detect horizontal errors
  
  int    tl_stolenght; // lenght for edge analysis
  int    tl_stomaxerr; // maximum error allowed before rejecting (0 to DISABLE)

  //Pour added for rejection separation 20180927
  int    tl_stomaxerr2; // maximum error allowed before rejecting (0 to DISABLE)

  BOOL   tl_drawovl;  // if true draw overlay during processing (toplight)
  BOOL   bl_drawovl;  // if true draw overlay during processing (backlight)


  int    shl_height;  // height of shoulder check (from bottom of inspection area UP) - 0=disabled
  int    shl_maxslp;  // shoulder maximum acceptable slope
  int    shl_minslp;  // shoulder minimum acceptable slope
  int    shl_slplen;  // shoulder lenght for slope analysis
  int    shl_maxerr;  // shoulder maximum acceptable error

  int    curv_err;     // curvature computed error
  int    bulge_err;    // bulge shape computed error
  int    burns_err;    // burn/black spots computed error

  // new parameters for BOSEN crimping inspection
  int    edge_minval;  // minimum score for crimping dark rim 
  int    edge_minwdt;  // minimum width crimping dark rim 
  int    edge_maxwdt;  // maximum width crimping dark rim 

  int    edge_minhgt;  // minimum acceptable height of crimping dark rim 
  int    edge_maxhgt;  // maximum acceptable height of crimping dark rim 

  int    edge_minh;    // calculated and related to current height
  int    edge_maxh;    // calculated and related to current height

//  int    edge_err;     // calculated crimping error
//  int    edge_maxerr;  // max allowed crimping error
  int    edge_npt;     // number of good crimping points found
  int    edge_minnpt;  // minimum allowed number of good crimping points 
  int    edge_minside; // minimum left/right heights to validate the found bottom

  int    crmparea_w;   // width of crimping analysis area 
  int    crmparea_h;   // height of crimping analysis area 
  int    crmparea_vo;  // vertical offset of crimping analysis area 

  int    pwdrstpr_w;   // width of powder stopper analysis area 
  int    pwdrstpr_h;   // height of powder stopper analysis area 
  int    pwdrstpr_vo;  // vertical offset of  powder stopper analysis area 
  int    pwdrstpr_minl; // minimum brightness for powder stopper analysis 
  int    pwdrstpr_maxl; // maximum brightness for powder stopper analysis
  int    pwdrstpr_maxerr; // maximum allowed error for powder stopper analysis

  BYTE*  img;          // pointer to image to process
  int    dim, dimx, dimy;

  UINT   area_color;

  //Pour added on 20200320
  private:

  int findBoundaryBox(Mat& src, vector<targetBox>& boxList);
  //Pour added on 20200320
};









