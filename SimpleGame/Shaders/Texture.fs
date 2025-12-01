#version 330

layout(location=0) out vec4 FragColor;

uniform sampler2D u_TexID;
uniform float u_Time;

in vec2 v_Tex;

vec4 Lens()
{   vec2 uv = v_Tex;

    // -------------------------------------------------------
    // [울렁임 조절 레버]
    float speed = 2.0;       // 울렁거리는 속도
    float frequency = 10.0;  // 물결의 촘촘함 (클수록 물결이 많아짐)
    float amplitude = 0.015; // 울렁거리는 강도 (클수록 심하게 흔들림)
    // -------------------------------------------------------

    // 1. X축 흔들기 (가로로 넘실넘실)
    // Y좌표에 따라 다르게 흔들어야 물결처럼 보임 (uv.y 사용)
    float offsetX = sin(uv.y * frequency + u_Time * speed) * amplitude;

    // 2. Y축 흔들기 (세로로 꿀렁꿀렁)
    // X좌표와 엇박자로 흔들기 위해 cos 사용
    float offsetY = cos(uv.x * frequency + u_Time * speed) * amplitude;

    // 3. 좌표에 흔들림 적용
    // 원래 가져와야 할 위치보다 조금 옆(offset)의 색을 가져옴
    vec2 wobbledUV = uv + vec2(offsetX, offsetY);

    // 4. 화면 밖 검은색 처리 (선택사항)
    // 울렁거릴 때 화면 끝부분이 흉하게 늘어나는 걸 방지하려면 주석 해제하세요.
    /*
    if(wobbledUV.x < 0.0 || wobbledUV.x > 1.0 || wobbledUV.y < 0.0 || wobbledUV.y > 1.0)
        return vec4(0.0, 0.0, 0.0, 1.0);
    */

    return texture(u_TexID, wobbledUV);
}

vec4 vignette()
{   
    vec2 uv = v_Tex;

    // -------------------------------------------------------
    // [색수차 조절 레버]
    // 이 값이 클수록 색이 더 넓게 벌어집니다. (보통 0.01 ~ 0.05 사이가 적당)
    float spreadStrength = 0.035; 
    // -------------------------------------------------------

    // 1. 중심점 계산
    vec2 center = vec2(0.5, 0.5);
    
    // 2. 중심에서 현재 픽셀까지의 방향 벡터
    vec2 dir = uv - center;
    
    // 3. 거리 계산 (중심에서 멀수록 효과를 강하게 주기 위함)
    // dot(dir, dir)은 거리의 제곱입니다. sqrt를 안 써서 성능에 더 좋습니다.
    // 가장자리로 갈수록 기하급수적으로 강해지도록 제곱값을 사용합니다.
    vec2 distortionOffset = dir * dot(dir, dir) * spreadStrength;
    
    // [핵심 로직: 채널별 샘플링 분리]
    // R, G, B 색상을 각각 조금씩 다른 위치에서 가져옵니다.
    
    // Red 채널: 원래 위치보다 조금 안쪽(혹은 바깥쪽)에서 샘플링
    float r = texture(u_TexID, uv - distortionOffset).r;
    
    // Green 채널: 기준점이 되므로 원래 위치에서 샘플링 (가장 선명함)
    float g = texture(u_TexID, uv).g;
    
    // Blue 채널: Red와 반대 방향으로 샘플링하여 색을 분리시킴
    float b = texture(u_TexID, uv + distortionOffset).b;

    // 4. 분리된 채널을 다시 합쳐서 최종 색상 반환
    return vec4(r, g, b, 1.0);
}

float Hash21(vec2 p) {
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

vec4 retrofilter()
{
    vec2 uv = v_Tex;
    
    // 0. 기본 색상 가져오기
    vec4 color = texture(u_TexID, uv);
    
    // -------------------------------------------------------
    // [레트로 조절 레버]
    float sepiaStrength = 0.7;   // 세피아 강도 (0.0 = 원본, 1.0 = 완전 세피아)
    float vignetteSize = 0.3;    // 비네팅 영역 크기 (작을수록 어두운 영역이 넓어짐)
    float grainUsed = 1.0;       // 그레인 사용 여부 (1.0 = ON, 0.0 = OFF)
    float grainStrength = 0.1;  // 노이즈 강도
    // -------------------------------------------------------

    // --- 효과 1: 세피아 톤 (Sepia Tone) ---
    // RGB를 흑백으로 바꾸면서 누런 톤을 입히는 표준 행렬 연산입니다.
    float r = dot(color.rgb, vec3(0.393, 0.769, 0.189));
    float g = dot(color.rgb, vec3(0.349, 0.686, 0.168));
    float b = dot(color.rgb, vec3(0.272, 0.534, 0.131));
    vec3 sepiaColor = vec3(r, g, b);
    
    // 원본 색상과 세피아 색상을 강도에 따라 섞습니다(mix).
    color.rgb = mix(color.rgb, sepiaColor, sepiaStrength);


    // --- 효과 2: 비네팅 (Vignette) ---
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(uv, center);
    // 중심에서 멀어질수록 어두워지게 계산 (smoothstep으로 부드럽게)
    float vignette = 1.0 - smoothstep(vignetteSize, 1.0, dist);
    
    // 계산된 어두움 값을 기존 색상에 곱해줍니다.
    color.rgb *= vignette;


    // --- 효과 3: 필름 그레인 (Film Grain) 노이즈 ---
    // u_Time이 없으면 0.0으로 고정되어 정지된 노이즈가 보입니다.
    float t = 0.0;
    // t = u_Time; // C++ 연결 후 주석 해제하세요!
    
    if (grainUsed > 0.5)
    {
        // 픽셀 위치와 시간을 기반으로 랜덤한 값을 만들어 색상에 더합니다.
        float noise = Hash21(uv * 100.0 + vec2(t * 10.0)); 
        color.rgb += (noise - 0.5) * grainStrength;
    }

    return color;
}

vec4 Pixelization()
{
    float resol = (sin(u_Time)+1)*100.f;
    float tx = floor(v_Tex.x * resol) / resol;
    float ty = floor(v_Tex.y * resol) / resol;
    return texture(u_TexID, vec2(tx, ty));
}

void main()
{
    //FragColor = texture(u_TexID, vec2(v_Tex.x, 1-v_Tex.y));
   // FragColor = Lens();
    FragColor = Pixelization();
}
