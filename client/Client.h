
#ifndef MICROMACHINES_CLIENT_H
#define MICROMACHINES_CLIENT_H

#include <client/ia/FakeClient.h>
#include "communication/EventSender.h"
#include "communication/Receiver.h"
#include "communication/UserInput.h"
#include "rendering/TextureLoader.h"
#include "rendering/GameRenderer.h"
#include "GameMap.h"
#include "GameLoop.h"
#include "../config/constants.h"

class serverNotRunning : public std::exception {
  virtual const char *what() const throw() {
	return "Server is not connected, maybe try running it first!\n";
  }
};

class Client {
  Socket skt;
  SafeQueue<InfoBlock> keyboard_e_queue;
  std::queue<SDL_Event> mouse_queue;
  std::queue<SDL_Event> text_queue;
  std::queue<std::string> sound_queue;
  std::queue<std::string> video_queue;
  std::queue<InfoBlock> receiver_queue;
  std::queue<InfoBlock> fake_player_queue;
  std::condition_variable ready_to_connect;
  GameLoop gameLoop;
  UserInput userInput;
  Receiver receiver;
  EventSender sender;
  FakeClient fc;
  std::string port;
  std::string service;

  /*Attempts to connect to the server. If it was possible, returns true.
   * if not, returns false*/
  bool attempt_connection();

  //true if connected, false if not
  bool connection_check();

  InfoBlock connection_state;

 public:
  Client(std::string &s, std::string &p);

  int play();

  void release();

  bool wait_for_connection();

  void wait_game_end();

  void wait_ready_button();

  bool connect_to_server();

  void play_game();
};

#endif //MICROMACHINES_CLIENT_H
