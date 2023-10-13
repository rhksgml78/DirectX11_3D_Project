//슬롯
cbuffer VS_W : register(b0)
{
    matrix World;
}
cbuffer VS_VP : register(b1)
{
    matrix ViewProj;
}

//스켈레톤 (애니메이션까지)
#define MAX_BONE 256
cbuffer VS_Bones : register(b2)
{
    matrix Bones[MAX_BONE];
}



cbuffer VS_V : register(b3)
{
    matrix View;
}
cbuffer VS_P : register(b4)
{
    matrix Proj;
}

cbuffer VS_Shadow : register(b5)
{
    matrix ShadowVP;
}
cbuffer VS_ClipPlane : register(b6)
{
    float4 ClipPlane;
}


cbuffer GS_VP : register(b0)
{
    matrix GSProj;
}

cbuffer GS_CubeMap : register(b1)
{
    matrix CubeViews[6];
    matrix CubeProjection;
};




cbuffer PS_ViewPos : register(b0)
{
    float4 ViewPos;
}

cbuffer PS_Material : register(b1)
{
    float4 Ka;
    float4 Kd;
    float4 Ks;
    float4 Ke;
    float Shininess;
    float Opacity;
    float environment;
    float shadow;
}

cbuffer PS_DirLight : register(b2)
{
    float4 DirLight;
    float4 DirColor;
}


#define MAX_LIGHT 16
struct Light
{
    //4
    int isActive;
    int Type;
    float Inner;
    float Outer;
	//
    int Size;
    float3 Direction;
    //4
    float3 Position;
    float Radius;
    //4
    float4 Color;
};
cbuffer PS_Lights : register(b3)
{
    Light lights[MAX_LIGHT];
}

cbuffer PS_ShadowMap : register(b5)
{
    int ShadowQuality;
    float ShadowBias;
    float2 ShadowSize;
}




Texture2D TextureN : register(t0);
SamplerState SamplerN : register(s0);

Texture2D TextureD : register(t1);
SamplerState SamplerD : register(s1);

Texture2D TextureS : register(t2);
SamplerState SamplerS : register(s2);

Texture2D TextureE : register(t3);
SamplerState SamplerE : register(s3);

TextureCube TextureBG : register(t4);
SamplerState SamplerBG : register(s4);

TextureCube TextureC : register(t5);
SamplerState SamplerC : register(s5);




Texture2D ShadowMap : register(t7);
SamplerState SamplerDefault
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4 DiffuseMapping(float2 Uv)
{
    [flatten]
    if (Kd.a)
        return TextureD.Sample(SamplerD, Uv);
    
    return float4(1, 1, 1, 1);
}
float3 SpecularMapping(float2 Uv)
{
    [flatten]
    if (Ks.a)
        return TextureS.Sample(SamplerS, Uv).rgb;
    
    return float3(1, 1, 1);
}

float3 NormalMapping(float3 N, float3 T, float3 B, float2 Uv)
{
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);
    
    [flatten]
    if (Ka.a)
    {
        
        // 130,130,255 (기본값) -> 0,0,1
        // 0.5,0.5,1.0
        float3 normal = TextureN.Sample(SamplerN, Uv).rgb;
        //float3 normal = float3(0.5, 0.5, 1.0);
        
        //세개의 법선으로 회전행렬을 만듦
        float3x3 TBN = float3x3(T, B, N);
        
        // rgb (0~1,0~1,0~1) *2
        // rgb (0~2,0~2,0~2) -1
        // rgb (-1~1,-1~1,-1~1)
        N = normal * 2.0f - 1.0f;
        //매핑한 법선을 회전시키기  normal * Matrix
        N = normalize(mul(N, TBN));
    }
    
    return N;
}

float3 EmissiveMapping(float3 BaseColor, float2 Uv, float3 Normal, float3 wPosition)
{
    //Emissive
    float3 Emissive = 0;
    float3 EmissiveMap = BaseColor;
    float3 ViewDir = normalize(ViewPos.xyz - wPosition);
    //반사색이 있을때만 Emissive 값을 계산한다.
    [flatten]
    //rgb중에 값이 하나라도 0 이 아니면
    if (any(Ke.rgb))
    {
        //계수
        float emissive = 1.0f - saturate(dot(Normal, ViewDir));
        emissive = smoothstep(0.0f, 1.0f, emissive);
        Emissive = emissive * Ke.rgb;
    }
    //EmissiveMap이 존재할때 텍스쳐 색상을 입혀준다.
    [flatten]
    if (Ke.a)
    {
        EmissiveMap = TextureE.Sample(SamplerE, Uv).rgb;
    }
    Emissive *= EmissiveMap;
    return saturate(Emissive);
}


