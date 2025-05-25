#include <gtest/gtest.h>

#include "graph/YamlGraphBuilder.h"

class GraphBuilderTest : public testing::Test {
public:
    GraphBuilderTest() {}
};

TEST_F(GraphBuilderTest, parseConfig) {
    auto builder = YamlGrapBuilder();
    auto pipelineNodes = builder.fromConfig("config/test-config.yaml");

    for (auto p : pipelineNodes) {
        auto n = p->getName();
        auto c = p->getChildren();
        int a = 0;
    }

    EXPECT_FALSE(pipelineNodes.empty());
}
