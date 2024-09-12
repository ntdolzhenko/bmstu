#pragma once

#include <iostream>

using namespace std;

class Transact{

private: 
    unsigned id;
    unsigned type; 
    unsigned status;            // статус обработки заявки: 0 - поступила в систему, в ожидании; 1 - в обработке; 2 - в очереди на обработку
    
    unsigned workerType;
    unsigned workerId;

    double enterTime;           // время поступления заявки в систему
    
    double startTime;           // время начала обработки заявки
    double endTime;             // время ухода заявки из системы после выполнения 
    
    double nextActionTime;      // время следующего действия над заявкой

public:

    // конструктор
    Transact(unsigned id_, unsigned type_, double modelTime);
    
    // геттеры
    unsigned getId();
    unsigned getType();
    unsigned getStatus();
    
    unsigned getWorkerType();
    unsigned getWorkerId();
    
    double getStartTime();
    double getEndTime();
    double getNextActionTime();

    //сеттеры
    void setStatus(unsigned value);
    void setWorkerType(unsigned value); 
    void setWorkerId(unsigned value); 
    void setStartTime(double value);
    void setEndTime(double value);
    void setNextActionTime(double value);

};
