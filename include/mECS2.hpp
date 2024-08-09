#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <functional>
#include <tuple>
#include <memory>

template<typename T>
using vec_sptr = std::vector<std::shared_ptr<T>>;

template<typename... Ts>
using tup_sptr = std::tuple<std::shared_ptr<Ts>...>;

template<typename... Ts>
using tup_vec_sptr = std::tuple<vec_sptr<Ts>...>;

template<typename... Ts>
using vec_tup_sptr = std::vector<tup_sptr<Ts...>>;

namespace ecs
{
	template<typename... CompTs>
	struct entity
	{
		std::size_t id;
		bool alive;
		tup_sptr<CompTs...> comp;
	};

	template<typename... CompTs, typename CompT>
	std::shared_ptr<CompT> c_add(entity<CompTs...>&);

	template<typename CompT, typename E>
	inline std::shared_ptr<CompT>& c_get(E& entity)
	{
		return std::get<std::shared_ptr<CompT>>(entity->comp);
	};
	
	template<typename CompT, typename... CompTs>
	inline vec_sptr<CompT>& c_get(tup_vec_sptr<CompTs...>& comps)
	{
		return std::get<vec_sptr<CompT>>(comps);
	};

	template<typename... CompTs>
	tup_vec_sptr<CompTs...> c_get();
	
	template
	<
		template<typename...> typename EntityT,
		template<typename...> typename CContainerT,
		typename... CompTs
	>
	struct registry
	{
		template<typename...Ts> using entity_t = EntityT<Ts...>;
		template<typename...Ts> using c_container_t = CContainerT<Ts...>;
		
		c_container_t<CompTs...> data;
		std::size_t max_id;

		template<typename... Ts>
		std::shared_ptr<entity_t<Ts...>> e_create(Ts&&... args)
		{
			max_id++;
			auto e = std::make_shared<entity_t<Ts...>>();
			((c_get<Ts>(e) = std::make_shared<Ts>(args)), ...);

			(c_get<Ts>(data).push_back(c_get<Ts>(e)), ...);
			return e;
		};
	};

	template<typename... CompTs>
	using df_registry = ecs::registry<ecs::entity, tup_vec_sptr, CompTs...>;

	template<typename RsT, typename...Ts>
	using system1 = std::function<RsT(Ts&...)>;
	
	template
	<
		template<typename...> typename ArchitectureT,
		typename... CompTs
	>
	struct world
	{
		ArchitectureT<CompTs...>* comp_arch;
	};
};
