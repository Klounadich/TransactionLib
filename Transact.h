#ifndef TRANSACT_H
#define TRANSACT_H

#include <algorithm>

template<typename T>
class Transaction {
private:
T* orig;
T snapshot;
T copy;
bool activ;
public:
Transaction(T &obj) : orig(&obj) , snapshot(obj) , activ(true){}; //base constructor 

Transaction (const Transaction&) = delete; // copy lock on  initialization 
Transaction& operator = (const Transaction&) = delete; // copy lock on created object

Transaction (Transaction&& other) noexcept : orig(other.orig) , snapshot(std::move(other.snapshot) ) , activ(other.activ) {
    other.orig = nullptr; // clear memory pointer
    other.activ = false;
} //movement constructor 


~Transaction(){ // destructor
    if(activ && orig) {
        rollback();
    }
}

T& get() { //entry 
   
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

bool is_active() { //check status
    return activ;
}

};

template<typename T>
Transaction<T> begin_transaction(T& obj) { // fabric func
    return Transaction<T>(obj);
}

#endif