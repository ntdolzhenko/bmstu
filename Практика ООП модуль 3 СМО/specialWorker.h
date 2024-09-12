#pragma once

#include "worker.h"

#include <random>

class SpecialWorker: public Worker{
    
    Queue* queuePtr2;                                                           // указатель на вторую очередь заявок
    
    random_device rd2;
    mt19937 seed2;
    exponential_distribution<double> expGenerator2;
    
public:
    SpecialWorker(unsigned id_, unsigned type_, unsigned averageTimeToOperate1_, unsigned averageTimeToOperate2_, Queue* queue1, Queue* queue2);
    virtual double setTimeToOperate(unsigned transactType);
    virtual bool sendTransactToWorker(double globalTime);
};