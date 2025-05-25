#pragma once

#include "IGraphBuilder.h"

class GraphNode : public IGraphNode {
public:
    using Ptr = std::shared_ptr<GraphNode>;

private:
    const std::string name;
    const std::string factory;
    std::unordered_map<std::string, std::string> properties;
    std::list<IGraphNode::Ptr> links;
    std::list<IGraphNode::Ptr> children;
    std::string capsName;

public:
    GraphNode(const std::string name, const std::string& factory, const std::string& caps);

public:  // IGraphNode
    std::string getName() const override;
    std::string getFactory() const override;
    std::unordered_map<std::string, std::string> getProperties() const override;
    std::list<IGraphNode::Ptr> getLinks() const override;
    std::string getCapsName() const override;
    std::list<IGraphNode::Ptr> getChildren() const override;

public:
    void addLink(IGraphNode::Ptr target);
    void addChild(IGraphNode::Ptr child);
    void setProperty(const std::string& name, const std::string value);
};