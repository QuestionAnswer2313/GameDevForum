#include "Renderer.h"

/* Base Renderer */
void
renderer_set_viewport(u32 width, u32 height)
{
    glViewport(0, 0, width, height);
}

void
renderer_clear(v4 color)
{
    glClearColor(color.R, color.G, color.B, color.A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void
renderer_enable_wireframe_mode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void
renderer_enable_fill_mode()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void
renderer_enable_face_culling()
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    ////Порядок обхода граней ClockWise
    glFrontFace(GL_CW);
}

void
renderer_enable_blending()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
renderer_disable_face_culling()
{
    glDisable(GL_CULL_FACE);
}

void
renderer_enable_depth_testing()
{
    glEnable(GL_DEPTH_TEST);
}

void
renderer_disable_depth_testing()
{
    glDisable(GL_DEPTH_TEST);
}

void
renderer_disable_blending()
{
    glDisable(GL_BLEND);
}

/* 3D Renderer */

#define CubeVerticeCount 24
#define DefaultCubeIndicesCount 36
#define DefaultMaxCubes 100

static i32 CubeVerticesSize = 0;

static const f32 vp = 0.5f;
static const f32 dv = 2 * vp;
static v4 DefaultCubePositions[24] = {
    // Front
    //0              1              2                3
    {-vp,vp,vp,dv}, {vp,vp,vp,dv}, {vp,-vp,vp,dv}, {-vp,-vp,vp,dv},

    // Back
    //4               5                6                 7
    {-vp,vp,-vp,dv}, {vp, vp,-vp,dv}, {vp,-vp,-vp,dv}, {-vp,-vp,-vp,dv},

    // Left
    //8               9                10                11
    {-vp,vp,vp,dv}, {-vp,vp,-vp,dv}, {-vp,-vp,-vp,dv}, {-vp,-vp,vp,dv},

    // Right
    //12             13              14               15
    { vp,vp,vp,dv}, {vp,vp,-vp,dv}, {vp,-vp,-vp,dv}, {vp,-vp,vp,dv},

    // Bottom
    //16               17               18               19
    {-vp,-vp,-vp,dv}, {vp,-vp,-vp,dv}, {vp,-vp,vp,dv}, {-vp,-vp,vp,dv},

    // Up
    //20              21              22              23
    {-vp,vp,-vp,dv}, {vp,vp,-vp,dv}, {vp,vp,vp,dv}, {-vp,vp,vp,dv}
};

static v2 CubeUV[24] = {
    // Front
    //0         1         2         3
    { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 },
    // Back
    //4         5         6         7
    { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 },
    // Left
    //8         9         10        11
    { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 },
    // Right
    //12        13        14        15
    { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 },
    // Bottom
    //16        17        18        19
    { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 },
    // Up
    //20        21        22        23
    { 0, 1 }, { 1, 1 }, { 1, 0 }, { 0, 0 }
};

static f32* Render3DDataBuffer;

typedef struct RenderableCube
{
    v3 Position[CubeVerticeCount];
    v2 UV[CubeVerticeCount];
    i32 TextureID[CubeVerticeCount];
} RenderableCube;

typedef struct CubeRenderer
{
    Shader Shader;
    Camera* Camera;
    VertexArray VAO;
    RenderableCube* Data;
    TextureList List;
    u32 NextID;
} CubeRenderer;

static CubeRenderer CubeRendererData;

#define CubeRenderer(shader, camera, vao, data, list, nextId)		\
    ({									\
	(CubeRenderer) { .Shader = shader, .Camera = camera, .VAO = vao, .Data = data, .List = list, .NextID = nextId }; \
    })

force_inline u32*
_create_cube_indices_array(u32 count)
{
    i32 i;
    u32 temp, *in = memory_allocate(count * sizeof(u32));
    for (i = 0, temp = 0; i < count; i += 36, temp += 24)
    {
	// Front - CW
	in[i]     = 0 + temp;
	in[i + 1] = 1 + temp;
	in[i + 2] = 3 + temp;
	in[i + 3] = 3 + temp;
	in[i + 4] = 1 + temp;
	in[i + 5] = 2 + temp;

	// Back - CCW
	in[i + 6]  = 4 + temp;
	in[i + 7]  = 7 + temp;
	in[i + 8]  = 5 + temp;
	in[i + 9]  = 5 + temp;
	in[i + 10] = 7 + temp;
	in[i + 11] = 6 + temp;

	// LEFT - CCW
	in[i + 12] = 8  + temp;
	in[i + 13] = 11 + temp;
	in[i + 14] = 9  + temp;
	in[i + 15] = 9  + temp;
	in[i + 16] = 11 + temp;
	in[i + 17] = 10 + temp;

	// RIGHT - CW
	in[i + 18] = 12 + temp;
	in[i + 19] = 13 + temp;
	in[i + 20] = 15 + temp;
	in[i + 21] = 15 + temp;
	in[i + 22] = 13 + temp;
	in[i + 23] = 14 + temp;

	// BOTTOM - CCW
	in[i + 24] = 16 + temp;
	in[i + 25] = 19 + temp;
	in[i + 26] = 17 + temp;
	in[i + 27] = 17 + temp;
	in[i + 28] = 19 + temp;
	in[i + 29] = 18 + temp;

	// UP    - CW
	in[i + 30] = 20 + temp;
	in[i + 31] = 21 + temp;
	in[i + 32] = 23 + temp;
	in[i + 33] = 23 + temp;
	in[i + 34] = 21 + temp;
	in[i + 35] = 22 + temp;
    }
    return in;
}

void
renderer3d_init_cubes(Camera* camera)
{
    DataType types[] = { Float3, Float2, Float1 };
    BufferLayout* bufferLayout = buffer_layout_create(types, 3);
    i32 layoutSize = buffer_layout_get_size(bufferLayout);
    {
	vassert(layoutSize > 0 && "[Cube] Buffer layout size error!!!");
	GINFO("layoutSize: %d\n", layoutSize);
    }
    i32 verticesCount = DefaultMaxCubes * (CubeVerticeCount * layoutSize);
    CubeVerticesSize = verticesCount * sizeof(f32);
    GERROR("CubeVerticesSize: %d\n", CubeVerticesSize);
    Render3DDataBuffer = (f32*) memory_allocate(CubeVerticesSize);
    memset(Render3DDataBuffer, '\0', CubeVerticesSize);

    const i32 localIndicesCount  = DefaultMaxCubes * DefaultCubeIndicesCount;
    u32* indices = _create_cube_indices_array(localIndicesCount);
    Shader shader = shader_compile_safe(asset_shader("CubeShader.glsl"));
    VertexBuffer vbo = vertex_buffer_create_allocated(CubeVerticesSize);
    vertex_buffer_add_layout(&vbo, bufferLayout);
    IndexBuffer ibo = index_buffer_create(indices, localIndicesCount);
    VertexArray vao = vertex_array_create(vbo, ibo);
    vertex_array_unbind(&vao);

    RenderableCube* data = NULL;
    array_reserve(data, DefaultMaxCubes);
    array_header(data)->Count = 0;

    CubeRendererData = CubeRenderer(shader, camera, vao, data, texture_list_create(), 0);
}

void
renderer3d_init(Camera* camera)
{
    renderer3d_init_cubes(camera);
}

i32
_renderer3d_submit_texture(Texture2D texture)
{
    i32 textureID = texture_list_submit_texture_or_flush(&CubeRendererData.List, texture.ID);
    return textureID;
}

void
renderer3d_submit_cube(m4 transform,
		       Texture2D front, Texture2D back,
		       Texture2D left, Texture2D right,
		       Texture2D bottom, Texture2D up)
{
    i32 frontID = _renderer3d_submit_texture(front);
    i32 backID = _renderer3d_submit_texture(back);
    i32 leftID = _renderer3d_submit_texture(left);
    i32 rightID = _renderer3d_submit_texture(right);
    i32 bottomID = _renderer3d_submit_texture(bottom);
    i32 upID = _renderer3d_submit_texture(up);

    RenderableCube cube;
    u32 ids[6] = { frontID, backID, leftID, rightID, bottomID, upID };
    for (i32 i = 0; i < CubeVerticeCount; ++i)
    {
	//3
	cube.Position[i]  = v3_v4(m4_mul_v4(transform, DefaultCubePositions[i]));
	//2
	cube.UV[i]        = CubeUV[i];
	//1
	i32 id = 0;
	if (i < 4)
	{
	    id = 0;
	}
	else if (i >= 4 && i < 8)
	{
	    id = 1;
	}
	else if (i >= 8 && i < 12)
	{
	    id = 2;
	}
	else if (i >= 12 && i < 16)
	{
	    id = 3;
	}
	else if (i >= 16 && i < 20)
	{
	    id = 4;
	}
	else if (i >= 20 && i < 24)
	{
	    id = 5;
	}
	cube.TextureID[i] = ids[id];
    }

    array_insert(CubeRendererData.Data,
		 {
		     renderer3d_flush_cubes();
		 }, cube);
}

void
renderable_cubes_to_array(RenderableCube* cubes, f32* destination)
{
    i32 i, ind = 0, count = array_count(cubes);
    for (i = 0; i < count; ++i)
    {
	RenderableCube cube = cubes[i];
	i32 textureIDShouldBe;
	for (i32 c = 0; c < CubeVerticeCount; ++c)
	{
	    destination[ind + 0] = cube.Position[c].X;
	    destination[ind + 1] = cube.Position[c].Y;
	    destination[ind + 2] = cube.Position[c].Z;
	    destination[ind + 3] = cube.UV[c].X;
	    destination[ind + 4] = cube.UV[c].Y;
	    destination[ind + 5] = cube.TextureID[c];

	    ind += 6;
	}
    }
}

void
renderer3d_submit_cube_position(v3 position,
				Texture2D front, Texture2D back,
				Texture2D left, Texture2D right,
				Texture2D bottom, Texture2D up)
{
    m4 transform = m4_transform(position, v3_new(1, 1, 1), v3_new(0, 0, 0));
    renderer3d_submit_cube(transform, front, back, left, right, bottom, up);
}

void
renderer3d_flush_cubes()
{
    renderer_enable_depth_testing();

    renderable_cubes_to_array(CubeRendererData.Data, Render3DDataBuffer);

    shader_bind(&CubeRendererData.Shader);
    vertex_array_bind(&CubeRendererData.VAO);
    texture_list_bind(&CubeRendererData.List);

    shader_set_m4(&CubeRendererData.Shader, "u_ViewProjection", 1, 0, &CubeRendererData.Camera->ViewProjection.M[0][0]);

    i32 count = array_count(CubeRendererData.Data);
    vertex_buffer_set_data(&CubeRendererData.VAO.Vertex, Render3DDataBuffer, count*sizeof(RenderableCube));
    glDrawElements(GL_TRIANGLES,
		   count * DefaultCubeIndicesCount,
		   GL_UNSIGNED_INT, NULL);

    array_clear(CubeRendererData.Data);
    shader_unbind(CubeRendererData.Shader);
    vertex_array_unbind(CubeRendererData.VAO);
    texture_list_unbind(&CubeRendererData.List);
    array_clear(CubeRendererData.Data);

}

/* Flush functions */
void
renderer3d_flush()
{
    renderer3d_flush_cubes();
}
