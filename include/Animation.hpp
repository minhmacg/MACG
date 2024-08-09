#ifndef M_ANIMATION_H
#define M_ANIMATION_H

#include<Tile.hpp>

//use tilset as device for spritesheet animation


template<typename T>
struct ANM_device
{
	inline T* _this() {return static_cast<T*>(this);};

	using texture_t = typename T::Texture;

};
template<typename Tex, typename Animation_device = tileset<Tex>>
struct Animation: Animation_device
{
	using src_t = typename Animation_device::src_t;
	int speed, current = 0, start = 0, end, counter = 0;
};

template<typename Tex, typename Animation_device = tileset<Tex>>
inline void play_anim(Animation<Tex>& a)
{
	if (a.counter >= a.speed)
	{
		a.current++;
		a.counter = 0;
	};

	if (a.current == a.end) a.current = a.start;

	a.counter++;
};

template<typename Tex, typename Animation_device = tileset<Tex>>
inline void draw_anim(Animation<Tex>& a, tileset<Tex>& src, SDL_Renderer* renderer)
{
	SDL_Rect src_rect = src.rect_at(a.current);
	SDL_Rect dst_rect = {200,200,src._tilesize, src._tilesize};
	SDL_RenderCopy(renderer, src._src, &src_rect, &dst_rect);
};

#endif
