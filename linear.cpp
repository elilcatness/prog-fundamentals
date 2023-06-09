#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
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

using namespace std;

struct NumList {
	int num;
	NumList* next;
};

void showList(NumList* head) {
	while (head != NULL) {
		cout << head->num << " ";
		head = head->next;
	}
	cout << endl;
}

void appendElem(NumList*& head, int val) {
	NumList* cur = head;
	NumList* p = new NumList;
	p->num = val;
	if (cur == NULL) {
		p->next = NULL;
		head = p;
	}
	else {
		while (cur->next) {
			cur = cur->next;
		}
		cur->next = p;
		p->next = NULL;
	}
}

void addElem(NumList*& head, int val) {
	NumList* p = new NumList{val, head};
	head = p;
}

void insertElem(NumList*& head, unsigned idx, int val) {
	if (idx == 0) {
		addElem(head, val);
	}
	else if (idx == 1 && head != NULL && head->next == NULL) {
		appendElem(head, val);
	}
	else if (idx >= 1 && (head == NULL || head->next == NULL)) {
		cout << "Out of range" << endl;
		exit(1);
	}
	else {
		NumList* prev = head;
		NumList* cur = head->next;
		unsigned curIdx = 1;
		bool inserted = false;
		while (cur != NULL) {
			if (curIdx == idx) {
				NumList* p = new NumList{ val, cur };
				prev->next = p;
				inserted = true;
				break;
			}
			prev = cur;
			cur = cur->next;
			curIdx++;
		}
		if (!inserted) {
			cout << "Out of range" << endl;
			exit(1);
		}
	}
}

void deleteElemByIdx(NumList*& head, unsigned idx) {
	NumList* cur = head;
	NumList* prev = NULL;
	for (unsigned curIdx = 0; cur != NULL; curIdx++) {
		if (curIdx == idx) {
			if (prev != NULL && cur->next != NULL)
				prev->next = cur->next;
			else if (prev != NULL && cur->next == NULL)
				prev->next = NULL;
			else
				head = cur->next;
			delete cur;
			break;
		}
		prev = cur;
		cur = cur->next;
	}
}

int findElem(NumList* head, int val) {
	for (unsigned curIdx = 0; head != NULL; curIdx++) {
		if (head->num == val)
			return curIdx;
		head = head->next;
	}
	return -1;
}

void freeList(NumList*& head) {
	NumList* cur;
	while (head->next) {
		cur = head;
		head = head->next;
		delete cur;
	}
	delete head;
}

int main() {
	NumList* list = NULL;
	for (int i = 5; i <= 10; i++)
		if (i != 7)
			appendElem(list, i);
	cout << "original list: ";
	showList(list);
	addElem(list, 0);
	cout << "added zero into the beginning: ";
	showList(list);
	insertElem(list, 3, 7);
	cout << "added missing 7: ";
	showList(list);
	deleteElemByIdx(list, 5);
	cout << "removed 5th elem: ";
	showList(list);
	int elem;
	while (true) {
		cout << "type a number to be found: ";
		cin >> elem;
		if (cin.fail() || cin.get() != '\n') {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "invalid input" << endl;
		}
		else break;
	}
	cout << "find(" << elem << ") = " << findElem(list, elem) << endl;
	freeList(list);
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
	_CrtDumpMemoryLeaks();
}