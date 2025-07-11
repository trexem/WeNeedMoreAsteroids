#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <memory>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "surface.hpp"
#include "FPair.h"
#include "utils.hpp"

struct SDL_Surface_Deleter {
	void operator()(SDL_Surface* surface) {
		SDL_DestroySurface(surface);
	}
};

class Texture {
public:
	Texture(void);
	Texture(SDL_Renderer* g_renderer);
	Texture(SDL_Renderer* renderer, Surface surface);
	~Texture(void);

	bool loadFromFile(std::string t_path);
	bool loadFromText(std::string t_texture_text, SDL_Color t_text_color, TTF_Font* g_font);
	bool loadMultilineText(const std::string& text, SDL_Color color, TTF_Font* font, int maxWidth);
	bool loadFromSurface(SDL_Surface* surface);
	void createEmptyTexture(int w, int h);
	void free(void);
	void render(int t_x, int t_y, float t_scale = 1.0f);
	void renderEx(int t_x, int t_y, SDL_FRect* t_clip, double t_angle, SDL_FPoint* t_center, 
		SDL_FlipMode t_flip, float t_scale = 1.0f, const SDL_Color* color = nullptr);
	void renderEx(int t_x, int t_y, SDL_FRect* t_clip, double t_angle, SDL_FPoint* t_center, 
		SDL_FlipMode t_flip, FPair size, const SDL_Color* color = nullptr);
	void render9Grid(const SDL_FRect* srcrect, float left, float right, float top, float bottom,
		float scale, const SDL_FRect* destrect);
	void setAlphaMod(Uint8 alpha);
	void colorMod(const SDL_Color& color);

	float getWidth();
	float getHeight();
	FPair getSize();
	SDL_Texture* getTexture();

	SDL_Renderer* m_renderer{nullptr};
	RotationPoint rotationPoint {RotationPoint::CenterCenter};
private:
	SDL_Texture* m_texture{nullptr};
	float m_width{0};
	float m_height{0};
};

extern Texture g_ship_texture;
extern Texture g_shot_texture;
extern Texture g_rocket_texture;
extern Texture g_particle_texture;
extern Texture g_particle_shimmer_texture;
extern Texture g_asteroid_big_texture;
