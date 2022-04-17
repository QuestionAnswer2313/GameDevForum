#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <Graphics/OpenGLBase.h>
#include <Math/SimpleMath.h>

typedef struct SpriteComponent
{
    i32 IsTextured;
    v4 Color;
    Texture2D Texture;
} SpriteComponent;

#define SpriteComponent(color, texture) _sprite_component(color, texture)
#define SpriteComponent_Color(color) _sprite_component_color(color)
#define SpriteComponent_ColorXYZW(x, y, z, w) _sprite_component_color(v4_new(x, y, z, w))
#define SpriteComponent_Texture(texture) _sprite_component_texture(texture)

force_inline SpriteComponent
_sprite_component(v4 color, Texture2D texture)
{
    SpriteComponent component;
    component.IsTextured = 1;
    component.Texture = texture;
    component.Color = v4_copy(color);
    return component;
}

force_inline SpriteComponent
_sprite_component_color(v4 color)
{
    SpriteComponent component;
    component.IsTextured = 0;
    component.Texture = (Texture2D) {0};
    component.Color = v4_copy(color);
    return component;
}

force_inline SpriteComponent
_sprite_component_texture(Texture2D texture)
{
    SpriteComponent component;
    component.IsTextured = 1;
    component.Texture = texture;
    component.Color = v4_new(1.0f, 1.0f, 1.0f, 1.0f);
    return component;
}

#endif