//						diffuseMap
float3 DirLighting(float3 BaseColor, float3 SpecularMap, float3 Normal, float3 wPostion)
{
    float3 DirectionLight = normalize(DirLight.xyz);
    float Diffuse = saturate(dot(-DirectionLight, Normal));
    
    float3 RecflectLight = reflect(DirectionLight, Normal);
    float3 ViewDir = normalize(ViewPos.xyz - wPostion);
    float Specular = saturate(dot(ViewDir, RecflectLight));
    Specular = pow(Specular, Shininess);
    
    //         계수 * 머터리얼 *  DiffuseMap
    float3 D = Diffuse * Kd.rgb * BaseColor;
    float3 S = Specular * Ks.rgb * SpecularMap;
    return saturate((D + S) * DirColor.rgb);
    //return S;
}

float3 PointLighting(float3 BaseColor, float3 SpecularMap, float3 Normal, float3 wPosition, int idx)
{
    //return float3(1, 1, 1);
    float3 DirectionLight = lights[idx].Position - wPosition;
    float distanceToLight = length(DirectionLight);
    DirectionLight /= distanceToLight;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / lights[idx].Radius);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    //빛의계수
    float Diffuse = saturate(dot(DirectionLight, Normal)) * attention;
  
    
    float3 RecflectLight = reflect(DirectionLight, Normal);
    float3 ViewDir = normalize(ViewPos.xyz - wPosition);
    float Specular = saturate(dot(ViewDir, RecflectLight)) * attention;
    Specular = pow(Specular, Shininess);
    
    float3 D = Diffuse * Kd.rgb * BaseColor;
    float3 S = Specular * Ks.rgb * SpecularMap;
    
    return saturate((D + S) * lights[idx].Color.rgb);
}

float3 SpotLighting(float3 BaseColor, float3 SpecularMap, float3 Normal, float3 wPosition, int idx)
{
    //픽셀에서 라이트향하는 방향
    float3 DirectionLight = lights[idx].Position - wPosition;
    float distanceToLight = length(DirectionLight);
    DirectionLight /= distanceToLight;
    
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / lights[idx].Radius);
    float attention = distanceToLightNormal * distanceToLightNormal;
    
    float cosAngle = saturate(dot(normalize(-lights[idx].Direction),
    DirectionLight));
    
    //예시용
    ////84도 -> 코사인값
    //float inner = cos(radians(lights[idx].Inner));
    ////Inner 이하인값만
    //if (cosAngle > inner)
    //{
    //    attention = 1.0f;
    //}
    
    // 0 ~ 90 -> 1 ~ 0 
    float outer = cos(radians(lights[idx].Outer));
    // 0 ~ 90   -> 1 ~ 무한
    float inner = 1.0f / cos(radians(lights[idx].Inner));
    
    cosAngle = saturate((cosAngle - outer) * inner);
    attention *= cosAngle;
    
    //빛의계수
    float Diffuse = saturate(dot(DirectionLight, Normal)) * attention;
  
    
    float3 RecflectLight = reflect(DirectionLight, Normal);
    float3 ViewDir = normalize(ViewPos.xyz - wPosition);
    float Specular = saturate(dot(ViewDir, RecflectLight)) * attention;
    Specular = pow(Specular, Shininess);
    
    float3 D = Diffuse * Kd.rgb * BaseColor;
    float3 S = Specular * Ks.rgb * SpecularMap;
    
    return saturate((D + S) * lights[idx].Color.rgb);
}



float3 EnvironmentMapping(float3 Normal, float3 wPosition)
{
	 [flatten]
    if (environment != 0.0f)
    {
        float3 ViewDir = normalize(wPosition - ViewPos.xyz);
        float3 reflection = reflect(ViewDir, Normal);
        return TextureBG.Sample(SamplerBG, reflection.xyz) * environment;
		
		//float3 Refract = refract(ViewDir, Normal,2);
		//return TextureBG.Sample(SamplerBG, Refract.xyz) * environment;
		
    }
    return float3(0, 0, 0);
}


