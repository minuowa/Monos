#include "stdafx.h"
#include "Bundle.h"
#include "Core\Connection.h"

BundleReceiver::BundleReceiver(Connection* ss, char* data, u16 len)
    : mSocket(&ss->getSocket())
    , mLength(len)
    , mBuffer(data) {

}

BundleReceiver::~BundleReceiver() {
}

bool BundleReceiver::valid() const {
    u16 len = *((u16*)mBuffer);
    return len == mLength;
}

PKG* BundleReceiver::get() const {
    //char* data = new char[mLength - HEADER_LENGTH];
    //dMemoryCopy(data, mBuffer + HEADER_LENGTH, mLength - HEADER_LENGTH);
    return (PKG*)(mBuffer + HEADER_LENGTH);
}

BundleSender::BundleSender(Poco::Net::StreamSocket& ss)
    : mSocket(&ss)
    , mBuffer(512) {

}

void BundleSender::send(PKG* pkg, int len) {
    mBuffer.clear();
    mLength = HEADER_LENGTH + len;
    dMemoryCopy(mBuffer.getPointer(), &mLength, HEADER_LENGTH);
    dMemoryCopy(mBuffer.getPointer() + HEADER_LENGTH, pkg, len);
    mSocket->sendBytes(mBuffer.getPointer(), mLength);
}
