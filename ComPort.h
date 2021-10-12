/*
 * ComPort.h
 *
 *  Created on: 23.04.2020
 *      Author: DenisPC
 */
#pragma once /* Защита от двойного подключения заголовочного файла */
#ifndef COMPORT_H_
#define COMPORT_H_

using namespace std;
using std::vector;

static const  char enter[2] = {0x0D, '\0'};
static const char esc[2] = {0x1B, '\0'};

class ComPort{
private:	
	HWND hwndPar;
	HANDLE handleComPort,hCPort;
	char* lpPortName;
	//DCB ComDCM;
	DWORD nb;
	DWORD baudrate;
	//OVERLAPPED ov;
	char writeBuffer[40];
	char readBuffer[40];	
	char par[20];
	char msg[40];
	char msgSN[30];
	char resultCommand[3];
	char sn[11];
	vector<char*> listComPorts;
	vector<string> slistComPorts;
	bool rec;// = false;
	bool portOpen;// = false;
	bool portInWork;// = false;
	bool passwordConfirmed;// = false;
	bool readState;// = false;
	bool connectionState;// = false;
	int pcount;// = 0;
	HWND buttonConnect, buttonPassword , buttonSet, hPassConf;

public:	
	ComPort();
	~ComPort();		
	void setHWND(HWND hWnd);
	void connect(char* portName, int baudrate);
	void disconnect();
	void clearWriteBuffer();
	void clearReadBuffer();
	void clearListComPorts();
	void clearPar();
	void clearResultCommand();
	void clearPortBuffer();
	void startClearPortBuffer();
	bool isCheckConnection();
	void WriteData(const char *data);
	void WriteDataWithoutEnter(const char *data);
	void ReadData();
	void setParameter(const char parameter[], const char value[]);
	void clearArray(char array[]);
	vector<string> initListComPorts();
	vector<string> getComPorts();
	char* getParameter(const char parameter[]);
	char* getResultCommand();
	char* getSerialNumber();
	void setCommand(const char parameter[]);
	void setSerialNumber(const char parameter[], char value[]);
	bool setPassword(const char password[]);
	char* getName();
	bool isPortOpen();
	bool isPortInWork();
	void setPortInWork(bool w);
	bool getConnectionState();
	void setButtonConnect(HWND hwnd);
	void setButtonPassword(HWND hwnd);
	void setButtonSet(HWND hwnd);
	void setTextFieldPassConf(HWND hwnd);
};

#endif /* COMPORT_H_ */
