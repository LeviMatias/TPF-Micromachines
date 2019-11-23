#ifndef MICROMACHINES_CLIENT_CAR_H
#define MICROMACHINES_CLIENT_CAR_H

#include <SDL_rect.h>
#include <SDL_events.h>
#include "rendering/LTexture.h"
#include "tiles/Tile.h"
#include "rendering/interfaces/Explosion.h"

class Car {
  SDL_Rect mBox;
  int rotation;
  LTexture *texture;
  Explosion explosion;
  short id;
  int health;

 public:

  /*Constructor of the class*/
  explicit Car(short id);

  /*Adds a texture to the car in order to represent the car in the screen
   * at the moment of rendering*/
  void addTexture(LTexture *my_texture);

  /*Centers the camera over the car using the level values for this.
  * PRE: level_width and level_height must be greather than 0*/
  void setCamera(SDL_Rect &camera, int level_width, int level_height);

  /*Renders the texture stored in the car.
   * PRE: For this function to work properly, it is necessary that a texture
   * has been previously saved using the add_texture method*/
  void render(SDL_Rect &camera, SDL_Renderer *renderer);
    void render(SDL_Rect &camera, SDL_Renderer *renderer, int frames);

    void move(int x, int y, int r);

    int get_health();

    void modify_health(int newLife);

  short get_id();

  void move(int x, int y, int r);

  void loadAnimations(SDL_Renderer *gRenderer);

  ~Car();
};

#endif