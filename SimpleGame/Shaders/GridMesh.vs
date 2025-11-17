#version 330
#define MAX_POINTS 100

in vec3 a_Position;

out vec4 v_Color;

uniform float u_Time;
const float c_PI = 3.141592;

uniform vec4 u_Points[MAX_POINTS];

//const vec4 c_Points[MAX_POINTS] = vec4[](vec4(0,0,2,3), vec4(0.5,0,3,4),vec4(-0.5,0,4,5));

void exam1()    // 세로줄 출력
{
    vec4 newPosition = vec4(a_Position, 1);
    float valueX = newPosition.x + 0.5;
    float greyScale = sin(2 * c_PI * valueX * 4);
    v_Color = vec4(greyScale);
    v_Color.a = 1.0;
    gl_Position = newPosition;

}

//void exam2() // 다이아몬트 형태로 출력하는거....
//{
//    vec4 newPosition = vec4(a_Position, 1);
//    float valueX = newPosition.x + 0.5;
//    float valueY = newPosition.y + 0.5;
//    float greyScale = sin(2 * c_PI * valueX * 4);
//    greyScale += sin(2 * c_PI * valueY * 4);
//    v_Color = vec4(greyScale);
//    v_Color.a = 1.0;
//    gl_Position = newPosition;
//}

//void exam3()    // 싸인함수 출력
//{
//    vec4 newPosition = vec4(a_Position, 1);
//    newPosition.x += 2 * (a_Value - 0.5);
//    newPosition.y += sin(2 * (a_Value - 0.5)*c_PI);

//}  

//void exam4()    // 원형 출력
//{
    
//}

//void exam5()    // 원형 출력 + x축으로 커짐
//{
    
//}

//void exam6()    // 소용돌이 + 빙글빙글돔
//{
    
//}

void Flag() // 1014
{
    vec4 newPosition = vec4(a_Position, 1);

    float value = (a_Position.x + 1.0) / 2.0;   // 0~1사이..

    newPosition.y = newPosition.y * (1 - value);
    // value는 0~1 사이
    // 깃발의 왼쪽 끝은 value = 0 -> 즉 y값 그대로 -> 높이변화 x
    // 깃발의 오른쪽 끝은 value = 1 -> 즉 y값 점점 작아짐 -> 높이가 줄어든다
	
    float dX = 0.f;
    float dY = 0.5 * value * sin(2 * value * c_PI - u_Time * 4);
    // 이를 통해 깃발 펄럭임이 구현됨
    // 2*value*c_PI는 한 주기의 파동이고..
    // u_Time으로 시간이 변하면서 sin파동이 오른쪽으로 갈수록.. sin 파동이 오른쪽으로 흐르는것처럼 보임
    float newColor = 0.5 + (sin(2 * value * c_PI - u_Time * 4) + 1) / 4;
	
    newPosition += vec4(dX, dY, 0, 0);
	
    gl_Position = newPosition;

    v_Color = vec4(newColor);
}

void Wave()
{
    vec4 newPosition = vec4(a_Position, 1);
    float dX = 0;
    float dY = 0;
    
    vec2 pos = vec2(a_Position.xy);
    vec2 cen = vec2(0, 0);
    
    float d = distance(pos, cen);   // pos와 cen이 얼마나 떨어져있는지
    float v = 2 *clamp(0.75 - d, 0, 1);    
    // clamp(x, 0, 1) : x값을 0과 1 사이로 제한함
    // 여기서 v는 d가 0.75보다 작을때만 값이 생김.. 즉, 반지름 0.75 안에서만 파동이 생김
    
    float newColor = v * sin(d * 4 * c_PI * 10 - u_Time * 15);
    // 물결 색상..
    
    newPosition += vec4(dX, dY, 0, 0);
    gl_Position = newPosition;
    v_Color = vec4(newColor);
}

void RainDrop()
{
    vec4 newPosition = vec4(a_Position, 1);
    float dX = 0;
    float dY = 0;
    float newColor = 0;
    
    vec2 pos = vec2(a_Position.xy);
    
    for (int i = 0; i < MAX_POINTS; i++)
    {
        float sTime = u_Points[i].z;    // 시작 시간
        float lTime = u_Points[i].w + 0.2f;    // 지속 시간 (수명)
        float newTime = u_Time - sTime; // 빗방울 생성 후 지난 시간
        if (newTime > 0)
        {
            float baseTime = fract(newTime / lTime);    
            // fract는 소수점 아래 부분만.. 
            // 즉, baseTime은 0~1사이의 값으로 계속 반복됨 (파동이 계속 반복되도록)
            
            float oneMinus = 1 - baseTime; // 시간이 지날수록 1에서 0으로 감소
            // lifeTime이 다 되어가면 파동이 옅어지도록..
            
            //float t = newTime;                 // 이렇게 하면 순차적으로 파동 생성
            float t = baseTime * lTime;          // 이렇게 하면 동시에 모든 파동 생성
            float range = baseTime * lTime / 7; // 퍼져나가는 범위
            vec2 cen = u_Points[i].xy;           // 빗방울 중심 위치
            float d = distance(pos, cen);        // 빗방울 중심과의 거리
            float v = 20 * clamp(range - d, 0, 1); // 이때 앞에 range 는.. 퍼져나가는 범위, 
            // 맨 앞 숫자는.. 진하기
            
            
            newColor += oneMinus * v * sin(d * 4 * c_PI * 8 - t*15);
            // sin(d - t)
            // d가 커질수록 파동이 퍼져나가는 효과 -> 링과 링 사이 간격 조절 (값 커질수록 촘촘)
            // 값 작아질수록 링과 링 사이 간격 넓어짐
            // t가 커질수록 링이 퍼져나가는 속도 조절..
            // 값 클수록, 링이 빨리 퍼져나감
        }
    }
    
    newPosition += vec4(dX, dY, 0, 0);
    gl_Position = newPosition;
    
    v_Color = vec4(newColor);
}


void main()
{
    //Flag();
    //Wave();
    //RainDrop();
    exam1();
}
