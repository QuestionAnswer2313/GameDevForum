#ifndef OPENGLBASE
#define OPENGLBASE

#include <glad/glad.h>
#include <Utils/SimpleStandardLibrary.h>
#include <Math/SimpleMath.h>

typedef struct Camera
{
    m4 View;
    m4 Projection;
    m4 ViewProjection;
} Camera;
force_inline void
camera_set_view(Camera* camera, m4 view)
{
    camera->View = m4_copy(view);
}
force_inline void
camera_set_projection(Camera* camera, m4 projection)
{
    camera->Projection = m4_copy(projection);
}
force_inline void
camera_set_view_projection(Camera* camera, m4 viewProjection)
{
    camera->ViewProjection = m4_copy(viewProjection);
}
force_inline Camera
camera_create(m4 projection, m4 view, m4 viewProjection)
{
    Camera camera;
    camera.View           = m4_copy(view);
    camera.Projection     = m4_copy(projection);
    camera.ViewProjection = m4_copy(viewProjection);
    return camera;
}
force_inline void
camera_recalculate_view(Camera* camera, v3 position, v3 rotationAxis, f32 rotationValue)
{
    m4 translate = m4_translate_identity(position);
    m4 rotate = m4_rotate(m4_identity(), rotationAxis, rotationValue);
    m4 transform = m4_mul(translate, rotate);
    camera->View = m4_inverse(transform);
}
force_inline void
camera_recalculate_view_projection(Camera* camera)
{
    camera->ViewProjection = m4_mul(camera->Projection, camera->View);
}

/* Arcball camera - camera around object */
typedef struct ArcballCamera
{
    i32 i;
} ArcballCamera;
force_inline void
arcball_camera()
{
}

i32 opengl_context_init(GLADloadproc gladLoadProc);
void opengl_error_callback(GLenum source, GLenum error, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);

typedef enum DataType
{
    Float1 = 0, Float2, Float3, Float4,
    Int1, Int2, Int3, Int4
} DataType;

//typedef struct BufferLayout {
//    BufferElement* Elements;
//    i32 Stride;
//} BufferLayout;

typedef struct BufferLayout
{
    i8 IsNormalized;
    DataType Type;
    i32 Size;
    i32 Count;
    i32 Offset;
} BufferLayout;

typedef struct VertexBuffer
{
    u32 RendererID;
    i32 Stride;
    f32* Vertices;
    BufferLayout* Layout;
} VertexBuffer;

typedef struct IndexBuffer
{
    u32 RendererID;
    u32 Count;
    u32* Indices;
} IndexBuffer;

typedef struct VertexArray
{
    u32 RendererID;
    VertexBuffer Vertex;
    IndexBuffer Index;
} VertexArray;

force_inline u32
data_type_get_size(DataType type)
{
    switch (type)
    {
    case Float1: return 4;
    case Float2: return 4 * 2;
    case Float3: return 4 * 3;
    case Float4: return 4 * 4;
    case Int1: return 4;
    case Int2: return 4 * 2;
    case Int3: return 4 * 3;
    case Int4: return 4 * 4;
    }

    vassert(0 && "Wrong datatype!");
    return 0;
}

force_inline u32
data_type_get_count(DataType type)
{
    switch (type)
    {
    case Float1: return 1;
    case Float2: return 2;
    case Float3: return 3;
    case Float4: return 4;
    case Int1: return 1;
    case Int2: return 2;
    case Int3: return 3;
    case Int4: return 4;
    }
}

BufferLayout* buffer_layout_create(DataType types[], i32 count);
i32 buffer_layout_get_size(BufferLayout* layout);

i32 vertex_buffer_get_current();
void vertex_buffer_set_data(VertexBuffer* buffer, f32* data, u32 size);
VertexBuffer vertex_buffer_create(f32* vertices, u32 size);
VertexBuffer vertex_buffer_create_allocated(u32 size);
void vertex_buffer_add_layout(VertexBuffer* buffer, BufferLayout* layout);
void vertex_buffer_bind(VertexBuffer* buffer);
void vertex_buffer_unbind();

i32 index_buffer_get_current();
IndexBuffer index_buffer_create(u32* indices, u32 size);
void index_buffer_bind(IndexBuffer* buffer);
void index_buffer_unbind();

