#include <iostream>
#include <fstream>
#include <windows.h>
#include <functional>

const int n = 35; //макс кол-во слов
const int m = 15; // макс кол-во букв

bool is_token(char c);
void skip_token(std::ifstream& file, char& c);
int read_word(std::ifstream& file, char& c, char* word);
void print_word(char* word, int len, bool draft = false);
void simple_exchange(char** matrix, int length[], int row);
void task(std::ifstream& input_file);
char** memory_allocation(int Rows, int Cols);
void free_memory(char** matr, int Rows2);
int check_file(std::ifstream& file);
void write_word(std::ofstream& file, char* word, int len);
int create_matrix(std::ifstream& file, char** matrix, int* len);

int main()
{
    SetConsoleOutputCP(1251);
    srand(GetTickCount64());
    std::ifstream inpfile("input.txt");
    if (check_file(inpfile) == -1)
        std::cout << "file not exist\n";
    else if (check_file(inpfile) == 0)
        std::cout << "file empty\n";
    else
    {
       task(inpfile);
    }
    std::cin.get();
}

bool is_token(char c)
{
    return (c == '.' || c == ',' || c == ':' || c == ';' || c == ' ' || c == '-' || c == '\n' || c == EOF);
}

void skip_token(std::ifstream& file, char& c)
{
    while (!file.eof() && is_token(c))
        c = file.get();
}

int read_word(std::ifstream& file, char& c, char* word)
{
    int len = 0;
    while (!file.eof() && !is_token(c))
    {
        word[len++] = c;
        c = file.get();
    }
    return len;
}

void print_word(char* word, int len, bool draft)
{
    if (draft)
        std::cout << '=';
    for (int i = 0; i < len; ++i)
        std::cout << word[i];
    std::cout << (draft ? "=" : "") << '\n';
}

template<typename T>
void my_swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

void swap_words(char* word_1, int length_1, char* word_2, int length_2)
{
    int max_length = length_1 > length_2 ? length_1 : length_2;
    for (int i = 0; i < max_length; ++i)
    {
        my_swap(word_1[i], word_2[i]);
    }
}

bool contains_on(char* word, int len)
{
    bool result = false;
    int i = 0;
    while (i < len-1 && !result)
    {
        if (word[i] == 'o' && word[i + 1] == 'n' || word[i] == 'n' && word[i + 1] == 'o')
            result = true;
        ++i;
    }
    return result;
}


int compare(char word_1[], int length_1, char word_2[], int length_2)
{
    auto sign = [](int x) {return x == 0 ? 0 : x / abs(x); };
    int min_length = length_1 < length_2 ? length_1 : length_2;
    int result{}, i{};
    while (i < min_length && word_1[i] == word_2[i])
        ++i;
    if (i < min_length)
        result = sign(word_1[i] - word_2[i]);
    else
        result = sign(length_1 - length_2);
    return result;
}

void simple_exchange(char** matrix, int length[], int row)
{
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < row - i - 1; ++j)
        {
            if (compare(matrix[j], length[j], matrix[j + 1], length[j + 1]) > 0)
            {
                my_swap(length[j], length[j + 1]);
                swap_words(matrix[j], length[j], matrix[j + 1], length[j + 1]);
            }
        }
}

int create_matrix(std::ifstream& file, char** matrix, int* len)
{
    /*
        Создаем матрицу с словами, максимальной длины.
        Пока файл не кончится мы пропускаем все токены, если символ не является токеном, то мы записываем в row позицию
        слово, если же длина этого слова больше максимальной длины, значит нужно перезаписать это слово в нулевую позицию
        обновить длину нулевой позиции, количество строчек в матрице, максимальную длину.
        Если длина слова равна длине максимальной, то мы просто идем дальше (++row)
        Возвращает количество строк матрицы.. количество слов.
    */

    int row{};
    char c = file.get();
    while (!file.eof())
    {
        skip_token(file, c);
        if (!is_token(c))
        {
            len[row] = read_word(file,c ,matrix[row]);

            if (contains_on(matrix[row], len[row]))
            {
                ++row;
            }


        }
    }
    return row;
}

void task(std::ifstream& input_file)
{
    char** matrix = memory_allocation(n, m);
    int* len = new int[n];
    std::ofstream output_file("output.txt");
    int row = create_matrix(input_file, matrix, len);
   
    simple_exchange(matrix, len, row);
    for (int i = 0; i < row; ++i)
        write_word(output_file, matrix[i], len[i]);
    std::cout << "File out.txt has created\n";
    delete[] len;
    len = nullptr;
    free_memory(matrix, row);
    
}

char** memory_allocation(int Rows, int Cols)
{
    char** matr = new char* [Rows];
    for (int i = 0; i < Rows; ++i)
        matr[i] = new char[Cols];
    return matr;
}

void free_memory(char** matr, int Rows2)
{
    for (int i = 0; i < Rows2; ++i)
        delete[]matr[i];
    delete[]matr;
}

int check_file(std::ifstream& file)
{
    int result = 1;
    if (!file)
        result = -1; //not exist
    else
        if (file.peek() == EOF)
            result = 0; // empty
    return result;
}

void write_word(std::ofstream& file, char* word, int len)
{
    for (int i = 0; i < len; ++i)
    {
        file << word[i];
    }
    file << '\n';
}
