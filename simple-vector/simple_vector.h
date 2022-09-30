#pragma once

#include <cassert>
#include <initializer_list>
#include <array>
#include <algorithm>
#include <stdexcept>
#include "array_ptr.h"

class ReserveProxyObj
{
public:
    ReserveProxyObj(size_t capacity)
        :capacity_(capacity)
    {
    }

    size_t capacity_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve)
{
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector
{
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size)
        : size_(size)
        , capacity_(size)
        , arr_(size)
    {
        std::fill(arr_.Get(), arr_.Get() + size_, 0);
    }

    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value)
        : size_(size)
        , capacity_(size)
        , arr_(size)
    {
        std::fill(arr_.Get(), arr_.Get() + size, value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init)
        :SimpleVector(init.size())
    {
        std::copy(init.begin(), init.end(), arr_.Get());
    }

    SimpleVector(ReserveProxyObj capacity)
        : size_(0)
        , capacity_(capacity.capacity_)
    {
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept
    {
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept
    {
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept
    {
        return (size_ == 0);
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept
    {
        if (index > size_)
        {
            throw std::out_of_range("");
        }
        return arr_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept
    {
        if (index > size_)
        {
            throw std::out_of_range("");
        }
        return arr_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index)
    {
        if (index >= capacity_)
        {
            throw std::out_of_range("");
        }
        return arr_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const
    {
        if (index >= capacity_)
        {
            throw std::out_of_range("");
        }
        return arr_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept
    {
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size)
    {
        if (new_size > size_)
        {
            ArrayPtr<Type> new_arr(new_size);
            std::move(this->begin(), this->end(), new_arr.Get());
            std::fill(new_arr.Get() + size_, new_arr.Get() + new_size, 0);
            std::generate(new_arr.Get() + size_, new_arr.Get() + new_size, []() {return Type();});
            arr_.swap(new_arr);
            capacity_ = new_size;
        }
        else if (new_size == size_)
        {
            std::fill(arr_.Get(), arr_.Get() + size_, 0);
        }
        size_ = new_size;
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept
    {
        return arr_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept
    {
        return (arr_.Get() + size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept
    {
        return arr_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept
    {
        return (arr_.Get() + size_);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept
    {
        return (arr_.Get());
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept
    {
        return (arr_.Get() + size_);
    }

    SimpleVector(const SimpleVector& other) 
        :size_(other.GetSize())
        ,capacity_(other.GetCapacity())
        ,arr_(other.GetCapacity())
    {
        std::copy(other.begin(), other.end(), arr_.Get());
    }

    SimpleVector(SimpleVector&& other)
        :size_(other.GetSize())
        , capacity_(other.GetCapacity())
        , arr_(other.GetCapacity())
    {
        std::move(std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()), arr_.Get());
        other.Clear();
    }

    SimpleVector& operator=(const SimpleVector& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        SimpleVector rhs_copy(rhs);
        swap(rhs_copy);
        return *this;
    }

    SimpleVector& operator=(SimpleVector&& rhs)
    {
        if (this == &rhs)
        {
            return *this;
        }
        SimpleVector rhs_copy(std::move(rhs));
        swap(rhs_copy);
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) 
    {
        Insert(this->end(), item);
    }

    void PushBack(Type&& item)
    {
        Insert(this->end(), std::move(item));
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    /*Iterator Insert(ConstIterator pos, const Type& value) 
    {
        if (pos < begin() || pos > end())
        {
            throw std::out_of_range("");
        }
        Iterator it = const_cast<Iterator>(pos);
        if (size_ != capacity_)
        {
            Iterator result_it = std::copy_backward(it, end(), end() + 1) - 1;
            *result_it = value;
            ++size_;
            return result_it;
        }
        else
        {
            if (capacity_ == 0)
            {
                capacity_ = 1;
            }
            else
            {
                capacity_ = capacity_ * 2;
            }
            SimpleVector new_arr(capacity_);
            new_arr.size_ = size_ + 1;
            Iterator result_it = std::copy(begin(), it, new_arr.begin());
            *result_it = value;
            std::copy_backward(it, end(), new_arr.end());
            swap(new_arr);
            return result_it;
        }
    }*/

    Iterator Insert(ConstIterator pos, Type&& value)
    {
        if (pos < begin() || pos > end())
        {
            throw std::out_of_range("");
        }
        Iterator it = const_cast<Iterator>(pos);
        if (size_ != capacity_)
        {
            Iterator result_it = std::move_backward(it, end(), end() + 1) - 1;
            *result_it = std::move(value);
            ++size_;
            return result_it;
        }
        else
        {
            if (capacity_ == 0)
            {
                capacity_ = 1;
            }
            else
            {
                capacity_ = capacity_ * 2;
            }
            SimpleVector new_arr(capacity_);
            new_arr.size_ = size_ + 1;
            Iterator result_it = std::move(begin(), it, new_arr.begin());
            *result_it = std::move(value);
            std::move_backward(it, end(), new_arr.end());
            swap(new_arr);
            return result_it;
        }
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept 
    {
        arr_[size_] = 0;
        if (size_ == 0)
        {
            throw std::invalid_argument("size = 0");
        }
        --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) 
    {
        if (pos < begin() || pos > end())
        {
            throw std::out_of_range("");
        }
        Iterator it = const_cast<Iterator>(pos);
        std::move(it + 1, end(), it);
        if (size_ == 0)
        {
            throw std::invalid_argument("size = 0");
        }
        --size_;
        return it;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept 
    {
        arr_.swap(other.arr_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    void Reserve(size_t new_capacity)
    {
        if (new_capacity < capacity_)
        {
            return;
        }
        SimpleVector copy(new_capacity);
        size_t size = size_;
        std::copy(this->begin(), this->end(), copy.begin());
        swap(copy);
        size_ = size;
    }

private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    ArrayPtr<Type> arr_;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    if (lhs.GetSize() == rhs.GetSize())
    {
        if (std::equal(lhs.begin(), lhs.end(), rhs.begin()))
        {
            return true;
        }   
    }
    return false;
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return !(lhs > rhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) 
{
    return !(lhs < rhs);
}