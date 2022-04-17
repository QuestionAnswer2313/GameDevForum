#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <Math/SimpleMath.h>

typedef struct TransformComponent
{
    m4 Transform;
} TransformComponent;

#define TransformComponent_(position, scale, rotation) _transform_from_tsr(position, scale, rotation)
#define TransformComponent_Position(x, y, z) _transform_from_tsr(v3_new(x, y, z), v3_new(1.0f, 1.0f, 1.0f), v3_new(0.0f, 0.0f, 0.0f))
#define TransformComponent_PositionV3(xyz) _transform_from_tsr(xyz, v3_new(1.0f, 1.0f, 1.0f), v3_new(0.0f, 0.0f, 0.0f))

#define TransformComponent_Array(arr) _transform_from_array(arr)

force_inline TransformComponent
_transform_from_tsr(v3 translation, v3 scale, v3 rotation)
{
    TransformComponent component;
    component.Transform = m4_transform(translation, scale, rotation);
    return component;
}

force_inline TransformComponent
_transform_from_array(f32* arr)
{
    TransformComponent component;
    component.Transform.M[0][0] = arr[0];
    component.Transform.M[0][1] = arr[1];
    component.Transform.M[0][2] = arr[2];
    component.Transform.M[0][3] = arr[3];

    component.Transform.M[1][0] = arr[4];
    component.Transform.M[1][1] = arr[5];
    component.Transform.M[1][2] = arr[6];
    component.Transform.M[1][3] = arr[7];

    component.Transform.M[2][0] = arr[8];
    component.Transform.M[2][1] = arr[9];
    component.Transform.M[2][2] = arr[10];
    component.Transform.M[2][3] = arr[11];

    component.Transform.M[3][0] = arr[12];
    component.Transform.M[3][1] = arr[13];
    component.Transform.M[3][2] = arr[14];
    component.Transform.M[3][3] = arr[15];

    return component;
}

#endif
