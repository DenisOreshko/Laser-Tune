#include "stdafx.h"
#include "ComPort.h"
ComPort::ComPort(){	
	rec = false;
	portOpen = false;
	portInWork = false;
	passwordConfirmed = false;
	readState = false;
	connectionState = false;
	pcount = 0;
	initListComPorts();
}
ComPort::~ComPort(){
	PurgeComm(handleComPort,PURGE_RXCLEAR);
	PurgeComm(handleComPort,PURGE_TXCLEAR);
}

void ComPort::connect(char* portName, int baudrate){
	/*
	FILE* fp = NULL;
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout);
	cout << "\n" << portName << " "<< baudrate;
	*/
	disconnect();
	lpPortName = portName;	
	this->baudrate = baudrate;	
	handleComPort = CreateFileA(portName,GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);	
	if (handleComPort != INVALID_HANDLE_VALUE){
		DCB dcb;		
		GetCommState(handleComPort, &dcb);
		dcb.BaudRate = baudrate;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;

		COMMTIMEOUTS cto;
		GetCommTimeouts(handleComPort,&cto);
		cto.ReadIntervalTimeout = 20;
		cto.ReadTotalTimeoutConstant = 0;
		cto.ReadTotalTimeoutMultiplier = 2;
		SetCommTimeouts(handleComPort,&cto);				
		if (SetCommState(handleComPort, &dcb)) {
			portOpen = true;
		}
		else {
			portOpen = false;
		}		
	}
	else{
		portOpen = false;
	}
}
void ComPort::setHWND(HWND hWnd) {
	this->hwndPar = hWnd;
}

