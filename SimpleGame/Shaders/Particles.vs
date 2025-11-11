#version 330

in vec3 a_Position;
in float a_Value;
in float a_Period;	// 0930
in vec4 a_Color;
in float a_STime;
in vec3 a_Vel;
in float a_Mass;		// f = m*a

in float a_LifeTime;

out vec4 v_Color;

uniform float u_Time;
uniform vec3 u_Force;

const float c_PI = 3.141592;
const vec2 c_G = vec2(1, -9.8); // 0923

void raining()
{
	float lifeTime = a_LifeTime;
	float particleTime = 1.0;
	float newAlpha = 1.0;
    float newTime = u_Time - a_STime;  // newTime은 생성된 후 얼마나 지났는지 나타내게됨(즉..나이)
	vec4 newPosition = vec4(a_Position, 1);

    if (newTime > 0)
    {	// (파티클이 살아있을 때의 로직)
		float t = fract(newTime / lifeTime)*lifeTime;		// t는 0 ~ lifeTime 왓다리갓다리
		float tt = t * t;
		float forceX = u_Force.x * 10 + c_G.x*a_Mass;	// 알짜힘 (x축기준 -1~1 진동하는 운동함)
		float forceY = u_Force.y + c_G.y*a_Mass;		// 알짜힘 어차피 c_G.y는 0으로 cpp에서 설정해둠

		float aX = forceX / a_Mass;
		float aY = forceY / a_Mass;

		float x = a_Vel.x * t + 0.5 * aX * tt;				
		float y = a_Vel.y * t + 0.5 * aY * tt; 

		newPosition.xy += vec2(x,y);	
		newAlpha = 1 - t/lifeTime;
	}
	else{
		newPosition.xy = vec2(-100000,0);	
	}
	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, newAlpha);
}

// 색깔 보간해보는거 mix
void sinParticle()
{
	vec4 centerC = vec4(1,0,0,1);
	vec4 borderC = vec4(1,1,1,0);

	//mix(centerC, borderC, 0.5);

	float newTime = u_Time - a_STime;	// 파티클 나이
	float lifeTime = a_LifeTime;
	float newAlpha = 1.0;
	vec4 newPosition = vec4(a_Position, 1);

	vec4 newColor;

	if(newTime > 0)
	{
		float period = a_Period;
		float t = fract(newTime / lifeTime)*lifeTime;	// 생명만큼.. 0-lifeTime
		//float t = newTime;
		float tt = t*t;
        float x = 2 * t - 1; // 파티클 왼쪽에서 오른쪽으로..t가 (0 -> lifeTime)으로 변할 때
		float y = t*sin(2 * t * c_PI * period) * ((a_Value-0.5) * 2);	// t를 곱할수록 진동 폭 커짐 
		y *= sin(fract(newTime / lifeTime)*c_PI);	// 진동 움직임 부드럽게 해주는..

		newPosition.xy += vec2(x,y);
		newAlpha = 1 - t/lifeTime;

		newColor = mix(centerC, borderC, abs(y*4));
    }
	else
	{
		newPosition.xy = vec2(-100000,0);	
	}
	
	gl_Position = newPosition;
	v_Color = vec4(newColor.rgb, newAlpha);
}

// 반주기만? 나오게 하는것.. 색이 끝으로 갈수록 연해지고... sin 그래프 반만나오게하는(커져삳가 다시 작아지는 한주기만 그림)
void sinParticle2()
{
    float newTime = u_Time - a_STime;
    float lifeTime = a_LifeTime;
    float newAlpha = 1.0;
    vec4 newPosition = vec4(a_Position, 1);
    vec4 newColor;
    if (newTime > 0)
    {
        float period = a_Period;
        float t = fract(newTime / lifeTime) * lifeTime;
      //float t = newTime;
        float tt = t * t;
        float x = 2 * t - 1;
        float y = t * sin(2 * t * c_PI * period) * ((a_Value - 0.5) * 2);
        y *= sin(fract(newTime / lifeTime) * c_PI);

        newPosition.xy += vec2(x, y);
        newAlpha = 1 - t / lifeTime;
		
        newColor = mix(vec4(1, 0, 0, 1), vec4(1, 1, 1, 0), abs(y * 4));

    }
    else
    {
        newPosition.xy = vec2(-100000, 0);
    }
    gl_Position = newPosition;
    v_Color = vec4(newColor.rgb, newAlpha);
}


void circleParticle()
{
	float newTime = u_Time - a_STime;	// 파티클 나이
	float lifeTime = a_LifeTime;
	float newAlpha = 1.0;
	vec4 newPosition = vec4(a_Position, 1);
	
    if (newTime > 0)	// newTime나이에 존재하는애들만
	{
        float t = fract(newTime / lifeTime) * lifeTime; // (0 ~ lifeTime) 반복 타이머
		float tt = t*t;

		float x = cos(2*c_PI*a_Value); // 여기서 value 값은 무작위 각도 결정..
		float y = sin(2*c_PI*a_Value);
		
		float newX = x + 0.5*c_G.x*tt;	// 기본 위치 + 1/2*중력 가속도*tt
		float newY = y + 0.5*c_G.y*tt;	
		// cg.x는 1이라서 무의미고.. cg.y에는 -9.8 존재 -> 밑으로 하락

		newPosition.xy += vec2(newX,newY);

		newAlpha = 1 - t/lifeTime;	// 떨어지면서 투명해진다
	}
	else
	{
		newPosition.xy = vec2(-100000,0);	
	}
	
	gl_Position = newPosition;
	v_Color = vec4(a_Color.rgb, newAlpha);
}



void main()
{
	//raining();
	sinParticle2();
	//circleParticle();
}
