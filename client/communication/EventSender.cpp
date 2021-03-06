#include "EventSender.h"

#include "../../common/infostream/Protocol.h"
#include "../../config/constants.h"

EventSender::EventSender(Socket& skt, SafeQueue<InfoBlock>* safeQueue) :
    skt(skt) {
    this->safeQueue = safeQueue;
}

bool _isFinalEvent(InfoBlock& ib){
    if (!ib.exists(ACTION_TYPE)) return false;
    auto actionType = ib.get<char>(ACTION_TYPE);
    return actionType == QUIT;
}

void EventSender::_run() {
    this->safeQueue->setOpen(true);
    while (this->isAlive()){
        InfoBlock ib = this->safeQueue->pop();
        if (_isFinalEvent(ib)){
            break;
        }
        if (!Protocol::sendMsg(this->skt, ib)){
            break;
        }
    }
    this->close();
}

void EventSender::close() {
    this->safeQueue->setOpen(false);
    BaseThread::close();
}

EventSender::~EventSender() {

}
