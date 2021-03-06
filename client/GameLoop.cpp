#include "GameLoop.h"

void GameLoop::_runProgram() {
  this->_checkVideoRecording();
  if (!this->recording || this->current_frame % 5 != 0) {
	SDL_SetRenderTarget(starter.get_global_renderer(), nullptr);
  } else {
	videoRecorder.setTarget(starter.get_global_renderer());
  }
  SDL_RenderClear(starter.get_global_renderer());
  if (state == GAME_STATE) {
	runGame(frame_dif);
  } else if (!in_menu.load()) {
	runLobby(frame_dif);
  } else if (in_menu.load()) {
	runMenu(frame_dif);
  }
  SDL_RenderPresent(starter.get_global_renderer());
  if (this->recording && this->current_frame % 5 == 0) videoRecorder.record(starter.get_global_renderer());
  soundSystem.play(state == GAME_STATE || isIaPlayer);
}

void GameLoop::_checkVideoRecording() {
  if (!videoQueue->empty()) {
	videoQueue->pop();
	if (!videoRecorder.get_rec_value()) {
	  videoRecorder.init(starter.get_global_renderer());
	  videoRecorder.set_rec_value(true);
	  this->recording = true;
	  return;
	}
	this->recording = !this->recording;
  }
}

void GameLoop::_run() {
  Stopwatch c;
  float timestep_goal = 1.0 / FPS;
  float timesleep = timestep_goal;

  while (this->isAlive()) {
	_runProgram();
	this->sleep(timesleep);
	float t_elapsed = c.diff();
	timesleep = std::max(0.0f, timestep_goal - std::fmod(t_elapsed, timestep_goal));
	c.reset();
	frame_dif = std::ceil(t_elapsed / timestep_goal);
	this->current_frame = (current_frame + frame_dif) % FPS;
  }
  close();
}

void GameLoop::runMenu(int frame_id) {
  menu.process_events_mouse();
  menu.process_events_keyboard();
  starter.get_screen_dimensions(&screenWidth, &screenHeight);
  menu.render_first_menu(screenWidth, screenHeight);
}

void GameLoop::runGame(int frame_id) {
  InfoBlock *gameState;
  if (renderQueue->empty()) {
	//if no states to load use last
	gameState = &previous_game_state;
  } else {
	while (renderQueue->size() > 1) {
	  renderQueue->pop();
	  //get the very last event
	}
	if (renderQueue->front().exists(RACE_RESULTS)) {
	  gameState = &previous_game_state;
	  gameRenderer.init_leaderboard(renderQueue->front());
	} else gameState = &renderQueue->front();
  }

  if (!gameState->exists(GAME_END)) {
	int height;
	int width;
	if (isIaPlayer) fakePlayerQueue->push(*gameState);
	starter.get_screen_dimensions(&width, &height);
	gameRenderer.render(*gameState, frame_id, width, height);
	previous_game_state = *gameState;
  } else {
	menu.display_notification("Please wait Loading");
	state = -1;
	in_menu.store(true);
	client_ping->notify_all();
	gameRenderer.clean_map();
	SDL_StartTextInput();
  }
}

void GameLoop::runLobby(int frame_id) {
  if (!renderQueue->empty()) {
	InfoBlock *gameState = &renderQueue->front();
	if (gameState->exists(RACE_ID)) {
	  auto ri = gameState->getString(RACE_ID);
	  previous_game_state = *gameState;
	  state = GAME_STATE;
	  gameRenderer.init(starter.get_global_renderer(), *gameState);
	  fakePlayerQueue->push(*gameState);
	}
	renderQueue->pop();
  }
  menu.process_events_mouse();
  menu.process_events_keyboard();
  starter.get_screen_dimensions(&screenWidth, &screenHeight);
  if (leader) menu.render_as_leader(screenWidth, screenHeight);
  else menu.render_as_follower(screenWidth, screenHeight);

}

void GameLoop::proceedToLobby(bool is_leader, bool isIa) {
  isIaPlayer = isIa;
  SDL_StopTextInput();
  leader = is_leader;
  menu.start_lobby_buttons();
  if (is_leader) {
	menu.display_notification("Choose a map for the game");
  } else {
	menu.display_notification("Waiting for the host  to  start  the game");
  }
  std::cout << "im in lobby now" << std::endl;
  in_menu.store(false);
}

GameLoop::~GameLoop() {
  starter.close();
}

GameLoop::GameLoop(std::queue<InfoBlock> &rq,
				   std::queue<SDL_Event> &queue,
				   std::queue<SDL_Event> &mouseQueue,
				   std::condition_variable &r,
				   ThreadQueue *sq,
				   std::queue<InfoBlock> &fpq,
				   std::queue<std::string> &vq)
	: starter(SCREEN_WIDTH, SCREEN_HEIGHT),
	  soundSystem(sq), recording(false), gameRenderer(sq) {
  current_frame = 0;
  frame_dif = 1;
  state = -1;
  in_menu.store(true);
  renderQueue = &rq;
  soundQueue = sq;
  videoQueue = &vq;
  fakePlayerQueue = &fpq;
  starter.init();
  soundSystem.init();
  menu.init(starter.get_global_renderer(), &mouseQueue, &queue, &r, sq);
  menu.set_main_menu_mode();
  client_ping = &r;
}
