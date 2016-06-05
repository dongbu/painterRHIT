#pragma once
#include <stdafx.h>
#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
class SerialPortHelper
{
public:

	SerialPortHelper()
	{
	}

	~SerialPortHelper()
	{
	}

	boost::asio::io_service io;
	
};

