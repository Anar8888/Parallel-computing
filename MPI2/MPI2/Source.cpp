#include <mpi.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iomanip>
using namespace std;
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
{																		// � �������� � �������� ���� �� start �� end. ���� ������������ �������� ������ ��� �������� 
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
		char buf[10000];
		//double rexM = 2;
		//sprintf(buf, "%i", start);
		strcpy(buf, sT.c_str());
		MPI_Send(buf, sizeof(buf), MPI_CHAR, 0, 0, MPI_COMM_WORLD);		//������� �������� MPI. buf �������� ������������ �����, 0 - ����� �������� ��������.
	}
	
	//cout << buf << endl;
	return;
}





int main(int argc, char **argv) {
	setlocale(LC_ALL, "rus");

	std::cout << "Program was started" << endl;

	string fileName("C:\\Users\\Bocman\\Documents\\5Kurse2SEM\\Strychkov\\1\\War2.txt");
	int numberOfProcesses = 4;
	if (argc >= 3)
	{
		fileName = argv[1];
		numberOfProcesses = atoi(argv[2]);
	}

	vector<string> *vecText = filePars(fileName);	//������� ������� ������. �������� ������� filePars, ������� ��������� ����� �� ������
	//� ���������� �� � ������� ������
	if (vecText == nullptr)
	{
		std::cout << "error in reading data" << endl;
		return 1;
	}

	unsigned int timeAfterReading = clock();		//����������� ����� ������ ��������� �� ������ �� ����� 
	int textSize = vecText->size();					//�������� ������� formatFile, ������ ������� ����� � �������. � ���������� 
													//��������� ������


	MPI_Init(&argc, &argv);							//���������������� MPI

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);			//�������� id �������� ����������
	MPI_Comm_size(MPI_COMM_WORLD, &size);			//�������� ����������� ���������� �����������
	numberOfProcesses = size-1;
	if (rank) //�������. ���� ��� �� ������� ���������.
	{
		int step = textSize / numberOfProcesses; //������: 8-����� ������, 2- ����� ����. 8/2=4
		int start = 0;						//�� 0
		int end = step - 1;					//�� 3 � �� 4 �� 7
		//cout <<"Text size: "<< textSize << endl;
		for (int i = 1; i <= numberOfProcesses; i++)
		{
			if (i == rank){					//�������� �� ������������ ������� ����� �������� ��������
				formatFileV2(vecText, start, end);
				cout << "I Process " << rank << " Start:" << start << " End:" << end << endl;
			}
			start = end;

			//��������� ������� ����������� ��� ����� �� �����
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}

		step = textSize / numberOfProcesses; //������: 8-����� ������, 2- ����� ����. 8/2=4
		start = 0;						//�� 0
		end = step - 1;					//�� 3 � �� 4 �� 7

		for (int i = 1; i <= numberOfProcesses; i++)
		{
			if (i == rank)//�������� �� ������������ ������� ����� �������� ��������
			�ounterWords2( i, vecText, start, end);


			start = end;

			//��������� ������� ����������� ��� ����� �������
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}
		
	}
	else { //������ �������� ����������
		
		cout << "Process 0 started" << endl;
		int step = textSize / numberOfProcesses; //������: 8-����� ������, 2- ����� ����. 8/2=4
		int start = 0;						//�� 0
		int end = step - 1;					//�� 3 � �� 4 �� 7
		//cout << "Text size: " << textSize << endl;
		for (int i = 1; i <= numberOfProcesses; i++) //������ ������� ����� ����� ������� ����
		{
			
			start = end;

			//��������� ������� ����������� ��� ����� �� �����
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}


		for (int i(1); i < end; ++i) //���� �������� ���� �� ���� �� ������� ���������
		{
			MPI_Status s;													// ������� ���������� MPI ��������
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);		// ������ ��� ������������ ������� � ���������� ��� � ������� ���������� MPI �������� s
			int count;
			MPI_Get_count(&s, MPI_CHAR, &count);							// ������ ����� ���������� ��������

			
			char *buf = new char[count];
			MPI_Recv(buf, count, MPI_CHAR,									// ��������� ���������(�����)
				MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
			//cout << "Process " << s.MPI_SOURCE << ": "<< "End work" << endl;
			//double x = atof(buf);
			string str(buf);												// ������������ ���������� char � string
			//cout << "Info "<<buf <<endl;
			map2[str]++;													// ���������� � ��� ��� ��������
			delete[] buf;
			

		}
		unsigned int timeEnd = clock();
		ofstream fout("Statistics.txt", ios_base::app); // ��������� ���� ��� ���������� ���������� � ����� �����

		if (!fout.is_open()) // ���� ���� �� ������
			cout << "���� �� ����� ���� ������!\n";
		for (auto it = map2.begin(); it != map2.end(); ++it) {
			//if(it->first!="")
			//cout << it->first << " : " << it->second << endl;/////////////////////////////////////////////////////////////////////////////////////
			

			fout << it->first << " : " << it->second << endl;
			

		}
		fout.close();
		unsigned int workTime = (timeEnd - timeAfterReading);// / 1000; //� ��������
		cout << "Time = " << workTime << endl; // ����� ������ ���������
	}
	MPI_Finalize();					// ���������� MPI
		
	
	 
	return 0;
}




