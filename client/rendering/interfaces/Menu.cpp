#include <iostream>
#include <utility>
#include "Menu.h"
#include "../../../config/constants.h"
#include "ConnectButton.h"
#include "TextLabel.h"

void Menu::init(SDL_Renderer *sdl_renderer,
				std::queue<SDL_Event> *mouseEventsQueue,
				std::queue<SDL_Event> *textQueue,
				std::condition_variable *attemptConnectionCV,
                ThreadQueue *soundQueue) {
  this->text_queue = textQueue;
  this->mouse_queue = mouseEventsQueue;
  this->sound_queue = soundQueue;
  this->gRenderer = sdl_renderer;
  this->game_ready_cv = attemptConnectionCV;
  map_selected = "race_1";
  car_selected = "RED_CAR";
  ready = false;
  iaOn = false;
  this->iaButton = new LuaButton(gRenderer,
								 textureLoader.load_texture("buttons/tick.png",
															gRenderer));
  iaButton->change_opacity(120);
}

void Menu::display_notification(std::string msg) {
  notification.stage_text_change(std::move(msg));
}

void Menu::set_main_menu_mode() {
  try {
	if (!carButtons.empty()) return;
	load_media();
	set_buttons_positions();
  } catch (...) {
	throw std::runtime_error("Failed to initialize textures!\n");
  }
  SDL_StartTextInput();
  if (!open_games.empty()) return;
  set_elements();

}
void Menu::set_elements() {
  SDL_Color gold{255, 189, 27, 0xFF};
  SDL_Color white{255, 255, 255, 0xFF};
  label_choose_car.init("CLICK A CAR TO SELECT", SCREEN_WIDTH / 2, 200, 28,
						gold, gRenderer);
  label_choose_car.set_intermittent_rate(FPS * 2 / 3);
  textbox_lobby_name.init("START TYPING THE NAME OF YOUR SESSION",
						  SCREEN_WIDTH / 2, 450, 30, gold, gRenderer);
  notification.init(" ", SCREEN_WIDTH / 2, SCREEN_HEIGHT - 150, 35,
					white, gRenderer);
  flavor_text.init("fiuba 2019    all rights reserved",
				   SCREEN_WIDTH / 2, SCREEN_HEIGHT - 30, 25, white, gRenderer);
  active_buttons = &carButtons;

  for (int i = 0; i < 6; i++) {
	open_games.emplace_back();
	open_games.back().init(" ",
						   450 + 400 * (i % 2), 580 + 75 * (int) (i / 2),
						   25, white, gRenderer);
  }
}

void Menu::_updateOpenGames() {
  if (!open_games_update.empty()) {
	int size = open_games_update.front().get<int>("SIZE");
	int i = 0;
	for (auto &label : open_games) {
	  std::string text;
	  if (i < size) {
		std::string key = "g" + std::to_string(i);
		text = open_games_update.front().getString(key);
	  } else {
		text = "empty";
	  }
	  label.stage_text_change(std::to_string(i + 1) + "          " + text);
	  i++;
	}
	open_games_update.pop();
  }
}

bool Menu::process_events_mouse() {
  while (!mouse_queue->empty()) {
	auto event = &mouse_queue->front();
	if (event->type == SDL_QUIT) return true;
	for (auto &button : *active_buttons) {
	  if (button.handle_event(event, sound_queue)) {
		while (!mouse_queue->empty()) mouse_queue->pop();
		return false;
	  }
	}
	if (iaButton->handle_event(event, sound_queue)) {
	  while (!mouse_queue->empty()) mouse_queue->pop();
	  return false;
	}
	if (connectButton->handle_event(event, sound_queue)) {
	  while (!mouse_queue->empty()) mouse_queue->pop();
	  return false;
	}
	mouse_queue->pop();
  }
  return false;
}

void Menu::process_events_keyboard() {
  while (!text_queue->empty()) {
	textbox_lobby_name.receiveInput(&text_queue->front());
	text_queue->pop();
  }
  textbox_lobby_name.updateBounds();
}

void Menu::render_first_menu(float screenWidth, float screenHeight) {
  wallpaper.render_with_size(0, 0, 0, gRenderer, SCREEN_HEIGHT,
							 SCREEN_WIDTH, true);
  for (auto &button : carButtons) {
	button.render(screenWidth, screenHeight);
  }
  label_choose_car.render(screenWidth, screenHeight);
  textbox_lobby_name.render(screenWidth, screenHeight);
  flavor_text.render(screenWidth, screenHeight);
  notification.render(screenWidth, screenHeight);
  connectButton->render(screenWidth, screenHeight);

  _updateOpenGames();
  for (auto &label : open_games) {
	label.render(screenWidth, screenHeight);
  }
  iaButton->render(screenWidth, screenHeight);
}

void Menu::start_lobby_buttons() {
  for (auto &label : open_games) {
	label.stage_text_change(" ");
  }
}

void Menu::render_as_leader(int screenWidth, int screenHeight) {
  wallpaper.render_with_size(0, 0, 0, gRenderer, SCREEN_HEIGHT, SCREEN_WIDTH, true);
  for (auto &button : mapButtons) {
	button.render(screenWidth, screenHeight);
  }
  connectButton->render(screenWidth, screenHeight);
  notification.render(screenWidth, screenHeight);
}

