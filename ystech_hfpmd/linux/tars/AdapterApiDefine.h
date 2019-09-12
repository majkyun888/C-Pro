////////////////////////////////////////////////////////////////////////////////
///@brief: windows下输出标记
////////////////////////////////////////////////////////////////////////////////

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the TALIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// TALIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#pragma once

// by wyc

#ifdef WIN32
#ifdef ADAPTER_API_SOURCE
#define ADAPTER_API
#elif ADAPTER_API_EXPORTS
#define ADAPTER_API __declspec(dllexport)
#else
#define ADAPTER_API __declspec(dllimport)
#endif
#else
#define ADAPTER_API
#endif
