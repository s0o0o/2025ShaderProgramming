#version 330

layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 FragColor1;
in vec2 v_UV;

uniform sampler2D u_RGBTexture;
uniform sampler2D u_DigitTexture;
uniform sampler2D u_NumTexture;

uniform float u_Time;

const float c_PI = 3.141592;

vec4 Test()
{
    vec2 newUV = v_UV;
    float dx = 0.1 * sin(v_UV.y * 2 * c_PI * 3 + u_Time);
    float dy = 0.1 * sin(v_UV.x * 2 * c_PI * 3 + u_Time);
    newUV += vec2(dx, dy);
    vec4 sampledColor = texture(u_RGBTexture, newUV);
    return sampledColor;
}

vec4 Circle()
{ 
    vec2 newUV = v_UV;  // 0~1, 0,0 left top
    vec2 center = vec2(0.5, 0.5);
    vec4 newColor = vec4(0);
    float d = distance (newUV, center);
    float value = sin(d*c_PI*10 - u_Time*5);
    newColor = vec4(value);
    return newColor;
}

vec4 Flag()
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
     //discard;
    }
    return newColor;
}

vec4 Q1()
{
    vec2 newUV = vec2(v_UV.x,v_UV.y);  // 0~1, 0,0 left bottom
    float x = newUV.x;
    float y = 1 - abs(2*(v_UV.y - 0.5));     // 0~1~0
    vec4 newColor = texture(u_RGBTexture, vec2(x, y));

    return newColor;
}

vec4 Q2()
{
    vec2 newUV = vec2(v_UV.x,v_UV.y);  // 0~1, 0,0 left bottom
    float x = fract(newUV.x*3);
    float y = (2-floor(newUV.x*3))/3 + newUV.y/3;     // 0~1~0
    vec4 newColor = texture(u_RGBTexture,  vec2(x, y));
    return newColor;
}

vec4 Q3()
{
    vec2 newUV = vec2(v_UV.x,v_UV.y);  // 0~1, 0,0 left bottom
    float x = fract(newUV.x*3);
    float y = (3+floor(newUV.x*3))/3 + newUV.y/3;     // 0~1~0
    vec4 newColor = texture(u_RGBTexture,  vec2(x, y));
    return newColor;
}


vec4 Brick_Horizontal()
{
    vec2 newUV = vec2(v_UV.x,v_UV.y);  // 0~1, 0,0 left bottom
    float rCount = 2;   // 몇번씩 반복될건징..
    float sAmount = 0.5;
    float x = fract(newUV.x*rCount) +  floor(newUV.y*rCount)*sAmount; // 0~1 0~1
    float y = fract(newUV.y*2); // 0~1 0~1
    vec4 newColor = texture(u_RGBTexture,  vec2(x, y));
    return newColor;
}


vec4 Brick_Vertical()
{
    vec2 newUV = vec2(v_UV.x,v_UV.y);  // 0~1, 0,0 left bottom
    float x = fract(newUV.x*2); // 0~1 0~1
    float y = fract(newUV.y*2) + floor(newUV.x*2)*0.5; // 0~1 0~1
    vec4 newColor = texture(u_RGBTexture,  vec2(x, y));
    return newColor;
}


vec4 Brick_Horizontal_AI()
{
    vec2 newUV = vec2(v_UV.x, v_UV.y);  // 0~1, 0,0 left bottom

 // [1] 변수 설정 (이 숫자로 벽돌 개수와 틈새 조절)
    float countX = 3.0;   // 가로 반복 횟수
    float countY = 6.0;   // 세로 반복 횟수
    float gap = 0.05;     // 줄눈 두께

    // [2] 격자 만들기 (Tiling)
    vec2 st = newUV * vec2(countX, countY);

    // [3] 지그재그 쌓기 (Offset)
    // 현재 행(y)이 홀수라면 x를 0.5(반 칸) 이동
    if (mod(floor(st.y), 2.0) > 0.5)
    {
        st.x += 0.5;
    }

    // [4] 벽돌 vs 줄눈 구하기 (마스크)
    vec2 f = fract(st); // 0~1 사이의 칸 내부 좌표
    // 가장자리 gap보다 안쪽이면 1(벽돌), 아니면 0(줄눈)
    float isBrick = step(gap, f.x) * step(gap, 1.0 - f.x) *
                    step(gap, f.y) * step(gap, 1.0 - f.y);

    // [5] 최종 좌표 결정 (Mix)
    // 벽돌(1)이면: 계산된 st 좌표 사용 (텍스처 반복)
    // 줄눈(0)이면: vec2(0, 0) 사용 (텍스처의 구석 색깔로 칠함)
    vec2 finalUV = mix(vec2(0.0, 0.0), st, isBrick);

    // [6] x, y 값 추출 (아래 주어진 틀에 맞추기 위해)
    float x = finalUV.x;
    float y = finalUV.y;
    vec4 newColor = texture(u_RGBTexture,  vec2(x, y));
    return newColor;
}

