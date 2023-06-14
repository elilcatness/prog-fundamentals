// Для обнаружения утечек памяти
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define newDBG_NEW
#endif
#endif

#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

const char* filename = "input.txt";
const int wordLenLimit = 1000;

struct List {
	const char* s;
	List* next;
};

bool isLessThan(const char* s1, const char* s2) {
	int len1 = strlen(s1), len2 = strlen(s2);
	if (!len1)
		return true;
	if (!len2)
		return false;
	int minLen = min(len1, len2);
	for (int i = 0; i < minLen; i++)
		if (tolower(s1[i]) < tolower(s2[i]))
			return true;
	return false;
}

bool isEqual(const char* s1, const char* s2) {
	if (strlen(s1) != strlen(s2))
		return false;
	for (int i = 0; i < strlen(s1); i++)
		if (s1[i] != s2[i])
			return false;
	return true;
}

void addElem(List*& head, const char* s) {
	List* p = new List{ s, NULL };
	List* cur = head;
	if (!cur)
		head = p;
	else if (!cur->next) {
		if (isLessThan(s, cur->s)) {
			p->next = cur;
			head = p;
		}
		else
			cur->next = p;
	}
	else {
		List* prev = NULL;
		while (cur->next) {
			if (isLessThan(s, cur->s)) {
				p->next = cur;
				if (prev)
					prev->next = p;
				return;
			}
			prev = cur;
			cur = cur->next;
		}
		if (isLessThan(s, cur->s)) {
			p->next = cur;
			if (prev)
				prev->next = p;
		}
		else
			cur->next = p;
	}
}

bool deleteElem(List*& head, const char* s) {
	List* prev = NULL;
	List* cur = head;
	while (cur) {
		if (isEqual(cur->s, s)) {
			if (prev && cur->next)
				prev->next = cur->next;
			else if (prev)
				prev->next = NULL;
			else if (!prev && cur->next)
				head = cur->next;
			if (cur == head)
				head = NULL;
			delete[] cur->s;
			delete cur;
			return true;
		}
		prev = cur;
		cur = cur->next;
	}
	return false;
}

void freeList(List*& head) {
	if (!head)
		return;
	List* cur;
	while (head->next) {
		cur = head;
		head = head->next;
		delete[] cur->s;
		delete cur;
	}
	delete[] head->s;
	delete head;
}

void showList(List* head) {
	while (head) {
		cout << head->s << endl;
		head = head->next;
	}
}

char* getDynamicStr(char* s0) {
	int len = strlen(s0);
	char* s = new char[len + 1];
	for (int i = 0; i < len; i++) {
		s[i] = s0[i];
		s0[i] = '\0';
	}
	s[len] = '\0';
	return s;
}

int readWords(List*& list, const char* filename) {
	ifstream f(filename);
	if (!f.is_open())
		return 0;
	char s0[wordLenLimit];
	int wordsCount = 0;
	while (!f.eof()) {
		f >> s0;
		if (s0 == "\n" || s0[0] == '\0') {
			s0[0] = '\0';
			continue;
		}
		addElem(list, getDynamicStr(s0));
		wordsCount++;
	}
	f.close();
	return wordsCount;
}

int getAction() {
	int action;
	while (true) {
		cout << "Выберите действие:\n" << "0 — считать из файла\n" << "1 — удалить слово\n"
			<< "2 — вывести список\n" << "3 — выход из программы"  << endl;
		cin >> action;
		if (cin.fail() || action < 0 || action > 3 || cin.get() != '\n') {
			cin.clear();
			cin.ignore(cin.rdbuf()->in_avail(), '\n');
			cout << "\nНекорректный ввод!\n" << endl;
		}
		else return action;
	}
}

char* getWord() {
	char s0[wordLenLimit];
	while (true) {
		cout << "Введите слово для удаления: ";
		cin.getline(s0, wordLenLimit, '\n');
		if (cin.fail() || s0[0] == '\0') {
			cin.ignore(cin.rdbuf()->in_avail(), '\n');
			cin.clear();
			cout << "Некорректный ввод!" << endl;
		}
		else return getDynamicStr(s0);
	}
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	List* list = NULL;
	int action, wordsCount, totalWordsCount = 0;
	char* word;
	bool working = true;
	while (working) {
		action = getAction();
		cout << endl;
		switch (action) {
		case 0:
			wordsCount = readWords(list, filename);
			if (wordsCount == 0) {
				cout << "Не удалось открыть файл " << filename << endl;
				exit(1);
			}
			totalWordsCount += wordsCount;
			cout << "Считано слов из файла: " << wordsCount << ", общее кол-во слов: " 
				<< totalWordsCount << endl << endl;
			break;
		case 1:
			word = getWord();
			if (deleteElem(list, word)) {
				cout << "Первое вхождение слова \"" << word << "\" было успешно удалено!\n" << endl;
				totalWordsCount--;
			}
			else
				cout << "Слова \"" << word << "\" нет в списке!\n" << endl;
			delete[] word;
			
			break;
		case 2:
			if (list) {
				cout << "Список:" << endl;
				showList(list);
			}
			else
				cout << "Список пуст!" << endl;
			cout << endl;
			break;
		case 3:
			cout << "Конец работы!" << endl;
			working = false;
			break;
		}
	}
	freeList(list);
	// Для обнаружения утечек памяти
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
}
