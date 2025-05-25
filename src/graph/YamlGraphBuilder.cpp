#include "YamlGraphBuilder.h"

#include <spdlog/spdlog.h>

YamlGrapBuilderException::YamlGrapBuilderException(const std::string message)
    : message{message} {}

const char* YamlGrapBuilderException::what() const noexcept { return message.c_str(); }

std::list<IGraphNode::Ptr> YamlGrapBuilder::fromConfig(const std::string& fileName) {
    std::list<IGraphNode::Ptr> pipelines;

    spdlog::info("Open config file: {}", fileName);

    auto rootNode = YAML::LoadFile(fileName);

    if (rootNode.IsMap() == false) {
        throw YamlGrapBuilderException("The root node should be map");
    }

    auto pipelinesNode = rootNode["pipelines"];
    pipelines = parsePipelines(pipelinesNode);

    auto linksNode = rootNode["links"];
    parseLinks(linksNode);

    return pipelines;
}

std::list<IGraphNode::Ptr> YamlGrapBuilder::parsePipelines(const YAML::Node& pipelinesNode) {
    spdlog::debug("Parse pipelines");

    std::list<IGraphNode::Ptr> result;

    for (const auto& nodePair : pipelinesNode) {
        auto nodeName = nodePair.first.as<std::string>();
        auto factoryType = getScalarValue("type", nodePair.second, nodeName);

        auto pipelineGraphNode = std::make_shared<GraphNode>(nodeName, factoryType, "");

        parseBin(nodePair.second, pipelineGraphNode, nodeName);

        result.push_back(pipelineGraphNode);
    }

    return result;
}

void YamlGrapBuilder::parseBin(const YAML::Node& parentNode,
                               GraphNode::Ptr graphParent,
                               const std::string& pipelineName) {
    for (const auto& nodePair : parentNode) {
        if (nodePair.second.IsMap() == false) {
            continue;
        }

        auto nodeName = nodePair.first.as<std::string>();

        if (nodeMapping[pipelineName].find(nodeName) != nodeMapping[pipelineName].end()) {
            throw YamlGrapBuilderException(
                fmt::format("The node '{}' already exists in pipeline '{}'", nodeName, pipelineName));
        }

        auto nodeFactory = getScalarValue("type", nodePair.second, nodeName);
        std::string caps;

        try {
            caps = getScalarValue("caps", nodePair.second, nodeName);
        } catch (const YamlGrapBuilderException&) {
            caps = "ANY";
        }

        auto graphNode = std::make_shared<GraphNode>(nodeName, nodeFactory, caps);

        nodeMapping[pipelineName][nodeName] = graphNode;

        parseProperties(nodePair.second, graphNode);

        if (nodeFactory == "bin") {
            parseBin(nodePair.second, graphNode, pipelineName);
        }

        graphParent->addChild(graphNode);
    }
}

std::string YamlGrapBuilder::getScalarValue(const std::string& nodeName,
                                            const YAML::Node& yamlNode,
                                            const std::string& parentNodeName) {
    auto yamlTypeNode = yamlNode[nodeName];
    if (!yamlTypeNode) {
        throw YamlGrapBuilderException(
            fmt::format("The value '{}' for element '{}' not found", nodeName, parentNodeName));
    }

    if (yamlTypeNode.IsScalar() == false) {
        throw YamlGrapBuilderException(
            fmt::format("The value '{}' for element '{}' is not scalar", nodeName, parentNodeName));
    }

    return yamlTypeNode.as<std::string>();
}

void YamlGrapBuilder::parseProperties(const YAML::Node& yamlNode, GraphNode::Ptr graphNode) {
    for (const auto& yamlNodePair : yamlNode) {
        if (yamlNodePair.second.IsScalar() == false) {
            continue;
        }

        auto nodeName = yamlNodePair.first.as<std::string>();
        if (nodeName == "type" || nodeName == "caps") {
            continue;
        }

        graphNode->setProperty(nodeName, yamlNodePair.second.as<std::string>());
    }
}

void YamlGrapBuilder::parseLinks(const YAML::Node& yamlPipelinesNode) {
    for (const auto& yamlPipelineNodePair : yamlPipelinesNode) {
        auto pipelineNodeName = yamlPipelineNodePair.first.as<std::string>();
        if (nodeMapping.find(pipelineNodeName) == nodeMapping.end()) {
            throw YamlGrapBuilderException(
                fmt::format("The pipeline '{}' not found in pipelines section", pipelineNodeName));
        }

        for (const auto& yamlNodePair : yamlPipelineNodePair.second) {
            auto nodeName = yamlNodePair.first.as<std::string>();
            if (yamlNodePair.second.IsScalar() == false && yamlNodePair.second.IsSequence() == false) {
                throw YamlGrapBuilderException(fmt::format("The node '{}' should be sequence or scalar", nodeName));
            }

            if (nodeMapping[pipelineNodeName].find(nodeName) == nodeMapping[pipelineNodeName].end()) {
                throw YamlGrapBuilderException(fmt::format(
                    "The node '{}' for pipeline '{}'not found in pipelines section", nodeName, pipelineNodeName));
            }

            if (yamlNodePair.second.IsScalar()) {
                auto targetNode = yamlNodePair.second.as<std::string>();
                if (nodeMapping[pipelineNodeName].find(targetNode) == nodeMapping[pipelineNodeName].end()) {
                    throw YamlGrapBuilderException(fmt::format(
                        "The node '{}' for pipeline '{}'not found in pipelines section", targetNode, pipelineNodeName));
                }

                nodeMapping[pipelineNodeName][nodeName]->addLink(nodeMapping[pipelineNodeName][targetNode]);
            } else {
                for (const auto& yamlValue : yamlNodePair.second) {
                    if (yamlValue.IsScalar() == false) {
                        throw YamlGrapBuilderException(
                            fmt::format("The sequence should contains only scalar values in node {}", nodeName));
                    }

                    auto targetNode = yamlValue.as<std::string>();
                    if (nodeMapping[pipelineNodeName].find(targetNode) == nodeMapping[pipelineNodeName].end()) {
                        throw YamlGrapBuilderException(
                            fmt::format("The node '{}' for pipeline '{}'not found in pipelines section", targetNode,
                                        pipelineNodeName));
                    }

                    nodeMapping[pipelineNodeName][nodeName]->addLink(nodeMapping[pipelineNodeName][targetNode]);
                }
            }
        }
    }
}