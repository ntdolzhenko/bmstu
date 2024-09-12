#include "worker.h"
#include "transact.h"
#include "queue.h"

//seed(rd()) - задали генератор рандомного seedа для использования экпоненнциального генератора 
// expGenerator - определили рандомный экспоненциальный генератор
// использование - expGenerator(seed)
Worker::Worker(unsigned id_, unsigned type_, unsigned averageTimeToOperate_, Queue* queuePtr_): \
                            id(id_), type(type_), seed(rd()), isBusy(false), queuePtr(queuePtr_), transactInWork(nullptr), expGenerator(1.0/averageTimeToOperate_){} 
    
double Worker::setTimeToOperate(){ return int(expGenerator(seed) * 10000)/10000.0;}       // определяем время, которое оператор затратит на обработку заявки 
                                                                                            //(точность - 4 знака после запятой)
        
void Worker::finishTheTransact(){                                                           // заканчиваем обработку заявки
    
    isBusy = false;
    transactInWork = nullptr;       
}                                               
    
bool Worker::sendTransactToWorker(double globalTime){

    if(isBusy == true) return false;                    // если рабочий занят или заявка не того типа, то не удалось отдать ему заявку в работу
    else{                                                                                   // если рабочий свободен - отдаем заявку в работу
        
        Transact* transact = queuePtr->getTransact();                                       //следующая для обработки заявка - первая в очереди
        if(type != transact->getType()) return false;
        
        queuePtr->deleteTransactFromQueue();                                      // удалили заявку из очереди
        
        this->setTransactInWork(transact);                                                  // отдали заявку рабочему
        this->setIsBusy(true);                                                              // изменили статус рабочего, теперь рабочий занят
        
        transact->setStatus(1);                                                             // изменили статус заявки, теперь заявка в обработке
        transact->setWorkerType(type);                                                      // установили тип рабочего, принявшего заявку 
        transact->setWorkerId(id);                                                          // установили id рабочего, принявшего заявку 
        
        transact->setStartTime(globalTime);                                                 // назначили время начала обработки заявки
                                                                                            // это время в модели, когда заявка выходит из очереди + отдается  рабочему
        
        transact->setEndTime( transact->getStartTime() + this->setTimeToOperate() );        // названчили время окончания обработки заявки, 
                                                                                            // это текущее время  в системе + время обработки  заявки
        
        transact->setNextActionTime(transact->getEndTime());                                // в следующее событие для заявки - окончание ее обработки
        
        return true;                                                                        //успешно отдали заявку рабочему
    }    
}

unsigned Worker::getTransactInWorkId(){return transactInWork->getId();}
unsigned Worker::getTransactInWorkType(){return transactInWork->getType();}


//геттеры
unsigned Worker::getType(){return type;}
bool Worker::getIsBusy(){return isBusy;}
unsigned Worker::getId(){return id;}
Transact* Worker::getTransactInWork(){return transactInWork;}

//сеттеры
void Worker::setIsBusy(bool value){isBusy = value;}
void Worker::setTransactInWork(Transact* transact){transactInWork = transact;}


