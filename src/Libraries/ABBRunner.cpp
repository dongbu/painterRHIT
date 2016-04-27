
#include "ABBRunner.h"
#include <qobject.h>
#include <tchar.h>
#include <stdio.h>


#define maxColors 8

//CONSTRUCTOR/DESTRUCTOR///////////////////////
ABBRunner::ABBRunner(int width, int height, Shapes *shape)
{
	this->width = width;
	this->height = height;
	this->connected = false;
}


ABBRunner::~ABBRunner()
{
}
///////////////////////////////////////////////


//ACCESSIBLE SERIAL COMMANDS///////////////////
//Used to send coordinates over serial.  Returns true if recieves success, false otherwise.
bool ABBRunner::sendCoord(int x, int y) {
	if (!connected) {
		return false;
	}
	std::string command = "COORD:X:" + std::to_string(x) + ",Y:" + std::to_string(y) + ";";
	if (sendSerial(command)) {
		if (getSerialResponse()) {
			return true;
		}
		return false;
	}
	return false;
}

//call this if something goes wrong.
void ABBRunner::abort() {
	connected = false;
	printf("Closing Serial Port \"COM");
	if (helps != NULL) {
		if (helps->portNum != NULL) {
			std::cout << helps->portNum;
		}
		else {
			std::cout << "#2#";
		}
	}
	else {
		std::cout << "#1#";
	}
	printf("\"\n");
	if (hSerial) {
		CloseHandle(hSerial);
	}
}

//tells the robot that the end of a stroke sequence has been reached
bool ABBRunner::next() {
	if (!connected) {
		return false;
	}
	if (sendSerial("NEXT;")) {
		if (!getSerialResponse()) {
			return false;
		}
		return true;
	}
	return false;
}

//tells the robot that we are done painting.  Calls abort at end
void ABBRunner::end() {
	if (!connected) {
		return;
	}
	sendSerial("END;");
	abort();
}

void ABBRunner::decidePaint(int r, int g, int b) {
	if (!connected) {
		return;
	}
	int closestId = 0;
	int closestNum = -1;
	//loop to find smallest distance
	for (int i = 0; i < maxColors; i++) {
		if (!colorUsed[i]) {
			continue;
		}
		int r1, g1, b1;
		r1 = colorR[i];
		g1 = colorG[i];
		b1 = colorB[i];
		int distance = sqrt(pow(r - r1, 2) + pow(g - g1, 2) + pow(b - b1, 2));
		if (distance < closestNum || closestNum == -1) {
			closestId = i;
			closestNum = distance;
		}
	}
	changeColor(closestId);
}
////////////////////////////////////////////////

//ACCESSIBLE PROPERTIES
void ABBRunner::setSize(int w, int h) {
	this->width = w;
	this->height = h;
	printf("ABB size set to: w:");
	std::cout << this->width << " h:" << this->height << "\n";
}
///////////////////////////////////////////////


//INTERNAL SERIAL WORK////////////////////////
bool ABBRunner::sendCanvasInfo() {
	if (!connected) {
		return false;
	}
	for (int i = 0; i < 60; i++) {
		char szBuff[2] = { 0 };
		DWORD dwBytesRead = 0;
		if (!ReadFile(hSerial, szBuff, 1, &dwBytesRead, NULL)) {
			printf("failed to read from serial\n");
			abort();
			return false;
		}
		if (szBuff[0] == 5) {
			printf("ABB Ready\n");
			break;
		}
		if (i == 59) {
			printf("ran out of time waiting for ABB to be ready\n");
			abort();
			return false;
		}
	}



	std::string message = "SIZE:X:" + std::to_string(this->width) + ",Y:" + std::to_string(this->height) + ";";
	if(sendSerial(message)) {
		if (getSerialResponse()) {
			return true;
		}
		return false;
	}
	return false;
}

bool ABBRunner:: sendSerial(std::string message) {
	if (!connected) {
		return false;
	}
	std::cout << message;
	printf("\n");
	char szBuff[sizeof(message) + 1];
	memcpy(szBuff, message.c_str(), sizeof(message));
	DWORD dwBytesWrite;
	if (!WriteFile(hSerial, &szBuff, sizeof(message), &dwBytesWrite, NULL)) {
		printf("failed to write to serial\n");
		abort();
		return false;
	}
	return true;
}

