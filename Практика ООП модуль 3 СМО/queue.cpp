#include "queue.h"

// конструктор
Queue::Queue(unsigned id_, unsigned typeOfQueue_): id(id), typeOfQueue(typeOfQueue_), lenOfQueue(0){}

// добавляем  транзакт в очередь
bool Queue::addTransactToQueue(Transact* transact){
    
    if(typeOfQueue != transact->getType()) return false;
    
    transactQueue.push(transact);                       //добавили заявку в очередь
    lenOfQueue++;                                       //увеличили длину очереди
    transact->setStatus(2);                             //изменили статус заявки
    return true;
}

bool Queue::deleteTransactFromQueue(){  
    if(lenOfQueue == 0) return false;                   // если очередь пустая, то не получилось удалить
    else{
        
        // если мы удаляем заявку, значит она уходит из очереди в обработку, берем первую заявку в очереди
        Transact* transact = transactQueue.front();
        transactQueue.pop();                            // убрали транзакт из начала очереди
        lenOfQueue--;                                   // уменьшили длину очереди
        return true;                                    // успешно удалили заявку из очереди
    }
}

Transact* Queue::getTransact(){return transactQueue.front();}

// геттеры
unsigned Queue::getLenOfQueue(){return lenOfQueue;}
unsigned Queue::getTypeOfQueue(){return typeOfQueue;}

//сеттеры
void Queue::setLenOfQueue(unsigned value){lenOfQueue = value;}

