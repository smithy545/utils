/* Created by Philip Smith on 2/10/22.
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

#ifndef UTILS_CONCEPTS_H
#define UTILS_CONCEPTS_H

#include <vector>
#include <type_traits>


namespace utils::concepts {
	template <typename T, typename Ret = T*>
	concept GraphNodeConcept = requires(T node) {
		{ node.neighbors() } -> std::convertible_to<std::vector<Ret>>;
	};

	template <typename T, typename Ret = T*>
	concept ChildNodeConcept = requires(T node) {
		{ node.parent() } -> std::convertible_to<Ret>;
	};

	template <typename T, typename Ret = T*>
	concept ParentNodeConcept = requires(T node) {
		{ node.children() } -> std::convertible_to<std::vector<Ret>>;
	};

	template <typename T, typename Ret = T*>
	concept BinaryParentNodeConcept = requires(T node) {
		{ node.left() } -> std::convertible_to<Ret>;
		{ node.right() } -> std::convertible_to<Ret>;
	};

	template <typename T, typename Ret = T*>
	concept TreeNodeConcept = ChildNodeConcept<T, Ret> && ParentNodeConcept<T, Ret>;

	template <typename T, typename Ret = T*>
	concept BinaryTreeNodeConcept = ChildNodeConcept<T, Ret> && BinaryParentNodeConcept<T, Ret>;

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

	template <typename T>
	concept PointerConcept = std::is_pointer_v<T>;

	template <typename T>
	concept SmartPointerConcept = requires (T val) {
		{ val.get() } -> PointerConcept;
	};

	template <typename T>
	concept PointerConvertible = PointerConcept<T> || SmartPointerConcept<T>;

	template <typename T>
	concept HasPtr = SmartPointerConcept<typename T::Ptr>;
} // namespace utils::concepts

#endif //UTILS_CONCEPTS_H
