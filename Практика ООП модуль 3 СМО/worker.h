#pragma once

#include "transact.h"
#include "queue.h"

#include <random>

class Worker{
protected:
    unsigned id;
    unsigned type;                                                          // тип рабочего 1, 2
    bool isBusy;                                                            // 0 - рабочий свободен, 1 - рабочий выполняет заявку
    
    unsigned averageTimeToOperate;                                          // среднее время обработки заявок
    
    Transact* transactInWork;                                               // указатель на заявку, которая обрабатывается в данный момент
    
    Queue* queuePtr;                                                        // указатель на очередь из соответствующего типа заявок
    
    //задаем генератор 
    random_device rd;
    mt19937 seed;                                                           // для seedа генератора
    exponential_distribution<double> expGenerator;                          // задали генератор с экпоненциальным законом распределения
    
public:
    Worker(unsigned id_, unsigned type_, unsigned averageTimeToOperate_, Queue* queuePtr_);   
    
    virtual double setTimeToOperate();                                            // определяем время, которое оператор затратит на обработку заявки
    
    void finishTheTransact();                                               // заканчиваем обработку заявки
    
    virtual bool sendTransactToWorker(double globalTime);
    
    unsigned getTransactInWorkId();
    unsigned getTransactInWorkType();

    //геттеры
    unsigned getType();
    bool getIsBusy();
    unsigned getId();
    Transact* getTransactInWork();
    
    //сеттеры    
    void setIsBusy(bool value);
    void setTransactInWork(Transact* transact);

};

