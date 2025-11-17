#version 330

layout(location=0) out vec4 FragColor;
in vec2 v_UV;

const float c_PI = 3.141592;

void main()
{
    vec4 newColor = vec4(0);
    float xValue = 1-pow(abs(sin(v_UV.x *2*c_PI*4)),0.5);
    newColor = vec4(xValue);
    FragColor = newColor;
}
