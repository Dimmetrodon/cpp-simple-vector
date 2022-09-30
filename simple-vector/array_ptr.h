#pragma once
#include <cassert>
#include <cstdlib>

template <typename Type>
class ArrayPtr
{
public:
    // �������������� ArrayPtr ������� ����������
    ArrayPtr() = default;

    // ������ � ���� ������ �� size ��������� ���� Type.
    // ���� size == 0, ���� raw_ptr_ ������ ���� ����� nullptr
    explicit ArrayPtr(size_t size)
    {
        if (size == 0)
        {
            raw_ptr_ = nullptr;
        }
        else
        {
            Type* numbers = new Type[size];
            raw_ptr_ = numbers;
        }
    }

    // ����������� �� ������ ���������, ��������� ����� ������� � ���� ���� nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept
    {
        raw_ptr_ = raw_ptr;
    }

    // ��������� �����������
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr()
    {
        delete[] raw_ptr_;
    }

    // ��������� ������������
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    // ���������� ��������� �������� � ������, ���������� �������� ������ �������
    // ����� ������ ������ ��������� �� ������ ������ ����������
    [[nodiscard]] Type* Release() noexcept
    {
        Type* copy = raw_ptr_;
        raw_ptr_ = nullptr;
        return copy;
    }

    // ���������� ������ �� ������� ������� � �������� index
    Type& operator[](size_t index) noexcept
    {
        return raw_ptr_[index];
    }

    // ���������� ����������� ������ �� ������� ������� � �������� index
    const Type& operator[](size_t index) const noexcept
    {
        return raw_ptr_[index];
    }

    // ���������� true, ���� ��������� ���������, � false � ��������� ������
    explicit operator bool() const
    {
        return (raw_ptr_ != nullptr);
    }

    // ���������� �������� ������ ���������, ��������� ����� ������ �������
    Type* Get() const noexcept
    {
        return raw_ptr_;
    }

    // ������������ ��������� ��������� �� ������ � �������� other
    void swap(ArrayPtr& other) noexcept
    {
        Type* copy = raw_ptr_;
        raw_ptr_ = other.raw_ptr_;
        other.raw_ptr_ = copy;
    }

private:
    Type* raw_ptr_ = nullptr;
};
