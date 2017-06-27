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
// имя семафора
const char lpSemaphoreName[] = "MySemaphore";

string formatStr(string& str)// Очищает входящий стринг от лишних символов и возвращает очищенную стрингу(очищенное слово)
{
	string newStr = "";		//Рабочий стринг. В него будет записано чистое слово

	for (int i = 0; i < str.size(); i++) //Цикл по символам грязногослова. От первого(0) символа до последнего(str.size())
	{
		if (!isalpha((unsigned char)str[i])) //Если попался символам, то ничего не далаем
			continue;

		char ch = tolower(str[i]);  //Продолжение если попалась Буква
		newStr.push_back(ch); //Записываем букву в конец рабочего стринга

	}
	return newStr; //Возврящаем полученное очищенное слово
}



void formatFileV2(vector<string>* text, int start, int end)//Очищает входящую ЧАСТЬ(С лишними символами)вектора слов, и возвращает очищенный
{
	for (int i = start; i <= end; i++) //Цикл, пока в входящей Части вектора (от start слов до end слов)  есть слова
	{
		text->at(i) = formatStr(text->at(i));//Вызов функции очищения одного слова и перезапись на грязное.
	}

	return;
}

vector<string>* filePars(string fileName)//Создание вектора слов(возможно с лишними символами).На вход получаем имя файла. На выходе вектор слов
{
	vector <string>* vec = new vector < string >; //Создаем вектор, который мы вернем
	ifstream file(fileName);	//Создаем входящий стрим нашей книги

	if (!file.is_open()) //если файл не открыт, тогда ретерн
		return nullptr;

	string word;  //создаем рабочий стринг

	while (file >> word)   //Пока в файле есть слова, записываем слово в рабочий стринг
	{
		vec->push_back(word); //в ранее созданный вектор записываем считанное слово из файла.
	}

	return vec; //возвращаем вектор, заполненный словами из книги
}


void сounterWords2(int  numth, vector <string>* text, int start, int end)//Функция подсчета слов. На вход принимает вектор  с чистыми словами. 
{																		// И работает с участком слов от start до end. Лова отправляются главному потоку для подсчёта 
	if (!text->size()) //Если хотя бы один вектор пуст
		return;

	if (start < 0 || end > text->size() - 1 || end < start)
		return;

	if (end - start + 1 < 1)
		return;

	int num = 0;

	for (int i = start; i <= end; i++)//Перебор слов в очищенном Участке векторе от start слова до последнего(end)
	{

		string sT;//создаем рабочий стринг
		sT = text->at(i);//в рабочий стринг записываем полученное слово из вектора в данном проходе цикла
		char buf[10000];
		//double rexM = 2;
		//sprintf(buf, "%i", start);
		strcpy(buf, sT.c_str());
		MPI_Send(buf, sizeof(buf), MPI_CHAR, 0, 0, MPI_COMM_WORLD);		//Функция отправки MPI. buf содержит отправляемое слово, 0 - адрес главного процесса.
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

	vector<string> *vecText = filePars(fileName);	//Создаем рабочий вектор. Вызываем функцию filePars, которая разбивает книгу по словам
	//и записывает их в рабочий вектор
	if (vecText == nullptr)
	{
		std::cout << "error in reading data" << endl;
		return 1;
	}

	unsigned int timeAfterReading = clock();		//Затраченное время работы программы на чтение из файла 
	int textSize = vecText->size();					//Вызываем функцию formatFile, которя очищает слова в векторе. И возвращаем 
													//очищенный вектор


	MPI_Init(&argc, &argv);							//Иницианилизируем MPI

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);			//Получаем id текущего процессора
	MPI_Comm_size(MPI_COMM_WORLD, &size);			//Получаем колличество запущенных процессоров
	numberOfProcesses = size-1;
	if (rank) //Условие. Если это не главный процессор.
	{
		int step = textSize / numberOfProcesses; //Пример: 8-длина текста, 2- число проц. 8/2=4
		int start = 0;						//от 0
		int end = step - 1;					//до 3 и от 4 до 7
		//cout <<"Text size: "<< textSize << endl;
		for (int i = 1; i <= numberOfProcesses; i++)
		{
			if (i == rank){					//Проверка на соответствие участка книги текущему процессу
				formatFileV2(vecText, start, end);
				cout << "I Process " << rank << " Start:" << start << " End:" << end << endl;
			}
			start = end;

			//последний процесс захватывает все слова до конца
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}

		step = textSize / numberOfProcesses; //Пример: 8-длина текста, 2- число проц. 8/2=4
		start = 0;						//от 0
		end = step - 1;					//до 3 и от 4 до 7

		for (int i = 1; i <= numberOfProcesses; i++)
		{
			if (i == rank)//Проверка на соответствие участка книги текущему процессу
			сounterWords2( i, vecText, start, end);


			start = end;

			//последний процесс захватывает все слова доконце
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}
		
	}
	else { //Запуск Главного Процессора
		
		cout << "Process 0 started" << endl;
		int step = textSize / numberOfProcesses; //Пример: 8-длина текста, 2- число проц. 8/2=4
		int start = 0;						//от 0
		int end = step - 1;					//до 3 и от 4 до 7
		//cout << "Text size: " << textSize << endl;
		for (int i = 1; i <= numberOfProcesses; i++) //Узнаем сколько всего будет принято слов
		{
			
			start = end;

			//последний процесс захватывает все слова до конца
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}


		for (int i(1); i < end; ++i) //Цикл принятия слов от всех не нлавных Процессов
		{
			MPI_Status s;													// Рабочая переменная MPI процесса
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);		// Узнаем тип принимаемого объекта и записываем его в Рабочую переменную MPI процесса s
			int count;
			MPI_Get_count(&s, MPI_CHAR, &count);							// Узнаем длину принимаего соощения

			
			char *buf = new char[count];
			MPI_Recv(buf, count, MPI_CHAR,									// Принимаем сообщение(слово)
				MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &s);
			//cout << "Process " << s.MPI_SOURCE << ": "<< "End work" << endl;
			//double x = atof(buf);
			string str(buf);												// Конвертируем полученный char в string
			//cout << "Info "<<buf <<endl;
			map2[str]++;													// Записываем в мап для подсчета
			delete[] buf;
			

		}
		unsigned int timeEnd = clock();
		ofstream fout("Statistics.txt", ios_base::app); // открываем файл для добавления информации к концу файла

		if (!fout.is_open()) // если файл не открыт
			cout << "Файл не может быть открыт!\n";
		for (auto it = map2.begin(); it != map2.end(); ++it) {
			//if(it->first!="")
			//cout << it->first << " : " << it->second << endl;/////////////////////////////////////////////////////////////////////////////////////
			

			fout << it->first << " : " << it->second << endl;
			

		}
		fout.close();
		unsigned int workTime = (timeEnd - timeAfterReading);// / 1000; //в секундах
		cout << "Time = " << workTime << endl; // время работы программы
	}
	MPI_Finalize();					// Завершение MPI
		
	
	 
	return 0;
}




