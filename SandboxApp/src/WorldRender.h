#ifndef WORLD_RENDER_H
#define WORLD_RENDER_H

#include <cglm/cglm.h>
#include <Graphics/Renderer.h>

static NativeWindow* Window = NULL;
static EditorCamera CurrentCamera;
static EditorCamera* world_render_get_camera(){return &CurrentCamera;}
static Camera* ActiveCamera = NULL;
static Framebuffer CurrentFramebuffer;
static Renderer2DStatistics RendererStatistics;
static Shader CurrentShader;

static Texture2D Up;
static Texture2D Bottom;
static Texture2D Front;
static Texture2D Back;
static Texture2D Left;
static Texture2D Right;

static i32 GrassTextureID;
static v4 WhiteColor;

static void
world_render_init(NativeWindow* window)
{
    Window = window;
    f32 aspectRatio = (f32) Window->Width / Window->Height;

    EditorCameraSettings settings = (EditorCameraSettings)
	{
	    .ZoomLevel = 5,
	    .AspectRatio = aspectRatio,
	    .Speed = 5,
	    .Rotation = 0,
	    .RotationVector = v3_new(-0.2, 0.8, 0),
	    .IsOrthographic = 0,
	    .Position = { -2.f, 5.f, 4.6f },
	    .Axis = { 0.0f, 0.0f, 0.0f },
	    .Near = 0.01f,
	    .Far = 5000.0f,
	    .FovAngle = 90.0f
	};
    CurrentCamera = editor_camera_create(settings);

    renderer3d_init(&CurrentCamera.InternalCamera);
    renderer_set_viewport(Window->Width, Window->Height);

    //renderer_enable_face_culling();
    renderer_enable_depth_testing();
    //renderer_enable_blending();

    Up = texture2d_create(asset_texture("ice.png"));
    Bottom = texture2d_create(asset_texture("dirt.png"));
    Front = texture2d_create(asset_texture("grass_block_side.png"));
    Back = texture2d_create(asset_texture("brown_wool.png"));
    Left = texture2d_create(asset_texture("crimson_stem_top.png"));
    Right = texture2d_create(asset_texture("grass_block_snow.png"));

    WhiteColor = v4_new(1.0f, 1.0f, 1.0f, 1.0f);
}

static void
world_render_on_event(Event* event)
{
    if (event->Type == KeyPressed)
    {
	KeyPressedEvent* keyPressedEvent = (KeyPressedEvent*) event;
	switch (keyPressedEvent->KeyCode)
	{
	case KEY_P:
	{
	    if (CurrentCamera.IsOrthographic)
	    {
		editor_camera_set_perspective(&CurrentCamera);
	    }
	    else
	    {
		editor_camera_set_orthograhic(&CurrentCamera);
	    }
	    break;
	}
	case KEY_E:
	{
	    static i32 wireframeModeEnabled = 0;
	    if (!wireframeModeEnabled)
	    {
		wireframeModeEnabled = 1;
		renderer_enable_wireframe_mode();
	    }
	    else
	    {
		wireframeModeEnabled = 0;
		renderer_enable_fill_mode();
	    }
	    break;
	}
	case KEY_F:
	{
	    static i32 isFaceCullingEnabled = 0;
	    if (!isFaceCullingEnabled)
	    {
		isFaceCullingEnabled = 1;
		renderer_enable_face_culling();
	    }
	    else
	    {
		isFaceCullingEnabled = 0;
		renderer_disable_face_culling();
	    }
	    break;
	}
	case KEY_G:
	{
	    static i32 isDepthTestingEnabled = 0;
	    if (!isDepthTestingEnabled)
	    {
		isDepthTestingEnabled = 1;
		renderer_enable_depth_testing();
	    }
	    else
	    {
		isDepthTestingEnabled = 0;
		renderer_disable_depth_testing();
	    }
	    break;
	}
	}
    }
}

//TODO(bies): rename
void
update_camera_positions()
{
#if 1
    CurrentCamera.Position.X = CurrentCamera.LookAt.X
	+ sin(CurrentCamera.RotationVector.X) * CurrentCamera.ZoomLevel;
    CurrentCamera.Position.Y = CurrentCamera.LookAt.Y
	+ cos(CurrentCamera.RotationVector.Y) * CurrentCamera.ZoomLevel;
    CurrentCamera.Position.Z = CurrentCamera.LookAt.Z
	+ cos(CurrentCamera.RotationVector.X) * CurrentCamera.ZoomLevel;
#else
#endif
}

static void
world_render_camera_move_left(f32 timestep)
{
    CurrentCamera.RotationVector.X -= CurrentCamera.Speed * timestep;
    update_camera_positions();
}
static void
world_render_camera_move_right(f32 timestep)
{
    CurrentCamera.RotationVector.X += CurrentCamera.Speed * timestep;
    update_camera_positions();
}
static void
world_render_camera_move_up(f32 timestep)
{
    CurrentCamera.RotationVector.Y -= 3.14f * timestep;
    update_camera_positions();
}
static void
world_render_camera_move_bot(f32 timestep)
{
    CurrentCamera.RotationVector.Y += 3.14f * timestep;
    update_camera_positions();
}

static void
world_render_flush(f32 timestep)
{
    renderer_clear(v4_new(0.1f, 0.1f, 0.1f, 1.0f));

    EditorCamera* camera = &CurrentCamera;

    editor_camera_on_update(camera);

    renderer3d_submit_cube_position(v3_new(0, 0, 0),
				    Front, Back,
				    Left, Right,
				    Bottom, Up);
    /* renderer3d_submit_cube_position(v3_new(0, -2, 0), Front, Back, */
    /*				    Left, Right, */
    /*				    Bottom, Up); */
    /* renderer3d_submit_cube_position(v3_new(-2, 0, 0), Front, Back, */
    /*				    Left, Right, */
    /*				    Bottom, Up); */

    renderer3d_flush_cubes();
}

Framebuffer
world_render_get_framebuffer()
{
    return CurrentFramebuffer;
}

void
world_render_destroy()
{
}

#endif