i32 vertex_array_get_current();
VertexArray vertex_array_create(VertexBuffer vertexBuffer, IndexBuffer indexBuffer);
void vertex_array_enable_layout(VertexArray* vertexArray);
void vertex_array_disable_layout(VertexArray* vertexArray);
void vertex_array_bind(VertexArray* va);
void vertex_array_unbind();

force_inline void
vertex_array_destroy(VertexArray* va)
{
    glDeleteVertexArrays(1, &va->RendererID);
}

typedef enum FrameBufferType
{
    FRAMEBUFFER_TYPE_NONE,

    FRAMEBUFFER_TYPE_RGBA8,
    FRAMEBUFFER_TYPE_RED_INTEGER,
    FRAMEBUFFER_TYPE_DEPTH24_STENCIL8
} FrameBufferType;

typedef struct Framebuffer
{
    i8 IsSwapChainTarget;
    u32 Width;
    u32 Height;
    u32 Samples;
    u32 RendererId;
    u32* ColorAttachments;
    u32 DepthAttachment;

    FrameBufferType* ColorAttachmentTypes;
    FrameBufferType DepthAttachmentType;
} Framebuffer;

void framebuffer_create(Framebuffer* framebuffer, u32 width, u32 height, FrameBufferType* attachments);
void framebuffer_invalidate(Framebuffer* framebuffer, u32 width, u32 height);
i32 framebuffer_read_pixel(Framebuffer* framebuffer, i32 attachment, i32 x, i32 y);
void framebuffer_read_pixel_color(Framebuffer* framebuffer, i32 attachment, i32 x, i32 y, v4 result);
void framebuffer_bind(Framebuffer* framebuffer);
void framebuffer_unbind();
void framebuffer_destroy(Framebuffer* framebuffer);


typedef struct Texture2D
{
    u32 ID;
    u32 Width;
    u32 Height;
    u32 Channels;
    u32 Slot;
    u32 DataFormat;
    const char* Path;
} Texture2D;

Texture2D texture2d_create(const char* path);
Texture2D texture2d_create_from_buffer(void* data, u32 width, u32 height, u8 channels);
void texture2d_set_data(Texture2D* texture, void* data);
// dont do this
// void texture2d_set_from_file(Texture2D* texture, const char* path);
void texture2d_bind(Texture2D* texture, u32 slot);
void texture2d_unbind(Texture2D* texture);
void texture2d_bind_index(u32 id, u32 slot);
void texture2d_unbind_index(u32 id, u32 slot);

void texture2d_delete(Texture2D* texture);
void texture2d_destroy(Texture2D texture);

typedef struct TextureAtlas
{
    f32 AtlasWidth;
    f32 AtlasHeight;
    f32 TextureWidth;
    f32 TextureHeight;
    Texture2D Texture;
} TextureAtlas;

void texture_atlas_create(TextureAtlas* atlas, const char* path, v2 atlasSize, v2 textureSize);

typedef struct ShaderSource
{
    char* name;
    const char* vertex_shader;
    const char* fragment_shader;
} ShaderSource;

typedef struct UniformTableType
{
    const char* Key;
    i32 Value;
} UniformTableType;

typedef struct Shader
{
    u32 ShaderID;
    UniformTableType* UniformTable;
} Shader;

ShaderSource shader_load(const char* shader_path);
Shader shader_compile(ShaderSource source);
Shader shader_create(const char* shaderPath);
Shader shader_compile_safe(const char* shaderPath);
void shader_delete(Shader* shader);
void shader_bind(Shader* shader);
void shader_unbind();
void shader_delete_collection();

force_inline i32
shader_get_location(Shader* shader, const char* uniformName)
{
    vassert(uniformName != NULL);

    i32 location = shash_geti(shader->UniformTable, uniformName);
    if (location == -1)
    {
	location = glGetUniformLocation(shader->ShaderID, uniformName);
	shash_put(shader->UniformTable, uniformName, location);
    }
    else
    {
	location = shash_get(shader->UniformTable, uniformName);
    }

    return location;
}

force_inline void
shader_set_1float(Shader* shader, const char* uniformName, f32 v0)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform1f(location, v0);
    }
}

