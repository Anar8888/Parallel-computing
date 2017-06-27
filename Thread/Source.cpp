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
{																		// И работает с участком слов от start до end 
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
		if (WaitForSingleObject(hSemaphore, 30000) == WAIT_FAILED)	//Объявляем семафор, который не даёт обратится к участки программы
																	//если он занят другим потоком. Как только освободится, мы занимаем его
			return;

		map2[sT]++;//Записываем слово в мап
		if (hSemaphore != NULL)
			ReleaseSemaphore(hSemaphore, 1, NULL);					//Освобождение семафора
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

	vector<string> *vecText = filePars(fileName);//Создаем рабочий вектор. Вызываем функцию filePars, которая разбивает книгу по словам
													//и записывает их в рабочий вектор

	if (vecText == nullptr)
	{
		std::cout << "error in reading data" << endl;
		return 1;
	}

	unsigned int vremPosleChtenia = clock();		//Затраченное время работы программы на чтение из файла 



	int textSize = vecText->size();					//Вызываем функцию formatFile, которя очищает слова в векторе. И возвращаем 
													//очищенный вектор

	if (vecText->size() <  numberOfProcesses)
	{
		//Не делим на процессы
		сounterWords2(1, vecText, 0, textSize - 1); //Числитель - число появлений конкретной nGramm-ы

	}
	else
	{
		//Делим на процессы
		vector<thread*> vecThreads;					//Создаем вектор потоков

		int step = textSize / numberOfProcesses; //Пример: 8-длина текста, 2- число проц. 8/2=4
		int start = 0;						//от 0
		int end = step - 1;					//до 3 и от 4 до 7

		for (int i = 1; i <= numberOfProcesses; i++)		//Цикл создание Потоков
		{
			auto th = new std::thread(formatFileV2, vecText, start, end);	//Создаем поток с функцией formatFileV2 и передаем границы работы
			vecThreads.push_back(th);										//Запись Потока в вектор потоков

			start = end;

			//последний процесс захватывает все слова до конца
			if (i == numberOfProcesses)
				end = textSize - 1;
			else
				end = end + step;
		}

		for (auto &th : vecThreads)				//Перебираем вектор Потоков
		{
			th->join();							//Ожидаем завершение потока
			delete th;
		}
		vecThreads.clear();

		step = textSize / numberOfProcesses; //Пример: 8-длина текста, 2- число проц. 8/2=4
		start = 0;						//от 0
		end = step - 1;					//до 3 и от 4 до 7

		for (int i = 1; i <= numberOfProcesses; i++)
		{
			//if(i==1)
			auto th = new std::thread(сounterWords2, i, vecText, start, end);

			vecThreads.push_back(th);

			start = end;

			//последний процесс захватывает все слова доконце
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
	unsigned int vremRaboti = (vremKonca - vremPosleChtenia);//в секундах

	for (auto it = map2.begin(); it != map2.end(); ++it) {
	//if(it->first!="")
	cout << it->first << " : " << it->second << endl;
	}

	cout << "Время работы программы  = " << vremKonca << endl; // время работы программы  
	cout << "Time = " << vremRaboti << endl; // время работы программы без чтения 
	CloseHandle(hSemaphore);
	system("pause");

	ofstream fout("Statistics.txt", ios_base::app); // открываем файл для добавления информации к концу файла

	if (!fout.is_open()) // если файл не открыт
		cout << "Файл не может быть открыт!\n"; // сообщить об этом

	fout << vremRaboti << endl;
	fout.close();

	return 0;
}
