#ifndef MICROMACHINES_FAKECLIENT_H
#define MICROMACHINES_FAKECLIENT_H


#include <queue>
#include "../../common/infostream/InfoBlock.h"
#include "lua/LuaWrapper.h"
#include "../../common/MapsLayer.h"
#include "../../common/conc/BaseThread.h"
#include "../../common/SafeQueue.h"

class FakeClient : public BaseThread {
    SafeQueue<InfoBlock>* keyboardQueue;
    std::queue<InfoBlock>* posQueue;
    LuaWrapper luaWrapper;
    MapsLayer mapsLayer; // TODO: Check if we can pass an instance of this class
    void _firstMove();
    void _run() override;
    bool _move(InfoBlock& ib, int x, int y, int r, int& lastMove);
public:
    FakeClient(SafeQueue<InfoBlock> &kq, std::queue<InfoBlock> &pos);
};


#endif //MICROMACHINES_FAKECLIENT_H