vector<string> ComPort::initListComPorts(){
	/*

	char lpName[12];
	char num[5];
	strcpy(lpName,"\0");
	strcpy(num,"\0");
	//clearListComPorts();
	for(int i = 0; i < 256; i++){
		sprintf(num, "%d", i);
		strcat(lpName,"COM");
		strcat(lpName,num);
		cout<<lpName;
		hCPort = CreateFile(lpName,GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hCPort != INVALID_HANDLE_VALUE){
			listComPorts.push_back(lpName);
			cout<<"\n Port: "<<lpName;
		}
		hCPort = NULL;
		strcpy(lpName,"\0");
		strcpy(num,"\0");
	}
	hCPort = NULL;
	PurgeComm(hCPort,PURGE_RXCLEAR);
	PurgeComm(hCPort,PURGE_TXCLEAR);
	*/

	char portNameTime[5] = {'\0'};
	int k = 0;

	TCHAR szDevices[65535];	
	DWORD dwChars = QueryDosDevice(NULL, szDevices, 65535);
	 if(dwChars){
		int i = 0;

		while(true){
			TCHAR* pszCurrentDevice = &szDevices[i];
			int nLen = _tcslen(pszCurrentDevice);

			if(nLen > 3 && _tcsnicmp(pszCurrentDevice, _T("COM"), 3) == 0){		
					
				for (k = 0;k < (int)_tcslen(pszCurrentDevice);k++)
					portNameTime[k] = (char)pszCurrentDevice[k];					

				slistComPorts.push_back(string(portNameTime));
			}
			while(szDevices[i] != _T('\0'))
				i++;
			i++;
			if(szDevices[i] == _T('\0'))
				break;
		}
	 }
	return slistComPorts;
}
vector<string> ComPort::getComPorts(){
	return slistComPorts;
}
void ComPort::setButtonConnect(HWND hwnd){
	this->buttonConnect = hwnd;
}
void ComPort::setButtonPassword(HWND hwnd){
	this->buttonPassword = hwnd;
}
void ComPort::setButtonSet(HWND hwnd){
	this->buttonSet = hwnd;
}
void ComPort::setTextFieldPassConf(HWND hwnd){
	this->hPassConf = hwnd;
}
void ComPort::disconnect(){
	if(portOpen)
		if(CloseHandle(handleComPort)){
			portOpen = false;
			connectionState = false;
			SetWindowTextA(buttonConnect, "Connect");
	}
	PurgeComm(handleComPort,PURGE_RXCLEAR);
	PurgeComm(handleComPort,PURGE_TXCLEAR);
	strcpy_s(writeBuffer, "\0");
	strcpy_s(readBuffer, "\0");
}
void ComPort::clearPortBuffer(){
	PurgeComm(handleComPort,PURGE_RXCLEAR);
	PurgeComm(handleComPort,PURGE_TXCLEAR);
}
void ComPort::startClearPortBuffer(){
	PurgeComm(handleComPort,PURGE_RXABORT);
	PurgeComm(handleComPort,PURGE_RXCLEAR);
	PurgeComm(handleComPort,PURGE_TXABORT);
	PurgeComm(handleComPort,PURGE_TXCLEAR);
}
void ComPort::WriteDataWithoutEnter(const char *data){
	
	if (handleComPort != INVALID_HANDLE_VALUE){
		FillMemory(writeBuffer, sizeof(writeBuffer) - 1, 0);
		strcpy_s(writeBuffer,data);
		WriteFile(handleComPort,writeBuffer,strlen(writeBuffer),&nb,NULL);
	}
}
void ComPort::WriteData(const char *data){
	if (handleComPort != INVALID_HANDLE_VALUE){
		FillMemory(writeBuffer, sizeof(writeBuffer) - 1, 0);	
		strcpy_s(writeBuffer,data);
		WriteFile(handleComPort,writeBuffer,strlen(writeBuffer),&nb,NULL);
		WriteFile(handleComPort,enter,strlen(enter),&nb,NULL);
	}
}
void ComPort::ReadData(){
	if (handleComPort != INVALID_HANDLE_VALUE){			
		FillMemory(readBuffer, sizeof(readBuffer) - 1, 0);		
		ReadFile(handleComPort,readBuffer,sizeof(readBuffer),&nb,NULL);
	}
}
void ComPort::clearWriteBuffer(){
	unsigned int x = 0;
	for(x = 0;x < sizeof(writeBuffer) ;x++){
		writeBuffer[x] = '\0';
	}
}
void ComPort::clearReadBuffer(){
	unsigned int x = 0;
	for(x = 0; x < sizeof(readBuffer); x++){
		readBuffer[x] = '\0';
	}
}
void ComPort::clearPar(){
	unsigned int x = 0;
	for(x = 0; x < sizeof(par); x++){
		par[x] = '\0';
	}
}
void ComPort::clearResultCommand(){
	unsigned int x = 0;
	for(x = 0; x < sizeof(resultCommand); x++){
		resultCommand[x] = '\0';
	}
}

