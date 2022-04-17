#include "OpenGLBase.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <glad/glad.h>
#include <Utils/stb_image.h>

i32
opengl_context_init(GLADloadproc gladLoadProc)
{
    i32 openGLLoadStatus = gladLoadGLLoader(gladLoadProc);
    if (openGLLoadStatus == 0)
    {
	GERROR("Failed to init GLAD\n");
	return -1;
    }

    GINFO("OpenGL version %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(opengl_error_callback, 0);
    return 1;
}

void
opengl_error_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (type)
    {
    case GL_NO_ERROR: { break; }
    case GL_INVALID_ENUM:
    {
	GERROR("%s\n", message);
	GERROR("GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n");
	break;
    }
    case GL_INVALID_VALUE:
    {
	GERROR("%s\n", message);
	GERROR("GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n");
	break;
    }
    case GL_INVALID_OPERATION:
    {
	GERROR("%s\n", message);
	GERROR("GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n");
	break;
    }
    case GL_INVALID_FRAMEBUFFER_OPERATION:
    {
	GERROR("%s\n", message);
	GERROR("GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n");
	break;
    }
    case GL_OUT_OF_MEMORY:
    {
	GERROR("%s\n", message);
	GERROR("GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
	break;
    }
    case GL_STACK_UNDERFLOW:
    {
	GERROR("%s\n", message);
	GERROR("GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
	break;
    }
    case GL_STACK_OVERFLOW:
    {
	GWARNING("%s\n", message);
	GERROR("GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n");
	break;
    }
    }
}

// change this to (DataType types[SOME_COUNT], i32 count)
BufferLayout*
buffer_layout_create(DataType types[], i32 count)
{
    BufferLayout* result = NULL;
    array_reserve(result, count);
    for (i32 i = 0; i < count; ++i)
    {
	DataType type = types[i];
	BufferLayout layout = {
	    .IsNormalized = 0, //NOTE(bies): always !normalized
	    .Type = type,
	    .Size = data_type_get_size(type),
	    .Count = data_type_get_count(type)
	};

	result[i] = layout;
    }

    return result;
}
i32
buffer_layout_get_size(BufferLayout* layouts)
{
    i32 i, finalSize = 0, count = array_count(layouts);
    for (i = 0; i < count; ++i)
    {
	BufferLayout layout = layouts[i];
	finalSize += layout.Size;
    }
    return finalSize;
}

i32
vertex_buffer_get_current()
{
    u32 id;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &id);
    return id;
}
void
vertex_buffer_set_data(VertexBuffer* buffer, f32* data, u32 size)
{
    buffer->Vertices = data;
    glBindBuffer(GL_ARRAY_BUFFER, buffer->RendererID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
VertexBuffer
vertex_buffer_create(f32* vertices, u32 size)
{
    VertexBuffer buffer;
    glGenBuffers(1, &buffer.RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, buffer.RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    buffer.Layout = NULL;
    buffer.Vertices = vertices;
    return buffer;
}

VertexBuffer
vertex_buffer_create_allocated(u32 size)
{
    VertexBuffer allocatedBuffer = vertex_buffer_create(NULL, size);
    return allocatedBuffer;
}
force_inline void
_vertex_buffer_stride_update(VertexBuffer* buffer)
{
    buffer->Stride = 0;

    i32 i, offset = 0, layoutsCount = array_count(buffer->Layout);
    for (i = 0; i < layoutsCount; ++i)
    {
	BufferLayout* layout = &buffer->Layout[i];
	layout->Offset  = buffer->Stride;
	buffer->Stride += layout->Size;
    }
}
void
vertex_buffer_add_layout(VertexBuffer* buffer, BufferLayout* layout)
{
    buffer->Layout = layout;
    _vertex_buffer_stride_update(buffer);
}
void
vertex_buffer_bind(VertexBuffer* vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo->RendererID);
}
void
vertex_buffer_unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

i32
index_buffer_get_current()
{
    u32 id;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &id);
    return id;
}
IndexBuffer
index_buffer_create(u32* indices, u32 count)
{
    IndexBuffer buffer;
    buffer.Count = count;
    buffer.Indices = indices;
    glGenBuffers(1, &buffer.RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(u32), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    return buffer;
}
void
index_buffer_bind(IndexBuffer* buffer)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->RendererID);
}
void
index_buffer_unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

force_inline void
_vertex_array_add_vbo(VertexArray* vertexArray, VertexBuffer vbo)
{
    vertexArray->Vertex = vbo;

    glBindBuffer(GL_ARRAY_BUFFER, vertexArray->Vertex.RendererID);

    vertex_array_enable_layout(vertexArray);
}
force_inline void
_vertex_array_add_ibo(VertexArray* vertexArray, IndexBuffer ibo)
{
    vertexArray->Index = ibo;
    glBindVertexArray(vertexArray->RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo.RendererID);
}
i32
vertex_array_get_current()
{
    i32 id = 0;
    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &id);
    return id;
}
VertexArray
vertex_array_create(VertexBuffer vertexBuffer, IndexBuffer indexBuffer)
{
    VertexArray vertexArray;
    glCreateVertexArrays(1, &vertexArray.RendererID);
    glBindVertexArray(vertexArray.RendererID);

    _vertex_array_add_vbo(&vertexArray, vertexBuffer);
    _vertex_array_add_ibo(&vertexArray, indexBuffer);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vertexArray;
}
void
vertex_array_enable_layout(VertexArray* vertexArray)
{
    BufferLayout* bufferLayouts = vertexArray->Vertex.Layout;
    i32 i,
	stride = vertexArray->Vertex.Stride,
	layoutsCount = array_count(bufferLayouts);
    for (i = 0; i < layoutsCount; ++i)
    {
	BufferLayout layout = bufferLayouts[i];
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, layout.Count, GL_FLOAT, layout.IsNormalized, stride, (const void*)layout.Offset);
    }
}
void
vertex_array_disable_layout(VertexArray* vertexArray)
{
    i32 i, layoutsCount = array_count(vertexArray->Vertex.Layout);
    for (i = 0; i < layoutsCount; ++i)
    {
	glDisableVertexAttribArray(i);
    }
}
void
vertex_array_bind(VertexArray* vertexArray)
{
    glBindVertexArray(vertexArray->RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexArray->Vertex.RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexArray->Index.RendererID);
    vertex_array_enable_layout(vertexArray);
}
void
vertex_array_unbind(VertexArray* vertexArray)
{
    vertex_array_disable_layout(vertexArray);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //For some reason this auto unbind
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

force_inline GLint
framebuffer_format_to_opengl(FrameBufferType type)
{
    switch (type)
    {
    case FRAMEBUFFER_TYPE_RGBA8:
	return GL_RGBA;

    case FRAMEBUFFER_TYPE_RED_INTEGER:
	return GL_RED_INTEGER;

    case FRAMEBUFFER_TYPE_DEPTH24_STENCIL8:
	return GL_DEPTH24_STENCIL8;
    }

    vassert(0 && "Can't get a opengl format from format! Probably unsupported framebuffer type!");
    return 0;
}

force_inline GLenum
opengl_format_to_internal_format(GLenum format)
{
    switch (format)
    {
    case GL_RGBA:
	return GL_RGBA8;
    case GL_RED_INTEGER:
	return GL_R32I;
    case GL_DEPTH24_STENCIL8:
	return GL_DEPTH_STENCIL;
    }

    vassert(0 && "Can't get a enum from format! Probably unsupported format!");
    return 0;
}

force_inline GLenum
opengl_format_to_type(GLint format)
{
    switch (format)
    {
    case GL_RGBA:
	return GL_UNSIGNED_BYTE;
    case GL_RED_INTEGER:
	return GL_UNSIGNED_BYTE;
    case GL_DEPTH24_STENCIL8:
	return GL_UNSIGNED_INT_24_8;
    }

    vassert(0 && "Can't get a type from format! Probably unsupported format!");
    return 0;
}

struct { const char* Key; i32 Value; }* g_DebugTable = NULL;
#define REGISTER_FORMAT(f) shash_put((g_DebugTable), #f, (f))
#define GET_FORMAT(f)

force_inline void
framebuffer_attachments_validate(Framebuffer* framebuffer, FrameBufferType* attachmentTypes)
{
    i32 depthAttachmentsCount = 0;
    i32 colorAttachmentsCount = 0;
    i32 count = array_count(attachmentTypes);

    GERROR("count: %d\n", count);

    vassert(count >= 2 && "At least 2 attachment types needed!");
    framebuffer->ColorAttachmentTypes = NULL;
    framebuffer->ColorAttachments = NULL;
    array_reserve(framebuffer->ColorAttachments, count);

    //NOTE(bies): убрать отсюда
    shash_put(g_DebugTable, TO_STRING(GL_RGBA8), GL_RGBA8);
    shash_put(g_DebugTable, TO_STRING(GL_RED_INTEGER), GL_RED_INTEGER);
    shash_put(g_DebugTable, TO_STRING(GL_DEPTH24_STENCIL8), GL_DEPTH24_STENCIL8);

    //GERROR("%s: %d\n", TO_STRING(GL_RGBA8), shash_get((g_DebugTable), TO_STRING(GL_RGBA8)));
    //GERROR("%s: %d\n", TO_STRING(GL_RED_INTEGER), shash_get((g_DebugTable), TO_STRING(GL_RED_INTEGER)));
    //GERROR("%s: %d\n", TO_STRING(GL_DEPTH24_STENCIL8), shash_get((g_DebugTable), TO_STRING(GL_DEPTH24_STENCIL8)));

    for (i32 i = 0; i < count; i++)
    {
	FrameBufferType attachmentType = attachmentTypes[i];
	switch (attachmentType)
	{
	case FRAMEBUFFER_TYPE_DEPTH24_STENCIL8:
	    ++depthAttachmentsCount;
	    framebuffer->DepthAttachmentType = attachmentType;
	    //GERROR("DepthAttachmentType: %d\n", attachmentType);
	    break;
	default:
	    ++colorAttachmentsCount;
	    array_push(framebuffer->ColorAttachmentTypes, attachmentType);
	    array_push(framebuffer->ColorAttachments, 0);
	    //GERROR("ColorAttachmentsTypes: %d\n", attachmentType);
	    break;
	}
    }

    vassert(depthAttachmentsCount == 1 && "Only 1 depth buffer attachment needed for Framebuffer!");
    vassert(colorAttachmentsCount >= 1 && "At least 1 color attachment needed for Framebuffer");
}

void
framebuffer_create(Framebuffer* framebuffer, u32 width, u32 height, FrameBufferType* attachments)
{
    framebuffer_attachments_validate(framebuffer, attachments);

    framebuffer_invalidate(framebuffer, width, height);
}

void
framebuffer_invalidate(Framebuffer* framebuffer, u32 width, u32 height)
{
    vassert(framebuffer->ColorAttachmentTypes && "You need framebuffer attachments if you want framebuffer to work!");

    if (framebuffer->RendererId)
    {
	framebuffer_destroy(framebuffer);
    }

    //Color attachment
    {
	glCreateTextures(GL_TEXTURE_2D, 1, framebuffer->ColorAttachments);
	glBindTexture(GL_TEXTURE_2D, framebuffer->ColorAttachments[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);
    }

    {
	//glCreateTextures(GL_TEXTURE_2D, 1, &framebuffer->DepthAttachment);
	glGenRenderbuffers(1, &framebuffer->DepthAttachment);
	glBindRenderbuffer(GL_RENDERBUFFER, framebuffer->DepthAttachment);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindTexture(GL_RENDERBUFFER, 0);
    }

    glCreateFramebuffers(1, &framebuffer->RendererId);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->RendererId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffer->ColorAttachments[0], 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebuffer->DepthAttachment);

    vassert(framebuffer->DepthAttachment > 0 && "DepthAttachment texture problem!");
    GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    vassert(framebufferStatus == GL_FRAMEBUFFER_COMPLETE && "Failed in framebuffer creation !!! :(");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    //glDrawBuffers(1, buffers);


    framebuffer->Width = width;
    framebuffer->Height = height;
}

i32
framebuffer_read_pixel(Framebuffer* framebuffer, i32 attachment, i32 x, i32 y)
{
    i32 pixelData;
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
    return pixelData;
}

void
framebuffer_read_pixel_color(Framebuffer* framebuffer, i32 attachment, i32 x, i32 y, v4 result)
{
    u8 data[4];
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
    // argb
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    result.X = data[0] / 255.0f;
    result.Y = data[1] / 255.0f;
    result.Z = data[2] / 255.0f;
    result.W = data[3] / 255.0f;
}

void
framebuffer_bind(Framebuffer* framebuffer)
{
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->RendererId);
    glViewport(0, 0, framebuffer->Width, framebuffer->Height);
}

