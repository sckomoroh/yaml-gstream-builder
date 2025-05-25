#include "GraphNode.h"

GraphNode::GraphNode(const std::string name, const std::string& factory, const std::string& caps)
    : name{name}
    , factory{factory}
    , capsName{caps} {}

std::string GraphNode::getName() const { return name; }

std::string GraphNode::getFactory() const { return factory; }

std::unordered_map<std::string, std::string> GraphNode::getProperties() const { return properties; }

std::list<IGraphNode::Ptr> GraphNode::getLinks() const { return links; }

std::string GraphNode::getCapsName() const { return capsName; }

std::list<IGraphNode::Ptr> GraphNode::getChildren() const { return children; }

void GraphNode::addLink(IGraphNode::Ptr target) { links.push_back(target); }

void GraphNode::addChild(IGraphNode::Ptr child) { children.push_back(child); }

void GraphNode::setProperty(const std::string& name, const std::string value) { properties[name] = value; }
