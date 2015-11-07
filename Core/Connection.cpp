#include "stdafx.h"
#include "Connection.h"
#include "Bundle.h"
#include "NetWork.h"
#include "App.h"
#include "Poco\Net\NetException.h"


void Connection::run()
{
	Poco::Net::StreamSocket& ss = socket();
	try
	{
		uBuffer buffer(512);
		int n = ss.receiveBytes(buffer.getPointer(), buffer.capacity());
		while (n > 0)
		{
			App::Net.addMessage(buffer.getPointer(), n, this);
			buffer.clear();
			n = ss.receiveBytes(buffer.getPointer(), buffer.capacity());
		}
	}
	catch (Poco::Net::ConnectionResetException& exc)
	{
		NetWork::ConnectArg arg;
		arg.connect = this;
		App::Net.onDisconnect.trigger(&arg);
		std::cerr << "Disconnect by remote!: " << exc.displayText() << std::endl;
	}
}

Connection::Connection(const Poco::Net::StreamSocket& s) : TCPServerConnection(s)
{
	this->socket().setBlocking(true);
}

Connection::~Connection()
{
	this->socket().close();
}


