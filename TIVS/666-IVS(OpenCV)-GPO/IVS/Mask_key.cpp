/********************************************************************/

/*  MaskKey.cpp                                              */


/* function: Using the lowlevel keyboard hook to shield defined keyboard keys   */

/********************************************************************/

 
 

//#define _WIN32_WINNT  0x0500                   // set windows version 

 
#include "StdAfx.h"
#include "windows.h"

 

// global variable

LPDWORD       g_lpdwVirtualKey = NULL;         // Keycode 数组的指针

int           g_nLength = 0;                   // Keycode 数组的大小

BOOL          g_bDisableKeyboard = FALSE;      // 是否屏蔽整个键盘

HINSTANCE     g_hInstance = NULL;              // 模块实例句柄

HHOOK         g_hHook = NULL;                  // 钩子句柄

 

 

 

// 底层键盘钩子函数

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)

{

     // 禁用键盘的某个按键, 如果 g_bDisableKeyboard 为 TRUE 则禁用整个键盘

     if (nCode == HC_ACTION)

     {

         if (g_bDisableKeyboard) return TRUE;

         KBDLLHOOKSTRUCT* pStruct = (KBDLLHOOKSTRUCT*)lParam;

         LPDWORD tmpVirtualKey = g_lpdwVirtualKey;

         for (int i = 0; i < g_nLength; i++)

         {

              if (pStruct->vkCode == *tmpVirtualKey++)

                   return TRUE;

         }

        

     }

     // 传给系统中的下一个钩子

     return CallNextHookEx(g_hHook, nCode, wParam, lParam);

}

 

/********************************************************************/

/* 开始屏蔽键盘按键                                                 */

/* 参数:                                                            */

/*            lpdwVirtualKey         Keycode 数组的指针             */

/*            nLength                Keycode 数组的大小             */

/*            bDisableKeyboard       是否屏蔽整个键盘               */

/* 返回值:    TRUE 成功, FALSE 失败                                 */

/********************************************************************/

BOOL WINAPI StartMaskKey(LPDWORD lpdwVirtualKey, int nLength, BOOL bDisableKeyboard )

{

     // 如果已经安装键盘钩子则返回 FALSE

     if (g_hHook != NULL) return FALSE;

    

     // 将用户传来的 keycode 数组保存在全局变量中

     g_lpdwVirtualKey = (LPDWORD)malloc(sizeof(DWORD) * nLength);

     LPDWORD tmpVirtualKey = g_lpdwVirtualKey;

     for (int i = 0; i < nLength; i++)

     {

         *tmpVirtualKey++ = *lpdwVirtualKey++;

     }

     g_nLength = nLength;

     g_bDisableKeyboard = bDisableKeyboard;

    

     // 安装底层键盘钩子

     g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, g_hInstance, NULL);

     if (g_hHook == NULL) return FALSE;

     return TRUE;

    

}

 

/********************************************************************/

/* 停止屏蔽键盘按键                                                 */

/*                                                                  */

/* 参数: (无)                                                       */

/*                                                                  */

/* 返回值:    TRUE 成功, FALSE 失败                                 */

/********************************************************************/

BOOL WINAPI StopMaskKey()

{
	 delete g_lpdwVirtualKey;
     // 卸载钩子

     if (UnhookWindowsHookEx(g_hHook) == 0) return FALSE;

     g_hHook = NULL;

     return TRUE;

}