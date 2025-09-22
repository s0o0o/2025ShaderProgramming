#version 330

in vec3 a_Position;
in float a_Radius;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time;
const float c_PI = 3.141592;

void main()
{
	float value = fract(u_Time) * 2 - 1; // -1~1
	float rad = (value + 1)*c_PI;
	float y = a_Radius*sin(rad);
	float x = a_Radius*-cos(rad);

	vec4 newPosition = vec4(a_Position, 1);
	newPosition.xy = newPosition.xy + fract(u_Time/2)*vec2(x,y);	// 이러면 나선형
	gl_Position = newPosition;

	v_Color = a_Color;
}
