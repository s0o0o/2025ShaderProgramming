#version 330

layout(location=0) out vec4 FragColor;
in vec2 v_UV;

uniform sampler2D u_RGBTexture;
uniform float u_Time;

const float c_PI = 3.141592;

void Test()
{
    vec2 newUV = v_UV;
    float dx = 0.1 * sin(v_UV.y * 2 * c_PI * 3 + u_Time);
    float dy = 0.1 * sin(v_UV.x * 2 * c_PI * 3 + u_Time);
    newUV += vec2(dx, dy);
    vec4 sampledColor = texture(u_RGBTexture, newUV);
    FragColor =sampledColor;
}

void Circle()
{ 
    vec2 newUV = v_UV;  // 0~1, 0,0 left top
    vec2 center = vec2(0.5, 0.5);
    vec4 newColor = vec4(0);
    float d = distance (newUV, center);
    float value = sin(d*c_PI*10 - u_Time*5);
    newColor = vec4(value);
    FragColor = newColor;
}

void Flag()
{ 
    vec2 newUV = vec2(v_UV.x , 1-v_UV.y - 0.5);  // 0~1, 0,0 left bottom
    vec4 newColor = vec4(0);

    float width = 0.2 * (1-newUV.x);
    float sinValue = newUV.x*0.2*sin(newUV.x * c_PI * 2 - u_Time*4);

    if(newUV.y < sinValue + width && newUV.y >sinValue - width)
    {
        newColor = vec4(1);
    }
    else {
     discard;
    }
    FragColor = newColor;
}

void Q1()
{
    vec2 newUV = vec2(v_UV.x,v_UV.y);  // 0~1, 0,0 left bottom
    float x = newUV.x;
    float y = 1 - abs(2*(v_UV.y - 0.5));     // 0~1~0
    vec4 newColor = texture(u_RGBTexture, vec2(x, y));

    FragColor = newColor;
}

void Q2()
{
    vec2 newUV = vec2(v_UV.x,v_UV.y);  // 0~1, 0,0 left bottom
    float x = fract(newUV.x*3);
    float y = (2-floor(newUV.x*3))/3 + newUV.y/3;     // 0~1~0
    vec4 newColor = texture(u_RGBTexture,  vec2(x, y));
    FragColor = newColor;
}

void Q3()
{
    vec2 newUV = vec2(v_UV.x,v_UV.y);  // 0~1, 0,0 left bottom
    float x = fract(newUV.x*3);
    float y = (3+floor(newUV.x*3))/3 + newUV.y/3;     // 0~1~0
    vec4 newColor = texture(u_RGBTexture,  vec2(x, y));
    FragColor = newColor;
}



void main()
{
    //Test();
    //Circle();
    //Flag();
    //Q1();
    //Q2();
    Q3();


}
