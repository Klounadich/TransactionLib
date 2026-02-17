#  <p align="center">TransactionLib *demo*</p>
Небольшая библиотека для языка C++ , добавляющая возможность использовать Транзакции 
## Библиотека позволяет:
1. Безопасно изменять объекты с возможностью отката (commit/rollback)
2. Гарантировать целостность данных при возникновении исключений
3. Работать с любыми копируемыми типами данных
4. Писать более чистый и безопасный код

## <p align="center">Установка</p>
данная библиотека использует принцип "header-only" так что достаточно лишь установить заголовочный файл [Transact.h](https://github.com/Klounadich/TransactionLib/blob/main/Transact.h) в директорию вашего проекта 
и подключить в коде следующим образом :
```cpp
#include "Transact.h"
```
## <p align="center">Функции библиотеки </p>
1. [begin_transaction(obj)](https://github.com/Klounadich/TransactionLib/blob/e280635ff534ac355ce16d7ddb9d21adfbd77abf/Transact.h#L58) - Создаёт новую транзакцию для объекта.
```cpp
auto t = begin_transaction(data);
```
2. [get()](https://github.com/Klounadich/TransactionLib/blob/e280635ff534ac355ce16d7ddb9d21adfbd77abf/Transact.h#L31) - Является точкой входа . Возвращает ссылку на рабочую копию объекта. Все изменения должны выполняться через этот метод.

```cpp

auto t = begin_transaction(data);
t.get().push_back(42);           
```
3. [commit()](https://github.com/Klounadich/TransactionLib/blob/e280635ff534ac355ce16d7ddb9d21adfbd77abf/Transact.h#L36) - Подтверждает изменения
  ]
```cpp

auto t = begin_transaction(data);
t.get().push_back(42);
t.commit();           
```
4.[rollback()](https://github.com/Klounadich/TransactionLib/blob/e280635ff534ac355ce16d7ddb9d21adfbd77abf/Transact.h#L43)  - Отменяет изменения
 ]
```cpp

auto t = begin_transaction(data);
t.get().push_back(42);
t.rollback();           
```
## <p align="center">Пример использования</p>
Базовый пример :
```cpp

#include "Transact.h"
#include <iostream>
#include <string>

int main() {
    std::string data = "Hello";
    
    auto t = begin_transaction(data);
    t.get() = "World";
    t.commit();
    
    std::cout << data << std::endl;
    
    return 0;
}
```

Использование совместно с try/catch/throw
```cpp
cout<<"Друг занял денег"<<endl;
    auto t =begin_transaction(friends);
    string name;
    try{

    
    cin >>name;
     vector<string>::iterator iter1 = find(t.get().begin() , t.get().end() , name);
     if(iter1 == t.get().end()) {
        throw runtime_error("Друг '" + name + "' не найден в списке!");
    }
    
    t.get().erase(iter1);
    t.commit();
    }
    catch(exception& ex) {
        cout<<"ошибка: " << ex.what() <<endl;
        t.rollback();
    }
```
Использование с перемещением
```cpp
void process(Transaction<string>&& t) {
    t.get() = "new";
    t.commit();
}

int main() {
    string data = "Original";
    
   
    auto t1 = begin_transaction(data);
    t1.get() = "123";
    
   
    process(std::move(t1));
    
    
    
    
    
    cout << "data: " << data << endl;  // выведет new
    
    return 0;
}
```
## <p align="center">Структура проекта</p>
 класс Transaction
 ```cpp
template<typename T>
class Transaction {
private:
    T* orig;              // Указатель на оригинал
    T backup;             // Снимок "до"
    T working;            // Рабочая копия
    bool activ;           // Флаг активности
};
```
запрещаем копирование  при создании и присваивании 
```cpp
Transaction (const Transaction&) = delete; 
Transaction& operator = (const Transaction&) = delete;
```
Конструктор перемещения (реализован с использованием семантики перемещения 
```cpp
Transaction (Transaction&& other) noexcept : orig(other.orig) , snapshot(std::move(other.snapshot) ) , activ(other.activ) {
    other.orig = nullptr; 
    other.activ = false;
} 
```
Деструктор (автоматически выполняет rollback)
```cpp
~Transaction(){ /
    if(activ && orig) {
        rollback();
    }
}
```

Релизации функций 
```cpp
T& get() {  
   
    return copy;
}

void commit() { 
     if(activ && orig) {
        *orig = copy; 
    }
    activ = false;
}

void rollback()  noexcept{
    if(activ) {
        activ = false;
        *orig=snapshot;
    }
    activ = false;
}

bool is_active() { 
    return activ;
}
```

