

#ifndef MICROMACHINES_GAMELOOP_H
#define MICROMACHINES_GAMELOOP_H

#include "../common/conc/BaseThread.h"
#include "../common/SafeQueue.h"
#include "../common/Stopwatch.h"
#include "../common/infostream/InfoBlock.h"
#include "rendering/interfaces/Menu.h"
#include "rendering/GameRenderer.h"
#include "sound_sys/SoundSystem.h"
#include "rendering/recording/VideoRecorder.h"
#include <SDL2/SDL.h>
#include <queue>
#include <atomic>
#include <common/ThreadQueue.h>

#define LOBBY_STATE 1
#define GAME_STATE 2

class GameLoop : public BaseThread {
  SDLStarter starter;
  SoundSystem soundSystem;
  int current_frame;
  int frame_dif; //difference in frames compared to previous iteration
  int state;
  InfoBlock previous_game_state;
  std::condition_variable *client_ping;
  VideoRecorder videoRecorder;
  GameMap map;
  GameRenderer gameRenderer;
  int screenWidth;
  int screenHeight;
  bool ready;
  bool leader;

  void _runProgram();

  void _run() override;

  void runMenu(int frame_id);

  void runGame(int frame_id);

  void runLobby(int frame_id);

  void _checkVideoRecording();

 public:
  Menu menu;
  std::atomic<bool> in_menu;
  std::atomic<bool> recording;
  std::queue<InfoBlock> *renderQueue;
  std::queue<std::string> *videoQueue;
  ThreadQueue *soundQueue;
  std::queue<InfoBlock> *fakePlayerQueue;
  bool isIaPlayer;

  /*Builder of the class gameLoop*/
  GameLoop(std::queue<InfoBlock> &rq,
		   std::queue<SDL_Event> &textQueue,
		   std::queue<SDL_Event> &mouseQueue,
		   std::condition_variable &ready,
		   ThreadQueue *sq,
		   std::queue<InfoBlock> &fpq,
		   std::queue<std::string> &vq);

  //PRE the game must be in menu mode
  //is_AI marks whether the player is using a bot or not
  //POS proceeds to the next interface screen
  //if is_leader is true, the next screen will
  //prompt the player to select a map
  void proceedToLobby(bool is_leader, bool is_AI);

  ~GameLoop();

};

#endif //MICROMACHINES_GAMELOOP_H
