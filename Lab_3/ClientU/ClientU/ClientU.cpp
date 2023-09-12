#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include <tchar.h>
#include <string>
#include <stdio.h> 
#include <time.h> 
#pragma warning(disable : 4996)
using namespace std;

string GetErrorMsgText(int code) // cформировать текст ошибки
{
	string msgText;
	switch (code)
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

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");
	SOCKET cC;
	WSADATA wsaData;
	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup:", WSAGetLastError());
		//------------------------------------------------------------------
		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());
		//------------------------------------------------------------------
		SOCKADDR_IN serv; 
		serv.sin_family = AF_INET; // используется ip-адресация 
		serv.sin_port = htons(2000); // порт 2000
		serv.sin_addr.s_addr = inet_addr("127.0.0.1"); 
		char ibuf[50], //буфер ввода
			obuf[50]; //буфер вывода
		int libuf = 0, //количество принятых байт
			lobuf = 0; //количество отправленных байт
		char number[10];
		int count;
		cout << "Введите число: ";
		cin >> count;
		clock_t start = clock();
		for (int i = 1; i <= count; i++)
		{
			itoa(i, number, 10);
			strcpy(obuf, "Hello from client ");
			strcat(obuf, number);
			if ((lobuf = sendto(cC, obuf, strlen(obuf) + 1, NULL, (sockaddr*)&serv, sizeof(serv))) == SOCKET_ERROR)
				throw SetErrorMsgText("recv:", WSAGetLastError());
			SOCKADDR_IN from; // параметры сокета клиента
			memset(&from, 0, sizeof(from)); // обнулить память
			int lc = sizeof(from);
			char bfrom[50]; //буфер ввода 
			int lbrom = 0; //количество принятых байт
			if (lbrom = recvfrom(cC, bfrom, sizeof(bfrom), NULL, (sockaddr*)&from, &lc) == SOCKET_ERROR)
				throw SetErrorMsgText("recv:", WSAGetLastError());
			std::cout << bfrom << endl;
		}

		clock_t end = clock();
		double seconds = (double)(end - start) / CLOCKS_PER_SEC;
		printf("The time: %f seconds\n", seconds);
		//------------------------------------------------------------------
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