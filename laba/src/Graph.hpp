//
// Created by kmv026 on 19.12.2021.
//

#pragma once

#include "Random.hpp"
#include "ListSequence.hpp"
#include "ArraySequence.hpp"
#include "Set.hpp"
#include "Dictionary.hpp"

template<typename TValue, typename TWeight = TValue>
class Graph {
private:
    class Edge;

    class Node {
    public:
        TValue value{};
        TWeight weight{};
        LinkedList<Edge *> edges{};

        Node() = default;

        Node(TValue value, TWeight weight) : value(value), weight(weight), edges{} {

        }

        Node(const Node &) = default;

        Edge *AddAdjacent(Node *node) {
            Edge *edge = new Edge(TWeight(), this, node);
            edges.Add(edge);
            return edge;
        }

        void AddAdjacent(Edge *edge) {
            edges.Add(edge);
        }

        bool IsAdjacent(Node *other) {
            for (auto el: edges)
                if (el->GetAdjacent(other) == this)
                    return true;

            return false;
        }

        ~Node() = default;
    };

    class Edge {
    public:
        TWeight weight;
        Node *from;
        Node *to;

        Edge(TWeight weight, Node *from, Node *to) : weight(weight), from(from), to(to) {}

        Edge(const Edge &) = default;

        Node *GetAdjacent(Node *node) {
            if (node == from)
                return to;
            if (node == to)
                return from;
            return nullptr;
        }
    };

    ArraySequence<Node *> nodes{};
    bool directed{};
    bool nodeWeighted{};
    bool edgeWeighted{};

public:

//    Iter<T> begin() const override {}

    Graph() = default;


    Graph(int count, int num) : Graph(size_t(count), size_t(num)) {}

    explicit Graph(size_t count, size_t num = 0, bool directed = true, bool nodeWeighted = false,
                   bool edgeWeighted = false, bool withCycles = true) :
            nodes(count), directed(directed), nodeWeighted(nodeWeighted), edgeWeighted(edgeWeighted) {
        for (size_t i = 0; i < nodes.Count(); ++i) {
            nodes[i] = new Node();
            if constexpr(std::is_same<TValue, int>::value) {
                nodes[i]->value = i;
            }
        }
        size_t maxEdges = directed ? count * count : count * (count - 1) / 2;
        if (maxEdges < num)
            throw invalid_argument("Too many nodes to generate");
        for (size_t k = 0; k < num; ++k) {
            auto i = Random<size_t>(0, count - 1), j = Random<size_t>(0, count - 1);
//            if(i!=j)
//                cout << endl;
            if (!nodes[i]->IsAdjacent(nodes[j]) &&
                (((!directed || !withCycles) && i != j) || (directed && edgeWeighted))) {
                TWeight rnd = Random<TWeight>();
                Edge *edge = nodes[i]->AddAdjacent(nodes[j]);
                edge->weight = rnd;
                if (!directed)
                    nodes[j]->AddAdjacent(edge);
            } else {
                --k;
            }
        }
    }

    ListSequence<size_t> TopologicalSort() {
        enum colors {
            white = 2,
            gray = 5,
            black = 9
        };
        for (auto node: nodes)
            node->weight = white;
        ListSequence<Node *> res;
        Set<Node *> passed;
        Stack<Node *> next;
        Dictionary<Node *, Node *> parents;
        cout << this->ToString(true, ListSequence<Node *>(), "greys9", 9) << endl;
        while (res.Count() != nodes.Count()) {

            if (next.IsEmpty()) {
                for (auto node: nodes)
                    if (node->weight == white) {
                        next.Add(node);
                        break;
                    }
            }
            Node *current = next.Top();
            if (!passed.Contains(current)) {
                current->weight = gray;
                cout << this->ToString(true, ListSequence<Node *>(), "greys9", 9) << endl;
                passed.Add(current);
                for (Edge *edge: current->edges) {
                    Node *other = edge->GetAdjacent(current);
                    if (other->weight == gray) {
                        ListSequence<Node *> path;
                        path.AddFirst(other);
                        do {
                            path.AddFirst(current);
                            current = parents[current];
                        } while (current != other);
                        path.AddFirst(current);
//                        cout << PathToIndexes(path) << endl;
                        cout << this->ToString(true, path, "greys9", 9) << endl;
                        throw std::logic_error("There is loop in this graph, it cant be sorted!");
                    }
                    if (other->weight == white) {
                        next.Push(other);
                        parents.Add(other, current);
                    }
                }
            }

            if (current == next.Top()) {
                next.Top()->weight = black;
                res.AddFirst(next.Pop());
                cout << this->ToString(true, ListSequence<Node *>(), "greys9", 9) << endl;
            }
        }
        return PathToIndexes(res);
    }

