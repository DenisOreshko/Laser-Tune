// Laser Tune.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Laser Tune.h"

#define MAX_LOADSTRING 100
#define ID_TIMER_PASSWORD 1006
using namespace std;
ComPort* comPort = new ComPort();
vector<string> listPorts;
vector<const char*> speedsComPort;//{ "115200","57600"};
unsigned int i_port = 0, i_speed = 0;
int uItem;
int nTimerPasswordID;
int num_sec = 0;
unsigned int updateDebug = 0;
unsigned int checkConnect = 0;
int delay_read_par = 0;
bool oneTime = false, twoTime = false;
char min[3];
char sec[3];
char timer_text[10];
char Buf[80];
char selectedPortName[7];
char selectedPortSpeed[7];
char laserName[100];
char required_pld[20];
char current_pld[20];
char cycle_longness[50];
char K_PPL[20];
char id[10];
char sn[20];
char debug[20];
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LASERTUNE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LASERTUNE));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LASERTUNE));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+0);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(
	   szWindowClass, 
	   szTitle, 
	   WS_OVERLAPPED| WS_SYSMENU,
	   CW_USEDEFAULT, 
	   0, 
	   501, 
	   310, 
	   NULL, 
	   NULL, 
	   hInstance, 
	   NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	static HWND hButtonConnect, hButtonPassword,
		hButtonSet, hEditID,
		hPassConf, hIdtext,	hStaticId, 
		hSn, hEditSN, hSn2, hButtonSetSN,
		hButtonDef, hButtonSave, 
		hStaticReq, hEditReq,hButtonSetReq, 
		hStaticCurrentPLDInfo, hStaticRequiredPLDInfo,
		hToolbar,hStaticSaveInfo, hStaticDefInfo, hButtonGetPar, 
		hComboBoxPorts,hComboBoxSpeed, 
		hStaticDbg,hStaticDbgText,hStaticCycleLongness,
		hEditKPPL,hStaticKPPL,hButtonSetKPPL,hStaticSetKPPLResult,
		hButtonDbg, hStaticWrk, hButtonWrk, hStaticConnStat, hWindowEx;

	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_CREATE:
		{
			CreateWindowA("button", "",
				WS_CHILD | BS_GROUPBOX | WS_VISIBLE,
				370, 0, 116, 181, hWnd,
				(HMENU)0, hInst, NULL);
			CreateWindowA("button", "",
				WS_CHILD | BS_GROUPBOX | WS_VISIBLE,
				10, 0, 360, 55, hWnd,
				(HMENU)0, hInst, NULL);
			CreateWindowA("button", "",
				WS_CHILD | BS_GROUPBOX | WS_VISIBLE,
				10, 47, 360, 134, hWnd,
				(HMENU)0, hInst, NULL);
			CreateWindowA("button", "",
				WS_CHILD | BS_GROUPBOX | WS_VISIBLE,
				10, 173, 476, 100, hWnd,
				(HMENU)0, hInst, NULL);
			hComboBoxPorts = CreateWindowA("ComboBox", NULL,
				CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL | CBS_SIMPLE | CBS_SORT,
				383, 20, 90, 60, hWnd,
				(HMENU)ID_COMBOBOX1, hInst, NULL);
			hComboBoxSpeed = CreateWindowA("ComboBox", NULL,
				CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL | CBS_SIMPLE | CBS_SORT,
				383, 50, 90, 120, hWnd,
				(HMENU)ID_COMBOBOX2, hInst, NULL);
			hButtonConnect = CreateWindowA("button", "Connect",
				WS_CHILD | WS_VISIBLE |  BS_PUSHLIKE | BS_PUSHBUTTON,
				383, 90, 90, 30, hWnd,
				(HMENU)ID_BUTTON1, hInst, NULL);
			hButtonPassword = CreateWindowA("button", "Password",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE ,
				155, 15, 90, 30, hWnd,
				(HMENU)ID_BUTTON2, hInst, NULL);
			hPassConf = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_CENTER,
				250, 15, 50, 30, hWnd,
				(HMENU)0, hInst, NULL);
			hStaticId = CreateWindowA("static", "id = ",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_CENTER,
				20, 67, 35, 15, hWnd,
				(HMENU)0, hInst, NULL);
			hEditID = CreateWindowA("edit", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | WS_BORDER | ES_LEFT | ES_NUMBER,
				58, 65, 82, 20, hWnd,
				(HMENU)ID_EDIT1, hInst, NULL);
			hButtonSet = CreateWindowA("button", "Set",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				155, 60, 50, 30, hWnd,
				(HMENU)ID_BUTTON3, hInst, NULL);
			hIdtext = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				210, 67, 90, 15, hWnd,
				(HMENU)0, hInst, NULL);
			hSn = CreateWindowA("static", "sn = ",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_CENTER,
				20, 107, 35, 15, hWnd,
				(HMENU)0, hInst, NULL);
			hEditSN = CreateWindowA("edit", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | WS_BORDER | ES_RIGHT,
				58, 107, 82, 20, hWnd,
				(HMENU)ID_EDIT2, hInst, NULL);
			hButtonSetSN = CreateWindowA("button", "Set",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				155, 102, 50, 30, hWnd,
				(HMENU)ID_BUTTON4, hInst, NULL);
			hSn2 = CreateWindowA("static", " ",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				210, 107, 140, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hStaticReq = CreateWindowA("static", "required_pld = ",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_CENTER,
				20, 147, 100, 17, hWnd,
				(HMENU)0, hInst, NULL);
			hEditReq = CreateWindowA("edit", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | WS_BORDER | ES_RIGHT | ES_NUMBER,
				120, 147, 30, 20, hWnd,
				(HMENU)ID_EDIT3, hInst, NULL);
			hButtonSetReq = CreateWindowA("button", "Set",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				156, 142, 50, 30, hWnd,
				(HMENU)ID_BUTTON7, hInst, NULL);
			hStaticCurrentPLDInfo = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				210, 137, 120, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hStaticRequiredPLDInfo = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				210, 157, 140, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hButtonSave = CreateWindowA("button", "Save",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				20, 187, 80, 30, hWnd,
				(HMENU)ID_BUTTON6, hInst, NULL);
			hStaticSaveInfo = CreateWindowA("static", " ",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				105, 194, 35, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hButtonDef = CreateWindowA("button", "Default",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				20, 227, 80, 30, hWnd,
				(HMENU)ID_BUTTON5, hInst, NULL);
			hStaticDefInfo = CreateWindowA("static", " ",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				105, 234, 35, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hButtonGetPar = CreateWindowA("button", "Get Parameters",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				20, 15, 120, 30, hWnd,
				(HMENU)ID_BUTTON8, hInst, NULL);
			hButtonDbg = CreateWindowA("button", "Set DBG",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				156, 187, 80, 30, hWnd,
				(HMENU)ID_BUTTON9, hInst, NULL);
			hStaticDbgText = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				280, 192, 55, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hStaticDbg = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				335, 192, 75, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hButtonWrk = CreateWindowA("button", "Set WRK",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				156, 227, 80, 30, hWnd,
				(HMENU)ID_BUTTON10, hInst, NULL);
			hStaticWrk = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				240, 234, 25, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hStaticCycleLongness = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				280, 215, 150, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hStaticKPPL = CreateWindowA("static", "K_PPL = ",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | SS_LEFT,
				280, 240, 60, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hEditKPPL = CreateWindowA("edit", "",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | WS_BORDER | ES_RIGHT | ES_NUMBER,
				340, 237, 45, 20, hWnd,
				(HMENU)ID_EDIT4, hInst, NULL);
			hButtonSetKPPL = CreateWindowA("button", "Set",
				WS_CHILD | WS_VISIBLE | WS_DISABLED | BS_PUSHBUTTON | BS_PUSHLIKE,
				395, 235, 40, 24, hWnd,
				(HMENU)ID_BUTTON12, hInst, NULL);
			hStaticSetKPPLResult = CreateWindowA("static", "",
				WS_CHILD | WS_VISIBLE | SS_LEFT,
				440, 240, 40, 20, hWnd,
				(HMENU)0, hInst, NULL);
			hStaticConnStat = CreateWindowA("static", "Disconnected",
				WS_CHILD | WS_VISIBLE | SS_CENTER,
				381, 141, 90, 30, hWnd,
				(HMENU)0, hInst, NULL);

			speedsComPort.push_back("115200");
			speedsComPort.push_back("57600");			
			comPort->setButtonConnect(hButtonConnect);
			listPorts = comPort->getComPorts();
			for (i_port = 0; i_port < listPorts.size(); i_port++) {
				SendMessageA(hComboBoxPorts, CB_ADDSTRING, 0, (LPARAM)listPorts[i_port].c_str());			
			}
			for (i_speed = 0; i_speed < speedsComPort.size(); i_speed++) {
				SendMessageA(hComboBoxSpeed, CB_ADDSTRING, i_speed, (LPARAM)speedsComPort[i_speed]);
			}
			SendMessage(hComboBoxPorts, CB_SETCURSEL, 1, 0L);
			SendMessage(hComboBoxSpeed, CBS_DROPDOWNLIST, 5, 0L);
			for (i_port = 0; i_port < listPorts.size(); i_port++) {
				strcpy_s(selectedPortName, listPorts[i_port].c_str());
				for (i_speed = 0; i_speed < speedsComPort.size(); i_speed++){
					strcpy_s(selectedPortSpeed, speedsComPort[i_speed]);
					comPort->connect(selectedPortName, atoi(selectedPortSpeed));
					if(comPort->isCheckConnection()){
						if (!comPort->isPortInWork()) {	
							SendMessage(hComboBoxSpeed, CB_SETCURSEL, i_speed, 0L);
							SendMessageA(hButtonConnect, BM_CLICK, 0, 0);
							break;
						}
					}
				}
			}			
			if (!comPort->getConnectionState()) {
				SendMessage(hComboBoxSpeed, CB_SETCURSEL, 0, 0L);
			}
			//FILE* fp = NULL;
			//AllocConsole();
			//freopen_s(&fp, "CONOUT$", "w", stdout);	
			return 0;
		}
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
			
			case ID_COMBOBOX1:
			{

				switch (HIWORD(wParam))
				{
				case CBN_ERRSPACE:
				{
					MessageBoxA(hWnd, "Мало памяти", "ComboBoxPort", MB_OK);
					return 0;
				}
				case CBN_DBLCLK:
				{
					uItem = (int)SendMessage(hComboBoxPorts, CB_GETCURSEL, 0, 0L);

					if (uItem != LB_ERR)
					{
						SendMessage(hComboBoxPorts, CB_GETLBTEXT, uItem, (LPARAM)Buf);
						MessageBox(hWnd, (LPCWSTR)Buf, L"ComboBoxPort", MB_OK);
					}
					return 0;
				}
				case CBN_SELENDOK:
				{
					uItem = (int)SendMessage(hComboBoxPorts, CB_GETCURSEL, 0, 0L);
					if (uItem != LB_ERR) {
						SendMessageA(hComboBoxPorts, CB_GETLBTEXT, uItem, (LPARAM)selectedPortName);						
					}
					return 0;
				}
				default:return 0;
				}
				return 0;
			}
			case ID_COMBOBOX2:
			{
				switch (HIWORD(wParam))
				{
					case CBN_ERRSPACE:
					{
						MessageBoxA(hWnd, "Мало памяти", "ComboBoxSpeed", MB_OK);
						return 0;
					}
					case CBN_SELENDOK:
					{
						uItem = (int)SendMessage(hComboBoxSpeed, CB_GETCURSEL, 0, 0L);
						if (uItem != LB_ERR)
						{
						SendMessageA(hComboBoxSpeed, CB_GETLBTEXT, uItem, (LPARAM)selectedPortSpeed);
						//MessageBoxA(hWnd,(LPSTR)selectedPortSpeed, "ComboBoxSpeed", MB_OK);
						}
					}
				}
				return 0;
			}
			case ID_BUTTON1:
			{			
			if(comPort->isPortInWork()){
				if (comPort->isCheckConnection()) {
					comPort->disconnect();
					EnableWindow(hButtonPassword, FALSE);
					EnableWindow(hPassConf, FALSE);
					EnableWindow(hButtonSet, FALSE);
					EnableWindow(hButtonSetSN, FALSE);
					EnableWindow(hEditID, FALSE);
					EnableWindow(hStaticId, FALSE);
					EnableWindow(hIdtext, FALSE);
					EnableWindow(hSn, FALSE);
					EnableWindow(hSn2, FALSE);
					EnableWindow(hEditSN, FALSE);
					EnableWindow(hStaticReq, FALSE);
					EnableWindow(hEditReq, FALSE);
					EnableWindow(hStaticCurrentPLDInfo, FALSE);
					EnableWindow(hStaticRequiredPLDInfo, FALSE);
					EnableWindow(hButtonSetReq, FALSE);
					EnableWindow(hButtonDef, FALSE);
					EnableWindow(hButtonSave, FALSE);
					EnableWindow(hStaticSaveInfo, FALSE);
					EnableWindow(hStaticDefInfo, FALSE);
					EnableWindow(hButtonGetPar, FALSE);
					EnableWindow(hComboBoxPorts, TRUE);
					EnableWindow(hComboBoxSpeed, TRUE);
					EnableWindow(hButtonDbg, FALSE);
					EnableWindow(hButtonWrk, FALSE);
					EnableWindow(hStaticDbg, FALSE);					
					EnableWindow(hStaticDbgText, FALSE);
					EnableWindow(hStaticCycleLongness, FALSE);
					EnableWindow(hStaticKPPL, FALSE);
					EnableWindow(hEditKPPL, FALSE);
					EnableWindow(hButtonSetKPPL, FALSE);
					EnableWindow(hStaticWrk, FALSE);
					//EnableWindow(hButtonClearPortBuffer, FALSE);
					EnableWindow(hStaticConnStat, TRUE);

					SetWindowTextA(hIdtext, "");
					SetWindowTextA(hEditID, "");
					SetWindowTextA(hEditSN, "");
					SetWindowTextA(hEditReq, "");
					SetWindowTextA(hStaticDbgText, "");
					SetWindowTextA(hStaticDbg, "");
					SetWindowTextA(hStaticWrk, "");
					SetWindowTextA(hPassConf, "");
					SetWindowTextA(hStaticCurrentPLDInfo, "");
					SetWindowTextA(hStaticRequiredPLDInfo, "");
					SetWindowTextA(hStaticCycleLongness, "");
					SetWindowTextA(hEditKPPL, "");
					SetWindowTextA(hStaticSetKPPLResult, "");
					SetWindowTextA(hSn2, "");
					SetWindowTextA(hStaticSaveInfo, "");
					SetWindowTextA(hStaticDefInfo, "");

					KillTimer(hWnd, ID_TIMER_PASSWORD);
					SetWindowTextA(hStaticConnStat, "Disconnected");
					strcpy_s(laserName, "\0");
					strcat_s(laserName, "Not connected to device");
					SetWindowTextA(hWnd, laserName);					
					comPort->setPortInWork(false);
				}				
				else {	
					SetWindowTextA(hStaticConnStat, "Disconnected");
					EnableWindow(hComboBoxPorts, TRUE);
					EnableWindow(hComboBoxSpeed, TRUE);
					KillTimer(hWnd, ID_TIMER_PASSWORD);
					comPort->disconnect();
					comPort->setPortInWork(false);
					
					//uItem = (int)SendMessage(hComboBoxPorts, CB_GETCURSEL, 0, 0L);
					//if (uItem != LB_ERR) {
					//	SendMessageA(hComboBoxPorts, CB_GETLBTEXT, uItem, (LPARAM)selectedPortName);
					//}

					//comPort->connect(selectedPortName, atoi(selectedPortSpeed));
					
					if (comPort->isCheckConnection()) 
					{
							comPort->setPortInWork(true);
							SetWindowTextA(hButtonConnect, "Disconnect");
							SetWindowTextA(hStaticConnStat, "Connected");
							nTimerPasswordID = SetTimer(hWnd, ID_TIMER_PASSWORD, 1000, NULL);						
							
							SetWindowTextA(hStaticDbgText, "debug = ");		

							if (comPort->setPassword("999")) {
								num_sec = 60;
								SetWindowTextA(hPassConf, "OK");
								EnableWindow(hButtonSet, TRUE);
								EnableWindow(hButtonSetSN, TRUE);
								EnableWindow(hIdtext, TRUE);
								EnableWindow(hSn2, TRUE);
								EnableWindow(hEditSN, TRUE);
								EnableWindow(hStaticCurrentPLDInfo, TRUE);
								EnableWindow(hStaticRequiredPLDInfo, TRUE);
								EnableWindow(hButtonSetReq, TRUE);
								EnableWindow(hButtonSetKPPL, TRUE);
							}
							else {
								SetWindowTextA(hPassConf, "Wrong");
								EnableWindow(hButtonSet, FALSE);
								EnableWindow(hButtonSetSN, FALSE);
								EnableWindow(hSn, FALSE);
								EnableWindow(hSn2, FALSE);
								EnableWindow(hEditSN, FALSE);
								EnableWindow(hStaticReq, FALSE);
								EnableWindow(hButtonSetReq, FALSE);
								EnableWindow(hButtonSetKPPL, FALSE);
								KillTimer(hWnd, ID_TIMER_PASSWORD);
							}
							EnableWindow(hStaticId, TRUE);
							EnableWindow(hButtonPassword, TRUE);
							EnableWindow(hPassConf, TRUE);
							EnableWindow(hEditID, TRUE);
							EnableWindow(hSn, TRUE);
							EnableWindow(hEditSN, TRUE);
							EnableWindow(hStaticReq, TRUE);
							EnableWindow(hEditReq, TRUE);
							EnableWindow(hButtonDef, TRUE);
							EnableWindow(hButtonSave, TRUE);
							EnableWindow(hStaticSaveInfo, TRUE);
							EnableWindow(hStaticDefInfo, TRUE);
							EnableWindow(hButtonGetPar, TRUE);
							EnableWindow(hComboBoxPorts, FALSE);
							EnableWindow(hComboBoxSpeed, FALSE);
							EnableWindow(hButtonDbg, TRUE);
							EnableWindow(hButtonWrk, TRUE);
							EnableWindow(hStaticDbg, TRUE);
							EnableWindow(hStaticDbgText, TRUE);
							EnableWindow(hStaticCycleLongness, TRUE);
							EnableWindow(hStaticKPPL, TRUE);
							EnableWindow(hEditKPPL, TRUE);							
							EnableWindow(hStaticWrk, TRUE);
							//EnableWindow(hButtonClearPortBuffer, TRUE);
							EnableWindow(hStaticConnStat, TRUE);
							SetWindowTextA(hPassConf, "Not entered");
							SendMessageA(hButtonGetPar, BM_CLICK, 0, 0);
					}
					else {
							strcpy_s(Buf,"\0");
							strcat_s(Buf, "\nOn port ");
							strcat_s(Buf, selectedPortName);
							strcat_s(Buf, "\nLaser not found!");
							//MessageBoxA(hWndParent, Buf , "Not connected to device", MB_OK);
					}	
													
				}
			}
			else {
				if (!comPort->getConnectionState()) {
					uItem = (int)SendMessage(hComboBoxPorts, CB_GETCURSEL, 0, 0L);
					if (uItem != LB_ERR) {
						SendMessageA(hComboBoxPorts, CB_GETLBTEXT, uItem, (LPARAM)selectedPortName);
					
					}
					uItem = (int)SendMessage(hComboBoxSpeed, CB_GETCURSEL, 0, 0L);
					if (uItem != LB_ERR) {
						SendMessageA(hComboBoxSpeed, CB_GETLBTEXT, uItem, (LPARAM)selectedPortSpeed);
					
					}
					
					comPort->connect(selectedPortName, atoi(selectedPortSpeed));
				}
				if (comPort->isCheckConnection())
				{
					comPort->setPortInWork(true);
					SetWindowTextA(hButtonConnect, "Disconnect");
					SetWindowTextA(hStaticConnStat, "Connected");
					nTimerPasswordID = SetTimer(hWnd, ID_TIMER_PASSWORD, 1000, (TIMERPROC) NULL);

					SetWindowTextA(hStaticDbgText, "debug = ");

					if (comPort->setPassword("999")) {
						num_sec = 60;
						SetWindowTextA(hPassConf, "OK");
						EnableWindow(hButtonSet, TRUE);
						EnableWindow(hButtonSetSN, TRUE);
						EnableWindow(hIdtext, TRUE);
						EnableWindow(hSn2, TRUE);
						EnableWindow(hEditSN, TRUE);
						EnableWindow(hStaticCurrentPLDInfo, TRUE);
						EnableWindow(hStaticRequiredPLDInfo, TRUE);
						EnableWindow(hButtonSetReq, TRUE);
						EnableWindow(hButtonSetKPPL, TRUE);
					}
					else {
						SetWindowTextA(hPassConf, "Wrong");
						EnableWindow(hButtonSet, FALSE);
						EnableWindow(hButtonSetSN, FALSE);
						EnableWindow(hSn, FALSE);
						EnableWindow(hSn2, FALSE);
						EnableWindow(hEditSN, FALSE);
						EnableWindow(hStaticReq, FALSE);
						EnableWindow(hButtonSetReq, FALSE);
						EnableWindow(hButtonSetKPPL, FALSE);
						//KillTimer(hWnd, ID_TIMER_PASSWORD);
					}
					EnableWindow(hStaticId, TRUE);
					EnableWindow(hButtonPassword, TRUE);
					EnableWindow(hPassConf, TRUE);
					EnableWindow(hEditID, TRUE);
					EnableWindow(hSn, TRUE);
					EnableWindow(hEditSN, TRUE);
					EnableWindow(hStaticReq, TRUE);
					EnableWindow(hEditReq, TRUE);
					EnableWindow(hButtonDef, TRUE);
					EnableWindow(hButtonSave, TRUE);
					EnableWindow(hStaticSaveInfo, TRUE);
					EnableWindow(hStaticDefInfo, TRUE);
					EnableWindow(hButtonGetPar, TRUE);
					EnableWindow(hComboBoxPorts, FALSE);
					EnableWindow(hComboBoxSpeed, FALSE);
					EnableWindow(hButtonDbg, TRUE);
					EnableWindow(hButtonWrk, TRUE);
					EnableWindow(hStaticDbg, TRUE);
					EnableWindow(hStaticDbgText, TRUE);
					EnableWindow(hStaticCycleLongness, TRUE);
					EnableWindow(hStaticKPPL, TRUE);
					EnableWindow(hEditKPPL, TRUE);					
					EnableWindow(hStaticWrk, TRUE);
					EnableWindow(hStaticConnStat, TRUE);				
					SendMessageA(hButtonGetPar, BM_CLICK, 0, 0);
					}
					else 
					{
						strcpy_s(Buf, "\0");
						strcat_s(Buf, "\nOn port ");
						strcat_s(Buf, selectedPortName);
						strcat_s(Buf, "\nLaser not found!");
						MessageBoxA(hWnd, Buf, "Not connected to device", MB_OK);
					}
				}
				break;
			}
			case ID_BUTTON2:
			{
				if (comPort->isPortOpen()) {
					if (comPort->setPassword("999")) {
						num_sec = 60;
						SetWindowTextA(hPassConf, "OK");
						EnableWindow(hButtonSet, TRUE);
						EnableWindow(hButtonSetSN, TRUE);
						EnableWindow(hIdtext, TRUE);
						EnableWindow(hSn2, TRUE);
						EnableWindow(hEditSN, TRUE);
						EnableWindow(hStaticCurrentPLDInfo, TRUE);
						EnableWindow(hStaticRequiredPLDInfo, TRUE);
						EnableWindow(hButtonSetReq, TRUE);
						EnableWindow(hButtonSetKPPL, TRUE);
					}
					else {
						SetWindowTextA(hPassConf, "Wrong");
						EnableWindow(hButtonSet, FALSE);
						EnableWindow(hButtonSetSN, FALSE);
						EnableWindow(hSn, FALSE);
						EnableWindow(hSn2, FALSE);
						EnableWindow(hEditSN, FALSE);
						EnableWindow(hStaticReq, FALSE);
						EnableWindow(hButtonSetReq, FALSE);
						EnableWindow(hButtonSetKPPL, FALSE);
						KillTimer(hWnd, ID_TIMER_PASSWORD);
					}
				}
				break;
			}
			case ID_BUTTON3:
			{
				if (comPort->isPortOpen()) {					
					strcpy_s(id, "\0");
					GetWindowTextA(hEditID, id, 5);
					comPort->setParameter("ID", id);
					FillMemory(id, sizeof(id) / sizeof(char) - 1, 0);
					strcat_s(id, "id = ");
					strcat_s(id, comPort->getParameter("id"));
					SetWindowTextA(hIdtext, id);
					MessageBoxA(hWnd, "Требуется рестарт контроллера", "Требуется рестарт контроллера", MB_OK);
				}
				break;
			}
			case ID_BUTTON4:
			{
				if (comPort->isPortOpen()) {
					FillMemory(sn, sizeof(sn)/sizeof(char)-1, 0);
					GetWindowTextA(hEditSN, sn, 11);
					comPort->setSerialNumber("sn", sn);
					FillMemory(sn, sizeof(sn) / sizeof(char) - 1, 0);
					strcat_s(sn, "sn = ");
					strcat_s(sn, comPort->getParameter("sn"));
					SetWindowTextA(hSn2, sn);
					comPort->setCommand("save");
					SetWindowTextA(hStaticSaveInfo, comPort->getResultCommand());
					MessageBoxA(hWnd, "Требуется рестарт контроллера", "Требуется рестарт контроллера", MB_OK);
				}
				break;
			}
			case ID_BUTTON5:
			{
				if (comPort->isPortOpen()) {
					comPort->setCommand("default");
					SetWindowTextA(hStaticDefInfo, comPort->getResultCommand());
					SetWindowTextA(hStaticSaveInfo, "");
					SetWindowTextA(hStaticWrk, "");
					updateDebug = 1;
				}
				break;
			}
			case ID_BUTTON6:
			{
				if (comPort->isPortOpen()) {
					comPort->setCommand("save");
					SetWindowTextA(hStaticSaveInfo, comPort->getResultCommand());
				}
				break;
			}
			case ID_BUTTON7:
			{
				if (comPort->isPortOpen()) {					
					FillMemory(required_pld, sizeof(required_pld) - 1, 0);
					GetWindowTextA(hEditReq, required_pld, 3);
					comPort->setParameter("required_pld", required_pld);
					FillMemory(required_pld, sizeof(required_pld) - 1, 0);
					strcat_s(required_pld, "required_pld = ");
					strcat_s(required_pld, comPort->getParameter("required_pld"));
					SetWindowTextA(hStaticRequiredPLDInfo, required_pld);
					comPort->setCommand("save");
					SetWindowTextA(hStaticSaveInfo, comPort->getResultCommand());
				}
				return 0;
			}
			case ID_BUTTON8:
			{
				if (comPort->isPortOpen()) {
					FillMemory(laserName, sizeof(laserName) - 1, 0);
					strcat_s(laserName, "Laser: ");
					strcat_s(laserName, comPort->getParameter("laser"));
					strcat_s(laserName, " (ID: ");
					strcat_s(laserName, comPort->getParameter("id"));
					strcat_s(laserName, " ");
					strcat_s(laserName, comPort->getParameter("sn"));
					strcat_s(laserName, " Ver: ");
					strcat_s(laserName, comPort->getParameter("name"));
					strcat_s(laserName, " )");
					SetWindowTextA(hWnd, laserName);
					SetWindowTextA(hEditID, comPort->getParameter("id"));
					SetWindowTextA(hEditSN, comPort->getParameter("sn"));
					SetWindowTextA(hEditReq, comPort->getParameter("required_pld"));
					SetWindowTextA(hStaticDbg, comPort->getParameter("debug"));

					FillMemory(id, sizeof(id) / sizeof(char) - 1, 0);
					strcat_s(id, "id = ");
					strcat_s(id, comPort->getParameter("id"));
					SetWindowTextA(hIdtext, id);

					FillMemory(sn, sizeof(sn) / sizeof(char) - 1, 0);
					strcat_s(sn, "sn = ");
					strcat_s(sn, comPort->getParameter("sn"));
					SetWindowTextA(hSn2, sn);

					FillMemory(current_pld, sizeof(current_pld) - 1, 0);
					strcat_s(current_pld, "current_pld = ");
					strcat_s(current_pld, comPort->getParameter("current_pld"));
					SetWindowTextA(hStaticCurrentPLDInfo, current_pld);

					FillMemory(required_pld, sizeof(required_pld) - 1, 0);
					strcat_s(required_pld, "required_pld = ");
					strcat_s(required_pld, comPort->getParameter("required_pld"));
					SetWindowTextA(hStaticRequiredPLDInfo, required_pld);	

					FillMemory(cycle_longness, sizeof(cycle_longness) - 1, 0);
					strcat_s(cycle_longness, "cycle_longness = ");
					strcat_s(cycle_longness, comPort->getParameter("cycle_longness"));
					SetWindowTextA(hStaticCycleLongness, cycle_longness);
					
					FillMemory(K_PPL, sizeof(K_PPL) - 1, 0);
					strcat_s(K_PPL, comPort->getParameter("K_PPL"));
					SetWindowTextA(hEditKPPL, K_PPL);
				}
				return 0;
			}
			case ID_BUTTON9:
			{
				if (comPort->isPortOpen()) {
					comPort->setParameter("debug", "$000D7F9F");
					SetWindowTextA(hStaticDbg, comPort->getParameter("debug"));
					SetWindowTextA(hStaticSaveInfo, "");
					SetWindowTextA(hStaticWrk, "");
					SetWindowTextA(hStaticDefInfo, "");
				}
				return 0;
			}
			case ID_BUTTON10:
			{
				if (comPort->isPortOpen()) {
					comPort->setCommand("wrk");
					SetWindowTextA(hStaticWrk, comPort->getResultCommand());
					SetWindowTextA(hStaticDbg, comPort->getParameter("debug"));
				}
				return 0;
			}
			case ID_BUTTON11:
			{
				if (comPort->isPortOpen()) {
					comPort->clearPortBuffer();
					comPort->isCheckConnection();
					comPort->ReadData();
				}
				return 0;
			}
			case ID_BUTTON12:
			{
					if (comPort->isPortOpen()) {
						FillMemory(K_PPL, sizeof(K_PPL) - 1, 0);
						GetWindowTextA(hEditKPPL, K_PPL, 5);
						comPort->setParameter("K_PPL", K_PPL);

						FillMemory(K_PPL, sizeof(K_PPL) - 1, 0);
						strcat_s(K_PPL, comPort->getParameter("K_PPL"));
						SetWindowTextA(hEditKPPL, K_PPL);
						SetWindowTextA(hStaticSetKPPLResult, K_PPL);
						
						comPort->setCommand("save");
						SetWindowTextA(hStaticSaveInfo, comPort->getResultCommand());
					}
					return 0;
			}	
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		switch(wParam){
			case ID_TIMER_PASSWORD:				
				checkConnect++;
				if (checkConnect > 0) {
					if (!comPort->getConnectionState()) {
						SetWindowTextA(hStaticConnStat, "Connecting...");
						
					}
				}
				if (checkConnect == 3) {
					if (comPort->isCheckConnection()) {
						SetWindowTextA(hStaticConnStat, "Connected");
						twoTime = true;
						if (oneTime) {
							oneTime = false;					
							EnableWindow(hButtonPassword, TRUE);
							EnableWindow(hPassConf, TRUE);
							//EnableWindow(hButtonSet,TRUE);
							//EnableWindow(hButtonSetSN,TRUE);
							//EnableWindow(hButtonSetReq,TRUE);
							EnableWindow(hButtonDef, TRUE);
							EnableWindow(hButtonDbg, TRUE);
							EnableWindow(hButtonWrk, TRUE);
							EnableWindow(hButtonGetPar, TRUE);
							EnableWindow(hButtonSave, TRUE);
							EnableWindow(hEditID, TRUE);
							EnableWindow(hEditSN, TRUE);
							EnableWindow(hEditReq, TRUE);
							//EnableWindow(hButtonClearPortBuffer, TRUE);
							//EnableWindow(hPassConf,TRUE);
							EnableWindow(hStaticDbg, TRUE);
							EnableWindow(hStaticReq, TRUE);
							EnableWindow(hStaticId, TRUE);
							EnableWindow(hSn, TRUE);
							EnableWindow(hStaticCycleLongness, TRUE);
							EnableWindow(hStaticDbgText, TRUE);
							EnableWindow(hStaticKPPL, TRUE);
							EnableWindow(hEditKPPL, TRUE);	
							EnableWindow(hButtonSetKPPL, TRUE);			
							SetWindowTextA(hStaticDbg, comPort->getParameter("debug"));
					
							FillMemory(laserName, sizeof(laserName) - 1, 0);
							strcpy_s(laserName, "\0");
							strcat_s(laserName, "Laser: ");
							strcat_s(laserName, comPort->getParameter("laser"));
							strcat_s(laserName, " (ID: ");
							strcat_s(laserName, comPort->getParameter("id"));
							strcat_s(laserName, " ");
							strcat_s(laserName, comPort->getParameter("sn"));
							strcat_s(laserName, " Ver: ");
							strcat_s(laserName, comPort->getParameter("name"));
							strcat_s(laserName, " )");
							SetWindowTextA(hWnd, laserName);
							if (comPort->setPassword("999")) {
								num_sec = 60;
								SetWindowTextA(hPassConf, "OK");
								EnableWindow(hButtonSet, TRUE);
								EnableWindow(hButtonSetSN, TRUE);
								EnableWindow(hIdtext, TRUE);
								EnableWindow(hSn2, TRUE);
								EnableWindow(hEditSN, TRUE);
								EnableWindow(hStaticCurrentPLDInfo, TRUE);
								EnableWindow(hStaticRequiredPLDInfo, TRUE);
								EnableWindow(hButtonSetReq, TRUE);
								EnableWindow(hButtonSetKPPL, TRUE);
							}
							else {
								SetWindowTextA(hPassConf, "Wrong");
								EnableWindow(hButtonSet, FALSE);
								EnableWindow(hButtonSetSN, FALSE);
							EnableWindow(hSn, FALSE);
							EnableWindow(hSn2, FALSE);
							EnableWindow(hEditSN, FALSE);
							EnableWindow(hStaticReq, FALSE);
							EnableWindow(hButtonSetReq, FALSE);
							EnableWindow(hButtonSetKPPL, FALSE);
							EnableWindow(hStaticRequiredPLDInfo, FALSE);
							KillTimer(hWnd, ID_TIMER_PASSWORD);
						}
						SendMessageA(hButtonGetPar,BM_CLICK,0,0);
					
						//MessageBox(hwnd, "Success! Connected to device", "Connected to device", MB_OK);
					}
				}
				else {
					if(twoTime){
						twoTime = false;
						SetWindowTextA(hStaticConnStat, "Disconnected");				
						EnableWindow(hButtonPassword, FALSE);
						EnableWindow(hButtonSet, FALSE);
						EnableWindow(hButtonSetSN, FALSE);
						EnableWindow(hButtonSetReq, FALSE);
						EnableWindow(hButtonDef, FALSE);
						EnableWindow(hButtonDbg, FALSE);
						EnableWindow(hButtonWrk, FALSE);
						EnableWindow(hButtonGetPar, FALSE);
						EnableWindow(hButtonSave, FALSE);
						EnableWindow(hEditID, FALSE);
						EnableWindow(hEditSN, FALSE);
						EnableWindow(hEditReq, FALSE);
						EnableWindow(hEditKPPL, FALSE);
						EnableWindow(hStaticKPPL, FALSE);
						EnableWindow(hStaticDbg, FALSE);
						EnableWindow(hStaticReq, FALSE);
						EnableWindow(hStaticRequiredPLDInfo, FALSE);
						EnableWindow(hStaticId, FALSE);
						EnableWindow(hSn, FALSE);
						EnableWindow(hPassConf, FALSE);
						EnableWindow(hStaticCycleLongness, FALSE);
						EnableWindow(hStaticDbgText, FALSE);
						EnableWindow(hButtonSetKPPL, FALSE);

						SetWindowTextA(hStaticDbg, "");
						SetWindowTextA(hPassConf,"");
						SetWindowTextA(hStaticCycleLongness, "cycle_longness = ");
						SetWindowTextA(hSn2, "");
						SetWindowTextA(hStaticCurrentPLDInfo, "");
						SetWindowTextA(hStaticRequiredPLDInfo, "");
						SetWindowTextA(hIdtext, "");
						SetWindowTextA(hEditID, "");
						SetWindowTextA(hEditSN, "");
						SetWindowTextA(hEditReq, "");
						SetWindowTextA(hEditKPPL, "");	
						SetWindowTextA(hStaticSaveInfo, "");
						SetWindowTextA(hStaticDefInfo, "");
						SetWindowTextA(hStaticSetKPPLResult, "");
						FillMemory(laserName, sizeof(laserName) - 1, 0);
						strcat_s(laserName, "Not connected to device");
						SetWindowTextA(hWnd, laserName);			
					}
					num_sec = 0;
					oneTime = true;				
					}
					checkConnect = 0;
				}
				if (delay_read_par > 0) {
					delay_read_par--;
				}
				if (num_sec > 0) {
					timer_text[0] = 0;
					num_sec--;
					sprintf_s(sec, "%d", num_sec);
					if (num_sec > 9)
						strcat_s(timer_text, "OK 00:");
					else
						strcat_s(timer_text, "OK 00:0");
					strcat_s(timer_text, sec);
					SetWindowTextA(hPassConf, timer_text);
				}
				if (num_sec == 0) {
					if (comPort->getConnectionState()) {	
						SetWindowTextA(hPassConf, "Not entered");
						EnableWindow(hButtonSet, FALSE);
						EnableWindow(hButtonSetSN, FALSE);
						EnableWindow(hButtonSetReq, FALSE);
						EnableWindow(hButtonSetKPPL, FALSE);
					}
				}
				if (updateDebug > 0) {
					updateDebug--;
					if (updateDebug == 0) {
						SetWindowTextA(hStaticDbg, comPort->getParameter("debug"));
					}
				}
				
				return 0;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		KillTimer(hWnd, ID_TIMER_PASSWORD);
		delete comPort;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
