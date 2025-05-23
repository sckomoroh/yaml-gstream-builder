#include "YamlGraphBuilder.h"


#include <spdlog/spdlog.h>

std::list<IGraphNode::Ptr> YamlGrapBuilder::fromConfig(const std::string& fileName) {
    std::list<IGraphNode::Ptr> pipelines;

    spdlog::info("Open config file: {}", fileName);

    auto rootNode = YAML::LoadFile(fileName);

    if (rootNode.IsMap() == false) {
        spdlog::error("The root node should be map");

        return pipelines;
    }

    auto pipelinesNode = rootNode["pipelines"];

    auto linksNode = rootNode["links"];

    return pipelines;
}

void YamlGrapBuilder::parsePipelines(const YAML::Node& pipelinesNode) {
    spdlog::debug("Parse pipelines");

    for (const auto& node : pipelinesNode) {
        auto nodeName = node.first.as<std::string>();
        auto factoryType = getFactoryName(node.second, nodeName);
        if (factoryType.empty()) {}
    }
}

void YamlGrapBuilder::parseBin(const YAML::Node& pipelinesNode, IGraphNode::Ptr graphParent) {}

std::string YamlGrapBuilder::getFactoryName(const YAML::Node& yamlNode, const std::string& nodeName) {
    auto yamlTypeNode = yamlNode["type"];
    if (yamlTypeNode) {
        spdlog::error("");
        return "";
    }

    if (yamlTypeNode.IsScalar() == false) {
        return "";
    }

    return yamlTypeNode.as<std::string>();
}
