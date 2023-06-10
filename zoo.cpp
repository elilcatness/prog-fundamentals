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
#include <windows.h>

using namespace std;

const unsigned animalNameSize = 50;
const unsigned animalColorSize = 20;

struct Animal {
    char name[50];
    int age;
    char color[20];
};

struct Zoo {
    unsigned population;
    Animal* animals;
};

Animal getAnimal() {
    cout << "Введите через пробел имя, возраст и цвет животного: ";
    Animal animal;
    cin.getline(animal.name, animalNameSize, ' ');
    cin >> animal.age;
    cin.get();
    cin.getline(animal.color, animalColorSize);
    return animal;
}

unsigned getPopulation() {
    unsigned population;
    while (true) {
        cout << "Введите количество животных в зоопарке: ";
        if (cin.peek() == '-')
            cin.putback('&');
        cin >> population;
        if (cin.fail() || population == 0 || cin.get() != '\n') {
            cin.clear();
            cin.ignore(cin.rdbuf()->in_avail(), '\n');
            cout << "Некорректный ввод!" << endl;
        }
        else return population;
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    Zoo zoo;
    zoo.population = getPopulation();
    zoo.animals = new Animal[zoo.population];
    for (unsigned i = 0; i < zoo.population; i++) {
        zoo.animals[i] = getAnimal();
    }
    cout << "Количество животных в зоопарке: " << zoo.population << "\nИ вот они!" << endl;
    for (unsigned i = 0; i < zoo.population; i++)
        cout << "Имя: " << zoo.animals[i].name << ", Возраст: " << zoo.animals[i].age 
            << ", Цвет: " << zoo.animals[i].color << endl;
    delete[] zoo.animals;
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
    _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
    _CrtDumpMemoryLeaks();
}
