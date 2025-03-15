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
    const int numLines = 4;      // Кількість рядків
    const int lineLength = 32;   // Довжина кожного рядка
    string lines[numLines];

    cout << "Введіть 4 рядки тексту (до 32 символів кожен):" << endl;
    for (int i = 0; i < numLines; i++) {
        getline(cin, lines[i]);
        // Якщо рядок більше 32 символів, обрізаємо його
        if (lines[i].length() > lineLength) {
            lines[i] = lines[i].substr(0, lineLength);
        }
        // Якщо рядок менше 32 символів, доповнюємо пробілами до 32 символів
        while (lines[i].length() < lineLength) {
            lines[i] += " ";
        }
    }

    // Відкриваємо бінарний файл для запису зашифрованих даних
    ofstream outFile("encrypted.bin", ios::binary);
    if (!outFile) {
        cerr << "Помилка відкриття файлу для запису!" << endl;
        return;
    }

    // Проходимо по кожному символу всіх рядків та шифруємо його
    for (int row = 0; row < numLines; row++) {
        for (int pos = 0; pos < lineLength; pos++) {
            // Отримуємо ASCII-код символу
            unsigned short symbol = lines[row][pos];

            // Будуємо 16-бітове значення без біту парності:
            // Біти 0-1: номер рядка (row)
            // Біти 2-6: позиція символу (pos)
            // Біти 7-14: ASCII-код символу (symbol)
            unsigned short value = 0;
            value |= (row & 0x03);             // bits 0–1
            value |= ((pos & 0x1F) << 2);        // bits 2–6
            value |= ((symbol & 0xFF) << 7);     // bits 7–14

            // Підраховуємо кількість одиниць у сформованому числі (без біту парності)
            int onesCount = countBits(value);
            // Встановлюємо біт парності (біт 15): якщо кількість одиниць непарна, ставимо 1
            unsigned short parity = (onesCount % 2) ? 1 : 0;
            value |= (parity << 15);             // встановлюємо біт 15

            // Записуємо 16-бітове значення у файл
            outFile.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
    }
    outFile.close();
    cout << "Шифрування завершено. Дані записано у файл encrypted.bin" << endl;
}

// Функція розшифрування
void decrypt() {
    const int numLines = 4;      // Кількість рядків
    const int lineLength = 32;   // Довжина кожного рядка

    // Створюємо матрицю для розшифрованих символів (32 символи + символ закінчення рядка)
    char decrypted[numLines][lineLength + 1];
    for (int i = 0; i < numLines; i++) {
        for (int j = 0; j < lineLength; j++) {
            decrypted[i][j] = ' ';
        }
        decrypted[i][lineLength] = '\0';
    }

    // Відкриваємо бінарний файл для читання
    ifstream inFile("encrypted.bin", ios::binary);
    if (!inFile) {
        cerr << "Помилка відкриття файлу для читання!" << endl;
        return;
    }

    // Зчитуємо 16-бітові значення з файлу та розшифровуємо їх
    for (int i = 0; i < numLines * lineLength; i++) {
        unsigned short value;
        inFile.read(reinterpret_cast<char*>(&value), sizeof(value));
        if (!inFile) break;

        // Виділяємо поля з 16-бітового значення:
        int row = value & 0x03;            // Біти 0-1: номер рядка
        int pos = (value >> 2) & 0x1F;       // Біти 2-6: позиція символу
        int ascii = (value >> 7) & 0xFF;     // Біти 7-14: ASCII-код символу
        int parity = (value >> 15) & 0x01;   // Біт 15: біт парності

        // (Необов’язково) Перевірка біту парності:
        unsigned short valueWithoutParity = value & 0x7FFF; // нижчих 15 біт
        int onesCount = countBits(valueWithoutParity);
        if ((onesCount % 2 == 1 && parity != 1) || (onesCount % 2 == 0 && parity != 0)) {
            cerr << "Помилка парності для символу на рядку " << row << " позиції " << pos << endl;
        }

        // Записуємо відновлений символ у відповідну позицію матриці
        decrypted[row][pos] = static_cast<char>(ascii);
    }
    inFile.close();

    // Виводимо розшифрований текст у консоль
    cout << "Розшифрований текст:" << endl;
    for (int i = 0; i < numLines; i++) {
        cout << decrypted[i] << endl;
    }

    // Записуємо розшифрований текст у текстовий файл
    ofstream outFile("decrypted.txt");
    if (!outFile) {
        cerr << "Помилка відкриття файлу для запису розшифрованої інформації!" << endl;
        return;
    }
    for (int i = 0; i < numLines; i++) {
        outFile << decrypted[i] << endl;
    }
    outFile.close();
}

// Головна функція з меню вибору режиму
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int choice;
    cout << "Виберіть режим:" << endl;
    cout << "1 - Шифрування" << endl;
    cout << "2 - Розшифрування" << endl;
    cout << "Ваш вибір: ";
    cin >> choice;
    cin.ignore();  // Видаляємо символ нового рядка після вводу числа

    if (choice == 1) {
        encrypt();
    }
    else if (choice == 2) {
        decrypt();
    }
    else {
        cout << "Невірний вибір. Завершення програми." << endl;
    }

    return 0;
}