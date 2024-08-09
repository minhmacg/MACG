#ifndef M_GAME_H
#define M_GAME_H
#include <type_traits>
#include <map>
#include <algorithm>

template <typename... Ts>
struct contains : std::false_type {};

template <typename T, typename Head, typename... Tail>
struct contains<T, Head, Tail...> : std::conditional<
                                        std::is_same_v<T, Head>,
                                        std::true_type,
                                        contains<T, Tail...>>::type {};
namespace mg
{
	template<typename T>
	using umap_str = std::unordered_map<std::string, std::shared_ptr<T>>;

	template<typename T>
	using map_str = std::map<std::string, std::shared_ptr<T>>;

	template<template<typename> typename ContainerT, typename T>
	struct resource
	{
		inline static std::size_t _max_id;
		ContainerT<T> _data;
	};
	
	template<template<typename> typename ContainerT, typename... RsrcTs>
	struct resource_manager_
	{
		template<typename T>
		using container_t = ContainerT<T>;
		template<typename T>
		using resource_t = resource<container_t,T>;

		std::tuple<resource_t<RsrcTs>...> storage;

		template<typename RsrcT>
		container_t<RsrcT>& get() {return std::get<resource_t<RsrcT>>(storage)._data;};

		template<typename RsrcT>
		std::optional<std::shared_ptr<RsrcT>> get(std::string&& name)
		{
			auto& to_get = get<RsrcT>();
			if (auto rs = std::find_if(std::begin(to_get), std::end(to_get), 
						[&](auto&& r) {return r.first == name;}); rs == to_get.end())
				return std::nullopt;
			else return rs->second;
		};
	
		template<typename RsrcT>
		std::shared_ptr<RsrcT> add(RsrcT&& arg, std::string&& custom_name = "")
		{
			static_assert(contains<RsrcT, RsrcTs...>().value, "resource type not in resource declaration");
			
			resource_t<RsrcT>& to_add = std::get<resource_t<RsrcT>>(storage); 
			std::shared_ptr<RsrcT> new_resource = std::make_shared<RsrcT>(arg);
			
			if (custom_name.empty()) 
				to_add._data.insert({std::to_string(to_add._max_id), new_resource});
			else
			{
				if (to_add._data.find(custom_name) != to_add._data.end())
				{
					std::cout << custom_name << "currently in use, using id instead";
					to_add._data.insert({std::to_string(to_add._max_id), new_resource});
				}
				else to_add._data.insert({custom_name, new_resource});
			};

			to_add._max_id++;
			
			return new_resource;
		};
		
		template<typename RsrcT>
		std::shared_ptr<RsrcT> add(std::shared_ptr<RsrcT> arg, std::string&& custom_name = "")
		{
			static_assert(contains<RsrcT, RsrcTs...>().value, "resource type not in resource declaration");
			resource_t<RsrcT>& to_add = std::get<resource_t<RsrcT>>(storage); 
			
			if (custom_name.empty()) 
				to_add._data.insert({std::to_string(to_add._max_id), arg});
			else
			{
				if (to_add._data.find(custom_name) != to_add._data.end())
				{
					std::cout << custom_name << "currently in use, using id instead";
					to_add._data.insert({std::to_string(to_add._max_id), arg});
				}
				else to_add._data.insert({custom_name, arg});
			};

			to_add._max_id++;
			return arg;
		};

	};

	template<typename...Ts>
	using resource_manager_m = resource_manager_<map_str, Ts...>; 
	template<typename...Ts>
	using resource_manager_um = resource_manager_<umap_str, Ts...>; 
}
#endif
