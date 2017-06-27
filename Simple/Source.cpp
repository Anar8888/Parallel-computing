#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <sstream>
#include <map>

using namespace std;
map <string, int> map2;

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


vector<string>* formatFile(vector<string>* text)//������� �������� (� ������� ���������)������ ����, � ���������� ���������
{

	for (string& str : *text) //����, ���� � �������� ������� ���� �����
	{
		str = formatStr(str); //����� ������� �������� ������ �����
	}

	return text; //���������� ������ � ������� �������
}



int �ounterWords(vector <string>* text) //������� �������� ����. �� ���� ��������� ������  ������� �������
{
	if (!text->size()) //���� ���� ������ ������, ���������� 0
		return 0;

	int num = 0;

	for (int i = 0; i < text->size(); i++) //������� ���� � ��������� ������� �� ������� ����� �� ����������
	{

		string  sT;   //������� ������� ������
		sT = text->at(i); //� ������� ������ ���������� ���������� ����� �� ������� � ������ ������� �����
		map2[sT]++; //���������� ����� � ���

	}

	return num;
}



int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "rus");

	cout << "Program was started" << endl;

	string fileName("C:\\Users\\Bocman\\Documents\\5Kurse2SEM\\Strychkov\\1\\War2.txt");


	if (argc >= 2)
	{
		fileName = argv[1];

	}

	vector<string> *vecText = filePars(fileName);	//������� ������� ������. �������� ������� filePars, ������� ��������� ����� �� ������
													//� ���������� �� � ������� ������
	
	if (vecText == nullptr)
	{
		cout << "File not found\n" << endl;
		delete vecText;
		return 1;
	}


	unsigned int vremPosleChtenia = clock();		//����������� ����� ������ ��������� �� ������ �� ����� 

	vecText = formatFile(vecText);					//�������� ������� formatFile, ������ ������� ����� � �������. � ���������� 
													//��������� ������
	
	�ounterWords(vecText);							//�������� ������� �������� ���� �� ������� ����



	delete vecText;									//������� ������� ������

	unsigned int vremKonca = clock();				//���������� ����� ������ ���� ���������

	unsigned int vremRaboti = (vremKonca - vremPosleChtenia);//��������� ����� ������ ��������� ��� ������ �����
	
	
	for (auto it = map2.begin(); it != map2.end(); ++it) { //�������� ����� � ������� �� ������� � ������� �� �������� ����
		if (it->first != "")
			cout << it->first << " : " << it->second << endl; 
	}


	cout << "����� ���������� = " << vremKonca << endl; // ����� ������ ���������  
	cout << "Time = " << vremRaboti << endl; // ����� ������ ���������  													

	return 0;
}
