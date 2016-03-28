
#include "ABBRunner.h"
#include <qobject.h>



ABBRunner::ABBRunner()
{

	helps= new ABBHelper();
	helps->show();
	connect(helps, SIGNAL(acceptedABB()), this, SLOT(acceptedWin()));
	connect(helps, SIGNAL(canceledABB()), this, SLOT(canceledWin()));
	this->connected = false;
	

}


ABBRunner::~ABBRunner()
{
}


//Used to send coordinates over serial.  Returns true if recieves success, false otherwise.
bool ABBRunner::sendCoord(int x, int y) {
	//TODO
	std::string command = "COORD:X:" + std::to_string(x) + ",Y:" + std::to_string(y) + ";";
	std::string response;
	if (sendSerial(command)) {
		response = getSerialResponse();
		std::cout << response;
		printf("\n");
		if (response == "FAILED") {
			return false;
		}
		return true;
	}
	return false;
}


//connects to the suplied port, and returns true if successful, false otherwise.
bool ABBRunner::connectToSerial(int port) {

	//setting up connection (only works windows right now)
	std::string portLabel = "COM" + std::to_string(port);
	LPCWSTR portS = (LPCWSTR)portLabel.c_str();
	this->hSerial = CreateFile(portS,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (hSerial == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			printf("serial port \"");
			std::cout << portLabel;
			printf("\" does not exist\n");
		}
		else {
			printf("Failed to connect\n");
		}
		return false;
	}

	//setting parameters (baud rate, stop bits, etc)
	DCB dcbSerialParams = { 0 };
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (!GetCommState(hSerial, &dcbSerialParams)) {
		printf("Error getting state of serial port\n");
		abort();
		return false;
	}
	dcbSerialParams.BaudRate = CBR_9600;//baud rate of 19200
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	dcbSerialParams.fRtsControl = RTS_CONTROL_DISABLE;


	if (!SetCommState(hSerial, &dcbSerialParams)) {
		printf("Error setting state of serial port\n");
		abort();
		return false;
	}


	//setting timeouts
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 100;
	timeouts.ReadTotalTimeoutConstant = 100;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 100;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(hSerial, &timeouts)) {
		printf("Error occured while setting timeouts\n");
		abort();
		return false;
	}

	return true;
}

//used to let the ABB know over serial that we are switching colors. Returns true if recieves success, false otherwise.
bool ABBRunner::changeColor(int colorNum) {
	std::string command = "SWAP:" + std::to_string(colorNum) + ";";
	std::string response;
	if (sendSerial(command)) {
		response = getSerialResponse();
		std::cout << response;
		printf("\n");
		if (response == "FAILED") {
			return false;
		}
		return true;
	}
	return false;
}

//call this if something goes wrong.
void ABBRunner::abort() {
	printf("Closing Serial Port \"COM");
	std::cout << helps->portNum;
	printf("\"\n");
	CloseHandle(hSerial);
	//TODO force close serial port
}

void ABBRunner::acceptedWin() {
	for (int i = 0; i < 6; i++) {
		this->colorUsed[i] = helps->colorUsed[i];
		this->colorR[i] = helps->colorR[i];
		this->colorG[i] = helps->colorG[i];
		this->colorB[i] = helps->colorB[i];
	}


	if (connectToSerial(helps->portNum)) {
		printf("successfully connected to serial\n");
		sendCanvasInfo();
	}
	
}

bool ABBRunner::sendCanvasInfo() {
	//TODO, finish this
	std::string message = "hello there";
	if(sendSerial(message)) {
		std::cout << getSerialResponse();
		return false;
	}
	return false;
}

bool ABBRunner:: sendSerial(std::string message) {
	char szBuff[sizeof(message) + 1] = { *message.c_str() };
	DWORD dwBytesWrite = 0;
	if (!WriteFile(hSerial, szBuff, sizeof(message), &dwBytesWrite, NULL)) {
		printf("failed to write to serial\n");
		abort();
		return false;
	}
	return true;
}

std::string ABBRunner::getSerialResponse() {
	//TODO, make size of value reading out 
	char szBuff[10+1] = { 0 };
	DWORD dwBytesRead = 0;
	if (!ReadFile(hSerial, szBuff, 10, &dwBytesRead, NULL)) {
		printf("failed to read from serial\n");
		abort();
		return "ERROR";
	}

	return szBuff;
	
}

void ABBRunner::canceledWin() {
	//let other stuff know we never tried to connect
	connected = false;
}