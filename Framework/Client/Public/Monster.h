#pragma once

#include "GameObject.h"
#include "PartObject.h"
#include "Client_Defines.h"

#include "QuestMgr.h"

BEGIN(Engine)
class CModel;
class CTexture;
END

BEGIN(Client)

class CUIBase;
enum ParriedPower
{
	PARRIEDWEAK = 0,
	PARRIEDNORMAL,
	PARRIEDSTRONG
};

class CMonster : public Engine::CGameObject
{
public:
	struct MONSTER_DESC
	{
		shared_ptr<CGameObject> pTarget = nullptr;
	};

public:
	CMonster();
	virtual ~CMonster()  override;

public:
	virtual HRESULT Initialize(MONSTER_DESC* _pMonsterDesc, CTransform::TRANSFORM_DESC* _pTransformDesc);
	virtual void	PriorityTick(_float _fTimeDelta)  override;
	virtual void	Tick(_float _fTimeDelta)  override;
	virtual void	LateTick(_float _fTimeDelta)  override;
	virtual HRESULT Render()  override;

public:
	virtual HRESULT RenderShadow() override;
	virtual HRESULT RenderEdgeDepth() override;
	virtual HRESULT RenderEdge() override;

public:
	_float3	GetCurrentPos() { return m_vCurrentPos; }
	void	SetCurrentPos(_float3 _vCurrentPos) { m_vCurrentPos = _vCurrentPos; }
	_bool	GetDead() { return m_bDead; }
	//_bool	GetParried() { return m_bParried; }
	string	GetModelName() { return m_strModelName; }

	/* UI */
	wstring& GetDisplayName() { return m_strDisplayName; }
	_float4x4& GetCombinedBoneMat(const string& _boneName);

public:
	void OnRimLight() { m_bUsingRimLight = true; }
	void OffRimLight() { m_bUsingRimLight = false; }

public:
	void SpawnMoster(_float3 _vPosition, _float3 _vLookPos = _float3(0.f, 0.f, 0.f));
	void		SimulationOn();
	void		SimulationOff();
	void		MovePos(_float3 _vPos);
	
protected:
	void		UpdateDirection();
	void		MoveToTarget(_float _fTimeDelta);
	_float		UpdateDistanceToCerPos(_float3 _vCerPos);
	void		MovePosXZ(_float3 _vPos);
	void		OnCollisionWeapon();
	void		OffCollisionWeapon();
	void		SetWeaponRimLight(_bool _bOn,  _float3 _fRimLight =
		_float3(1.f, 0.f, 0.f), _float _fPow = 1.5f);

	void		DamagedByPlayer(const COLLISIONDATA& _ColData, const string& _szMyShapeTag);	// ������ ��� �� ����

protected: /* World UI */
	HRESULT PrepareMonsterHPBar(const wstring& _strMonsterTag);
	void UpdateUIPos();
	void UpdateHPBar();
	void ToggleHPBar();
	void ToggleOffHPBar();

protected:
	void ShowEffect(const COLLISIONDATA& _ColData, const string& _szMyShapeTag);




protected:
	shared_ptr<CModel> m_pModelCom = nullptr;
	shared_ptr<CRigidBody>	m_pRigidBody = { nullptr };				// Rigid Body
	string m_strModelName = "";										// Model �̸�
	wstring m_strDisplayName = L"";

protected:
	// ����
	map<string, shared_ptr<CPartObject>>		m_pMonsterParts;

	// Render
	_bool		m_bRender = true;

	// �ִϸ��̼� ����
	_bool		m_bAnimStop = false;

	// Sound
	_float		m_fVoiceSize = 0.5f;
	_float		m_fEffectSize = 0.5f;

	// Ÿ��
	_float			m_fDistanceToTarget = 0.f;
	shared_ptr<CGameObject>		m_pTarget = nullptr;
	_float3			m_vTargetPos = _float3(0.f, 0.f, 0.f);
	_float3			m_vCurrentPos = _float3(0.f, 0.f, 0.f);
	_float3			m_vNormLookDirXZ = _float3(0.f, 0.f, 0.f);

	_float3			m_vTargetDir = _float3(0.f, 0.f, 0.f);
	_float3			m_vNormTargetDirXZ = _float3(0.f, 0.f, 0.f);

	// �ǰ� ����
	_bool			m_bHitted = false;		// �Ϲ��ǰ�
	_bool			m_bHitted2 = false;		// ª���ǰ�
	_bool			m_bHitted3 = false;		// ���ǰ�
	_bool			m_bStun = false;		// ����
	_bool			m_bShocked = false;		// ����
	_bool			m_bDown = false;		// �Ѿ�߸���

	_float			m_fCalculateTime = 0.f;
	
	// �и� ����
	_bool			m_bParried = false;
	_int			m_iParriedPower = PARRIEDNORMAL;

	// �ݳ� ����
	_bool			m_bFury = false;
	_bool			m_bBeforeDead = false;

	// �׾����� ����(�ǰ� 0���� �۾�������)
	_bool			m_bDead = false;
	// �ױ� �����ߴ��� ����
	_bool		m_bDeadStart = false;
	_float		m_fDeadDisolveTime = 1.f;

	_float4		m_vDisolveColor = _float4(0.427f, 0.227f, 0.573f, 1.f);

	// Ư������1����
	_bool			m_bGimicPattern1 = false;
	// Ư������2����
	_bool			m_bGimicPattern2 = false;
	
	// ����
	_float			m_fSpeed = 1.f;
	_float			m_fHp = 100.f;
	/* For UI Test */
	_float			m_fMaxHp = 100.f;

	// ��Ű�����Ӵ� �ð�
	_float			m_fKeyFrameSpeed = 0.0413f;

	// ����Ʈ ���࿩��
	_bool			m_bEffectReady = false;

	// QuickBatch
	_bool		m_bQuickBatch = false;

protected:
	_bool m_isSpawn = false;
	_float3 m_vSpwnPos = _float3();
	_float3	m_vLookPos = _float3(0.f, 0.f, 0.f);	// ������ ���� ����

protected:
	_bool	m_bUsingRimLight = { false } ;
	_float	m_fLineThick = 1.f;
	_float4	m_vLineColor = { 1.f, 0.3f, 0.3f, 1.f };

protected: /* UI */
	shared_ptr<CUIBase> m_pHPBar{ nullptr };
	vector<shared_ptr<CUIBase>> m_pHPUIs;

};

END