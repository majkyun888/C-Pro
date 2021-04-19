#pragma once

#include "Globals.h"
#include "Results_manager.h"


#define VISION_TYPE        (1<<10)   // Controllo da visione/telecamere
#define HEIGHT_TYPE        (1<<11)   // Controllo altezza 
#define VERTICALITY_TYPE   (1<<12)   // Controllo verticalità
#define PLANEITE_TYPE      (1<<13)   // Controllo planarità baga
#define WIDTH_TYPE         (1<<14)   // Controllo larghezza
#define THICKNESS_TYPE     (1<<15)   // Controllo spessore

#define GAGE_TYPE          (1<<20)   // Controllo da tampone




struct control_type
{
  char   code[5]; // SEM_CTR ascii code
  int    type;    // 0=default min-max-avg
  double min[16];
  double max[16];
  double avg[16];
  Results_manager* results; 
};



class COutput_data
{
public:
//  COutput_data( int control_number , int bottle_number );
  COutput_data( void );
  ~COutput_data( void );

//  int Allocate(void);
//  int Deallocate(void);

  int Add_control( char* code, int type, Results_manager* results );
  int Vision_process_data( int bottle_id, int bottle_cav );
  int Gage_process_data( int bottle_id, int bottle_cav );
  
  UINT bottle_num;

//  UINT control_num;
  UINT vision_nc; // number of controls on vision station
  UINT gage_nc;   // number of controls on gage station


//  control_type vision_data[30]; // dati relativi ai controlli di visione
//  control_type gage_data[MAX_NUM_DIAM];    // dati relativi ai controlli del tampone

//  res_type** result;  //  [bottles][controls]
//  res_type result[6][13];

  int File_write(void);
};

