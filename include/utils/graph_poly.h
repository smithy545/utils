/* Created by Philip Smith on 1/31/22.
MIT License

Copyright (c) 2021 Philip Arturo Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef UTILS_GRAPH_POLY_H
#define UTILS_GRAPH_POLY_H

#include <cstddef>
#include <concepts>
#include <entt/entt.hpp>
#include <type_traits>
#include "concepts.h"
#include <vector>


using std::vector;

// define entt node concepts for use in graph traversal algorithms. Similar concepts can be found in utils/concepts.h
namespace utils::graph {
template <typename Node>
struct GraphNode : entt::type_list<vector<Node>()> {
	template <typename Base>
	struct type: Base {
		vector<Node> neighbors() {
			entt::poly_call<0>(*this);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&Type::neighbors>;
};

template <typename Node>
struct ChildNode : entt::type_list<Node()> {
	template <typename Base>
	struct type: Base {
		Node parent() {
			entt::poly_call<0>(*this);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&Type::parent>;
};

template <typename Node>
struct ParentNode : entt::type_list<vector<Node>()> {
	template <typename Base>
	struct type: Base {
		vector<Node> children() {
			entt::poly_call<0>(*this);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&Type::children>;
};

template <typename Node>
struct TreeNode : entt::type_list_cat_t<
		decltype(as_type_list(std::declval<ChildNode<Node>>())),
		decltype(as_type_list(std::declval<ParentNode<Node>>()))> {
	template<typename Base>
	struct type:
			ParentNode<Node>::template type<Base>,
			ChildNode<Node>::template type<Base> {};

	template <typename Type>
	using impl = entt::value_list_cat_t<
			typename ChildNode<Node>::template impl<Type>,
			typename ParentNode<Node>::template impl<Type>>;
};

template <typename Node>
struct ForwardLinkedNode : entt::type_list<Node()> {
	template <typename Base>
	struct type: Base {
		Node next() {
			entt::poly_call<0>(*this);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&Type::next>;
};

template <typename Node>
struct MutableForwardLinkedNode : entt::type_list_cat_t<void(Node),
        decltype(as_type_list(std::declval<ForwardLinkedNode<Node>>()))> {
	template<typename Base>
	struct type: ForwardLinkedNode<Node>::template type<Base> {
		static constexpr auto base = std::tuple_size_v<typename entt::poly_vtable<ForwardLinkedNode<Node>,0,0>::type>;
		void set_next(Node node) {
			entt::poly_call<base + 0>(*this, node);
		}
	};

	template <typename Type>
	using impl = entt::value_list_cat_t<
			typename ForwardLinkedNode<Node>::template impl<Type>,
			entt::value_list<&Type::set_next>>;
};

template <typename Node>
struct BackwardLinkedNode : entt::type_list<Node()> {
	template <typename Base>
	struct type: Base {
		Node previous() {
			entt::poly_call<0>(*this);
		}
	};

	template <typename Type>
	using impl = entt::value_list<&Type::previous>;
};

template <typename Node>
struct MutableBackwardLinkedNode : entt::type_list_cat_t<void(Node),
		decltype(as_type_list(std::declval<BackwardLinkedNode<Node>>()))> {
	template<typename Base>
	struct type: BackwardLinkedNode<Node>::template type<Base> {
		static constexpr auto base = std::tuple_size_v<typename entt::poly_vtable<BackwardLinkedNode<Node>,0,0>::type>;
		void set_previous(Node node) {
			entt::poly_call<base + 0>(*this, node);
		}
	};

	template <typename Type>
	using impl = entt::value_list_cat_t<
			typename BackwardLinkedNode<Node>::template impl<Type>,
			entt::value_list<&Type::set_previous>>;
};

template <typename Node>
struct DoublyLinkedNode : entt::type_list_cat_t<
		decltype(as_type_list(std::declval<BackwardLinkedNode<Node>>())),
		decltype(as_type_list(std::declval<ForwardLinkedNode<Node>>()))> {
	template<typename Base>
	struct type:
			BackwardLinkedNode<Node>::template type<Base>,
			ForwardLinkedNode<Node>::template type<Base> {};

	template <typename Type>
	using impl = entt::value_list_cat_t<
			typename BackwardLinkedNode<Node>::template impl<Type>,
			typename ForwardLinkedNode<Node>::template impl<Type>>;
};

template <typename Node>
struct MutableDoublyLinkedNode : entt::type_list_cat_t<
		decltype(as_type_list(std::declval<MutableBackwardLinkedNode<Node>>())),
		decltype(as_type_list(std::declval<MutableForwardLinkedNode<Node>>()))> {
	template<typename Base>
	struct type:
			MutableBackwardLinkedNode<Node>::template type<Base>,
			MutableForwardLinkedNode<Node>::template type<Base> {};

	template <typename Type>
	using impl = entt::value_list_cat_t<
			typename MutableBackwardLinkedNode<Node>::template impl<Type>,
			typename MutableForwardLinkedNode<Node>::template impl<Type>>;
};
} // namespace utils::graph

#endif //UTILS_GRAPH_POLY_H