//recieves response from ABB, aborting if failed.
bool ABBRunner::getSerialResponse() {
	if (!connected) {
		return false;
	}
	char szBuff[2] = { 0 };
	DWORD dwBytesRead = 0;
	if (!ReadFile(hSerial, szBuff, 1, &dwBytesRead, NULL)) {
		printf("failed to read from serial\n");
		abort();
		return false;
	}
	if (szBuff[0] == 6) {
		printf("command success\n");
		return true;
	}
	else if (szBuff[0] == 21) {
		printf("command failed\n");
		abort();
		return false;
	}
	else {
		printf("recieved:");
		std::cout<<(szBuff[0]);
		printf("\n");
		printf("expected:");
		std::cout << "ACK";
		printf("  or:");
		std::cout << "NAK";
		printf("\n");
		abort();
		return false;
	}

}

//connects to the suplied port, and returns true if successful, false otherwise.
bool ABBRunner::connectToSerial(int port) {
	//setting up connection (only works windows right now)
	std::string portLabel = "COM" + std::to_string(port);
	wchar_t fn[16];
	wsprintf(fn, L"\\\\.\\COM%d", port);
	TCHAR *pcCommPort = (TCHAR*)(fn);
	printf("Attempting to connect to: \"");
	std::cout << portLabel;
	printf("\"\n");
	this->hSerial = CreateFile(pcCommPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (hSerial == INVALID_HANDLE_VALUE) {
		DWORD error = GetLastError();
		if (error == ERROR_FILE_NOT_FOUND) {
			printf("serial port \"");
			std::cout << portLabel;
			printf("\" does not exist\n");
		}
		else {
			printf("Unnable to connect\n");
			std::ostringstream stream;
			stream << error;
			std::cout << "error code: " << stream.str();
			printf("\n");

		}
		abort();
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
	dcbSerialParams.BaudRate = CBR_115200;//baud rate of 19200
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
	timeouts.ReadIntervalTimeout = 25000;
	timeouts.ReadTotalTimeoutConstant = 25000;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.WriteTotalTimeoutConstant = 100;
	timeouts.WriteTotalTimeoutMultiplier = 10;

	if (!SetCommTimeouts(hSerial, &timeouts)) {
		printf("Error occured while setting timeouts\n");
		abort();
		return false;
	}
	connected = true;
	return true;
}

//used to let the ABB know over serial that we are switching colors. Returns true if recieves success, false otherwise.
bool ABBRunner::changeColor(int colorNum) {
	if (!connected) {
		return false;
	}
	std::string colorLet = "A";
	switch (colorNum) {
	case 0:
		colorLet = "A";
		break;
	case 1:
		colorLet = "B";
		break;
	case 2:
		colorLet = "C";
		break;
	case 3:
		colorLet = "D";
		break;
	case 4:
		colorLet = "E";
		break;
	case 5:
		colorLet = "F";
		break;
	case 6:
		colorLet = "G";
		break;
	case 7:
		colorLet = "H";
		break;
	default:
		colorLet = "J"; //J for junk
		printf("check max colors and compare with changeColor function\n");
		break;
	}
	std::string command = "SWAP:" + colorLet + ";";
	if (sendSerial(command)) {
		if (!getSerialResponse()) {
			return false;
		}
		return true;
	}
	return false;
}
///////////////////////////////////////////////

//SETUP WINDOWS/////////////////////////////////
//shows the window for connecting to ABB
void ABBRunner::connectWin() {
	printf("connect win called\n");
	helps = new ABBHelper();
	helps->show();
	connect(helps, SIGNAL(acceptedABB()), this, SLOT(acceptedWin()));
	connect(helps, SIGNAL(canceledABB()), this, SLOT(canceledWin()));
}

void ABBRunner::acceptedWin() {
	for (int i = 0; i < maxColors; i++) {
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

void ABBRunner::canceledWin() {
	//let other stuff know we never tried to connect
	connected = false;
}
///////////////////////////////////////////////