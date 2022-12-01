# cpp-simple-vector
## Краткое описание
Репозиторий предствляет собой авторскую реализацию SimpleVector контейнера std::vector на основе "умного указателя" ArrayPtr. Проект интересен тем, что позволил немного взглянуть на "кухню" изнутри и усовершенствовать навыки работы с динамической памятью, в том числе с ее "подводными камнями", такими как, например, утечки.
## Инструкция по развертыванию и пользованию
Требуемая версия языка - С++14 и выше. В остальном конкретных требований нет, компилятор подойдет любой: gcc, MinGW, Microsoft Visual C++.  

SimpleVector - шаблонный класс, в конструкторе может принимать:
* Размер `SimpleVector<int>(42)`
* Размер и значение, которым вектор будет заполнен по умолчанию `SimpleVector<std::string>(42, "foo"s)`
* Список инициализации `SimpleVector<double>({1.1, 2.3, 4.2, 42.0})`
* Константную ссылку на другой объект SimpleVector (конструктор копирования) `SimpleVector<int>(other_int_SimpleVector)`
* rvalue-ссылку на другой объект SimpleVector (конструктор перемещения) `SimpleVector<int>(std::move(other_int_SimpleVector))`
* Объект вспомогательного класса ReserveProxyObj, позволяющий зарезервировать нужное количество памяти. `SimpleVector<int>(Reserve(42))`

Для вектора определены итераторы:
```
using Iterator = Type*;
using ConstIterator = const Type*; 
```

Доступные пользователю методы контейнера SimpleVector:
* `void PushBack(Type&&)` и `void PushBack(const Type&)` - вставка элемента в конец вектора.
* `void PopBack() noexcept` - удаление последнего элемента.
* `Iterator Insert(ConstIteartor, Type&&)` и  `Iterator Insert(ConstIteartor, const Type&)` - вставка элемента на произвольную позицию. Принимает итератор на позицию, на которую должна быть произведена вставка и ссылку на вставляемый элемент (rvalue или lvalue). Возвращает итератор на вставленное значение.
* `Iterator Erase(ConstIterator)` - удаляет элемент вектора в указанной позиции. Возвращает итератор на элемент, следующий за удаленным.
* `void swap(SimpleVector&)` - обменивает содержимое с другим объектом SimpleVector.
* `void Reserve(size_t)` - резервирует необходимый объем памяти. Определяет значение параметра capacity.
* `size_t GetSize()` - возвращает размер вектора.
* `size_t GetCapacity()` - возвращает значение параметра capacity вектора.
* `bool IsEmpty()` - возвращает true, если вектор пуст и false в противном случае.
* `const Type& At(size_t index)` - возвращает константную ссылку на элемент с индексом index.
* `void Clear()` - очищает вектор.
* `void Resize(size_t)` - изменяет размер вектора.
* `Iterator begin()` - возвращает итератор на начало массива.
* `Iterator end()` - возвращает итератор на элемент, следующий за последним.
* `ConstIterator begin()` - возвращает константный итератор на начало вектора.
* `ConstIterator end()` - возвращает константный итератор на элемент, следующий за последним.
* `ConstIterator cbegin()` - возвращает константный итератор на начало вектора.
* `ConstIterator cend()` - возвращает константный итератор на элемент, следующий за последним.

Также определены операторы:
* `Type& operator[](size_t index)`
* `const Type& operator[](size_t index)`
* `SimpleVector& operator=(const SimpleVector& other)`
* `SimpleVector& operator=(SimpleVector&& other)`
* `inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)`
* `inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)`
* `inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)`
* `inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)`
* `inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)`
* `inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs)`
