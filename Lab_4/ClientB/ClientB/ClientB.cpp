#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include <tchar.h>
#include <string>
#pragma warning(disable : 4996)
using namespace std;



string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);
bool GetServer(char* call, short port, struct sockaddr* from, int* flen);

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


		SOCKADDR_IN serv; // параметры сокета сервера
		int ls = sizeof(serv);

		while (true) {

			if (GetServer((char*)"Hello", cC, (SOCKADDR*)&serv, &ls))
			{
				cout << "Server: " << inet_ntoa(serv.sin_addr) << ":" << htons(serv.sin_port) << endl;
			}
			else
			{
				cout << "Error" << endl;
				break;
			}
			Sleep(10000);
		}

		/*strcpy(obuf, "Helllo");
		if ((lobuf = sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
			throw SetErrorMsgText("recv:", WSAGetLastError());
		if ((libuf = recvfrom(cC, ibuf, sizeof(ibuf), NULL, (sockaddr*)&serv, &ls)) == SOCKET_ERROR)
			throw SetErrorMsgText("recv:", WSAGetLastError());
		cout << ibuf << endl;*/

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
bool GetServer(char* call, short port, struct sockaddr* from, int* flen)
{
	((SOCKADDR_IN*)from)->sin_family = AF_INET;
	((SOCKADDR_IN*)from)->sin_port = htons(2000);
	((SOCKADDR_IN*)from)->sin_addr.s_addr = INADDR_BROADCAST;
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