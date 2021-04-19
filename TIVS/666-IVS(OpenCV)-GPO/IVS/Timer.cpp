#include "stdafx.h"
#include "Timer.h"


/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/


CTimer::CTimer( )
{
  VERIFY( QueryPerformanceFrequency( &start ) );
//  coeff_us  = 1000000.0 / double( start.LowPart );
//  coeff_ms  =    1000.0 / double( start.LowPart );
//  coeff_sec =       1.0 / double( start.LowPart );

	coeff_us  = 1000000.0 / double( start.QuadPart );
	coeff_ms  =    1000.0 / double( start.QuadPart );
	coeff_sec =       1.0 / double( start.QuadPart );

	
	//  coeff_hi  = double( 0xFFFFFFFF ) * coeff_sec; //  / double( start.LowPart );
  reset();
}




CTimer::~CTimer()
{
}

// Ritorna il tempo trascorso in secondi (double)
// ATTENZIONE: Questa è l'unica funzione di classe che riporta il valore corretto anche
// in caso di cambio della parte alta del contatore a 64bit.
double CTimer::elapsed() 
{
  QueryPerformanceCounter( &end );
//  return( coeff_hi * ( end.HighPart - start.HighPart ) + coeff_sec * ( end.LowPart - start.LowPart ) );
	return( coeff_sec*(end.QuadPart-start.QuadPart) );
}

int CTimer::elapsed_us()
{
  QueryPerformanceCounter( &end );
//  return( int( coeff_us * ( end.LowPart - start.LowPart ) ) );
	return( int(coeff_us*(end.QuadPart-start.QuadPart)) );
}

int CTimer::elapsed_ms()
{
  QueryPerformanceCounter( &end );
  return( int(coeff_ms*(end.QuadPart-start.QuadPart)) );
}

int CTimer::elapsed_sec()
{
  QueryPerformanceCounter( &end );
  return( int(coeff_sec*(end.QuadPart-start.QuadPart)) );
}

void CTimer::reset()
{
/*  QueryPerformanceCounter( &end );
  start.LowPart  = end.LowPart;
  start.HighPart = end.HighPart; */

  QueryPerformanceCounter( &start ); 
}
