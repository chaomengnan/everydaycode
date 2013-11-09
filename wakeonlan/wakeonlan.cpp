#include <string>
#include <iostream>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/NumberParser.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Net/DatagramSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/NetException.h>

bool ParseMacAddress(std::string strMac, unsigned char* pOut)
{
	Poco::StringTokenizer st(strMac, "-");

	unsigned char pBuf[6] = { 0 };
	memset(pBuf, 0, 6);

	int nIndex = 0;

	try{
		for(Poco::StringTokenizer::Iterator it = st.begin();
			it != st.end();
			it ++)
		{
			if(nIndex >= 6) break;

			std::string strCurr = (*it);
			unsigned char nNum = Poco::NumberParser::parseHex(strCurr);
			pBuf[nIndex++] = nNum;
		}

		memcpy(pOut, pBuf, 6);
	}
	catch(Poco::Exception& e)
	{
		std::cout<<"ParseMacAddress Exception: "<<e.displayText()<<std::endl;
		std::cout<<"Input Mac: "<<strMac<<std::endl;
		return false;
	}

	return true;
}

bool SendMagicPacket(unsigned char* pOut)
{
	unsigned char packet[102] = { 0 };
	for(int i=0; i<6; i++){
		packet[i] = 0xFF;
	}

	for(int i=0; i<16; i++){
		int nIndex = 6 + i*6;
		memcpy(&packet[nIndex], pOut, 6);
	}

	Poco::Net::DatagramSocket ss;
	ss.setBroadcast(true);

	Poco::Net::SocketAddress destAddr = Poco::Net::SocketAddress(Poco::Net::IPAddress::broadcast(), 0);

	int nRet = -1;

	try{
		nRet = ss.sendTo(packet, 102, destAddr);
	}
	catch(Poco::Exception& e)
	{
		std::cout<<"SendMagicPacket failed! \nException: "<<e.displayText()<<std::endl;
		return false;
	}

	return nRet > 0;
}

int main(int argc, char** argv)
{
	if(argc != 2){
		std::cout<<"Wrong input param"<<std::endl;
		return -1;
	}

	std::string strMac = argv[1];
	unsigned char pBuf[6] = { 0 };

	if(!ParseMacAddress(strMac, pBuf)) return -1;

	if(SendMagicPacket(pBuf)){
		std::cout<<"Success send wake packet to "<<strMac<<std::endl;
	}

	return 0;
}