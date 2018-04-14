
#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float factor;

void main()
{
    FragColor = mix(texture(texture1, vec2(TexCoord.x / 2.0, TexCoord.y / 2.0)), texture(texture2, vec2(TexCoord.x, TexCoord.y)), factor);
//    FragColor = texture(texture1, TexCoord);
}
