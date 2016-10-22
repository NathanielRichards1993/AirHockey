#pragma once
#ifndef MESSAGEPREAMBLE_H
#define MESSAGEPREAMBLE_H
#include"PackUnPackh.h"
class Message_Preamble
{
private:
	unsigned short code;
	unsigned short length;
public:
	Message_Preamble(unsigned short, unsigned short);
	Message_Preamble();
	~Message_Preamble();
	
	void PackPreamble(unsigned char*);//buff

	void UnPackPreamble(unsigned char*);//buff
	unsigned short getPreambleCode();
	unsigned short getPreambleLength();
};

#endif