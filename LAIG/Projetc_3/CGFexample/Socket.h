#pragma once

#include <winsock2.h>
#include <iostream>
#include <exception>
#include <string>

using namespace std;

class Socket{
protected:
	SOCKET m_socket;
public:
	Socket();
	~Socket();
	string sendMessage(string message);
};