#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include <tchar.h>
#include <string>
#pragma warning(disable : 4996)
using namespace std;



string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);

bool GetServer(char* call, short port, struct sockaddr* from, int* flen)
{
	int optval = 1;

	if (setsockopt(port, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
		throw SetErrorMsgText("opt:", WSAGetLastError());

	int sendlen;
	if ((sendlen = sendto(port, call, sizeof(call) + 1, NULL, from, *flen)) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto:", WSAGetLastError());

	char ibuf[50];
	int recvlen;
	if (recvlen = recvfrom(port, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
			return false;
		else
			throw SetErrorMsgText("recvfrom: ", WSAGetLastError());
	}
	if (strcmp(ibuf, call) == 0)
		return true;
	else
		return false;
}

bool  GetServerByName(const char* name, const char* call, struct sockaddr* from, int* flen)
{
	SOCKET sS = socket(AF_INET, SOCK_DGRAM, NULL);

	auto host = gethostbyname(name);
	if (!host)
		throw SetErrorMsgText("gethostbyname:", WSAGetLastError());

	char* ip_addr = inet_ntoa(*(in_addr*)(host->h_addr));
	cout << "\nHostname: " << host->h_name;
	cout << "\nIP: " << ip_addr;

	SOCKADDR_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(2000);
	server.sin_addr.s_addr = inet_addr(ip_addr);


	char message[10];
	int len = 0;
	if (len = sendto(sS, call, strlen(call) + 1, NULL, (const sockaddr*)&server, *flen) == SOCKET_ERROR)
		throw SetErrorMsgText("sendto:", WSAGetLastError());
	if ((len = recvfrom(sS, message, sizeof(message), NULL, (sockaddr*)&server, flen)) == SOCKET_ERROR)
		throw SetErrorMsgText("recvfrom:", WSAGetLastError());


	cout << "\nСообщение: " << message << "\n";
	*from = *((sockaddr*)&server);
	closesocket(sS);
	return true;
}

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");


	SOCKET cC;
	WSADATA wsaData;
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup:", WSAGetLastError());
		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());
		//...........................................................


		SOCKADDR from;
		int length = sizeof(from);
		GetServerByName("ROMAN", "Hello", &from, &length);


		//...........................................................
		if (closesocket(cC) == SOCKET_ERROR)
			throw SetErrorMsgText("closesocket:", WSAGetLastError());
		if (WSACleanup() == SOCKET_ERROR)
			throw SetErrorMsgText("Cleanup:", WSAGetLastError());
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}
	return 0;
}


string GetErrorMsgText(int code) // cформировать текст ошибки
{
	string msgText;
	switch (code)                      // ïðîâåðêà êîäà âîçâðàòà  
	{
	case WSAEINPROGRESS:msgText = "WSAEINPROGRESS"; break;
	case WSAENOTSOCK:msgText = "WSAENOCTSOCK"; break;
	case WSAEDESTADDRREQ:msgText = "WSAEDESTADDRREQ"; break;
	case WSAEMSGSIZE:msgText = "WSAEMSGSIZE"; break;
	case WSAEPROTOTYPE:msgText = "WSAEPROTOTYPE"; break;
	case WSAENOPROTOOPT:msgText = "WSAENOPROTOOPT"; break;
	case WSAEPROTONOSUPPORT:msgText = "WSAEPROTONOSUPPORT"; break;
	case WSAESOCKTNOSUPPORT:msgText = "WSAESOCKTNOSUPPORT"; break;
	case WSAEOPNOTSUPP:msgText = "WSAEOPNOTSUPP"; break;
	case WSAEPFNOSUPPORT:msgText = "WSAEPFNOSUPPORT"; break;
	case WSAEAFNOSUPPORT:msgText = "WSAEAFNOSUPPORT"; break;
	case WSAEADDRINUSE:msgText = "WSAEADDRINUSE"; break;
	case WSAEADDRNOTAVAIL:msgText = "WSAEADDRNOTAVAIL"; break;
	case WSAENETDOWN:msgText = "WSAENETDOWN"; break;
	case WSAENETUNREACH:msgText = "WSAENETUNREACH"; break;
	case WSAENETRESET:msgText = "WSAENETRESET"; break;
	case WSAECONNABORTED:msgText = "WSAECONNABORTED"; break;
	case WSAECONNRESET:msgText = "WSAECONNRESET"; break;
	case WSAENOBUFS:msgText = "WSAENOBUFS"; break;
	case WSAEISCONN:msgText = "WSAEISCONN"; break;
	case WSAENOTCONN:msgText = "WSAENOTCONN"; break;
	case WSAESHUTDOWN:msgText = "WSAESHUTDOWN"; break;
	case WSAETIMEDOUT:msgText = "WSAETIMEDOUT"; break;
	case WSAECONNREFUSED:msgText = "WSAECONNREFUSED"; break;
	case WSAEHOSTDOWN:msgText = "WSAEHOSTDOWN"; break;
	case WSAEHOSTUNREACH:msgText = "WSAEHOSTUNREACH"; break;
	case WSAEPROCLIM:msgText = "WSAEPROCLIM"; break;
	case WSASYSNOTREADY:msgText = "WSASYSNOTREADY"; break;
	case WSAVERNOTSUPPORTED:msgText = "WSAVERNOTSUPPORTED"; break;
	case WSANOTINITIALISED:msgText = "WSANOTINITIALISED"; break;
	case WSAEDISCON:msgText = "WSAEDISCON"; break;
	case WSATYPE_NOT_FOUND:msgText = "WSATYPE_NOT_FOUND"; break;
	case WSAHOST_NOT_FOUND:msgText = "WSAHOST_NOT_FOUND"; break;
	case WSATRY_AGAIN:msgText = "WSATRY_AGAIN"; break;
	case WSANO_RECOVERY:msgText = "WSANO_RECOVERY"; break;
	case WSANO_DATA:msgText = "WSANO_DATA"; break;
	case WSA_INVALID_HANDLE:msgText = "WSA_INVALID_HANDLE"; break;
	case WSA_INVALID_PARAMETER:msgText = "WSA_INVALID_PARAMETER"; break;
	case WSA_IO_INCOMPLETE:msgText = "WSA_IO_INCOMPLETE"; break;
	case WSA_IO_PENDING:msgText = "WSA_IO_PENDING"; break;
	case WSA_NOT_ENOUGH_MEMORY:msgText = "WSA_NOT_ENOUGH_MEMORY"; break;
	case WSA_OPERATION_ABORTED:msgText = "WSA_OPERATION_ABORTED"; break;
	case WSAEINVALIDPROCTABLE:msgText = "WSAEINVALIDPROCTABLE"; break;
	case WSAEINVALIDPROVIDER:msgText = "WSAEINVALIDPROVIDER"; break;
	case WSAEPROVIDERFAILEDINIT:msgText = "WSAEPROVIDERFAILEDINIT"; break;
	case WSASYSCALLFAILURE:msgText = "WSASYSCALLFAILURE"; break;
	default: msgText = "??? UNKKOWN ERROR ???"; break;
	};
	return msgText;
};
string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
};