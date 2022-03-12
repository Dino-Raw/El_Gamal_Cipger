#include <iostream>
#include <fstream>
#include <ctime>
#include <string>


using namespace std;

const unsigned int p = 23189;
const unsigned int g = 23;
const unsigned int shift = 10;
string symbols = "абвгдежзийклмнопрстуфхцчшщъыьэюяabcdefghijklmnopqrstuvwxyz_,.";


int get_index(char symbol)
{
	for (int code = 0; code <= symbols.length(); code++)
		if (symbol == symbols[code])
			return code + shift;
	return -1;
}


int* convet_text_to_numbers(string text)
{
	int* num = new int[text.length()];
	for (unsigned int count = 0; count < text.length(); count++)
	{
		num[count] = get_index(text[count]);
	}
	cout << endl;
	return num;
}


string convet_numbers_to_text(unsigned _int64* num, int sizeNum)
{
	string text = "";

	for (unsigned int count = 0; count < sizeNum; count++)
	{
		text += symbols[num[count] - shift];
	}
	return text;
}


unsigned _int64 fast_exponentiatios(int num, int exp)
{
	unsigned _int64 result = 1;
	while (exp)
	{
		if (exp % 2 == 0)
		{
			exp /= 2;
			num = ((num % p) * (num % p));
		}
		else
		{
			exp--;
			result = ((result % p) * (num % p));
		}
	}
	return result % p;
}


int euclid(int a, int b)
{
	while (b)
	{
		a %= b;
		swap(a, b);
	}
	return a;
}


int get_random_number(int min, int max)
{
	return abs(min + rand() % (max - min + 1));
}


unsigned _int64** encode(int* num, int x, int sizeNum)
{
	unsigned _int64** result;
	result = new unsigned _int64 * [sizeNum];

	for (unsigned int count = 0; count < sizeNum; count++)
	{
		unsigned _int64 y = fast_exponentiatios(g, x);
		int k = 0;

		while (true)
		{
			k = get_random_number(2, p - 2);
			// взаимно простое число
			if (euclid(p - 1, k) == 1)
				break;
		}

		unsigned _int64 a = fast_exponentiatios(g, k);
		unsigned _int64 b = (fast_exponentiatios(y, k) * (num[count] % p)) ;

		result[count] = new unsigned _int64[2];
		result[count][0] = a;
		result[count][1] = b;
	}
	return result;
}


unsigned _int64* decode(unsigned _int64** num, int x, int sizeNum)
{
	unsigned _int64* result = new unsigned _int64[sizeNum];

	for (unsigned int count = 0; count < sizeNum; count++)
	{
		result[count] = (num[count][1] * fast_exponentiatios(num[count][0], p - 1 - x)) % p;
	}

	return result;
}


int main()
{
	setlocale(LC_ALL, "");
	srand(time(NULL));
	int x = get_random_number(2, p - 1);
	string text = "";
	ifstream fin("input.txt");

	if (!fin.is_open())
	{
		cout << "Нет файла для чтения" << endl;
		return -1;
	}

	fin >> text;
	fin.close();

	if (text == "")
	{
		cout << "Нет сообщения!" << endl;
		return -1;
	}

	int* num = convet_text_to_numbers(text);

	unsigned _int64** encodeNum = encode(num, x, text.length());
	unsigned _int64* decodeNum = decode(encodeNum, x, text.length());
	string decodeText = convet_numbers_to_text(decodeNum, text.length());

	ofstream fout("output.txt", ios_base::trunc);

	if (decodeText == "")
	{
		cout << "Ошибка" << endl;
		fout.close();
		return -1;
	}
	for (unsigned int count = 0; count < text.length(); count++)
	{
		fout << encodeNum[count][0];
		fout << " ";
		fout << encodeNum[count][1];
		fout << "\n";
	}

	fout << decodeText;
	fout.close();

	cout << "Успешно завершено." << endl;

	return 0;
}