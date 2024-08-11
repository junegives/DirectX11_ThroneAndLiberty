#pragma once


#include "Client_Defines.h"

BEGIN(Client)

class CAnimalPool
{
public:
	enum EANIMALTYPE {
		EDEER,						// ���� �罿
		ERABBIT,					// ���� �䳢
		ERETRIEVEL,					// ��Ʈ���� ����
		ECROW,						// ���
		ERETRIEVEL2,				// ��Ʈ���� ����
		END_ENUM
	};

	struct SpawnDesc {
		EANIMALTYPE eAnimalType;
		_float3 vPosition;
	};

public:
	CAnimalPool();
	~CAnimalPool() = default;

public:
	/*Create Animal Pool*/
	HRESULT Initialize();

	/*Spawn Animal*/
	void ActiveAnimal(vector<SpawnDesc>* _pSpawnInfo);
	void ActiveAnimal(string _eventKeyName);

public:
	void ClearPool();
	
private:
	void AwakeAnimal(EANIMALTYPE _eAnimalType, _float3 _vPosition, _float3 _vLookPos = _float3(0.f, 0.f, 0.f));

private:
	/*Animal List*/
	vector<shared_ptr<class CNPC>> m_pAnimals[EANIMALTYPE::END_ENUM];

private:
	_uint m_iMaxAnimal1Num = 1;			// ���� ��ü	
	_uint m_iMaxAnimal2Num = 20;		// ��� ��
	_uint m_iMaxAnimal3Num = 5;			// ���� ������ ��

public:
	static shared_ptr<CAnimalPool> Create();
	
};

END