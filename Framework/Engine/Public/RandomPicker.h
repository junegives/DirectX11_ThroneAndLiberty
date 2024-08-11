#pragma once

/* =============================================== */
//	[RandomPicker]
// 
//  �Ű������� ������ ������ �������� ������ ���ڸ� ��� ��ȯ�ϴ� ����� ������ Ŭ�����Դϴ�
//  _int�� _float �� ������ �����ϸ� ������ Funtion �ؿ� �ۼ��ص�
//
/* =============================================== */

#include "Engine_Defines.h"
#include <random>

BEGIN(Engine)

class CRandomPicker final
{
public:
	CRandomPicker();
	~CRandomPicker() = default;

public:
	HRESULT Initialize();

public:
	/*�Ű������δ� ���� ������ �������� ���κ��� �޽��ϴ�*/
	/*ex) 30 ~ 50������ �������ڷ� ������ �ΰ� �ʹٸ� PickRandomIntNum(30, 50)�� �����ָ� �� */
	_int PickRandomInt(_int _iStartLimit, _int _iEndLimit, _bool _bSort = false);
	_float PickRandomFloat(_float _fStartLimit, _float _fEndLimit, _bool _bSort = false);

private:
	random_device m_RandomDevice;
	mt19937_64 m_RandomNumber;

public:
	static shared_ptr<CRandomPicker> Create();

};

END