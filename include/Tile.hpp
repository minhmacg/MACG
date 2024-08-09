#ifndef M_MAP_H
#define M_MAP_H

#include <iostream>
#include <SDL.hpp>
#include <geometry.hpp>
#include <MG.hpp>
#include <json.hpp>
#include <fstream>
#include <optional>
#include <Utilities.hpp>
#include <memory>
#include <variant>

constexpr unsigned TILED_FLIPFLAG_H = 0x80000000;
constexpr unsigned TILED_FLIPFLAG_V = 0x40000000;
constexpr unsigned TILED_FLIPFLAG_D = 0x20000000; // no equivalent in sdl

constexpr unsigned TILED_FLIPFLAG_MASK = TILED_FLIPFLAG_H | TILED_FLIPFLAG_V | TILED_FLIPFLAG_D;

inline SDL_RendererFlip get_sdlflipflag(const unsigned& id)
{
	unsigned flipflag = SDL_FLIP_NONE;

	if (id & TILED_FLIPFLAG_H) flipflag |= SDL_FLIP_HORIZONTAL;
	if (id & TILED_FLIPFLAG_V) flipflag |= SDL_FLIP_VERTICAL;
	
	return static_cast<SDL_RendererFlip>(flipflag);
};
template<typename T>
struct tiling
{
	inline T& _this() { return static_cast<T&>(*this);};
	inline SDL_Rect rect_at(const int& id)
	{
		auto div = std::div(id,_this()._width);
		if (div.quot > _this()._height) std::cout << "invalid id, exceed height of tile resource";
		return SDL_Rect
		{
			.x = div.rem * _this()._tilesize,
			.y = div.quot * _this()._tilesize,
			.w = _this()._tilesize,
			.h = _this()._tilesize,
		};
	};
	inline SDL_Rect rect_at(const point2d& point)
	{
		if (point.x > _this()._width || point.y > _this()._height) 
			std::cout << "invalid id, exceeded dimension of tile resource";
		return SDL_Rect
		{
			.x = point.x * _this()._tilesize,
			.y = point.y * _this()._tilesize,
			.w = _this()._tilesize,
			.h = _this()._tilesize,
		};
	};
};

namespace tiled
{
	using tile = int;
	struct object
	{
		int _tileid;
		rectangle _rect;
	};
	struct image
	{
		std::filesystem::path _img;
		point2d _pos;
	};

	using ltile = std::vector<tile>;
	using lobject = std::vector<object>;
	using limage = image;
	
	using layer_t = std::variant<ltile, lobject, limage>;
	using group_t = std::vector<std::shared_ptr<layer_t>>;
};


// TILESET
template<typename TexT>
struct tileset : tiling<tileset<TexT>>
{
	using src_t = TexT;

	int _tilesize, _width, _height;
	std::shared_ptr<src_t> _src;
};

template<typename TexT>
std::optional<tileset<TexT>> ts_from_path(const std::filesystem::path& ts_path, int tilesize, SDL_Renderer* renderer)
{
	if (!std::filesystem::exists(ts_path))
	{
		std::cout << "tileset path not existed\n";
		return std::nullopt;
	};

	std::shared_ptr<SDL_Texture> rs_tex = {SDL_CreateTextureFromSurface(renderer, 
										IMG_Load(ts_path.c_str())), 
										SDL_DestroyTexture};
	
	tileset<TexT> rs {._tilesize = tilesize, ._src = rs_tex};
	
	SDL_QueryTexture(rs_tex.get(), NULL, NULL, &rs._width, &rs._height);

	rs._width /= rs._tilesize;
	rs._height /= rs._tilesize;

	return rs;
};

// TILEMAP
template<typename TexT>
struct tilemap : tiling<tilemap<TexT>>
{
	using layers_data_t = mg::resource_manager_m<tiled::ltile,
		  									tiled::lobject, 
											tiled::limage>;
	int _tilesize, _width, _height;
	layers_data_t _layers_data;
	tiled::group_t _groups_data;
	std::shared_ptr<tileset<TexT>> _tileset;
	std::string _bgcolor; //hex
};
template<typename T>
concept Tiles = requires(T x){x++;};

template<typename TexT>
void load_layer(tilemap<TexT>& map, SDL_Renderer* renderer)
{
	auto deletetex = [](SDL_Texture* tex)
	{
		SDL_DestroyTexture(tex);
		std::cout << "hjiohihih\n";
	};
	std::cout << "loading layer\n";
	for (auto& layer: map._layers)
	{
		SDL_Texture* tex = SDL_CreateTexture(
				renderer,SDL_PIXELFORMAT_RGBA4444, SDL_TEXTUREACCESS_TARGET, 
				map._width * map._tilesize, map._height * map._tilesize);

		auto [r,g,b] = hexToRGB(map._bgcolor); 
		
		SDL_SetRenderTarget(renderer, tex);
		SDL_SetRenderDrawColor(renderer, r, g, b, 255);
		SDL_RenderClear(renderer);
		
		for (int i = 0; i != layer._data.size(); i++)
		{
			std::cout << "ID: " << i << '\n';
			int id = layer._data[i] - 1;
			
			if (id > 0)
			{
				int id_filtered = id & ~ TILED_FLIPFLAG_MASK;
				
				std::cout << id << ", " << id_filtered << std::endl;
				
				SDL_Rect src = map._tileset->rect_at(id_filtered);
				std::cout << "SRC: " << src.x << ", " << src.y << '\n';
				
				SDL_Rect dst = map.rect_at(i);
				std::cout << "DST: "<< dst.x << ", " << dst.y << '\n';
				
				SDL_RendererFlip flipflag = get_sdlflipflag(id);

				SDL_RenderCopyEx(renderer, map._tileset->_src.get(), 
						&src, &dst,
						0, NULL,
						flipflag);
			};

			std::cout << "===================================\n";
		
		};

		std::cout << "done loading\n";

		layer._tex = {tex, deletetex};
	};
};

template<typename TilemapT>
std::shared_ptr<tiled::layer_t> tm_parse_layer(const nlohmann::json& layerj, TilemapT& map)
{
	using namespace tiled;
};
template<typename TexT>
std::optional<tilemap<TexT>> tm_from_path(const std::filesystem::path& map_path)
{
	using namespace tiled;
	if (!std::filesystem::exists(map_path))
	{
		std::cout << "map path not existed\n";
		return std::nullopt;
	};

	std::ifstream f;
	nlohmann::json js;
	f.open(map_path);
	f >> js;
	f.close();
	
	tilemap<TexT> rs_map;
	
	for (auto& layer: js.at("layers"))
	{
		tm_parse_layers(layer, rs_map);
	};

	return tilemap<TexT>
	{
		._tilesize = js.at("tilewidth"),
		._width = js.at("width"),
		._height = js.at("height"),
		._bgcolor = js.at("backgroundcolor"),
	};
};

#endif
