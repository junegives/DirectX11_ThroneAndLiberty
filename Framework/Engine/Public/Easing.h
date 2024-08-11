#pragma once

#include "Engine_Defines.h"
#define PI 3.14f

// Easing Equations : �ð� �帧�� ���� �Ű������� ��ȭ�� (���� ����)
// https://easings.net/ko : �׷��� Ȯ�� ���� ����Ʈ

BEGIN(Engine)

class CEasing
{
public:
	CEasing();
	~CEasing() = default;

public:
	HRESULT Initialize();

	// �� ���� ���ĵ�
	// IN : ó���� ������ ������ ������
	// OUT : ó���� ������ ������ ������
	// IN_OUT : ó���� ���� ��������� ������, �߰��� ������
private:
	// Linear
	_float EaseLinear(_float _fTimeRatio);

	// Sine
	_float EaseInSine(_float _fTimeRatio);
	_float EaseOutSine(_float __fTimeRatio);
	_float EaseInOutSine(_float _fTimeRatio);

	// Quad
	_float EaseInQuad(_float _fTimeRatio);
	_float EaseOutQuad(_float _fTimeRatio);
	_float EaseInOutQuad(_float _fTimeRatio);

	// Cubic
	_float EaseInCubic(_float _fTimeRatio);
	_float EaseOutCubic(_float _fTimeRatio);
	_float EaseInOutCubic(_float _fTimeRatio);

	// Quart
	_float EaseInQuart(_float _fTimeRatio);
	_float EaseOutQuart(_float _fTimeRatio);
	_float EaseInOutQuart(_float _fTimeRatio);

	// Quint
	_float EaseInQuint(_float _fTimeRatio);
	_float EaseOutQuint(_float _fTimeRatio);
	_float EaseInOutQuint(_float _fTimeRatio);

	// Expo
	_float EaseInExpo(_float _fTimeRatio);
	_float EaseOutExpo(_float _fTimeRatio);
	_float EaseInOutExpo(_float _fTimeRatio);

	// Circ
	_float EaseInCirc(_float _fTimeRatio);
	_float EaseOutCirc(_float _fTimeRatio);
	_float EaseInOutCirc(_float _fTimeRatio);

	// Back
	_float EaseInBack(_float _fTimeRatio);
	_float EaseOutBack(_float _fTimeRatio);
	_float EaseInOutBack(_float _fTimeRatio);

	// Elastic
	_float EaseInElastic(_float _fTimeRatio);
	_float EaseOutElastic(_float _fTimeRatio);
	_float EaseInOutElastic(_float _fTimeRatio);

	// Bounce
	_float EaseInBounce(_float _fTimeRatio);
	_float EaseOutBounce(_float _fTimeRatio);
	_float EaseInOutBounce(_float _fTimeRatio);

public:
	// (���� Ÿ��, ��� �ð�, �� �ð�, ���� ��, �� ��)
	_float		Ease(EASE_TYPE _eEase, _float _eElapsedTime, _float _fDuration, _float _fStartValue, _float _fEndValue);
	_float2		Ease(EASE_TYPE _eEase, _float _eElapsedTime, _float _fDuration, _float2 _fStartValue, _float2 _fEndValue);
	_float3		Ease(EASE_TYPE _eEase, _float _eElapsedTime, _float _fDuration, _float3 _fStartValue, _float3 _fEndValue);
	_float4		Ease(EASE_TYPE _eEase, _float _eElapsedTime, _float _fDuration, _float4 _fStartValue, _float4 _fEndValue);

public:
	const char** GetEaseTypeKeys() { return m_strEasingTypes; }

public:
	const char* m_strEasingTypes[EASE_END] = 
	{
		"Linear",
		"InSine", "OutSine", "InOutSine",
		"InQuad", "OutQuad", "InOutQuad",
		"InCubic", "OutCubic", "InOutCubic",
		"InQuart", "OutQuart", "InOutQuart",
		"InQuint", "OutQuint", "InOutQuint",
		"InExpo", "OutExpo", "InOutExpo",
		"InCric", "OutCric", "InOutCric",
		"InBack", "OutBack", "InOutBack",
		"InElastic", "OutElastic", "InOutElastic",
		"InBounce", "OutBounce", "InOutBounce"
	};

public:
	static shared_ptr<CEasing> Create();
};

END