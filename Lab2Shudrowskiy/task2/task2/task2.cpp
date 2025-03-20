#include <iostream>
#include <fstream>  // Для роботи з файлами
#include <string>   // Для роботи з рядками
#include <windows.h>  // Для підтримки кирилиці у консолі Windows

using namespace std;

int countBits(unsigned short x) {
    int count = 0;
    while (x) {
        count += x & 1;
        x >>= 1;
    }
    return count;
}

void encrypt() {
    const int numLines = 4, lineLength = 32;
    string lines[numLines];
    ofstream outFile("encrypted.bin", ios::binary);
    if (!outFile) { cerr << "Помилка відкриття файлу!" << endl; return; }

    cout << "Введіть 4 рядки тексту (до 32 символів):\n";
    for (int i = 0; i < numLines; i++) {
        getline(cin, lines[i]);
        lines[i].resize(lineLength, ' ');
        for (int j = 0; j < lineLength; j++) {
            unsigned short value = (i & 0x03) | ((j & 0x1F) << 2) | ((lines[i][j] & 0xFF) << 7);
            if (countBits(value) % 2) value |= (1 << 15);
            outFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
    }
    cout << "Шифрування завершено!" << endl;
}

void decrypt() {
    const int numLines = 4, lineLength = 32;
    char decrypted[numLines][lineLength + 1] = {};
    ifstream inFile("encrypted.bin", ios::binary);
    if (!inFile) { cerr << "Помилка відкриття файлу!" << endl; return; }

    for (int i = 0; i < numLines * lineLength; i++) {
        unsigned short value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));
        int row = value & 0x03, pos = (value >> 2) & 0x1F, ascii = (value >> 7) & 0xFF;
        decrypted[row][pos] = static_cast<char>(ascii);
    }

    cout << "Розшифрований текст:\n";
    for (int i = 0; i < numLines; i++) cout << decrypted[i] << endl;
}

int main() {
    SetConsoleCP(1251); SetConsoleOutputCP(1251);
    int choice;
    cout << "1 - Шифрування\n2 - Розшифрування\nВаш вибір: ";
    cin >> choice; cin.ignore();
    choice == 1 ? encrypt() : decrypt();
    return 0;
}
