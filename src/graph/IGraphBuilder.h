#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

enum class EGraphFactory { YAML };

class IGraphNode {
public:
    using Ptr = std::shared_ptr<IGraphNode>;

public:
    virtual ~IGraphNode() = default;

public:
    virtual std::string getName() const = 0;
    virtual std::string getFactory() const = 0;
    virtual std::unordered_map<std::string, std::string> getProperties() const = 0;
    virtual std::list<Ptr> getLinks() const = 0;
    virtual std::string getCapsName() const = 0;
    virtual std::list<Ptr> getChildren() const = 0;
};

class IGraphBuilder {
public:
    using Ptr = std::shared_ptr<IGraphBuilder>;

public:
    virtual ~IGraphBuilder() = default;

public:
    virtual std::list<IGraphNode::Ptr> fromConfig(const std::string& fileName) = 0;
};

IGraphBuilder::Ptr createFactory(EGraphFactory factory);
