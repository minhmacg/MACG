#ifndef M_BOX_H
#define M_BOX_H

#include <SDL.hpp>

#include <iostream>
#include <functional>
#include <vector>
#include <optional>
#include <memory>

template
<
	int WW = 1280,
	int WH = 720,
	int LWW = 320, /* logical width */
	int LWH = 240, /* logical height */
	int FR = 30 /* frame per seconds */
>
struct SDL_Application
{
	SDL_Application()
	{
		SDL_Init(SDL_INIT_EVERYTHING);
		IMG_Init(IMG_INIT_PNG);

		TTF_Init();
		SDL_CreateWindowAndRenderer(WW,WH,SDL_WINDOW_SHOWN,&window,&renderer);
		if (!window || !renderer) std::cout << "could not create window or renderer \n"
			<< SDL_GetError() << std::endl;
		SDL_RenderSetLogicalSize(renderer, LWW, LWH);
	};
	
	
	bool run()
	{
		frame.prev = frame.curr = SDL_GetTicks();
		while (running)
		{
			frame.curr = SDL_GetTicks();
			
			if (float dt = frame.elapsed(); dt >= 1000.0/FR)
			{
				if (!update(dt, *this)) 
				{
					std::cerr << "update function returned false\n";
					return false;
				};
				draw(*this);
				frame.prev = SDL_GetTicks();
			};
		};
		return true;
	};

	inline void setf_update(std::function<bool(const float&, SDL_Application&)>&& updatef)
	{
		update = updatef;
	};

	inline void setf_draw(std::function<void(SDL_Application&)>&& drawf)
	{
		draw = drawf;
	};
	
	~SDL_Application()
	{
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	};
	struct frame_tracker 
	{
		unsigned prev, curr;
		inline float elapsed() {return curr-prev;}
	};
	frame_tracker frame;
	std::function<bool(const float&, SDL_Application&)> update;
	std::function<void(SDL_Application&)> draw;
	bool running = true;
	SDL_Window* window;
	SDL_Renderer* renderer;
	const int width = WW,
			height = WH,
			lwidth = LWW,
			lheight = LWH;
};

using sdl_app = SDL_Application<1280, 720, 320, 240, 30>;
inline bool SDLApp_closing(SDL_Event* e)
{
	if (e->type == SDL_QUIT)
			return true;
	return false;
};
#endif
