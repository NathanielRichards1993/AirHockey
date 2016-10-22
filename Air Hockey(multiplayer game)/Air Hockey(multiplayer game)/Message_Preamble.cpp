#include "Message_Preamble.h"


Message_Preamble::Message_Preamble(unsigned short cod,unsigned short len)
{
	code = cod;
	length = len;
}


Message_Preamble::~Message_Preamble()
{
}

void Message_Preamble::PackPreamble(unsigned char* buff)
{
	PackShort(buff, code);
	PackShort(buff, length);
}

void Message_Preamble::UnPackPreamble(unsigned char* buff)
{
	UnPackShort(buff, code);
	UnPackShort(buff, length);
}

unsigned short Message_Preamble::getPreambleCode()
{
	return code;
}

unsigned short Message_Preamble::getPreambleLength()
{
	return length;
}