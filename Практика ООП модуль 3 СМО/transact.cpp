#include "transact.h"
    
// конструктор
Transact:: Transact(unsigned id_, unsigned type_, double enterTime): \
                            id(id_), type(type_), status(0), workerType(0), workerId(0), enterTime(enterTime), startTime(0), endTime(0), nextActionTime(enterTime){}
                                                        
// геттеры
unsigned Transact::getId(){return id;} 
unsigned Transact::getType(){return type;}
unsigned Transact::getStatus(){return status;} 
unsigned Transact::getWorkerType(){return workerType;}
unsigned Transact::getWorkerId(){return workerId;}

double Transact::getStartTime(){return startTime;}
double Transact::getEndTime(){return endTime;}
double Transact::getNextActionTime(){return nextActionTime;}

//сеттеры
void Transact::setStatus(unsigned value){ status = value; }  
void Transact::setWorkerType(unsigned value){ workerType = value; } 
void Transact::setWorkerId(unsigned value){ workerId = value; } 

void Transact::setStartTime(double value){ startTime = value; }  
void Transact::setEndTime(double value){ endTime = value; }  
void Transact::setNextActionTime(double value){ nextActionTime = value;}