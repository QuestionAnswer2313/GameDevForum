#ifndef CAMERA_COMPONENT_H
#define CAMERA_COMPONENT_H

#include <Utils/SimpleStandardLibrary.h>
#include <Graphics/OpenGLBase.h>

typedef struct CameraComponent
{
    i32 IsPrimary;
    Camera Camera;
} CameraComponent;

#define CameraComponent_(isPrimary, projection) _camera_component(isPrimary, projection)

force_inline CameraComponent
_camera_component(i32 isPrimary, m4 projection)
{
    CameraComponent result;

    Camera camera = {};
    camera.Projection = m4_copy(projection);
    result.Camera = camera;
    result.IsPrimary = isPrimary;

    return result;
}

#endif
