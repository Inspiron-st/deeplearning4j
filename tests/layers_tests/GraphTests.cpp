//
// @author raver119@gmail.com
//

#include "testlayers.h"
#include <flatbuffers/flatbuffers.h>
#include <graph/generated/node_generated.h>
#include <graph/generated/graph_generated.h>
#include <graph/Node.h>
#include <graph/Graph.h>
#include <graph/NodeFactory.h>
#include <NDArray.h>

using namespace nd4j::graph;

class GraphTests : public testing::Test {
public:
    int *cShape = new int[8]{2, 2, 2, 2, 1, 0, 1, 99};
    int *fShape = new int[8]{2, 2, 2, 1, 2, 0, 1, 102};
};

TEST_F(GraphTests, SingleInput1) {
    Graph *graph = new Graph();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    graph->getVariableSpace()->putVariable(-1, x);

    auto nodeA = new Node(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node(OpType_TRANSFORM, 2, 2, {1}, {3});
    auto nodeC = new Node(OpType_TRANSFORM, 0, 3, {2}, {});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(3, graph->totalNodes());

    graph->execute();

    ASSERT_NEAR(0.4161468, x->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, DoubleInput1) {
    Graph *graph = new Graph();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto y = new NDArray<float>(5, 5, 'c');
    y->assign(-1.0);

    auto z = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, y);
    graph->getVariableSpace()->putVariable(-3, z);

    auto nodeA = new Node(OpType_TRANSFORM, 0, 1, {-1}, {3});
    auto nodeB = new Node(OpType_TRANSFORM, 0, 2, {-2}, {3});
    auto nodeC = new Node(OpType_PAIRWISE, 0, 3, {1, 2}, {-3});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);

    ASSERT_EQ(2, graph->rootNodes());
    ASSERT_EQ(3, graph->totalNodes());

    graph->execute();

    ASSERT_NEAR(3.0, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, SingleInput3) {
    Graph *graph = new Graph();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto v0 = new NDArray<float>(5, 5, 'c');
    auto v1 = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, v0);
    graph->getVariableSpace()->putVariable(-3, v1);

    auto nodeA = new Node(OpType_TRANSFORM, 0, 1, {-1}, {2, 3});
    auto nodeB = new Node(OpType_TRANSFORM, 14, 2, {1}, {-2});
    auto nodeC = new Node(OpType_TRANSFORM, 26, 3, {1}, {-3});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(3, graph->totalNodes());

    graph->execute();

    ASSERT_NEAR(1.4142135, v0->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(1.0, v1->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}

TEST_F(GraphTests, SingleInput4) {
    Graph *graph = new Graph();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto v0 = new NDArray<float>(5, 5, 'c');
    auto v1 = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, v0);
    graph->getVariableSpace()->putVariable(-3, v1);

    auto nodeA = new Node(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node(OpType_TRANSFORM, 14, 2, {1}, {3});
    auto nodeC = new Node(OpType_TRANSFORM, 6, 3, {2}, {4, 5});

    auto nodeS = new Node(OpType_TRANSFORM, 26, 4, {3}, {-2});
    auto nodeE = new Node(OpType_TRANSFORM, 27, 5, {3}, {-3});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);
    graph->addNode(nodeS);
    graph->addNode(nodeE);

    ASSERT_EQ(1, graph->rootNodes());
    ASSERT_EQ(5, graph->totalNodes());

    graph->execute();

    ASSERT_NEAR(1.0, v0->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(-1.4142135, v1->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, DoubleInput2) {
    Graph *graph = new Graph();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto y = new NDArray<float>(5, 5, 'c');
    y->assign(-1.0);

    auto z0 = new NDArray<float>(5, 5, 'c');
    auto z1 = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, y);
    graph->getVariableSpace()->putVariable(-3, z0);
    graph->getVariableSpace()->putVariable(-4, z1);


    auto nodeA = new Node(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node(OpType_TRANSFORM, 14, 2, {1}, {3});
    auto nodeC = new Node(OpType_TRANSFORM, 6, 3, {2}, {-3});

    auto nodeT = new Node(OpType_TRANSFORM, 0, 11, {-2}, {12});
    auto nodeU = new Node(OpType_TRANSFORM, 14, 12, {11}, {13});
    auto nodeV = new Node(OpType_TRANSFORM, 6, 13, {12}, {-4});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);
    graph->addNode(nodeT);
    graph->addNode(nodeU);
    graph->addNode(nodeV);

    ASSERT_EQ(2, graph->rootNodes());
    ASSERT_EQ(6, graph->totalNodes());

    graph->execute();

    ASSERT_NEAR(-1.4142135, z0->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(-1.0, z1->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, DoubleInput3) {
    Graph *graph = new Graph();

    auto x = new NDArray<float>(5, 5, 'c');
    x->assign(-2.0);

    auto y = new NDArray<float>(5, 5, 'c');
    y->assign(-1.0);

    auto z0 = new NDArray<float>(5, 5, 'c');
    auto z1 = new NDArray<float>(5, 5, 'c');


    auto w = new NDArray<float>(5, 5, 'c');

    graph->getVariableSpace()->putVariable(-1, x);
    graph->getVariableSpace()->putVariable(-2, y);
    graph->getVariableSpace()->putVariable(-3, z0);
    graph->getVariableSpace()->putVariable(-4, z1);
    graph->getVariableSpace()->putVariable(-5, w);


    auto nodeA = new Node(OpType_TRANSFORM, 0, 1, {-1}, {2});
    auto nodeB = new Node(OpType_TRANSFORM, 14, 2, {1}, {3});
    auto nodeC = new Node(OpType_TRANSFORM, 6, 3, {2}, {-3, 21});

    auto nodeT = new Node(OpType_TRANSFORM, 0, 11, {-2}, {12});
    auto nodeU = new Node(OpType_TRANSFORM, 14, 12, {11}, {13});
    auto nodeV = new Node(OpType_TRANSFORM, 6, 13, {12}, {-4, 21});

    auto nodeW = new Node(OpType_PAIRWISE, 0, 21, {3, 13}, {22});
    auto nodeZ = new Node(OpType_TRANSFORM, 0, 22, {21}, {-5});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);
    graph->addNode(nodeT);
    graph->addNode(nodeU);
    graph->addNode(nodeV);
    graph->addNode(nodeW);
    graph->addNode(nodeZ);

    ASSERT_EQ(2, graph->rootNodes());
    ASSERT_EQ(8, graph->totalNodes());

    graph->execute();

    ASSERT_NEAR(-1.4142135, z0->reduceNumber<simdOps::Mean<float>>(), 1e-5);
    ASSERT_NEAR(-1.0, z1->reduceNumber<simdOps::Mean<float>>(), 1e-5);

    ASSERT_NEAR(2.4142135, w->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}


TEST_F(GraphTests, QuadInput1) {
    Graph *graph = new Graph();

    auto x0 = new NDArray<float>(5, 5, 'c');
    x0->assign(0.0);

    auto x1 = new NDArray<float>(5, 5, 'c');
    x1->assign(-1.0);

    auto x2 = new NDArray<float>(5, 5, 'c');
    x2->assign(-2.0);

    auto x3 = new NDArray<float>(5, 5, 'c');
    x3->assign(-3.0);

    auto z = new NDArray<float>(5, 5, 'c');
    z->assign(119.0);

    graph->getVariableSpace()->putVariable(-1, x0);
    graph->getVariableSpace()->putVariable(-2, x1);
    graph->getVariableSpace()->putVariable(-3, x2);
    graph->getVariableSpace()->putVariable(-4, x3);
    graph->getVariableSpace()->putVariable(-5, z);

    auto nodeA = new Node(OpType_TRANSFORM, 0, 1, {-1}, {11});
    auto nodeB = new Node(OpType_TRANSFORM, 0, 2, {-2}, {11});
    auto nodeC = new Node(OpType_TRANSFORM, 0, 3, {-3}, {21});
    auto nodeD = new Node(OpType_TRANSFORM, 0, 4, {-4}, {21});

    auto nodeP1 = new Node(OpType_PAIRWISE, 0, 11, {1, 2}, {31});
    auto nodeP2 = new Node(OpType_PAIRWISE, 0, 21, {3, 4}, {31});

    auto nodeZ = new Node(OpType_PAIRWISE, 0, 31, {11, 21}, {-5});

    graph->addNode(nodeA);
    graph->addNode(nodeB);
    graph->addNode(nodeC);
    graph->addNode(nodeD);
    graph->addNode(nodeP1);
    graph->addNode(nodeP2);
    graph->addNode(nodeZ);

    ASSERT_EQ(4, graph->rootNodes());
    ASSERT_EQ(7, graph->totalNodes());

    graph->execute();

    ASSERT_NEAR(6.0, z->reduceNumber<simdOps::Mean<float>>(), 1e-5);
}