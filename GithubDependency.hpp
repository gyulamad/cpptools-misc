#pragma once

#include "Dependency.hpp"

class GithubDependency: public Dependency {
public:
    using Dependency::Dependency;
    virtual ~GithubDependency() {}

protected:
};