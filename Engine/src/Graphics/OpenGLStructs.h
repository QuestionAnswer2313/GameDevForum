#ifndef OPENGL_STRUCTS_H
#define OPENGL_STRUCTS_H

#include "OpenGLBase.h"

//NOTE(bies): GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS also can be used
//GL_MAX_TEXTURE_IMAGE_UNITS
#define MaxTextureSlotsNumber 128
typedef struct TextureList
{
    u32 NextTextureIndex; // it can be called count
    u32 StartIndex;
    u32 MaxIndex;
    u32 Textures[MaxTextureSlotsNumber];
} TextureList;

force_inline TextureList
texture_list_create()
{
    TextureList result = (TextureList) { .NextTextureIndex = 0, .StartIndex = 0 };
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &result.MaxIndex);
    GINFO("Max texture slots: %d\n", result.MaxIndex);
    return result;
}

force_inline void
texture_list_set_immutable(TextureList* list, u32 textureId)
{
    list->Textures[list->StartIndex] = textureId;
    ++list->StartIndex;
}

force_inline TextureList
texture_list_create_default(u32 defaultTextureId)
{
    TextureList result = (TextureList) { .NextTextureIndex = 0, .StartIndex = 0 };
    //texture_list_set_immutable(&result, defaultTextureId);
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &result.MaxIndex);
    GINFO("Max texture slots: %d\n", result.MaxIndex);
    return result;
}
force_inline i32
texture_list_is_full(TextureList* list)
{
    return list->NextTextureIndex >= list->MaxIndex;
}
force_inline i32
texture_list_contains(TextureList* list, u32 textureID)
{
    i32 i;
    for (i = list->StartIndex; i < list->NextTextureIndex; ++i)
    {
	if (list->Textures[i] == textureID)
	{
	    return i;
	}
    }

    return -1;
}
force_inline void
texture_list_add(TextureList* list, u32 textureId, i32 textureIndex)
{
    list->Textures[textureIndex] = textureId;
    ++list->NextTextureIndex;
}
force_inline i32
texture_list_add_to_begining(TextureList* list, u32 textureId)
{
    i32 textureIndex = list->NextTextureIndex;
    texture_list_add(list, textureId, list->NextTextureIndex);
    return textureIndex;
}
force_inline void
texture_list_bind(TextureList* list)
{
    i32 i;
    for (i = 0; i < list->NextTextureIndex; ++i)
    {
	//GINFO("Bound texture %d on slot %d\n", list->Textures[i], i);
	texture2d_bind_index(list->Textures[i], i);
    }
}
force_inline void
texture_list_unbind(TextureList* list)
{
    i32 i;
    for (i = 0; i < list->NextTextureIndex; i++)
    {
	texture2d_unbind_index(list->Textures[i], i);
    }
}
/*
NOTE(bies):
Зависимость texture_list от рендерер API абсолютно абсурдна, тк это просто базовая структура, которая реализует цикличный буфер текстур, для Batching'а (совмещения) отрисовки.
Функция texture_list_submit_texture_or_flush делает две вещи:
1. Если в буфере есть место, то добавляет текстуру
2. Возвращает ответ в виде индекса найденной или только добавленной текстуры
Чтобы вписать в рендеринг данную функцию следует возвращать -1, когда на стороне клиента нужно сделать следующее:
i32 textureId = texture_list_submit_texture_or_flush(&RendererData.List, texture);
if (textureId == -1)
{
    renderer2d_flush();
    texture_list_add_to_begining(&RendererData.List, texture);
}

При этом renderer2d_flush() должен вызываться при переполнении не в функции texture_list.

*/
force_inline i32
texture_list_submit_texture_or_flush(TextureList* list, u32 textureId)
{
    if (textureId == 0)
    {
	return 0;
    }

    i32 index = texture_list_contains(list, textureId);
    if (index != -1)
    {
	return index;
    }

    if (texture_list_is_full(list))
    {
	vassert_break();
	return -1;
    }

    i32 textureIndex = list->NextTextureIndex;
    texture_list_add(list, textureId, textureIndex);
    return textureIndex;
}

#endif
