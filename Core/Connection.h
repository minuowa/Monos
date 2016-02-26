#pragma once
#include "Poco\Net\TCPServerConnection.h"
#include "PKG.h"
class Connection : public Poco::Net::TCPServerConnection
{
public:
	Connection(const Poco::Net::StreamSocket& s);

	~Connection();

	void run();

	inline Poco::Net::StreamSocket& getSocket();

private:

	uArray<PKG*> mPKGList;

	uString mAddress;
};
inline Poco::Net::StreamSocket& Connection::getSocket()
{
	return socket();
}
