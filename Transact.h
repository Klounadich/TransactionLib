#ifndef TRANSACT_H
#define TRANSACT_H

#include <algorithm>
#include <exception>
#include <experimental/filesystem>
template<typename T>
class Transaction {
private:
T* orig;
T copy;
bool activ;
public:
Transaction(T &obj) : orig(&obj) , copy(obj) , activ(true){}; //base constructor 

Transaction (const Transaction&) = delete; // copy lock on  initialization 
Transaction& operator = (const Transaction&) = delete; // copy lock on created object

Transaction (Transaction&& other) noexcept : orig(other.orig) , copy(std::move(other.copy) ) , activ(other.activ) {
    other.orig = nullptr; // clear memory pointer
    other.activ = false;
}

~Transaction(){ // destructor
    if(activ && orig) {
        rollback();
    }
}

T& get() {
   
    return copy;
}

void commit() {
    
    activ = false;
}

void rollback()  noexcept{
    if(activ) {
        activ = false;
        *orig=copy;
    }
    activ = false;
}

bool is_active() {
    return activ;
}

};

template<typename T>
Transaction<T> begin_transaction(T& obj) {
    return Transaction<T>(obj);
}

#endif