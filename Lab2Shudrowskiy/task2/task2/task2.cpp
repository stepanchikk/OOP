#include <iostream>
#include <fstream>  // Для роботи з файлами
#include <string>   // Для роботи з рядками
#include <windows.h>  // Для підтримки кирилиці у консолі Windows

using namespace std;


//Ця функція підраховує кількість одиничних бітів (1) у 16-бітному числі x
int countBits(unsigned short x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}

//Шифрування
void encrypt() {
    const int numLines = 4, lineLength = 32;
    string lines[numLines];  //створення масиву для збереження тексту
    ofstream outFile("encrypted.bin", ios::binary);  //відкриває файл у бінарному режимі
    if (!outFile) { cerr << "Помилка відкриття файлу!" << endl; return; }

    cout << "Введіть 4 рядки тексту (до 32 символів):\n";
    for (int i = 0; i < numLines; i++) {
        getline(cin, lines[i]); //читає рядок з клавіатури
        lines[i].resize(lineLength, ' ');  //доповнює рядок до 32 символів (якщо він коротший)
        for (int j = 0; j < lineLength; j++) {
            unsigned short value = (i & 0x03) | ((j & 0x1F) << 2) | ((lines[i][j] & 0xFF) << 7);  //(j & 0x1F) << 2 – зберігає позицію символу у 5 бітах (0-31);  (lines[i][j] & 0xFF) << 7 – бере ASCII-код символу та зміщує його вліво на 7 біт
            if (countBits(value) % 2) value |= (1 << 15); //перевіряє, чи кількість 1 у числі непарна.Якщо так, встановлюється старший (15-й) біт для забезпечення парної кількості 1

            outFile.write(reinterpret_cast<const char*>(&value), sizeof(value)); //записує 16-бітне число у файл
        }
    }
    cout << "Шифрування завершено!" << endl;
}


//Розшифрування
void decrypt() {
    const int numLines = 4, lineLength = 32;
    char decrypted[numLines][lineLength + 1] = {}; //масив для збереження відновлених рядків
    ifstream inFile("encrypted.bin", ios::binary); //відкриває файл для читання
    if (!inFile) { cerr << "Помилка відкриття файлу!" << endl; return; } 

    for (int i = 0; i < numLines * lineLength; i++) {
        unsigned short value; //змінна для тимчасового зберігання кожного 16-бітного закодованого символу


        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));  // зчитує 16-бітне число
        int row = value & 0x03, pos = (value >> 2) & 0x1F, ascii = (value >> 7) & 0xFF; //row = value & 0x03 – визначає номер рядка;pos = (value >> 2) & 0x1F – визначає позицію символу;ascii = (value >> 7) & 0xFF – отримує ASCII-код символу
        decrypted[row][pos] = static_cast<char>(ascii); //заповнюється відновленим символом
    }

    cout << "Розшифрований текст:\n";
    for (int i = 0; i < numLines; i++) cout << decrypted[i] << endl;
}

int main() {
    SetConsoleCP(1251); SetConsoleOutputCP(1251);
    int choice;
    cout << "1 - Шифрування\n2 - Розшифрування\nВаш вибір: ";
    cin >> choice; cin.ignore(); //очищає буфер після cin
    choice == 1 ? encrypt() : decrypt(); //скороченя форма if-else
    return 0;
}
