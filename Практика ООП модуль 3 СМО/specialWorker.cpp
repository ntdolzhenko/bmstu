#include "specialWorker.h"

SpecialWorker::SpecialWorker(unsigned id_, unsigned type_, unsigned averageTimeToOperate1_, unsigned averageTimeToOperate2_, Queue* queue1, Queue* queue2): \
                            Worker(id_, type_, averageTimeToOperate1_, queue1), queuePtr2(queue2), seed2(rd2()), expGenerator2(1.0/averageTimeToOperate2_){}

double SpecialWorker::setTimeToOperate(unsigned transactType){
    if(transactType == 1){
        return expGenerator(seed);
    }
    else return expGenerator2(seed2);
}

bool SpecialWorker::sendTransactToWorker(double globalTime){

    if(isBusy == true) return false;                            // если рабочий занят или заявка не того типа, то не удалось отдать ему заявку в работу
    else{                                                                                   // если рабочий свободен - отдаем заявку в работу
        Transact* transact;
        
        if(queuePtr->getLenOfQueue() >= queuePtr2->getLenOfQueue()){                        // берем  заявку из первой очереди
        
            transact = queuePtr->getTransact();                                             //следующая для обработки заявка - первая в очереди
            queuePtr->deleteTransactFromQueue();                                            // удалили заявку из очереди
        }
        else{                                                                               // берем заявку из второй очереди
            transact = queuePtr2->getTransact();                                            //следующая для обработки заявка - первая в очереди
            queuePtr2->deleteTransactFromQueue();                                           // удалили заявку из очереди        
        }
        
        this->setTransactInWork(transact);                                                  // отдали заявку рабочему
        this->setIsBusy(true);                                                              // изменили статус рабочего, теперь рабочий занят
        
        transact->setStatus(1);                                                             // изменили статус заявки, теперь заявка в обработке
        transact->setWorkerType(type);                                                      // установили тип рабочего, принявшего заявку 
        transact->setWorkerId(id);                                                          // установили id рабочего, принявшего заявку 

        
        transact->setStartTime(globalTime);                                                 // назначили время начала обработки заявки
                                                                                            // это время в модели, когда заявка выходит из очереди + отдается  рабочему
        
        transact->setEndTime( transact->getStartTime() + this->setTimeToOperate(transact->getType()) );         // названчили время окончания обработки заявки, 
                                                                                                                // это текущее время  в системе + время обработки  заявки
        
        transact->setNextActionTime(transact->getEndTime());                                // в следующее событие для заявки - окончание ее обработки
        
        return true;                                                                        //успешно отдали заявку рабочему
    }    
}
