#ifndef RENDERER_H
#define RENDERER_H

#include <Graphics/OpenGLStructs.h>
#include <Graphics/Window.h>

/* Base Renderer */
void renderer_set_viewport(u32 width, u32 height);
void renderer_clear(v4 color);

void renderer_enable_wireframe_mode();
void renderer_enable_fill_mode();
void renderer_enable_face_culling();
void renderer_enable_blending();

void renderer_disable_face_culling();
void renderer_enable_depth_testing();
void renderer_disable_depth_testing();
void renderer_disable_blending();

/*
	     2D Renderer
*/
typedef struct Renderer2DStatistics
{
    //Empty for now
} Renderer2DStatistics;
void renderer2d_init(Camera* camera);
void renderer2d_destroy();
void renderer2d_submit(m4 transform, v4 color, Texture2D texture);
void renderer2d_submit_rectangle(v3 position, v2 size, v4 color, Texture2D texture);
void renderer2d_submit_rectangle_color(v3 position, v2 size, v4 color);
void renderer2d_submit_empty_rectangle(v3 position, v2 size, v4 color);

void renderer2d_submit_line(v3 start, v3 end, v4 color);

void renderer2d_set_line_thickness(f32 thickness);
void renderer2d_flush_rectangles();
void renderer2d_flush_lines();
void renderer2d_flush();

/*
	     3D Renderer
*/
void renderer3d_init(Camera* camera);
i32 renderer3d_register_texture(Texture2D newTexture, v2 textureCoords[4]);

void renderer3d_submit_cube(m4 transform, Texture2D front, Texture2D back, Texture2D left, Texture2D right, Texture2D bottom, Texture2D up);
void renderer3d_submit_cube_position(v3 position,
				     Texture2D front, Texture2D back,
				     Texture2D left, Texture2D right,
				     Texture2D bottom, Texture2D up);
void renderer3d_submit_craft_cube(v3 center, Texture2D up, Texture2D down, Texture2D side);

/* Flush functions */
void renderer3d_flush_cubes();
void renderer3d_flush();


#endif
