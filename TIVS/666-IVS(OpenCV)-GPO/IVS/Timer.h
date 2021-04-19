#pragma once

class CTimer
{
protected:

// Attributes
protected:
public:

// Operations
public:


// Implementation
public:
   CTimer();
	~CTimer();
	void reset      ( void );
  double  elapsed ( void );
  int  elapsed_us ( void );
	int  elapsed_ms ( void );
	int  elapsed_sec( void );

private:
  LARGE_INTEGER start, end;
//  __int64 start, end;
  double coeff_ms, coeff_sec, coeff_us;

	
//	double coeff_hi; // coefficiente per la conversione della parte alta in secondi
};