vec4 Digit()
{
    return texture(u_DigitTexture, v_UV);
}

vec4 Digit_Num()
{
    int digit = int(u_Time)%10; // 이러면 숫자가 바뀐당
    int tileIndex = (digit + 9)%10;

    float offX = float(tileIndex % 5)/5;
    float offY = floor(float(tileIndex) / 5)/2;

    float tx = v_UV.x*0.2 + offX;
    float ty = v_UV.y*0.5 + offY;
    return texture(u_NumTexture,  vec2(tx, ty));
}

vec4 Digit_Num_AI()
{
    // [설정] 표시하고 싶은 전체 숫자 (예: 시간)
    int totalNumber = int(u_Time); 
    
    // [설정] 총 몇 자리수로 표시할 것인가? (5자리)
    float digitCount = 5.0; 

    // -------------------------------------------------------
    // 1. 현재 픽셀이 '몇 번째 자리(slot)'인지 계산
    // v_UV.x가 0.0~0.2면 0번 칸, 0.2~0.4면 1번 칸... 식으로 나뉩니다.
    float slotIndex = floor(v_UV.x * digitCount); // 0, 1, 2, 3, 4 (왼쪽부터)
    
    // 2. 칸 내부의 새로운 UV 좌표 (0~1 범위로 리셋)
    // 화면 전체가 아니라, 쪼개진 작은 칸 안에서의 0~1 좌표가 필요합니다.
    vec2 localUV = vec2(fract(v_UV.x * digitCount), v_UV.y);

    // -------------------------------------------------------
    // 3. 현재 칸(slotIndex)에 들어갈 숫자 값 추출하기
    // 예: 12345이고 slotIndex가 0(만단위)이라면 -> '1'을 뽑아내야 함
    
    // 자리수에 맞는 나누는 수 계산 (10000, 1000, 100, 10, 1 순서)
    // pow(10, 4 - 0) = 10000
    float divisor = pow(10.0, digitCount - 1.0 - slotIndex);
    
    // 전체 숫자에서 해당 자리수의 숫자 하나만 쏙 뽑아냄
    int digit = int(totalNumber / int(divisor)) % 10;

    // -------------------------------------------------------
    // 4. 텍스처에서 해당 숫자 이미지 가져오기 (기존 로직 활용)
    // ※ 텍스처 순서가 1,2,3...9,0 이라면 (digit + 9)%10 을 쓰시고,
    //    0,1,2...9 순서라면 그냥 digit을 쓰시면 됩니다. 
    int tileIndex = (digit + 9) % 10; // 기존 코드의 순서 보정 유지

    // 텍스처 아틀라스 매핑 (가로 5개, 세로 2개 가정)
    float offX = float(tileIndex % 5) / 5.0;
    float offY = floor(float(tileIndex) / 5.0) / 2.0;

    // localUV를 사용하여 각 칸마다 숫자가 꽉 차게 그림
    // x * 0.2 : 텍스처 가로폭이 1/5이므로 축소
    // y * 0.5 : 텍스처 세로폭이 1/2이므로 축소
    float tx = localUV.x * 0.2 + offX;
    float ty = localUV.y * 0.5 + offY;

    return texture(u_NumTexture, vec2(tx, ty));
}

void main()
{
    //Test();
    //Circle();
    //Flag();
    //Q1();
    //Q2();
    //Q3();
    //Brick_Horizontal();
    //Brick_Vertical();
    //Brick_Horizontal_AI();
    //Digit();
    //Digit_Num();
    //Digit_Num_AI(); // 5ㅈㅏ리 만들수있음
    FragColor = Circle();
    FragColor1 = Flag();
}
