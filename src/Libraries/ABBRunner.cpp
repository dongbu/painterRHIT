#include "stdafx.h"
#include "ABBRunner.h"
#include "abbhelper.h"


ABBRunner::ABBRunner()
{

	helps= new ABBHelper();
	helps->show();
	

}


ABBRunner::~ABBRunner()
{
}


//Used to send coordinates over serial.  Returns true if recieves success, false otherwise.
bool ABBRunner::sendCoord(int x, int y) {
	//TODO
	return false;
}

//open a dialog box where you can set colors coresponding to set positions.  Returns true if successful communication, false otherwise.
bool ABBRunner::setColors() {
	//TODO
	return false;
}

//connects to the suplied port, and returns true if successful, false otherwise.
bool ABBRunner::connectToSerial(int port) {
	//TODO
	return false;
}

//used to let the ABB know over serial that we are switching colors. Returns true if recieves success, false otherwise.
bool ABBRunner::changeColor(int colorNum) {
	//TODO
	return false;
}

//call this if something goes wrong.
void ABBRunner::abort(std::string fI) {
	std::cout << fI;
	//TODO force close serial port
}