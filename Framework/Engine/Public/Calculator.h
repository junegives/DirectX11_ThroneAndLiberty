
#pragma once

/* =============================================== */
//	[Calculator]
// 
//  ��Ÿ ���� ����� ���ִ� �Լ��� ���Ե� Ŭ���� �Դϴ�.
//
/* =============================================== */

#include "Engine_Defines.h"
#include <random>

BEGIN(Engine)

class CCalculator final
{
public:
	CCalculator();
	~CCalculator() = default;

public:
	HRESULT Initialize();

public:

	_float Lerp(_float _fStart, _float _fEnd, _float _fRatio);

private:

public:
	static shared_ptr<CCalculator> Create();

};

END