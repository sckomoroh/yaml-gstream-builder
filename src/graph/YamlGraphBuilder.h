#pragma once

#include <exception>
#include <unordered_map>
#include <list>

#include <yaml-cpp/yaml.h>

#include "GraphNode.h"
#include "IGraphBuilder.h"

class YamlGrapBuilderException : public std::exception {
private:
    std::string message;

public:
    YamlGrapBuilderException(const std::string message);

public:
    const char* what() const noexcept override;
};

class YamlGrapBuilder : public IGraphBuilder {
private:
    std::unordered_map<std::string, std::unordered_map<std::string, GraphNode::Ptr>> nodeMapping;

public:
    std::list<IGraphNode::Ptr> fromConfig(const std::string& fileName) override;

private:
    std::list<IGraphNode::Ptr> parsePipelines(const YAML::Node& yamlPipelinesNode);
    void parseLinks(const YAML::Node& yamlPipelinesNode);
    void parseBin(const YAML::Node& yamlPipelinesNode, GraphNode::Ptr graphParent, const std::string& pipelineName);
    std::string getScalarValue(const std::string& nodeName,
                               const YAML::Node& yamlNode,
                               const std::string& parentNodeName);

    void parseProperties(const YAML::Node& yamlNode, GraphNode::Ptr graphNode);
};
