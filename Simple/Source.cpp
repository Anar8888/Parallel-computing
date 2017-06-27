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


vector<string>* formatFile(vector<string>* text)//Очищает входящий (С лишними символами)вектор слов, и возвращает очищенный
{

	for (string& str : *text) //Цикл, пока в входящем векторе есть слова
	{
		str = formatStr(str); //Вызов функции очищения одного слова
	}

	return text; //Возвращаем вектор с чистыми словами
}



int сounterWords(vector <string>* text) //Функция подсчета слов. На вход принимает вектор  чистыми словами
{
	if (!text->size()) //Если хотя вектор пустой, возвращаем 0
		return 0;

	int num = 0;

	for (int i = 0; i < text->size(); i++) //Перебор слов в очищенном векторе от первого слова до последнего
	{

		string  sT;   //создаем рабочий стринг
		sT = text->at(i); //в рабочий стринг записываем полученное слово из вектора в данном проходе цикла
		map2[sT]++; //Записываем слово в мап

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

	vector<string> *vecText = filePars(fileName);	//Создаем рабочий вектор. Вызываем функцию filePars, которая разбивает книгу по словам
													//и записывает их в рабочий вектор
	
	if (vecText == nullptr)
	{
		cout << "File not found\n" << endl;
		delete vecText;
		return 1;
	}


	unsigned int vremPosleChtenia = clock();		//Затраченное время работы программы на чтение из файла 

	vecText = formatFile(vecText);					//Вызываем функцию formatFile, которя очищает слова в векторе. И возвращаем 
													//очищенный вектор
	
	сounterWords(vecText);							//Вызываем функцию подсчёта слов из вектора слов



	delete vecText;									//Удаляем рабочий вектор

	unsigned int vremKonca = clock();				//Записываем время работы всей программы

	unsigned int vremRaboti = (vremKonca - vremPosleChtenia);//Вычисляем время работы программы без чтения файла
	
	
	for (auto it = map2.begin(); it != map2.end(); ++it) { //Печатаем слова и частоту их встречи в консоль из рабочего Мапа
		if (it->first != "")
			cout << it->first << " : " << it->second << endl; 
	}


	cout << "Время выполнения = " << vremKonca << endl; // время работы программы  
	cout << "Time = " << vremRaboti << endl; // время работы программы  													

	return 0;
}
