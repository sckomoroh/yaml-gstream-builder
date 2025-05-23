#pragma once

#include "IGraphBuilder.h"

class GraphNode : public IGraphNode {
private:
    const std::string name;
    const std::string factory;
    std::unordered_map<std::string, std::string> properties;
    std::list<Ptr> links;

public:
    GraphNode(const std::string name, const std::string& factory);

public:  // IGraphNode
    std::string getName() const override;
    std::string getFactory() const override;
    std::unordered_map<std::string, std::string> getProperties() const override;
    std::list<Ptr> getLinks() const override;
};