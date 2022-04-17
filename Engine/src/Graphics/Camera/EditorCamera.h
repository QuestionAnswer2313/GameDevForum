#ifndef EDITOR_CAMERA_H
#define EDITOR_CAMERA_H

#include <Math/SimpleMath.h>
#include <Graphics/OpenGLBase.h>

typedef enum EditorMatrixType
{
    EditorMatrixType_Orthographic = 0,
    EditorMatrixType_Perspective = 1
} EditorMatrixType;

typedef struct EditorCameraSettings
{
    f32 ZoomLevel;
    f32 AspectRatio;
    f32 Speed;
    f32 Rotation;
    v3 RotationVector;
    i32 IsOrthographic;
    v3 Position;
    v3 Axis;
    f32 Near;
    f32 Far;
    f32 FovAngle;
} EditorCameraSettings;

typedef struct EditorCamera
{
    f32 ZoomLevel;
    f32 AspectRatio;
    f32 Speed;
    f32 Rotation;
    v3 RotationVector;
    f32 Far;
    f32 Near;
    f32 FOV;
    i32 IsOrthographic;
    v3 Position;
    v3 Axis;
    v3 LookAt;
    Camera InternalCamera;
} EditorCamera;

static void
editor_camera_set_orthograhic(EditorCamera* camera)
{
    camera->IsOrthographic = 1;
    f32 aspectZoom = camera->AspectRatio * camera->ZoomLevel;
    camera->InternalCamera.Projection = orthographic(-camera->ZoomLevel, camera->ZoomLevel, -aspectZoom, aspectZoom, camera->Near, camera->Far);

    //for ortho
#if 1
    camera->InternalCamera.View = m4_look_at(camera->Position,
					     camera->LookAt,
					     v3_new(0.0f, 1.0f, 0.0f));
#else
    camera_recalculate_view(&camera->InternalCamera, camera->Position, camera->Axis, camera->Rotation);
#endif
    camera_recalculate_view_projection(&camera->InternalCamera);
}

static void
editor_camera_set_perspective(EditorCamera* camera)
{
#if 1
    camera->IsOrthographic = 0;
    camera->InternalCamera.Projection = perspective(camera->Near, camera->Far, camera->AspectRatio, camera->FOV);

    //m4_look_at(v3 eye, v3 center, v3 up)
    camera->InternalCamera.View = m4_look_at(camera->Position,
					     camera->LookAt,
					     v3_new(0.0f, 1.0f, 0.0f));
    camera_recalculate_view_projection(&camera->InternalCamera);

#else
    GINFO("GLM!\n");
    camera->IsOrthographic = 0;
    glm_perspective(camera->FOV, camera->AspectRatio, camera->Near, camera->Far,
		    camera->InternalCamera.Projection.M);
    mat4 view;
    glm_lookat(camera->Position.V, camera->LookAt.V,
	       v3_new(0.0f, 1.0f, 0.0f).V,
	       view);
    glm_mat4_mul(camera->InternalCamera.Projection.M,
		 view,
		 camera->InternalCamera.ViewProjection.M);
#endif
}

static void
_editor_camera_set_projection(EditorCamera* camera)
{
    if (camera->IsOrthographic)
    {
	editor_camera_set_orthograhic(camera);
    }
    else
    {
	editor_camera_set_perspective(camera);
    }
}

force_inline EditorCamera
editor_camera_create_from_settings(EditorCameraSettings settings)
{
    EditorCamera this;
    this.ZoomLevel = settings.ZoomLevel;
    this.AspectRatio = settings.AspectRatio;
    this.Speed = settings.Speed;
    this.Rotation = settings.Rotation;
    this.RotationVector = settings.RotationVector;
    this.IsOrthographic = settings.IsOrthographic;
    this.Position = v3_copy(settings.Position);
    this.Axis = v3_copy(settings.Axis);
    this.Near = settings.Near;
    this.Far = settings.Far;
    this.FOV = rad(settings.FovAngle);
    return this;
}

force_inline EditorCamera
editor_camera_create(EditorCameraSettings settings)
{
    EditorCamera editorCamera = editor_camera_create_from_settings(settings);
    //place to settings
    editorCamera.LookAt = v3_new(0.0f, 0.0f, 0.0f);
    _editor_camera_set_projection(&editorCamera);
    return editorCamera;
}

force_inline void
editor_camera_on_update(EditorCamera* editorCamera)
{
    _editor_camera_set_projection(editorCamera);
}

force_inline void
editor_camera_resize(EditorCamera* editorCamera, f32 width, f32 height)
{
    editorCamera->AspectRatio = width / height;
    _editor_camera_set_projection(editorCamera);
}

#endif
