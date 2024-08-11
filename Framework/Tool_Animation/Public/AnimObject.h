#pragma once
#include "GameObject.h"
#include "Tool_Animation_Defines.h"
#include "Weapon.h"

BEGIN(Engine)
class CModel;
class CTexture;
class CRigidBody;
END

BEGIN(Tool_Animation)

class CAnimObject : public CGameObject
{
public:
	enum ANIMOBJECT_WEAPON_TYPE
	{
		WEAPON_TYPE_SWORD, WEAPON_TYPE_CROSSBOW, WEAPON_TYPE_STAFF, WEAPON_TYPE_END
	};

public:
	CAnimObject();
	virtual ~CAnimObject();

public:
	virtual HRESULT Initialize(shared_ptr<class CModel> _pModel);
	virtual void	PriorityTick(_float _fTimeDelta);
	virtual void	Tick(_float _fTimeDelta);
	virtual void	LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void	SetModel(shared_ptr<CModel> _pModel) { m_pModelCom = _pModel; }
	void	RenderWeapon(_float _fTimeDelta);

public:
	void SetCollider(CCollider::ECOL_TYPE _ColType);
	shared_ptr<CCollider> GetCollider();

	void EnableCollision(const char* _strShapeTag) override;
	void DisableCollision(const char* _strShapeTag) override;

	void	EnableCollider() { isColliderOn = true; }
	void	DisableCollider() { isColliderOn = false; }

private:
	HRESULT AddWeapons();		// ���� �߰�
	HRESULT	AddCollider();		// Collider �߰�

private:
	ANIMOBJECT_WEAPON_TYPE		m_eCurrentWeapon = { WEAPON_TYPE_SWORD };	// ���� ���� Ÿ�� ����

private:
	_bool	isColliderOn = { false };

private:

	shared_ptr<CModel>		m_pModelCom;
	map<string, shared_ptr<CWeapon>>	m_pPlayerWeapons;


	shared_ptr<CCollider>	m_pSphereCollider = { nullptr };
	shared_ptr<CCollider>	m_pBoxCollider = { nullptr };

	shared_ptr<CCollider>	m_pCurrentCollider = { nullptr };


public:
	static shared_ptr<CAnimObject> Create(shared_ptr<class CModel> _pModel);
};

END