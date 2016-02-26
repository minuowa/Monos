#pragma once
#include "Poco\Net\StreamSocket.h"
#include "PKG.h"
static const u32 HEADER_LENGTH = sizeof(u32);

class Connection;

class BundleReceiver
{
public:

	BundleReceiver(Connection* ss,char* data, u16 len);

	~BundleReceiver();

	bool valid() const;

	PKG* get() const;

private:

	Poco::Net::StreamSocket* mSocket;

	char* mBuffer;

	u32 mLength;
};

class BundleSender
{
public:
	BundleSender(Poco::Net::StreamSocket& ss);
	void send(PKG* pkg,int len);
private:
	Poco::Net::StreamSocket* mSocket;
	uBuffer mBuffer;
	u32 mLength;
};
#define SendPKG(streamSocket,pkg) {BundleSender sender(streamSocket);sender.send(&pkg,sizeof(pkg));} 


