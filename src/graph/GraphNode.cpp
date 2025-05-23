#include "GraphNode.h"

GraphNode::GraphNode(const std::string name, const std::string& factory) {}

std::string GraphNode::getName() const { return name; }

std::string GraphNode::getFactory() const { return factory; }

std::unordered_map<std::string, std::string> GraphNode::getProperties() const { return properties; }

std::list<IGraphNode::Ptr> GraphNode::getLinks() const { return links; }
