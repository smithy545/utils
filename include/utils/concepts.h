//
// Created by Philip Smith on 2/10/22.
//

#ifndef UTILS_CONCEPTS_H
#define UTILS_CONCEPTS_H

#include <type_traits>


namespace std {
	template <class From, class To>
	concept convertible_to = std::is_convertible_v<From, To> &&
	requires {
		static_cast<To>(std::declval<From>());
	};
} // namespace std


namespace utils::concepts {
	template <typename T, typename Ret = T*>
	concept GraphNodeConcept = requires(T node) {
		{ node.neighbors() } -> std::convertible_to<vector<Ret>>;
	};

	template <typename T, typename Ret = T*>
	concept ChildNodeConcept = requires(T node) {
		{ node.parent() } -> std::convertible_to<Ret>;
	};

	template <typename T, typename Ret = T*>
	concept ParentNodeConcept = requires(T node) {
		{ node.children() } -> std::convertible_to<vector<Ret>>;
	};

	template <typename T, typename Ret = T*>
	concept TreeNodeConcept = ChildNodeConcept<T, Ret> && ParentNodeConcept<T, Ret>;

	template <typename T, typename Ret = T*>
	concept ForwardLinkedNodeConcept = requires(T node) {
		{ node.next() } -> std::convertible_to<Ret>;
	};

	template <typename T, typename Ret = T*>
	concept MutableForwardLinkedNodeConcept = ForwardLinkedNodeConcept<T> && requires(T node, Ret inserted) {
		{ node.set_next(inserted) } -> std::convertible_to<void>;
	};

	template <typename T, typename Ret = T*>
	concept BackwardLinkedNodeConcept = requires(T node) {
		{ node.previous() } -> std::convertible_to<Ret>;
	};

	template <typename T, typename Ret = T*>
	concept MutableBackwardLinkedNodeConcept = BackwardLinkedNodeConcept<T> && requires(T node, Ret inserted) {
		{ node.set_previous(inserted) } -> std::convertible_to<void>;
	};

	template <typename T, typename Ret = T*>
	concept DoublyLinkedNodeConcept = ForwardLinkedNodeConcept<T, Ret> && BackwardLinkedNodeConcept<T, Ret>;

	template <typename T, typename Ret = T*>
	concept MutableDoublyLinkedNodeConcept = MutableForwardLinkedNodeConcept<T, Ret> && MutableBackwardLinkedNodeConcept<T, Ret>;

} // namespace utils::concepts

#endif //UTILS_CONCEPTS_H
