#include "Winsock2.h"
#pragma comment(lib, "WS2_32.lib")
#include <iostream>
#include <tchar.h>
#pragma warning(disable : 4996)
using namespace std;

int countServers = 0;

string GetErrorMsgText(int code);
string SetErrorMsgText(string msgText, int code);
bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen);
bool PutAnswerToClient(char* name, short port, struct sockaddr* to, int* lto);
void GetServer(char* call, short port, struct sockaddr* from, int* flen);

int main(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru");
	SOCKET sS;
	WSADATA wsaData;

	SOCKADDR_IN clnt; // параметры сокета клиента
	memset(&clnt, 0, sizeof(clnt)); // обнулить память
	int lc = sizeof(clnt);

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			throw SetErrorMsgText("Startup:", WSAGetLastError());
		//GetServer((char*)"Hello", 2000, (sockaddr*)&clnt, &lc);
		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw SetErrorMsgText("socket:", WSAGetLastError());
		//----------------------------------------------------------

		char hostname[25];

		if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR)
			throw SetErrorMsgText("Host name: ", WSAGetLastError());
		cout << "Host name: " << hostname << endl;

		SOCKADDR_IN serv; // параметры сокета sS
		serv.sin_family = AF_INET; // используется IP-адресация 
		serv.sin_port = htons(2000); // порт 2000
		serv.sin_addr.s_addr = INADDR_ANY;
		//serv.sin_addr.s_addr = inet_addr("192.168.56.1");
		//serv.sin_addr.s_addr = inet_addr("172.20.10.10");
		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
			throw SetErrorMsgText("bind:", WSAGetLastError());

		//----------------------------------------------------------


		while (true) {
			if (GetRequestFromClient((char*)"Hello", sS, (SOCKADDR*)&clnt, &lc))
			{
				countServers++;
				auto clntInfo = gethostbyaddr((const char*)&clnt.sin_addr, 4, AF_INET);
				cout << "Client socket:" << endl;
				cout << "Hostname: " << clntInfo->h_name << endl;
				cout << "IP: " << inet_ntoa(clnt.sin_addr) << endl;
				cout << "Port: " << htons(clnt.sin_port) << endl;
				cout << "Count: " << countServers << endl;
				cout << endl;

				if (!PutAnswerToClient((char*)"Hello", sS, (sockaddr*)&clnt, &lc))
					cout << "Error" << endl;

				countServers--;
			}
			else
				cout << "Не прошло!" << endl;

		}
		//...........................................................
		if (closesocket(sS) == SOCKET_ERROR)
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
bool GetRequestFromClient(char* name, short port, struct sockaddr* from, int* flen)
{
	char ibuf[50]; //буфер ввода
	int libuf = 0; //количество принятых байт
	cout << "Wait" << endl;

	while (true)
	{
		memset(from, 0, *flen);
		if (libuf = recvfrom(port, ibuf, sizeof(ibuf), NULL, from, flen) == SOCKET_ERROR)
		{
			cout << "Error:" << endl;
			if (WSAGetLastError() == WSAETIMEDOUT)
				return false;
			else
				throw SetErrorMsgText("Recvfrom: ", WSAGetLastError());
		}

		if (strcmp(ibuf, name) == 0)
			return true;
		else
			return false;
	}
}
bool PutAnswerToClient(char* name, short port, struct sockaddr* to, int* lto)
{
	int lobuf = 0; //количество отправленных 
	bool check;
	if ((check = sendto(port, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
		throw SetErrorMsgText("recv:", WSAGetLastError());
	if (check)
		return true;
	else
		return false;
}
void GetServer(char* call, short port, struct sockaddr* from, int* flen) {
	SOCKET cC;

	int timeout = 5000;

	int optval = 1;
	char buf[50];

	try {
		if ((cC = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
			throw  SetErrorMsgText("socket:", WSAGetLastError());


		if (setsockopt(cC, SOL_SOCKET, SO_BROADCAST, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
			throw  SetErrorMsgText("setsocketopt:", WSAGetLastError());
		if (setsockopt(cC, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(int)) == SOCKET_ERROR)
			throw  SetErrorMsgText("setsocketopt:", WSAGetLastError());

		SOCKADDR_IN all;
		all.sin_family = AF_INET;
		all.sin_port = htons(port);
		all.sin_addr.s_addr = INADDR_BROADCAST;


		int len;
		if (len = sendto(cC, call, strlen(call) + 1, NULL, (sockaddr*)&all, sizeof(all)) == SOCKET_ERROR)
			throw SetErrorMsgText("sendto:", WSAGetLastError());
		if (len = recvfrom(cC, buf, sizeof(buf), NULL, from, flen) == SOCKET_ERROR)
			throw  SetErrorMsgText("recvfrom:", WSAGetLastError());

		if (strcmp(call, buf) == 0) {
			countServers++;
			cout << "Server:" << countServers;
			hostent* shostname = gethostbyaddr((char*)&((SOCKADDR_IN*)from)->sin_addr, sizeof(SOCKADDR_IN), AF_INET);
			cout << "\tIP: " << inet_ntoa(((SOCKADDR_IN*)from)->sin_addr) << endl;
			cout << "\t\tPort: " << ntohs(((struct sockaddr_in*)from)->sin_port) << endl;
			cout << "\t\tHostname: " << shostname->h_name << endl;
			cin.get();
			exit(0);
		}
	}
	catch (string errorMsgText)
	{
		if (WSAGetLastError() == WSAETIMEDOUT) {
			cout << "Превышено время ожидания: " << countServers << endl;
			if (closesocket(cC) == SOCKET_ERROR) throw SetErrorMsgText("closesocket: ", WSAGetLastError());
		}
		else throw SetErrorMsgText("GetServer:", WSAGetLastError());
	}
}