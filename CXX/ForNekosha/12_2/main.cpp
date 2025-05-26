#include <iostream>
#include <ctime>
#include "CVector.h"
int main()
{
    setlocale(LC_ALL, "RU");
    try
    {
        int array[] = { 1,2,3,4,5 };
        CVector<int> masarr(array, 5);
        std::cout << masarr;
        std::cout << std::endl << (masarr.FindElement(5)) << std::endl;
        CVector<int> vector(3);
        std::cout << vector;
        vector.setsize(10);
        vector.random(2, 9);
        std::cout << vector << std::endl;
        vector.SortUp();
        std::cout << "Сортировка по возрастанию: " << vector << std::endl;
        vector.SortDown();
        std::cout << "Сортировка по убыванию: " << vector << std::endl;
        CVector<int> vector2(10);
        vector2.random(1, 10);
        std::cout << vector2 << std::endl;
        std::cout << (vector2+vector) << std::endl;
        CVector<int> vector3 = vector2;
        std::cout << "Вектор 2: " << vector2 << std::endl;
        std::cout << "Вектор 3: " << vector3 << std::endl;
        std::cout << "Разность вектора 3 и вектора 2: " << (vector2 - vector3) << std::endl;
        std::cout << "Сумма вектора 2 и вектора 3: " << (vector2 + vector3) << std::endl;
    }
    catch (std::exception& error)
    {
        std::cout << std::endl << "Ошибка! " << error.what();
    }
    catch (std::out_of_range& error)
    {
        std::cout << std::endl << "Ошибка! " << error.what();
    }
}