#vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

out vec4 vColor;
out vec2 vTexCoord;
flat out float vTexIndex;

uniform mat4 uProj;

void main(){
   gl_Position = uProj * position;
   vColor = aColor;
   vTexCoord = texCoord;
   vTexIndex = texIndex;
};

#fragment
#version 330 core

out vec4 color;

in vec4 vColor;
in vec2 vTexCoord;
flat in float vTexIndex;

uniform vec4 uColor;
uniform sampler2D uTexture[32];

void main(){
    int index = int(vTexIndex);
    vec4 texColor = texture(uTexture[index], vTexCoord);
    color = vColor * uColor * texColor;
};