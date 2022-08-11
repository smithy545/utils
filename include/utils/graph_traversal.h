/* Created by Philip Smith on 2/1/22.
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

#ifndef UTILS_GRAPH_TRAVERSAL_H
#define UTILS_GRAPH_TRAVERSAL_H

#include <functional>
#include <gsl/gsl>
#include <utils/concepts.h>
#include <utils/graph_poly.h>


using namespace utils::concepts;

namespace utils::graph {
template <typename T, typename Ref = T*> requires ParentNodeConcept<T, Ref>
void traverse_tree_recursive(Ref root, std::function<void(Ref)> op) {
	op(root);
	for (auto child: root->children())
		traverse_tree_recursive<T, Ref>(child, op);
}

template <typename T, typename Ref = T*> requires ParentNodeConcept<T, Ref>
void traverse_tree(Ref root, std::function<void(Ref)> op) {
	op(root);

	std::vector<Ref> children;
	for(auto child: root->children())
		children.push_back(child);

	while(!children.empty()) {
		std::vector<Ref> next_layer{};
		for(auto child: children) {
			op(child);
			for (auto grandchild: child->children())
				next_layer.push_back(grandchild);
		}
		children = next_layer;
	}
}

template <typename T, typename Ref = T*> requires ParentNodeConcept<T, Ref>
void traverse_tree_with_predicate(Ref root, std::function<bool(Ref)> op) {
	if(!op(root))
		return;
	std::vector<Ref> children;
	for(auto child: root->children())
		children.push_back(child);
	while(!children.empty()) {
		std::vector<Ref> next_layer{};
		for(auto child: children) {
			if(op(child)) {
				for (auto grandchild: child->children())
					next_layer.push_back(grandchild);
			}
		}
		children = next_layer;
	}
}

template <typename T, typename Ptr = T*>
requires ForwardLinkedNodeConcept<T, Ptr> && PointerConvertible<Ptr>
void forward_traverse_list(gsl::not_null<Ptr> head, std::function<void(Ptr)> op) {
	Ptr ptr = head;
	do {
		op(ptr);
		ptr = ptr->next();
	} while(ptr != nullptr);
}

template <typename T, typename Ptr = T*>
requires BackwardLinkedNodeConcept<T, Ptr> && PointerConvertible<Ptr>
void backward_traverse_list(gsl::not_null<Ptr> tail, std::function<void(Ptr)> op) {
	Ptr ptr = tail;
	do {
		op(ptr);
		ptr = ptr->previous();
	} while(ptr != nullptr);
}

} // namespace utils::graph

#endif //UTILS_GRAPH_TRAVERSAL_H