void ComPort::setParameter(const char parameter[], const char value[]){	
	FillMemory(msg, sizeof(msg) - 1, 0);
	strcat_s(msg,parameter);
	strcat_s(msg,"=");
	strcat_s(msg,value);
	WriteData(msg);
	ReadData();
}
void ComPort::setCommand(const char parameter[]){
	int r = 0,p = 0;
	rec = false;
	clearResultCommand();
	WriteData(parameter);
	ReadData();
	while(readBuffer[r] != '\0'){
		if(readBuffer[r]=='=')
			rec = true;
		r++;
		if(readBuffer[r]==';')
			break;
		if(rec){
			resultCommand[p] = readBuffer[r];
			p++;
		}
	}
}
bool ComPort::getConnectionState(){
	return connectionState;
}
bool ComPort::isCheckConnection(){
	if (portOpen)
	{
		WriteDataWithoutEnter(esc);
		ReadData();
		if ((readBuffer[0] == '!') && (readBuffer[1] == 0x0D) && (readBuffer[2] == 0x0A) && (readBuffer[3] == '>')) {
			connectionState = true;
		}
		else if ((readBuffer[0] == '\x1b') && (readBuffer[1] == '\n') && (readBuffer[2] == '!') && (readBuffer[3] == '\r')) {
				connectionState = true;
		}else {
				connectionState = false;
		}		
	}
	return connectionState;
}
bool ComPort::isPortOpen(){
	return portOpen;
}
bool ComPort::isPortInWork() {
	return portInWork;
}
void ComPort::setPortInWork(bool w) {
	portInWork = w;
}
char* ComPort::getResultCommand(){
	return resultCommand;
}
char* ComPort::getParameter(const char parameter[]){
	int r = 0,p = 0;
	rec = false;
	if (portOpen){			
		FillMemory(par, sizeof(par) - 1, 0);
		WriteData(parameter);
		ReadData();
		while(readBuffer[r] != '\0'){
			if(readBuffer[r]=='=')
				rec = true;
			r++;
			if(readBuffer[r]==';')break;
			if(readBuffer[r]=='\'')continue;
			if(rec){
				par[p] = readBuffer[r];
				p++;
			}
		}
	}
	return par;
}
char* ComPort::getSerialNumber(){
	int r = 0,p = 0;
	rec = false;
	if (portOpen) {
		FillMemory(sn, sizeof(sn) / sizeof(char) - 1, '\0');
		WriteData("sn");
		ReadData();
		while(readBuffer[r] != '\0'){
			if(readBuffer[r]=='=')
				rec = true;
			r++;
			if(readBuffer[r]==';')
				break;
			if(readBuffer[r]=='\'')
				continue;
			if(rec){
				sn[p] = readBuffer[r];
				p++;
			}
		}
	}
	return sn;
}
void ComPort::setSerialNumber(const char parameter[], char value[]){	
	FillMemory(msgSN, sizeof(msgSN) / sizeof(char) - 1, 0);
	strcat_s(msgSN,parameter);
	strcat_s(msgSN,"=\'");
	strcat_s(msgSN,value);
	strcat_s(msgSN,"\'");
	WriteData(msgSN);
	ReadData();
	/*
	//int r = 0;
	while(readBuffer[r] != '\0'){
		if(readBuffer[r]=='*' && readBuffer[r+3]=='O' && readBuffer[r+4]=='K')
				passwordConfirmed = true;
		r++;
	}*/
}
bool ComPort::setPassword(const char password[]){
	int r = 0;
	passwordConfirmed = false;
	if (portOpen) {
		WriteData("password");
		ReadData();
		WriteDataWithoutEnter(password);
		ReadData();
		while (readBuffer[r] != '\0') {
			if (readBuffer[r] == '*' && readBuffer[r + 3] == 'O' && readBuffer[r + 4] == 'K') {
				passwordConfirmed = true;
			}
			r++;
		}
		if (!passwordConfirmed){
			WriteDataWithoutEnter(esc);
			ReadData();
			r = 0;
			WriteData("password");
			ReadData();
			WriteData(password);
			ReadData();
			r += 5;
			while (readBuffer[r] != '\0') {
				if (readBuffer[r] == 'P' &&
					readBuffer[r + 1] == 'A' &&
					readBuffer[r + 2] == 'S' &&
					readBuffer[r + 3] == 'S' &&
					readBuffer[r + 4] == 'W' &&
					readBuffer[r + 5] == 'O' &&
					readBuffer[r + 6] == 'R' &&
					readBuffer[r + 7] == 'D' &&
					readBuffer[r + 8] == ' ' &&
					readBuffer[r + 9] == 'A' &&
					readBuffer[r + 10] == 'C' &&
					readBuffer[r + 11] == 'C' &&
					readBuffer[r + 12] == 'E' &&
					readBuffer[r + 13] == 'P' &&
					readBuffer[r + 14] == 'T' &&
					readBuffer[r + 15] == 'E' &&
					readBuffer[r + 16] == 'D' &&
					readBuffer[r + 17] == '!') {
					passwordConfirmed = true;
				}
				else {
					break;
				}
				r++;
			}
		}
	}
	return passwordConfirmed;
}
char* ComPort::getName(){
	return this->lpPortName;
}

