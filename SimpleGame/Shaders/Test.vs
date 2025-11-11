#version 330

in vec3 a_Position; 
in float a_Radius;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time;
const float c_PI = 3.141592;


void main()
{
	float value = fract(u_Time) * 2 - 1; // -1~1	// fract는 0~1 사이를 반복하게 만듬
    float rad = (value + 1) * c_PI; // 각도로 변환 -> 0~2PI
	float y = a_Radius * sin(rad);  // y 좌표 sin함수 이용해서 계산..
    float x = a_Radius * cos(rad); // x 좌표 cos함수 이용해서 계산..

	vec4 newPosition = vec4(a_Position, 1);
	newPosition.xy = newPosition.xy + vec2(x,y);	
	gl_Position = newPosition;

	v_Color = a_Color;
}
