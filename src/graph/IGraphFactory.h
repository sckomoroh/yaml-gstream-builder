#pragma once

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

class IGraphNode {
public:
    using Ptr = std::shared_ptr<IGraphNode>;

public:
    virtual ~IGraphNode() = default;

public:
    virtual std::string name() const = 0;
    virtual std::string factory() const = 0;
    virtual std::unordered_map<std::string, std::string> properties() const = 0;
    virtual std::list<Ptr> links() const = 0;
};

class IGraphFactory {
public:
    virtual ~IGraphFactory() = default;

public:
    virtual IGraphNode::Ptr fromConfig(const std::string& fileName) = 0;
};
