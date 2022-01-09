//
// Created by kmv026 on 10.10.2021.
//

#include <array>
#include <algorithm>
#include <random>
#include <type_traits>
#include <limits>
#include <chrono>
#include <gtest/gtest.h>
#include "../ListSequence.hpp"
#include "../ArraySequence.hpp"
#include "../Complex.hpp"
#include "../Utils.hpp"
#include "../Graph.hpp"

// https://stackoverflow.com/questions/55892577/how-to-test-the-same-behaviour-for-multiple-templated-classes-with-different-tem

// https://github.com/google/googletest/blob/master/docs/advanced.md#type-parameterized-tests


//TEST(DirectedGraph, Delete_Edge_Vertex)
//{
//    Graph<char, long> graph{};
//
//    graph . DeleteVertex(0);
//    graph . DeleteEdge(1, 2);
//
//    ASSERT_EQ(2, graph.ReturnVertexNumber());
//    ASSERT_EQ(0, graph.ReturnEdgeNumber());
//}

TEST(DirectedGraph, Dijkstra) {
    Graph<int> graph{};


    graph.AddEdge(2, 4, 1);
    graph.AddEdge(2, 3, 5);
    graph.AddEdge(4, 3, 3);
    graph.AddEdge(2, 5, 10);
    graph.AddEdge(3, 5, 10);
    graph.AddEdge(5, 0, 1);
    auto path = graph.Dijkstra(0, 5);
    ASSERT_TRUE(path == ListSequence<size_t>());
}


TEST(DirectedGraph, TopologicalSort) {
    Graph<int> graph{};
    graph.AddEdge(5, 0);
    graph.AddEdge(4, 0);
    graph.AddEdge(3, 1);
    graph.AddEdge(2, 3);
    graph.AddEdge(5, 2);
    graph.AddEdge(4, 1);
    cout << graph.ToString() << endl;
    auto res = graph.TopologicalSort();
    ASSERT_TRUE(res == ListSequence<size_t>({5, 4, 2, 3, 1, 0}));

}

TEST(UndirectedGraph, Coloring) {
    Graph<int> graph{4, 6, false};
    ASSERT_TRUE(graph.Colorize() == 4);
}