#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

// Функція для підрахунку кількості одиниць у числі (для обчислення біту парності)
int countBits(unsigned short x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}

// Функція шифрування
void encrypt() {
    const int numLines = 4;
    const int lineLength = 32;
    string lines[numLines];

    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Помилка відкриття input.txt!" << endl;
        return;
    }

    cout << "Зчитуємо рядки з файлу input.txt..." << endl;
    for (int i = 0; i < numLines && getline(inputFile, lines[i]); i++) {
        if (lines[i].length() > lineLength) {
            lines[i] = lines[i].substr(0, lineLength);
        }
        while (lines[i].length() < lineLength) {
            lines[i] += " ";
        }
    }
    inputFile.close();

    ofstream outFile("encrypted.bin", ios::binary);
    if (!outFile) {
        cerr << "Помилка відкриття файлу для запису!" << endl;
        return;
    }

    for (int row = 0; row < numLines; row++) {
        for (int pos = 0; pos < lineLength; pos++) {
            unsigned short symbol = lines[row][pos];
            unsigned short value = 0;
            value |= (row & 0x03);
            value |= ((pos & 0x1F) << 2);
            value |= ((symbol & 0xFF) << 7);
            int onesCount = countBits(value);
            unsigned short parity = (onesCount % 2) ? 1 : 0;
            value |= (parity << 15);
            outFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
    }
    outFile.close();
    cout << "Шифрування завершено. Дані записано у encrypted.bin" << endl;
}

// Функція розшифрування
void decrypt() {
    const int numLines = 4;
    const int lineLength = 32;
    char decrypted[numLines][lineLength + 1];
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < lineLength; j++) {
            decrypted[i][j] = ' ';
        }
        decrypted[i][lineLength] = '\0';
    }

    ifstream inFile("encrypted.bin", ios::binary);
    if (!inFile) {
        cerr << "Помилка відкриття файлу для читання!" << endl;
        return;
    }

    for (int i = 0; i < numLines * lineLength; i++) {
        unsigned short value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));
        if (!inFile) break;
        int row = value & 0x03;
        int pos = (value >> 2) & 0x1F;
        int ascii = (value >> 7) & 0xFF;
        decrypted[row][pos] = static_cast<char>(ascii);
    }
    inFile.close();

    ofstream outFile("decrypted.txt");
    if (!outFile) {
        cerr << "Помилка відкриття файлу для запису!" << endl;
        return;
    }
    for (int i = 0; i < numLines; i++) {
        outFile << decrypted[i] << endl;
    }
    outFile.close();
    cout << "Розшифрування завершено. Дані записано у decrypted.txt" << endl;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    cout << "Виберіть режим:" << endl;
    cout << "1 - Шифрування" << endl;
    cout << "2 - Розшифрування" << endl;
    cout << "Ваш вибір: ";
    cin >> choice;
    cin.ignore();

    if (choice == 1) {
        encrypt();
    } else if (choice == 2) {
        decrypt();
    } else {
        cout << "Невірний вибір. Завершення програми." << endl;
    }
    return 0;
}
