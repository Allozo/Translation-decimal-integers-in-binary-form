#include <iostream>
#include "cstring"
#include <cmath>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

// перевожу число в строку
string int_to_str(int value) {
    string str;
    ostringstream stream;
    stream << value;
    str = stream.str();
    return str;
}

// печатаю содержимое вектора
template<typename T>
void print(vector<T> &vec) {
    for (auto i: vec) {
        cout << i;
    }
    cout << endl;
}

// перевожу целое (!) 10-ное число в 2-ный вид
vector<int> binary_integer_value(string &value) {
    int value_int = atoi(value.c_str());
    vector<int> sdf;
//    cout << value_int;
    while (value_int != 1) {
        sdf.push_back(value_int % 2);
        value_int /= 2;
    }
    sdf.push_back(1);

    return sdf;
}

// перевожу дробное (!) 10-ное число в 2-ный вид
vector<int> binary_real_value(string &value) {
    double value_double = atof(value.c_str()); // перевели в вещественное число
    vector<int> res;
    for (int i = 0; i < 60; i++) {
        value_double *= 2;
        int r = (int) (floor(value_double));
        res.push_back(r);
        if (value_double >= 1) {
            value_double -= 1;
        }
        cout;
    }
    return res;
}

// перевожу строку, в которой записан double в его двоичный вид
vector<int> conversion_to_binary_double(string &value) {

    // найдем точку
    int pos_point = -1;
    for (int i = 0; i < value.size(); i++) {
        if (value[i] == '.') {
            pos_point = i;
        }
    }

    // определили знак
    bool sign = (value[0] == '-');

    // в integer_part лежит целая часть числа
    string integer_part;
    // начинаю с sign, так как если там 1 (знак есть), то мы должны начать с 2-го бита (отсчет с 1-цы)
    for (int i = sign; i < value.size(); i++) {
        if (value[i] == '.') {
            break;
        }
        integer_part.push_back(value[i]);
    }

    //// тут лежит целая часть в 2-ном виде (начало в конце)
    vector<int> binary_integer_part = binary_integer_value(integer_part);

    // вытащили дробную часть
    string real_part;
    for (int i = pos_point - 1; i < value.size(); i++) {
        real_part.push_back(value[i]);
    }
    real_part[0] = '0';

    //// тут лежит дробная часть в 2-ном виде (начало в начале)
    vector<int> binary_real_part = binary_real_value(real_part);

    // тут посчитали экспоненту
    string exp = int_to_str(binary_integer_part.size() - 1 + 1023); // экспонента

    //// бинарный вид экспоненты (начало в конце)
    vector<int> binary_exp = binary_integer_value(exp);

    // тут будет ответ (бинарный код double)
    vector<int> res;

    // добавили знак к ответу
    res.push_back(sign);

    // записали экспоненту к ответу
    for (int i = binary_exp.size() - 1; i >= 0; i--) {
        res.push_back(binary_exp[i]);
    }

    // записали мантиссу_целую_часть_исходного_числа к ответу
    for (int i = binary_integer_part.size() - 2; i >= 0; i--) {
        res.push_back(binary_integer_part[i]);
    }

    // записали мантиссу_дробную_часть_исходного_числа к ответу
    for (int i = 0; i < binary_real_part.size(); i++) {
        if (res.size() == 64) { // записали нужное кол-во битов
            break;
        }
        res.push_back(binary_real_part[i]);
    }

    // print(res);
    return res;
}

// перевожу двоичное число в целое десятичное
int binary_to_int (vector<int> &value) {
    reverse(value.begin(), value.end());
//    print(value);
    int res = 0;
    for (int i = 0; i < value.size(); i++) {
        res += value[i] * pow(2, i);
    }
    return res;
}

// перевожу двоичное дробное двоичное число в дробное десятичное
double binary_to_double (vector<int> &value) {
    double res = 0;

    for (int i = 0; i < value.size()/2; i++) {
        res += value[i] * pow(2, -i-1);
    }
    return res;
}

// перевожу вектор двоичного числа double обратно в 10-ное число
double conversion_to_double(vector<int> &value) {
    // тут будет ответ (а пока 0)
    double result = 0;

    //// определим знак
    bool sign = false;
    if (value[0] == 1) {
        sign = true;
    }

    //// получим экспоненту числа
    vector<int> bin_exp;
    for (int i = 1; i < 12; i++) {
        bin_exp.push_back(value[i]);
    }

    int exp = binary_to_int(bin_exp) - 1023;

    //// получим мантиссу числа
    vector<int> binary_mantissa;
    binary_mantissa.push_back(1);  // по определения мантиссы
    for (int i = 12; i < value.size(); i++) {
        binary_mantissa.push_back(value[i]);
    }

    //// получим десятичный код числа
    vector<int> binary_int_value;
    for (int i = 0; i < exp + 1; i++) {
        binary_int_value.push_back(binary_mantissa[i]);
    }
    int int_value = binary_to_int(binary_int_value);

    //// получим друбную часть числа
    vector<int> binary_real_value;
    for (int i = exp + 1; i < binary_mantissa.size(); i++) {
        binary_real_value.push_back(binary_mantissa[i]);
    }
    double real_value = binary_to_double(binary_real_value);

    //// прибавили дробную часть
    result += real_value;

    //// прибавили целую часть
    result += int_value;

    //// умножили на знак
    if (sign) {
        result *= -1;
    }

    return result ;
}

int* vector_to_array(vector<int> &vec) {
    size_t size = vec.size();
    int* int_array = new int[size];
    for (int i = 0; i < vec.size(); i++) {
        int_array[i] = vec[i];
    }
    return int_array;
}

int main() {
    string value = "234.345"; // тут будет наше число
    cout << value << endl;

    vector<int> result = conversion_to_binary_double(value); // перевели в двоичный вид
    print(result);

    int* pInt = vector_to_array(result);
    for(int i = 0 ; i < result.size(); i++) {
        cout << pInt[i];
    }
    cout << endl;

    double my_double = conversion_to_double(result); // вернем обратно в 10-ный вид
    cout << my_double << endl;
}
