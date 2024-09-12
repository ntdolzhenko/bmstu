#pragma once

#include "transact.h"
#include <queue>

class Queue{
    
    unsigned id;
    unsigned typeOfQueue; //тип заявок в очереди: 1 или 2
    
    queue<Transact*> transactQueue; // очередь заявок, которые нужно обслужить рабочему

    unsigned lenOfQueue;

public:
    Queue(unsigned id_, unsigned typeOfQueue_);
    
    bool addTransactToQueue(Transact* transact);
    
    bool deleteTransactFromQueue();
    
    Transact* getTransact();

    // геттеры
    unsigned getLenOfQueue();
    unsigned getTypeOfQueue();
    
    //сеттеры
    void setLenOfQueue(unsigned value);

};