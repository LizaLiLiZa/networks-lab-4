#include <iostream>
#pragma comment(lib, "ws2_32.lib")
// ��� ������ � �����
#include <winsock2.h>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
// ����������� ����� � ������
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>

#pragma warning(disable: 4996)

using namespace std;

int recv_file(SOCKET* socket_c) {
	char f_size_str[16];
	char file_name[32];
	char sms[100];
	char sms2[100];
	strcpy(sms2, "400");
	recv(*socket_c, sms, 100, 0);
	if (strcmp(sms, "sms2") == 0) {
		cout << "Error" << endl;
		return 1;
	}
	recv(*socket_c, f_size_str, 16, 0);
	int f_size = atoi(f_size_str);
	char* data = new char[f_size];

	recv(*socket_c, file_name, 32, 0);

	fstream file;

	file.open(file_name, ios_base::out | ios_base::binary);
	cout << "size: " << f_size << endl;
	cout << "name: " << file_name << endl;

	if (file.is_open()) {
		recv(*socket_c, data, f_size, 0);
		cout << "data: " << data << endl;
		file.write(data, f_size);
		cout << "All save!" << endl;
	}
	else {
		cout << "Error file open!" << endl;
	}
	delete[] data;
	file.close();
	return 0;
}

int main(int argc, char* argv[]) {
	//WSAStartup
	// ������� ��������� WSAData
	WSAData ws;
	// ������������� ������ ���������� winsock, ����� ��� �������� ����������
	WORD DLLVersion = MAKEWORD(2, 1);
	// �������� ���������� � �������� ����������
	if (WSAStartup(DLLVersion, &ws) != 0) {
		cout << "Error" << endl;
		exit(1);
	}

	// SOckaddr ������������ ��� �������� �������
	SOCKADDR_IN adres;
	adres.sin_addr.s_addr = inet_addr("127.0.0.1");
	// ���� ��� ��������� � ������������ �������
	adres.sin_port = htons(1001);
	// ��������� ����������. AF_INET - ��������� ��� �������� ����������
	adres.sin_family = AF_INET;

	// ���������� ��� �������� ������� ��������� SOCKADDR
	int sizeofadres = sizeof(adres);

	// �������� Socket sListen. ���������:
	// 1. �������� ����� ��������� ���������� ����� �������������
	// 2. ��������� �� ��������, ��������������� ����������
	// 3. 
	SOCKET firstSocket = socket(AF_INET, SOCK_STREAM, NULL);

	// �������� ������� Socket � ������� ������� bind. ���������:
	// 1. ������ Socket
	// 2. ��������� �� ��������� SOCKADDR
	// 3. ������ ��������� SOCKADDR
	bind(firstSocket, (SOCKADDR*)&adres, sizeofadres);

	// ������������� ����� � �������� ����������� ������� � ������� ������� listen. ���������:
	// 1. ������ Socket
	// 2. ������������ ����������� �������� ��������� � ���������
	listen(firstSocket, SOMAXCONN);

	// ������� ����� Socket, ����� ���������� ���������� � ��������. ������� accept ���������� ��������� �� ����� ������, ������� ����� ������������ ��� ������� � ��������.
	SOCKET newSocket = accept(firstSocket, (SOCKADDR*)&adres, &sizeofadres);

	// �������� �����������
	if (newSocket == 0) {
		cout << "Error #2\n" << endl;
	}
	else {
		cout << "Client Connected!" << endl;
		recv_file(&newSocket);
	}

	system("pause");
	return 0;
}