float4 Lighting(float4 BaseColor, float2 Uv, float3 Normal, float3 wPosition)
{
    float3 SpecularMap = SpecularMapping(Uv);
	
    float4 Result = float4(DirLighting(BaseColor.rgb, SpecularMap,
    Normal, wPosition),
    BaseColor.a);
	
    if (Result.a > Opacity)
        Result.a = Opacity;
	
	//광원 수만큼
	//반복

    for (int i = 0; i < lights[0].Size; i++)
    {
        [flatten]
        if (!lights[i].isActive)
            continue;
        
        [flatten]
        if (lights[i].Type == 0)
        {
            Result.rgb += PointLighting(BaseColor.rgb, SpecularMap,
			Normal, wPosition, i);
        }
        else if (lights[i].Type == 1)
        {
            Result.rgb += SpotLighting(BaseColor.rgb, SpecularMap,
            Normal, wPosition, i);
        }
    }
	
	//발광
	//Ambient
    Result.rgb += Ka.rgb * BaseColor.rgb;
    //Emissive
    Result.rgb += EmissiveMapping(BaseColor.rgb, Uv, Normal, wPosition);
	//Environment
    Result.rgb += EnvironmentMapping(Normal, wPosition);
	
    return saturate(Result);
}



matrix SkinWorld
(
	float4 indices, float4 weights)
{
    matrix transform = 0; //영행렬
    transform += mul(weights.x, Bones[(uint) indices.x]);
    transform += mul(weights.y, Bones[(uint) indices.y]);
    transform += mul(weights.z, Bones[(uint) indices.z]);
    transform += mul(weights.w, Bones[(uint) indices.w]);
	
	
    return transform;
}

float4 AddShadow(float4 BaseColor, float4 vPosition)
{
     [flatten]
    if (shadow == 0.0f)
        return BaseColor;
    
	
	
    float4 position = vPosition;
	
	//ndc로의변환
    position.xyz /= position.w;
    
	
	//지정범위 내에서만 그림자표현하기.
    [flatten]
    if (position.x < -1.0f || position.x > +1.0f ||
        position.y < -1.0f || position.y > +1.0f ||
        position.z < +0.0f || position.z > +1.0f)
    {
        //return float4(0, 0, 0, 1);
        return BaseColor;
    }
    
    // NDC좌표에서 UV좌표로 변환
    position.x = position.x * 0.5f + 0.5f;
    position.y = -position.y * 0.5f + 0.5f; // 텍스처좌표는 y가뒤집혀서 -로바꿔주는것임
    //텍스처에서 ndc z값 가져오기
	//ShadowDepth = ShadowMap.Sample(SamplerDefault, position.xy).r;
	//return float4(ShadowDepth, ShadowDepth, ShadowDepth, 1.0f);
    
	//                    지금ndc.z
    float CurrentDepth = position.z - ShadowBias;
    float ShadowDepth = 0.0f;
    float factor = 0.0f;
   
    if (ShadowQuality == 0)
    {
        ShadowDepth = ShadowMap.Sample(SamplerDefault, position.xy).r;
        factor = (float) (ShadowDepth >= CurrentDepth);
    }
    else if (ShadowQuality == 1) // 그림자 표현 포인터&리니어느낌차이
    {
        float2 size = 1.0f / ShadowSize;
        float2 offsets[] =
        { // 0,0(본인좌표)를 기준으로 주변의 8개의 픽셀들을 보간한다.
            float2(-size.x, -size.y), float2(0.0f, -size.y), float2(+size.x, -size.y),
            float2(-size.x, 0.0f), float2(0.0f, 0.0f), float2(+size.x, 0.0f),
            float2(-size.x, +size.y), float2(0.0f, +size.y), float2(+size.x, +size.y),
        };
        float2 uv = 0;
        float sum = 0;
        [unroll(9)]
        for (int i = 0; i < 9; i++) // 위의 9개픽셀을 다더해서 평균을 낸다.
        {
            uv = position.xy + offsets[i];
            ShadowDepth = ShadowMap.Sample(SamplerDefault, uv).r;
            factor = (float) (ShadowDepth >= CurrentDepth);
            sum += factor;
        }
        factor = sum / 9.0f; // 멀티샘플링 (ex.안티앨리어싱, Blur에서 비슷하게사용했었다)
    }
    factor = max(0.5f, factor);
    BaseColor.rgb = saturate(BaseColor.rgb * factor);
    
    return BaseColor;
}