force_inline void
shader_set_2float(Shader* shader, const char* uniformName, f32 v0, f32 v1)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform2f(location, v0, v1);
    }
}

force_inline void
shader_set_3float(Shader* shader, const char* uniformName, f32 v0, f32 v1, f32 v2)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform3f(location, v0, v1, v2);
    }
}

force_inline void
shader_set_4float(Shader* shader, const char* uniformName, f32 v0, f32 v1, f32 v2, f32 v3)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform4f(location, v0, v1, v2, v3);
    }
}


force_inline void
shader_set_1int(Shader* shader, const char* uniformName, i32 v0)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform1i(location, v0);
    }
}

force_inline void
shader_set_2int(Shader* shader, const char* uniformName, i32 v0, i32 v1)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform2i(location, v0, v1);
    }
}

force_inline void
shader_set_3int(Shader* shader, const char* uniformName, i32 v0, i32 v1, i32 v2)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform3i(location, v0, v1, v2);
    }
}

force_inline void
shader_set_4int(Shader* shader, const char* uniformName, i32 v0, i32 v1, i32 v2, i32 v3)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform4i(location, v0, v1, v2, v3);
    }
}

force_inline void
shader_set_1uint(Shader* shader, const char* uniformName, u32 v0)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform1ui(location, v0);
    }
}

force_inline void
shader_set_2uint(Shader* shader, const char* uniformName, u32 v0, u32 v1)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform2ui(location, v0, v1);
    }
}

force_inline void
shader_set_3uint(Shader* shader, const char* uniformName, u32 v0, u32 v1, u32 v2)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform3ui(location, v0, v1, v2);
    }
}

force_inline void
shader_set_4uint(Shader* shader, const char* uniformName, u32 v0, u32 v1, u32 v2, u32 v3)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform4ui(location, v0, v1, v2, v3);
    }
}


force_inline void
shader_set_float1(Shader* shader, const char* uniformName, i32 count, f32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform1fv(location, count, values);
    }
}

force_inline void
shader_set_float2(Shader* shader, const char* uniformName, i32 count, f32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform2fv(location, count, values);
    }
}

force_inline void
shader_set_float3(Shader* shader, const char* uniformName, i32 count, f32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform3fv(location, count,  values);
    }
}

force_inline void
shader_set_float4(Shader* shader, const char* uniformName, i32 count, f32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform4fv(location, count, values);
    }
}

force_inline void
shader_set_int1(Shader* shader, const char* uniformName, i32 count, i32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform1iv(location, count, values);
    }
}

force_inline void
shader_set_int2(Shader* shader, const char* uniformName, i32 count, i32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform2iv(location, count, values);
    }
}

force_inline void
shader_set_int3(Shader* shader, const char* uniformName, i32 count, i32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform3iv(location, count, values);
    }
}

force_inline void
shader_set_int4(Shader* shader, const char* uniformName, i32 count, i32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform4iv(location, count, values);
    }
}

force_inline void
shader_set_uint1(Shader* shader, const char* uniformName, i32 count, u32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform4uiv(location, count, values);
    }
}

force_inline void
shader_set_uint2(Shader* shader, const char* uniformName, i32 count, u32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform2uiv(location, count, values);
    }
}

force_inline void
shader_set_uint3(Shader* shader, const char* uniformName, i32 count, u32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform3uiv(location, count, values);
    }
}

force_inline void
shader_set_uint4(Shader* shader, const char* uniformName, i32 count, u32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniform4uiv(location, count, values);
    }
}

force_inline void
shader_set_mat2(Shader* shader, const char* uniformName, i32 count, i8 transpose, f32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniformMatrix2fv(location, count, transpose, values);
    }
}

force_inline void
shader_set_m3(Shader* shader, const char* uniformName, i32 count, i8 transpose, f32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniformMatrix3fv(location, count, transpose, values);
    }
}

force_inline void
shader_set_m4(Shader* shader, const char* uniformName, i32 count, i8 transpose, f32* values)
{
    i32 location = shader_get_location(shader, uniformName);
    if (location >= 0)
    {
	glUniformMatrix4fv(location, count, transpose, values);
    }
}

#endif
