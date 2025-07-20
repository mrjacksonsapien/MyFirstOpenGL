#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Node.hpp"

class Camera : Node {
    private:
        float near;
        float far;
        float fov;
};

#endif