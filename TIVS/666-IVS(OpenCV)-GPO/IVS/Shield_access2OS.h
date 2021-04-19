
/**********************************************************************/

/*	 Shield the access to OS by 2 ways:

/*	 1. Using the lowlevel keyboard hook to mask defined keyboard keys

/*	 2. Modify the Regtable key to disable/enable the TaskManager 

/**********************************************************************/

 
// Part 1 
BOOL WINAPI 
	StartMaskKey( LPDWORD lpdwVirtualKey, int nLength, BOOL bDisableKeyboard = FALSE );

BOOL WINAPI StopMaskKey();


 // Part 2

int ReadReg(LPCWSTR path, LPCWSTR key, char* value);

int WriteReg(LPCWSTR path, LPCWSTR key, DWORD value);
	
int DelReg(LPCWSTR path);