    Graph &Colorize() {
        Set<Node *> passed;
        Stack<Node *> next;
        for (auto node: nodes)
            node->weight = 1;

        while (passed.Count() != nodes.Count()) {
            if (next.IsEmpty()) {
                for (auto node: nodes)
                    if (!passed.Contains(node)) {
                        next.Add(node);
                        break;
                    }
            }
            Node *current = next.Pop();
            passed.Add(current);
            for (Edge *edge: current->edges) {
                Node *other = edge->GetAdjacent(current);
                if (current == other)
                    throw std::logic_error("There is loop in this graph, it cant be colorized!");
                if (other->weight == current->weight)
                    ++other->weight;
                if (!passed.Contains(other))
                    next.Push(other);
            }
        }
        return *this;
    }

    ListSequence<size_t> Dijkstra(size_t a, size_t b) {
        Set<Node *> passed;
        Dictionary<Node *, Node *> parents;
        nodeWeighted = true;
        for (auto node: nodes) {
            node->weight = std::numeric_limits<TWeight>::max();
            parents.Add(node, nullptr);
        }
        Stack<Node *> next;
        next.Push(nodes[a]);
        nodes[a]->weight = 0;
        while (!next.IsEmpty()) {
            Node *current = next.Top();
            for (Edge *edge: current->edges) {
                Node *adj = edge->GetAdjacent(current);
                TWeight newWeight = edge->weight + current->weight;
                if (newWeight < adj->weight) {
                    adj->weight = newWeight;
                    next.Add(adj);
                    parents[adj] = current;
                }
            }
            passed.Add(next.Pop());
        }
        Node *current = nodes[b];
        Node *start = nodes[a];
        ListSequence<Node *> tmp;
        if (parents[current] != nullptr) {
            do {
                tmp.AddFirst(current);
                current = parents[current];
            } while (current != start);
            tmp.AddFirst(start);
        } else if (a == b)
            tmp.AddFirst(start);
        return PathToIndexes(tmp);
    }

    string
    ToString(bool colored = false, const ListSequence<size_t> &indexes = {}, const string &colorscheme = "spectral11",
             TWeight maxColor = 11) {
        ListSequence<Node *> path;
        for (auto index: indexes)
            path.Add(nodes[index]);
        return ToString(colored, path, colorscheme, maxColor);
    }

private:
    ListSequence<size_t> PathToIndexes(ListSequence<Node *> tmp) {
        size_t done = 0;
        ListSequence<size_t> res(tmp.Count());
        for (size_t i = 0; i < nodes.Count(); ++i) {
            for (size_t j = 0; j < tmp.Count(); ++j) {
                if (tmp[j] == nodes[i]) {
                    res[j] = i;
                    ++done;
                    break;
                }
            }
            if (done == tmp.Count())
                break;
        }
        return res;
    }

    string
    ToString(bool colored, const ListSequence<Node *> &path, const string &colorscheme, TWeight maxColor) {
        stringstream ss;
        Set<Edge *> passed;
        Set<Edge *> pathEdges;
        string defaultColor = "green";

        for (size_t i = 1; i < path.Count(); ++i) {
            for (Edge *edge: path[i - 1]->edges) {
                if (edge->GetAdjacent(path[i - 1]) == path[i]) {
                    pathEdges.Add(edge);
                    break;
                }
            }
        }
        ss << (directed ? "digraph" : "graph") << " {" << endl;
        ss << "node [style=filled];" << endl;
        for (auto node: nodes) {
            ss << '"' << node << '"' << "[label=\"";
            Utils::PPrint(ss, node->value);
            ss << '"';
            if (colored) {
                if (node->weight <= maxColor) {
                    ss << ", colorscheme=" << colorscheme << ", color=" << node->weight;
                    if (colorscheme == "greys9" && node->weight > 5) {
                        ss << ", fontcolor=white";
                    }
                } else
                    ss << ", color=" << defaultColor;
            } else if (nodeWeighted) {
                ss << ", xlabel=\"";
                Utils::PPrint(ss, node->weight);
                ss << '"';
            }
            if (path.Contains(node)) {
                ss << ", penwidth=3";
            }
            ss << "];" << endl;
            for (auto edge: node->edges) {
                if (!passed.Contains(edge)) {
                    ss << '"' << node << '"' << (directed ? "->" : "--") << '"';
                    Utils::PPrint(ss, edge->GetAdjacent(node));
                    ss << '"';
                    if (edgeWeighted) {
                        ss << "[label=\" ";
                        Utils::PPrint(ss, edge->weight);
                        ss << "\"]";
                    }
                    if (pathEdges.Contains(edge)) {
                        ss << "[penwidth=3]";
                    }
                    ss << ';' << endl;
                    passed.Add(edge);
                }
            }
        }
        ss << '}' << endl;
//        ss << endl << passed.Count() << endl << passed << endl;
        return ss.str();
    }

public:
    ~Graph() {
        for (auto node: nodes) {
            for (auto edge: node->edges) {
                LinkedList<Edge *> *edges = &edge->GetAdjacent(node)->edges;
                if (edges->Contains(edge) && edge->GetAdjacent(node) != node)
                    edges->Remove(edge);
                delete edge;
            }
        }
        for (auto node: nodes) delete node;
    }
};