void
framebuffer_unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void
framebuffer_destroy(Framebuffer* framebuffer)
{
    GWARNING("Destroy framebuffer!\n");

    glDeleteFramebuffers(1, &framebuffer->RendererId);
    glDeleteTextures(1, framebuffer->ColorAttachments);
    glDeleteTextures(1, &framebuffer->DepthAttachment);
    array_clear(framebuffer->ColorAttachments);
    framebuffer->DepthAttachment = 0;
}

Texture2D
texture2d_create_from_buffer(void* data, u32 width, u32 height, u8 channels)
{
    GLenum dataFormat, internalFormat;
    Texture2D texture;

    //Enable blending
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (channels == 3)
    {
	dataFormat = GL_RGB;
	internalFormat = GL_RGB;
    }
    else if (channels == 4)
    {
	dataFormat = GL_RGBA;
	internalFormat = GL_RGBA;
    }
    else
    {
	vassert(0 && "Channels error!");
    }

    //GINFO("Format c3: %d %d\n", GL_RGB, GL_RGB8);
    //GINFO("Format c4: %d %d\n", GL_RGBA, GL_RGBA8);

    texture.Width = width;
    texture.Height = height;
    texture.Channels = channels;
    texture.DataFormat = dataFormat;
    texture.Slot = -1;
    //GDEBUG("Channels: %d\n", channels);

    glCreateTextures(GL_TEXTURE_2D, 1, &texture.ID);
    glBindTexture(GL_TEXTURE_2D, texture.ID);
    //glTextureStorage2D(texture.ID, 1, internalFormat, texture.Width, texture.Height);

    glTextureParameteri(texture.ID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(texture.ID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // x clamp to edge
    glTextureParameteri(texture.ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE/*GL_REPEAT*/);
    // y clamp to edge
    glTextureParameteri(texture.ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE/*GL_REPEAT*/);

    //glTextureSubImage2D(texture.ID, 0, 0, 0, texture.Width, texture.Height, dataFormat, GL_UNSIGNED_BYTE, data);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);


    return texture;
}

Texture2D
texture2d_create(const char* path)
{
    i32 width, height, channels;
    GLenum dataFormat, internalFormat;

    stbi_set_flip_vertically_on_load(1);

    // passing 4 instead of 0 for forcing 4 channels
    stbi_uc* data = stbi_load(path, &width, &height, &channels, 0);
    if (data == NULL)
    {
	assert(0 && "Failed to load a texture!");
    }
    Texture2D texture = texture2d_create_from_buffer(data, width, height, channels);
    texture.Path = path;

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void
texture2d_set_data(Texture2D* texture, void* data)
{
    glBindTextureUnit(texture->Slot, texture->ID);
    glTextureSubImage2D(texture->ID, 0, 0, 0, texture->Width, texture->Height, texture->DataFormat, GL_UNSIGNED_BYTE, data);
}

void
texture2d_bind(Texture2D* texture, u32 slot)
{
    texture->Slot = slot;
    //glBindTexture(GL_TEXTURE0 + slot, texture->ID);
    glBindTextureUnit(slot, texture->ID);
}

void
texture2d_unbind(Texture2D* texture)
{
    //glBindTexture(GL_TEXTURE0 + texture->Slot, 0);
    glBindTextureUnit(texture->Slot, 0);
}

#define NEW 1
void
texture2d_bind_index(u32 id, u32 slot)
{
    //4.5
#if NEW == 1
    glBindTextureUnit(slot, id);
#else
    //old
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, id);
#endif
}
void
texture2d_unbind_index(u32 id, u32 slot)
{
#if NEW == 1
    glBindTextureUnit(slot, id);
#else
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
#endif
}

void
texture2d_delete(Texture2D* texture)
{
    glDeleteTextures(1, &texture->ID);
}

void
texture2d_destroy(Texture2D texture)
{
    texture2d_delete(&texture);
}

void
texture_atlas_create(TextureAtlas* atlas, const char* path, v2 atlasSize, v2 textureSize)
{
    atlas->AtlasWidth    = atlasSize.X;
    atlas->AtlasHeight   = atlasSize.Y;
    atlas->TextureWidth  = textureSize.X;
    atlas->TextureHeight = textureSize.Y;
    atlas->Texture = texture2d_create(path);
}

Shader* g_Shaders = NULL;

ShaderSource
shader_load(const char* shader_path)
{
    i32 vertex_index;
    i32 fragment_index;
    i32 vertex_keyword_length;
    i32 fragment_keyword_length;
    char* shader_source;
    char* shader_name;
    const char* vertex_shader_source;
    const char* fragment_shader_source;
    ShaderSource source = {};

    shader_source = file_read_string(shader_path);
    shader_name = file_get_name_with_extension(shader_path);
    //GDEBUG(CYAN("shader source: %s\n"), shader_source);
    if (shader_source == NULL)
    {
	GERROR("shader file %s open error!\n", shader_path);
	vassert(0 && "shader file");
	return (ShaderSource) { };
    }

    vertex_index = string_index_of_string(shader_source, "#vertex shader");
    fragment_index = string_index_of_string(shader_source, "#fragment shader");
    vertex_keyword_length = string_length("#vertex shader");
    fragment_keyword_length = string_length("#fragment shader");

    vertex_shader_source = string_substring_range(shader_source, vertex_index + vertex_keyword_length + 1, (fragment_index - 1));
    fragment_shader_source = string_substring(shader_source, fragment_index + fragment_keyword_length + 1);

    GDEBUG("vertex shader:\n"YELLOW("%s\n"), vertex_shader_source);
    GDEBUG("fragment shader:\n"GREEN("%s\n"), fragment_shader_source);

    source.name = shader_name;
    source.vertex_shader   = (char*) vertex_shader_source;
    source.fragment_shader = (char*) fragment_shader_source;

    if (shader_source)
    {
	memory_free(shader_source);
    }

    return source;
}

static void
shader_error_check(u32 shaderID)
{
    i32 isCompiled = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled != GL_FALSE)
    {
	return;
    }

    GLint maxLength = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
    if (maxLength <= 0)
    {
	return;
    }
    char* message = memory_allocate(maxLength * sizeof(char));

    char errorMessage[1024];
    glGetShaderInfoLog(shaderID, maxLength, &maxLength, message);


    GERROR("shader_error_check[is_compiled: %d, log_length:%d]:\n        %s\n", isCompiled, maxLength, message);
    memory_free(message);
    GERROR("\n\nShader compilation ERROR!!!\n\n");
    vassert(0 && "Shader compilation error!!!");
    glDeleteShader(shaderID);
}

Shader
shader_compile(ShaderSource source)
{
    Shader shader = {};

    u32 vertex_shader_id;
    u32 fragment_shader_id;
    u32 shader_program_id;

    if (source.vertex_shader == NULL || source.fragment_shader == NULL || string_length(source.vertex_shader) < 0 || string_length(source.fragment_shader) < 0)
    {
	vassert(0 && "Shader source is not loaded correctly!");
	return (Shader) { .ShaderID = -1, .UniformTable = NULL };
    }

    //GDEBUG(GREEN("Compiling")" vertex shader\n");
    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader_id, 1, &source.vertex_shader, 0);
    glCompileShader(vertex_shader_id);
    shader_error_check(vertex_shader_id);

    //GDEBUG(GREEN("Compiling")" fragment shader\n");
    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader_id, 1, &source.fragment_shader, 0);
    glCompileShader(fragment_shader_id);

    shader_error_check(fragment_shader_id);

    // GDEBUG(GREEN("Linking")" program\n");
    shader_program_id = glCreateProgram();
    // GDEBUG("SHADER ID: %d\n", shader_program_id);
    glAttachShader(shader_program_id, vertex_shader_id);
    glAttachShader(shader_program_id, fragment_shader_id);
    glLinkProgram(shader_program_id);
    shader_error_check(shader_program_id);

    glDetachShader(shader_program_id, vertex_shader_id);
    glDetachShader(shader_program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    array_push(g_Shaders, shader);

    return (Shader) { .ShaderID = shader_program_id, .UniformTable = NULL };
}

Shader
shader_create(const char* shaderPath)
{
    ShaderSource sources = shader_load(shaderPath);
    Shader shader = shader_compile(sources);
    return shader;
}

Shader
shader_compile_safe(const char* shaderPath)
{
    ShaderSource shaderSource = shader_load(shaderPath);
    vassert(shaderSource.name);
    Shader shader = shader_compile(shaderSource);
    shader_unbind();
    return shader;
}

void
shader_delete(Shader* shader)
{
    glDeleteProgram(shader->ShaderID);
    shash_free(shader->UniformTable);
}

void
shader_bind(Shader* shader)
{
    glUseProgram(shader->ShaderID);
}

void
shader_unbind()
{
    glUseProgram(0);
}

void
shader_delete_collection()
{
    // GDEBUG(GREEN("Begin: delete collection of shaders\n"));

    if (g_Shaders == NULL)
    {
	return;
    }

    Shader shader;
    i32 i, count = array_count(g_Shaders);
    for (i = 0; i < count; i++)
    {
	shader = g_Shaders[i];
	shader_delete(&shader);
    }

    // GDEBUG(GREEN("End: delete collection of shaders\n"));
}
