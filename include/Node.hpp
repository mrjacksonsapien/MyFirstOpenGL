#ifndef NODE_HPP
#define NODE_HPP

#include "Transform.hpp"
#include "EngineFlags.hpp"

class Node {
    protected:
        uint32_t dirtyFlags = DIRTY_ALL;
        Transform transform;
};

#endif