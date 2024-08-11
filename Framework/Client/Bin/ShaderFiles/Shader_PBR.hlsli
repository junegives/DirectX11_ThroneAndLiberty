/*
https://blog.naver.com/fah204/221587912963
*BRDF : ����� �ݻ�����Լ� , ���� �� ���� ������ �� ���迡 ���� �ݻ���(�ݻ籤/�Ի籤)�� ����ϴ� �Լ�

�ǽð� ������ ���������ο����� Cook - Torrance BRDF ���� ���
*Cook - Torrance BRDF : ���ʿ��� Lambertian diffuse(diffuse Shading�� ����), �������� Cook-Torrance Specular���� �ִ�
Cook - Torrance Specular BRDF�� 3���� �Լ��� �̷�����ְ�, ���� ����ȭ ���� ����

Trowbridge-Reitz GGX(D), Fresnel-Schlick(F), Smith's Schlick-GGX(G) -> �𸮾������� ����ϴ� ���ξ�

*/


float fGamma = 2.2;
float PI = 3.14159;
float3 fFdielectric = 0.04;
float fEpsilon = 0.00001;


float ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;
    
    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

//D : halfway vector h�� ��ġ�ϴ� ǥ�� ������ �ٻ�ġ�� ����
float NormalDistributionGGXTR(float3 n, float3 h, float a)
{
    float a2 = a * a;
    float NdotH = max(saturate(dot(n, h)), 0.0);
    float NdotH2 = NdotH * NdotH;
    
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.f) + 1.f);
    denom = PI * denom * denom;
    
    return nom / denom;
    
}

// G: ������ ���� ��Ģ
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.f;
    float k = (r * r) / 8.f;
    
    float nom = NdotV;
    float denom = NdotV * (1.f - k) + k;
    
    return nom / denom;
}

float GeometrySmith(float3 n, float3 v, float3 l, float k)
{
    
    //Obstruction
    float NdotV = saturate(dot(n, v));
    
    //Shadowing
    float NdotL = saturate(dot(n, l));
    
    float ggx1 = GeometrySchlickGGX(NdotV, k);
    float ggx2 = GeometrySchlickGGX(NdotL, k);
  
    return ggx1 * ggx2;

}

float gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);

}

float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;
    return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
    
    
}


//F: ǥ�� ������ ���� �ݻ��� -> �츮�� ���� ������ ���� �ݻ� ����
//�����̳� �������� ���� �ݻ�ǰ� �����Ǵ� ������ �ٸ�, ������ �츮�� �װ� �ϳ��ϳ� ������ �־��� �� ���⶧���� ������ �̿��ؼ� �ٻ�ġ�� ����
float3 FresnelSchlick(float fCosTheta, float3 R0)
{
    return (R0 + (1.f - R0) * pow(clamp(1.0 - fCosTheta, 0.0, 1.0), 5.0f));
}



