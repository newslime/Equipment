
#include <fstream>



#ifdef _WIN32
	#include <winsock2.h>
	#include <windows.h>
	#include <Tlhelp32.h>
	#include "../res/resource.h"
#else
	#define PAUSE printf("Press Enter key to continue..."); fgetc(stdin);
#endif

#include "GemSDK.h"
#include "SystemTraySDK.h"

//#include "vld.h"

CSystemTray TrayIcon;

volatile static int s_license_loop	= 0;
volatile static int s_play_loop		= 0;

#ifdef _WIN32
	DWORD WINAPI LoopThread(LPVOID lParam)
#else
	void *LicenseLoopThread(void *ptr)
#endif
{ 
	s_play_loop = 1;
	GemHsmsCall::Get()->Initialize("", 5002, HSMS_MODE_PASSIVE);

	while(s_license_loop)
	{
#ifdef _WIN32
		::Sleep(500);
#else
		::usleep(500000);; //1 dwMilliseconds, need
#endif
	}

	GemHsmsCall::Get()->Deinitialize();
	GemHsmsCall::Free();
	s_play_loop = 0;

	return 0;
}

#ifdef _WIN32
BOOL CALLBACK DlgProc(HWND hDlg, UINT Message, WPARAM wParam, LPARAM lParam)
{		
	PAINTSTRUCT ps;
	HDC			hdc;

	switch(Message)
	{
		case WM_INITDIALOG:
			{
				//set logo
				HICON hIcon;
				hIcon = (HICON) LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 32, 32, 0);
				SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
			}

		case WM_ICON_NOTIFY:
            return TrayIcon.OnTrayNotification(wParam, lParam);

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_POPUP_EXIT:
					DestroyWindow(hDlg);
					break;

				default:
					break;
			}
			break;

		case WM_PAINT:
			hdc = BeginPaint(hDlg, &ps);
			// TODO: Add any drawing code here...
			EndPaint(hDlg, &ps);
			break;
		
		case WM_DESTROY:		
			PostMessage(hDlg, WM_QUIT, 0, 0);
			break;
		
		case WM_CLOSE:
			PostMessage(hDlg, WM_QUIT, 0, 0);
			break;
		
		default:
			return FALSE;
	}
	
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	/*if(FindWindow(NULL, "Equipment"))
	{
		return 0;
	}*/

	HWND	hDlg;
	HWND	hAct;
	MSG		msg; 

	//open GUI windows
	hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_MAIN), 0, DlgProc, 0);
    ShowWindow(hDlg, /*nCmdShow*/SW_HIDE);
	UpdateWindow(hDlg);

	TrayIcon.Create( hInstance,
					 hDlg,                            // Parent window
					 WM_ICON_NOTIFY,                  // Icon notify message to use
					 _T("Equipment 1.00"),  // tooltip
					 ::LoadIcon(hInstance, (LPCTSTR)IDI_ICON),
					 IDR_MENU2);

	/*s_license_loop = 1;
	CreateThread(NULL,0,LicenseLoopThread,hDlg,0,NULL);*/

	GemHsmsCall::Get()->Initialize("", 5001, HSMS_MODE_PASSIVE);

	memset(&msg,0,sizeof(MSG));

	while ( msg.message != WM_QUIT )
	{
		GetMessage(&msg, NULL, 0, 0);

		if( IsDialogMessage(hDlg, &msg) )
		{
			hAct = GetActiveWindow();

			if( hAct == hDlg )
				UpdateWindow(hDlg);
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} 
	}

	/*s_license_loop = 0;

	while( s_play_loop )
	{
		::Sleep(200);
	}*/

	GemHsmsCall::Get()->Deinitialize();
	GemHsmsCall::Free();

	return (int)msg.wParam;
}
#else
int main(int argc, char *argv[])
{
	return 0;
}
#endif