void Menu::render_as_follower(int screenWidth, int screenHeight) {
  wallpaper.render_with_size(0, 0, 0, gRenderer, SCREEN_HEIGHT, SCREEN_WIDTH, true);
  notification.render(screenWidth, screenHeight);
}

void Menu::set_buttons_positions() {
  carButtons[0].set_position(BLUE_CAR_BUTTON_X, BLUE_CAR_BUTTON_Y);
  carButtons[1].set_position(BLACK_CAR_BUTTON_X, BLACK_CAR_BUTTON_Y);
  carButtons[2].set_position(RED_CAR_BUTTON_X, RED_CAR_BUTTON_Y);
  carButtons[3].set_position(WHITE_CAR_BUTTON_X, WHITE_CAR_BUTTON_Y);
  iaButton->set_position(BLUE_CAR_BUTTON_X + 60, PLAY_BUTTON_Y - 170);
  connectButton->set_position(PLAY_BUTTON_X, PLAY_BUTTON_Y);
}

void Menu::set_buttons_map_screen() {
  create_map_buttons();
  auto callbackMap = [&](const std::string &clickedId) {
	for (auto &button : mapButtons) {
	  std::cout << clickedId << button.id << (button.id == clickedId) << std::endl;
	  if (button.id == clickedId) {
		button.change_color(255, 255, 255, -1);
		map_selected = button.id;
	  } else {
		button.change_color(80, 80, 80, -1);
	  }
	}
  };
  for (auto &button : mapButtons) {
	button.add_callback_function(callbackMap);
	button.change_color(80, 80, 80, -1);
	button.soundWhenPressed = SOUND_CAR_GEAR;
  }

  mapButtons[0].set_position(BLUE_CAR_BUTTON_X + 40, BLUE_CAR_BUTTON_Y);
  mapButtons[1].set_position(BLACK_CAR_BUTTON_X, BLUE_CAR_BUTTON_Y);
  mapButtons[2].set_position(RED_CAR_BUTTON_X, RED_CAR_BUTTON_Y);
  mapButtons[3].set_position(WHITE_CAR_BUTTON_X - 40, BLUE_CAR_BUTTON_Y);
}
void Menu::create_map_buttons() {
  mapButtons.emplace_back("race_1",
						  gRenderer,
						  textureLoader.load_texture("all_images/Decor/dragon.png",
													 gRenderer));
  mapButtons.emplace_back("race_2",
						  gRenderer,
						  textureLoader.load_texture("all_images/Decor/8Circuit.png",
													 gRenderer));
  mapButtons.emplace_back("race_3",
						  gRenderer,
						  textureLoader.load_texture("all_images/Decor/eggTrack.png",
													 gRenderer));
  mapButtons.emplace_back("race_4",
						  gRenderer,
						  textureLoader.load_texture("all_images/Decor/rainbow.png",
													 gRenderer));
}

void Menu::load_media() {
  wallpaper.load_from_file("assets/all_images/Decor/background.png",
						   gRenderer);
  auto callback_start_game = [&](const std::string &clickedId) {
	ready = true;
	connectButton->change_color(80, 80, 80, FPS / 2);
	if (active_buttons == &mapButtons) {
	  active_buttons = &carButtons;
	} else {
	  active_buttons = &mapButtons;
	}
	this->game_ready_cv->notify_all();
  };
  //car buttons
  auto callback = [&](const std::string &clickedId) {
	std::cout << clickedId << std::endl;
	for (auto &button : carButtons) {
	  if (button.id == clickedId) {
		button.change_color(255, 255, 255, -1);
		car_selected = button.id;
	  } else {
		button.change_color(80, 80, 80, -1);
	  }
	}
  };
  auto ai_callback = [&](const std::string &clickedId) {
	iaOn = !iaOn;
	if (iaOn) {
	  iaButton->change_opacity(255);
	} else {
	  iaButton->change_opacity(120);
	}
  };
  create_buttons_first_menu();
  connectButton->add_callback_function(callback_start_game);

  for (auto &button : carButtons) {
	button.add_callback_function(callback);
	button.change_color(80, 80, 80, -1);
	button.soundWhenPressed = SOUND_CAR_GEAR;
  }
  this->set_buttons_map_screen();
  iaButton->add_callback_function(ai_callback);

}
void Menu::create_buttons_first_menu() {
  carButtons.emplace_back("BLUE_CAR", gRenderer,
						  textureLoader.load_texture("cars/blue_car.png",
													 gRenderer));
  carButtons.emplace_back("BLACK_CAR", gRenderer,
						  textureLoader.load_texture("cars/black_car.png",
													 gRenderer));
  carButtons.emplace_back("RED_CAR", gRenderer,
						  textureLoader.load_texture("cars/red_car.png",
													 gRenderer));
  carButtons.emplace_back("WHITE_CAR", gRenderer,
						  textureLoader.load_texture("cars/white_car.png",
													 gRenderer));
  connectButton = std::make_shared<ConnectButton>(gRenderer,
												  textureLoader.load_texture("buttons/connect.png",
																			 gRenderer));
}

Menu::~Menu() {
  free(this->iaButton);
}
bool Menu::get_ia() {
  return iaOn;
}
