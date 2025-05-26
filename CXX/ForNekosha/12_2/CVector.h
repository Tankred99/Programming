#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

template <class T>
class CVector
{
private:
    std::vector<T> arr;

public:
    void setsize(int);
    void random(T, T);
    int getsize() const;
    ~CVector<T>() = default;
    CVector<T>(int size_vector = 0);
    CVector<T>(T* array, int sizearray);
    CVector<T>(const CVector&);
    CVector<T>(CVector&&) noexcept = default;
    CVector<T>& operator=(const CVector&);
    CVector<T>& operator=(CVector&&) noexcept = default;
    void SortUp();
    void SortDown();
    T* FindElement(T);
    CVector<T> operator+(const CVector&) const;
    CVector<T> operator-(const CVector&) const;
    T& operator[](int);
    const T& operator[](int) const;
    friend std::ostream& operator<<(std::ostream& os, const CVector& Vector)
    {
        os << std::endl;
        for (int i = 0; i < Vector.arr.size(); i++)
            os << Vector.arr[i] << " ";
        os << std::endl;
        return os;
    }
    friend std::istream& operator>>(std::istream& is, CVector& Vector)
    {
        int arrsize;
        do
        {
            std::cout << "Введите размер вашего массива: ";
            is >> arrsize;
        } while (arrsize < 0);
        Vector.setsize(arrsize);
        for (int i = 0; i < Vector.arr.size(); i++)
        {
            std::cout << std::endl << "Введите " << i + 1 << "элемент вашего массива: ";
            is >> Vector.arr[i];
        }
        return is;
    }
};

template <class T>
void CVector<T>::setsize(int arrsize)
{
    if (arrsize < 0)
        throw std::exception("Недопустимый размер массива! ");
    arr.resize(arrsize);
}

template <class T>
void CVector<T>::random(T min, T max)
{
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::uniform_int_distribution<T> distrib(min, max);

    for (int i = 0; i < arr.size(); i++)
        arr[i] = distrib(gen);
}

template <class T>
int CVector<T>::getsize() const
{
    return arr.size();
}

template <class T>
CVector<T>::CVector<T>(int size_vector) : arr(size_vector)
{
    if (size_vector < 0)
        throw std::exception("Недопустимый размер массива! ");
}

template <class T>
CVector<T>::CVector<T>(T* array, int sizearray) : arr(array, array + sizearray)
{
    if (sizearray < 0)
        throw std::exception("Недопустимый размер массива! ");
    if (array == nullptr)
        throw std::exception("Недопустимый массив! ");
}

template <class T>
CVector<T>::CVector<T>(const CVector& Vector) : arr(Vector.arr)
{
}

template <class T>
CVector<T>& CVector<T>::operator=(const CVector& Vector)
{
    arr = Vector.arr;
    return *this;
}

template <class T>
void CVector<T>::SortUp()
{
    std::sort(arr.begin(), arr.end());
}

template <class T>
void CVector<T>::SortDown()
{
    std::sort(arr.begin(), arr.end(), std::greater<T>());
}
//
template <class T>
T* CVector<T>::FindElement(T key)
{
    auto it = std::find(arr.begin(), arr.end(), key);
    if (it != arr.end())
        return &(*it);
    return nullptr;
}

template <class T>
CVector<T> CVector<T>::operator+(const CVector& Vector) const
{
    if (arr.size() != Vector.arr.size())
        throw std::exception("Разный размер массивов! ");
    CVector result(arr.size());
    for (int i = 0; i < arr.size(); i++)
        result.arr[i] = arr[i] + Vector.arr[i];
    return result;
}

template <class T>
CVector<T> CVector<T>::operator-(const CVector& Vector) const
{
    if (arr.size() != Vector.arr.size())
        throw std::exception("Разный размер массивов! ");
    CVector<T> result(arr.size());
    for (int i = 0; i < arr.size(); i++)
        result.arr[i] = arr[i] - Vector.arr[i];
    return result;
}

template <class T>
T& CVector<T>::operator[](int arrindex)
{
    if (arrindex >= 0 && arrindex < arr.size())
        return arr[arrindex];
    else throw std::out_of_range{ "Некорректный индекс" };
}

template <class T>
const T& CVector<T>::operator[](int arrindex) const
{
    if (arrindex >= 0 && arrindex < arr.size())
        return arr[arrindex];
    else throw std::out_of_range{ "Некорректный индекс" };
}