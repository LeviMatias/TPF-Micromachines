#ifndef MICROMACHINES_USERINPUT_H
#define MICROMACHINES_USERINPUT_H

#include <SDL2/SDL.h>
#include "../common/conc/BaseThread.h"
#include "../common/SafeQueue.h"
#include "../common/infostream/InfoBlock.h"

/*
 This thread reads user input.
 It adds event as infoblock to the safe queue received as parameter
 */

class UserInput : public BaseThread {
    SafeQueue<InfoBlock>* keyboard_input;
  std::queue<SDL_Event> *local_queue;
    void _run() override;
public:
  UserInput(SafeQueue<InfoBlock> *q_keyboard, std::queue<SDL_Event> *text_queue);
    void _rcvKeyInput(SDL_Event &e);
    void close() override;

};


#endif //MICROMACHINES_USERINPUT_H
