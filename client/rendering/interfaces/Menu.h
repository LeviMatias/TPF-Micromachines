#ifndef MICROMACHINES_CLIENT_RENDERING_INTERFACES_MENU_H_
#define MICROMACHINES_CLIENT_RENDERING_INTERFACES_MENU_H_

#include <SDL_system.h>
#include <queue>
#include "../LTexture.h"
#include "../../../config/constants.h"
#include "Button.h"
#include "TextLabel.h"
#include "TextBox.h"
#include <condition_variable>
#include "../TextureLoader.h"

class Menu {
 private:
  LTexture wallpaper;
  TextureLoader textureLoader;
  std::queue<SDL_Event> *mouse_queue;
  std::queue<SDL_Event> *text_queue;
  std::queue<std::string> *sound_queue;
  std::shared_ptr<Button> connectButton;
  SDL_Renderer *gRenderer;

  std::vector<Button> carButtons;
  std::vector<Button> mapButtons;
  std::vector<Button> *active_buttons;
  std::condition_variable *game_ready_cv;

  TextLabel label_choose_car;
  TextLabel flavor_text;
  TextLabel notification;
  TextBox textbox_lobby_name;
  bool mapIsSelected = false;

  void load_media();
  void set_buttons_positions();

 public:
  std::string car_selected;
  std::string map_selected;
  bool ready;

  void init(SDL_Renderer *sdl_renderer, std::queue<SDL_Event> *gQueue, std::queue<SDL_Event> *textQueue,
            std::condition_variable *attempConnectionCV, std::queue<std::string> *sq);

  void processEventsKeyboard();

  //PRE must be initialized
  void render_first_menu(float screenWidth, float screenHeight);

  void dummy_init_as_leader(int screenWidth, int screenHeight);

  void set_buttons_as_leader();

  void init_as_follower();

  bool processEventsMouse();

  void setMainMenuMode();

  void displayNotification(std::string msg);
  void start_lobby();
  bool map_is_selected();
};

#endif //MICROMACHINES_CLIENT_RENDERING_INTERFACES_MENU_H_
