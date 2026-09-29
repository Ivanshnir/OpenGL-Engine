#version 330 core
out vec4 FragColor;  
in vec2 textCoord;
in float colorPick;
uniform sampler2D texture1;
uniform sampler2D texture2;
void main()
{
    FragColor = mix(texture(texture1,textCoord), texture(texture2, textCoord), 0.2)*vec4(colorPick, 1.0f, 1.0f, 1.0f);
}