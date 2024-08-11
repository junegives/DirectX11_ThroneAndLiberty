/*������Ʈ color ���*/
/*_vRimCol = ������Ʈ ��, g_fRimPow�� ������ �������� ������ ������*/
float3 ComputeRimColor(float3 _vRimCol , float3 _vLook, float3 _vNormal, float fRimPow)
{
    float3 E = normalize(_vLook);
    
    float value = saturate(dot(_vNormal, E));
    float fEmissive = 1.0f - value;

    /*min ���ڰ� ������ �������� ���� ������*/
    fEmissive = smoothstep(0.f, 1.0f, fEmissive);

    /*���ڰ� ������ �������� ������ ������*/ 
    fEmissive = pow(fEmissive, fRimPow);
    float3 vRimFinColor = fEmissive * _vRimCol;

    return vRimFinColor;
}

float4 Dissolve(float _fNoiseR, float _fStart, float _fEnd, float _fThickness, float4 _vDslvColor, float4 _vColor)
{
    float4 vColor = _vColor;

    float fNoiseR = _fNoiseR * _fNoiseR;

    if (fNoiseR < _fStart)
    {
        // �״�� ���
    }
    else if (fNoiseR < _fEnd)
    {
        float fValue = smoothstep(0.0f, 1.0f, (_fEnd - fNoiseR) * _fThickness);

        vColor.rgb += lerp(_vColor.rgb, _vDslvColor.rgb, fValue);
    }
    else
    {
        discard;
    }

    return vColor;
}