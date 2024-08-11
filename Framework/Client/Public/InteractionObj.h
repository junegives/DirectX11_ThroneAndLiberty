#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CInteractionObj abstract : public CGameObject
{
public:
	CInteractionObj();
	virtual  ~CInteractionObj() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render() { return S_OK; };

public:
	_bool CheckDistance();
	virtual void InteractionOn() = 0;
	virtual void InteractionOff();

protected:
	void KeyInput();

/*Player(Target)*/
protected:
	_bool	m_bInteractAble = true;			// Interaction�� ������ ��������
	_bool m_IsInteractionOn = false;
	/*��ȸ�� ��ȣ�ۿ� ������Ʈ�� ��� -> true�� ��� �� �̻� üũ ����*/
	_bool m_IsInteractionDone = false;
	weak_ptr<CGameObject> m_pPlayer;

/*Interaction UI*/
protected:
	weak_ptr<class CUIPanel> m_pInteractionPanel;
	/*Object Position���κ��� �󸶳� �� ���� UI�� ����� ����*/
	_float3 m_vUIDistance = _float3(0.f, 0.f, 0.f);

	CRenderer::ERENDER_GROUP m_eRenderGroup = CRenderer::RENDER_NONBLEND;

protected:
	_float m_fInteractDistance = 4.f;

protected:
	shared_ptr<CModel> m_pModel = nullptr;

};

END