#pragma once

#include <vector>
#include <functional>
#include <stdio.h>
#include <memory>

using std::vector;
using std::size_t;

namespace graph {
    template <class T> class Node;

    template <class T>
    class Node {
    public:
        T value;
        vector<size_t> points_to;

        Node(T value, vector<size_t> points) : value(value), points_to(points) {};
    };

    template <class T>
    class Graph {
    private:
        vector<Node<T>> nodes;

        int walk(std::function<int(size_t, const vector<Node<T>>&)> with_node, vector<bool>& seen, size_t node) {
            if (seen[node]) return 0;
            auto ret = with_node(node, nodes);
            if (ret != 0) return ret;
            seen[node] = true;
            for (const auto& n : nodes[node].points_to) {
                ret = walk(with_node, seen, n);
                if (ret != 0) return ret;
            }

            return 0;
        }

    public:
        size_t root;

        Graph(vector<Node<T>> nodes, size_t root = 0) : nodes(nodes), root(root) {};
        void walk(std::function<int(size_t, const vector<Node<T>>&)> with_node) {
            vector<bool> seen(nodes.size(), false);
            walk(with_node, seen, root);
        }
        void walk(std::function<int(size_t, const vector<Node<T>>&)> with_node, size_t node) {
            vector<bool> seen(nodes.size(), false);
            walk(with_node, seen, node);
        }
    };
}
