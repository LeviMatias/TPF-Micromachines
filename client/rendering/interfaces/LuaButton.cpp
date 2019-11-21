#include "LuaButton.h"
void LuaButton::render(float screenWidth, float screenHeight) {
  Button::render(screenWidth, screenHeight);
  this->msg.render(screenWidth, screenHeight);
}
LuaButton::LuaButton(SDL_Renderer *sdl_renderer, LTexture *texture)
	: Button("ia", sdl_renderer, texture) {
}
void LuaButton::setPosition(int x, int y) {
  Button::setPosition(x, y);
  SDL_Color white{255, 255, 255, 0xFF};
  this->msg.init("CLICK FOR USING IA", this->mPosition.x + 100,
				 this->mPosition.y + 180, 27, white, gRenderer);
}
void LuaButton::changeOpacity(int i) {
  this->texture->setAlpha(i);
}
