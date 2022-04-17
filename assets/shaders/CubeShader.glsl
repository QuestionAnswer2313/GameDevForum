#vertex shader
#version 450 core
layout(location = 0) in vec3 i_Position;
layout(location = 1) in vec2 i_TextureCoordinates;
layout(location = 2) in float i_TextureID;

uniform mat4 u_ViewProjection;

out vec4 o_Color;
out vec2 o_TextureCoordinates;
out float o_TextureID;

void main()
{
    gl_Position = u_ViewProjection * vec4(i_Position, 1.0);
    o_TextureCoordinates = i_TextureCoordinates;
    o_TextureID = i_TextureID;
}

#fragment shader
#version 450 core
layout(location = 0) out vec4 Color;
layout(binding = 0) uniform sampler2D u_Textures[32];

in vec2 o_TextureCoordinates;
in float o_TextureID;

void main()
{
    int textId = int(o_TextureID);
    //Color = vec4(vec3(gl_FragCoord.z), 1.0);
    Color = vec4(texture(u_Textures[textId], o_TextureCoordinates).rgb, 1.0);
}
