#include <iostream>
using namespace std;

int main() {
    int a, b, c, d;

    cout << "Write down a, b, c, d: ";
    cin >> a >> b >> c >> d;

    // 1) 17*b 
    int term_b = (b << 4) + b;

    // 2) 3012*a 
    int term_a = (a << 11) + (a << 9) + (a << 8) + (a << 7) + (a << 6) + (a << 2);

    // Сума (17*b + 3012*a)
    int numerator = term_b + term_a;

    // 3) Ділення на 32 → >> 5
    int part = numerator >> 5;

    // 4) c*120
    int term_c = (c << 7) - (c << 3);

    // 5) d*127
    int term_d = (d << 7) - d;

    // Повний вираз:
    // ( (17*b + 3012*a) / 32 ) - c*120 + d*127
    int result = part - term_c + term_d;

    cout << "Result: " << result << endl;
    return 0;
}