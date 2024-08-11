#pragma once

#include "Tool_Defines.h"
#define MAX_EFFECT_NUM 20

BEGIN(Tool_Effect)

class CWindowPlay
{
public:
	CWindowPlay();
	~CWindowPlay() = default;

public:
	HRESULT Initialize();
	void	Tick(_float _fTimeDelta);

public:
	//// 0. �޽�
	//void	Properties_Mesh();
	//// 1. ��ǻ�� �ؽ�ó
	//void	Properties_Diffuse();
	//// 2. ����ũ �ؽ�ó
	//void	Properties_Mask();
	//// 3. ������ �ؽ�ó
	//void	Properties_Noise();
	//// 4. ����
	//void	Properties_Color();
	//// 5. ������
	//void	Properties_BillBoard();
	//// 6. ũ��
	//void	Properties_Scale();
	//// 7. ȸ��
	//void	Properties_Rotation();
	//// 8. �̵�
	//void	Properties_Translation();
	//// 9. ���̴�
	//void	Properties_Shader();

private:
	void	KeyInput();

private:
	// ����Ʈ ���� ��ü ��� �ð�
	_float			m_fTotalDuration = 5.f;
	_float			m_fTotalSpeed = 1.f;

	_float			m_fElapsedTime = 0.f;

	// �� ����Ʈ ��� �ð�
	vector<shared_ptr<class CEffect>> m_pEffects = {};
	vector<_float>	m_EffectPlayTimes = {};

	_bool			m_bLoop = false;
	_bool			m_bPlay = false;

private:
	ComPtr<ID3D11Device>				m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext>			m_pContext = nullptr;
};

END