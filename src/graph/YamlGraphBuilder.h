#pragma once

#include <exception>

#include <yaml-cpp/yaml.h>

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
public:
    std::list<IGraphNode::Ptr> fromConfig(const std::string& fileName) override;

private:
    void parsePipelines(const YAML::Node& yamlPipelinesNode);
    void parseBin(const YAML::Node& yamlPipelinesNode, IGraphNode::Ptr graphParent);
    std::string getFactoryName(const YAML::Node& yamlNode, const std::string& nodeName);
};
