#include <iostream>
#pragma comment(lib, "ws2_32.lib")
// Для работы с сетью
#include <winsock2.h>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
// Бибблиотека ввода и вывода
#include <iostream>
#include <string>
#include <fstream>
#include <experimental/filesystem>

#pragma warning(disable: 4996)

using namespace std;

void send_file(SOCKET* socket_s, const string& file_name) {
	fstream file;
	file.open(file_name, ios_base::in | ios_base::binary);
	char sms[100];
	if (file.is_open()) {
		int f_size = experimental::filesystem::file_size(file_name);

		char* data = new char[f_size];

		file.read((char*)data, f_size);

		cout << "size: " << f_size << endl;
		cout << "name: " << file_name << endl;
		cout << "data: " << data << endl;
		strcpy(sms, "200");
		send(*socket_s, sms, 100, 0);
		send(*socket_s, to_string(f_size).c_str(), 16, 0);
		send(*socket_s, file_name.c_str(), 32, 0);
		send(*socket_s, data, f_size, 0);
		delete[] data;
	}
	else {
		cout << "Error file open\n";
		strcpy(sms, "400");
		send(*socket_s, sms, 100, 0);
	}

	file.close();
}

int main(int argc, char* argv[]) {
	//WSAStartup
	// Создаем структуру WSAData
	WSAData ws;
	// Запрашиваемая версия библеотеки winsock, нужна для загрузки библиотеки
	WORD DLLVersion = MAKEWORD(2, 1);
	// Загрузка библиотеки и проверка соединения
	if (WSAStartup(DLLVersion, &ws) != 0) {
		cout << "Error" << endl;
		exit(1);
	}

	// SOckaddr используется для хранения адресса
	SOCKADDR_IN adres;
	adres.sin_addr.s_addr = inet_addr("127.0.0.1");
	// Порт для программы с поступающими данными
	adres.sin_port = htons(1001);
	// Семейство протоколов. AF_INET - константа для интернет протоколов
	adres.sin_family = AF_INET;

	// Переменная для хранения размера структуры SOCKADDR
	int sizeofadres = sizeof(adres);
	// Socket для соединения с сервером
	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&adres, sizeofadres) != 0) {
		cout << "Error\n" << endl;
		return 1;
	}
	else
	{
		cout << "Connection good!\n" << endl;
		cout << "Enter the file name: ";
		string file_name;
		cin >> file_name;
		send_file(&Connection, file_name);
	}

	system("pause");
	return 0;
}
