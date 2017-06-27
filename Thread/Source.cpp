#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <thread> 
#include <ctime>
#include <cctype>
#include <map>
#include <iostream>
#include <windows.h>
#include <process.h>
#include <iterator>
#include <algorithm>

using namespace std;
map <string, int> map2;

HANDLE hSemaphore;
// ��� ��������
const char lpSemaphoreName[] = "MySemaphore";

string formatStr(string& str)// ������� �������� ������ �� ������ �������� � ���������� ��������� �������(��������� �����)
{
	string newStr = "";		//������� ������. � ���� ����� �������� ������ �����

	for (int i = 0; i < str.size(); i++) //���� �� �������� �������������. �� �������(0) ������� �� ����������(str.size())
	{
		if (!isalpha((unsigned char)str[i])) //���� ������� ��������, �� ������ �� ������
			continue;

		char ch = tolower(str[i]);  //����������� ���� �������� �����
		newStr.push_back(ch); //���������� ����� � ����� �������� �������

	}
	return newStr; //���������� ���������� ��������� �����
}



void formatFileV2(vector<string>* text, int start, int end)//������� �������� �����(� ������� ���������)������� ����, � ���������� ���������
{
	for (int i = start; i <= end; i++) //����, ���� � �������� ����� ������� (�� start ���� �� end ����)  ���� �����
	{
		text->at(i) = formatStr(text->at(i));//����� ������� �������� ������ ����� � ���������� �� �������.
	}

	return; 
}

vector<string>* filePars(string fileName)//�������� ������� ����(�������� � ������� ���������).�� ���� �������� ��� �����. �� ������ ������ ����
{
	vector <string>* vec = new vector < string >; //������� ������, ������� �� ������
	ifstream file(fileName);	//������� �������� ����� ����� �����

	if (!file.is_open()) //���� ���� �� ������, ����� ������
		return nullptr;

	string word;  //������� ������� ������

	while (file >> word)   //���� � ����� ���� �����, ���������� ����� � ������� ������
	{
		vec->push_back(word); //� ����� ��������� ������ ���������� ��������� ����� �� �����.
	}

	return vec; //���������� ������, ����������� ������� �� �����
}


void �ounterWords2(int  numth, vector <string>* text, int start, int end)//������� �������� ����. �� ���� ��������� ������  � ������� �������. 
{																		// � �������� � �������� ���� �� start �� end 
	if (!text->size()) //���� ���� �� ���� ������ ����
		return;

	if (start < 0 || end > text->size() - 1 || end < start)
		return;

	if (end - start + 1 < 1)
		return;

	int num = 0;

	for (int i = start; i <= end; i++)//������� ���� � ��������� ������� ������� �� start ����� �� ����������(end)
	{

		string sT;//������� ������� ������
		sT = text->at(i);//� ������� ������ ���������� ���������� ����� �� ������� � ������ ������� �����
		if (WaitForSingleObject(hSemaphore, 30000) == WAIT_FAILED)	//��������� �������, ������� �� ��� ��������� � ������� ���������
																	//���� �� ����� ������ �������. ��� ������ �����������, �� �������� ���
			return;

		map2[sT]++;//���������� ����� � ���
		if (hSemaphore != NULL)
			ReleaseSemaphore(hSemaphore, 1, NULL);					//������������ ��������
	}
	return;
}

int main(int argc, char *argv[])
{

	setlocale(LC_ALL, "rus");
	std::cout << "Program was started" << endl;

	string fileName("C:\\Users\\Bocman\\Documents\\5Kurse2SEM\\Strychkov\\1\\War2.txt");
	int numberOfProcesses = 4;
	hSemaphore = CreateSemaphore(NULL, 1, 1, lpSemaphoreName);
	if (hSemaphore == NULL) {
		std::cout << "Sem dont created" << endl;
		return FALSE;
	}
	if (argc >= 3)
	{
		fileName = argv[1];
		numberOfProcesses = atoi(argv[2]);
	}

	vector<string> *vecText = filePars(fileName);//������� ������� ������. �������� ������� filePars, ������� ��������� ����� �� ������
													//� ���������� �� � ������� ������

	if (vecText == nullptr)
	{
		std::cout << "error in reading data" << endl;
		return 1;
	}

	unsigned int vremPosleChtenia = clock();		//����������� ����� ������ ��������� �� ������ �� ����� 



	int textSize = vecText->size();					//�������� ������� formatFile, ������ ������� ����� � �������. � ���������� 
													//��������� ������

	if (vecText->size() <  numberOfProcesses)
	{
		//�� ����� �� ��������
		�ounterWords2(1, vecText, 0, textSize - 1); //��������� - ����� ��������� ���������� nGramm-�

	}
	else
	{
		//����� �� ��������
		vector<thread*> vecThreads;					//������� ������ �������

		int step = textSize / numberOfProcesses; //������: 8-����� ������, 2- ����� ����. 8/2=4
		int start = 0;						//�� 0
		int end = step - 1;					//�� 3 � �� 4 �� 7

		for (int i = 1; i <= numberOfProcesses; i++)		//���� �������� �������
		{
			auto th = new std::thread(formatFileV2, vecText, start, end);	//������� ����� � �������� formatFileV2 � �������� ������� ������
			vecThreads.push_back(th);										//������ ������ � ������ �������

			start = end;

			//��������� ������� ����������� ��� ����� �� �����
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}

		for (auto &th : vecThreads)				//���������� ������ �������
		{
			th->join();							//������� ���������� ������
			delete th;
		}
		vecThreads.clear();

		step = textSize / numberOfProcesses; //������: 8-����� ������, 2- ����� ����. 8/2=4
		start = 0;						//�� 0
		end = step - 1;					//�� 3 � �� 4 �� 7

		for (int i = 1; i <= numberOfProcesses; i++)
		{
			//if(i==1)
			auto th = new std::thread(�ounterWords2, i, vecText, start, end);

			vecThreads.push_back(th);

			start = end;

			//��������� ������� ����������� ��� ����� �������
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}

		for (auto &th : vecThreads)
		{
			th->join();
			delete th;
		}


	}

	delete vecText;

	unsigned int vremKonca = clock();
	unsigned int vremRaboti = (vremKonca - vremPosleChtenia);//� ��������

	for (auto it = map2.begin(); it != map2.end(); ++it) {
	//if(it->first!="")
	cout << it->first << " : " << it->second << endl;
	}

	cout << "����� ������ ���������  = " << vremKonca << endl; // ����� ������ ���������  
	cout << "Time = " << vremRaboti << endl; // ����� ������ ��������� ��� ������ 
	CloseHandle(hSemaphore);
	system("pause");

	ofstream fout("Statistics.txt", ios_base::app); // ��������� ���� ��� ���������� ���������� � ����� �����

	if (!fout.is_open()) // ���� ���� �� ������
		cout << "���� �� ����� ���� ������!\n"; // �������� �� ����

	fout << vremRaboti << endl;
	fout.close();

	return 0;
}
