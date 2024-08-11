#pragma once

#include "Engine_Defines.h"

#include "Bone.h"
#include "Model.h"
#include "Channel.h"

BEGIN(Engine)


class ENGINE_DLL CAnimation : public std::enable_shared_from_this<CAnimation>
{
public:
	CAnimation();
	CAnimation(const shared_ptr<CAnimation> _pOrigin);
	~CAnimation() = default;

public:
	HRESULT Initialize(ifstream& _ifs, shared_ptr<CModel> _pModel, _float _fSpeed);
	_bool InvalidateTransformationMatrix(_float _fTimeDelta, const vector<shared_ptr<CBone>>& _Bones, _bool _isLoop);
	_bool SeparateTransformationMatrix(_float _fTimeDelta, const vector<shared_ptr<CBone>>& _Bones, _bool _isLoop, _bool _isUpper, _uint _iNumBaseBone, _uint _iNumUpperBone);

public:
	// �ִϸ��̼��� ���� �� ��, ����� �ִϸ��̼ǰ��� ���� ������ ���� �������� �غ��մϴ�.
	void	SetBlendingInfo(_float _fChangingDelay , vector<shared_ptr<class CBone>> _Bones, _bool _isUpper = true , _uint _iNumBaseBone = 0, _uint _iNumUpperBone = 0);

	// �ִϸ��̼��� ���� ��� ä�ε��� ���� BoneIndex�� Model�� ���� BoneIndex�������� ������Ʈ �մϴ�.
	void	UpdateChannelsBoneIndex(shared_ptr<CModel> _pModel);

public:
	_double		GetDuration()		const { return m_Duration; }
	_double		GetTrackPosition()	const { return m_TrackPosition; }
	const char* GetName()		const { return m_szName; }
	_float		GetAnimRatio()		const { return m_fAnimRatio; }
	_bool		GetCanCancel()		const { return m_bCanCancel; }

	void		SetTrackPosition(_double _fTrackPosition) { m_TrackPosition = _fTrackPosition; }
	void		SetAnimCancleRatio(_float _fAnimCancleRatio) { m_fAnimCancleRatio = _fAnimCancleRatio; }

public:
	void		SetOwnerObject(shared_ptr<class CGameObject> _pOwnerObject) { m_pOwnerObject = _pOwnerObject; }
	void		SetColStartTrackPos(_float _fStartTrackPos) { m_fColStartTrackPos = _fStartTrackPos; m_isColAnim = true; }
	void		SetColEndTrackPos(_float _fEndTrackPos) { m_fColEndTrackPos = _fEndTrackPos; }

public:
	void		DisableCollision();	// �ݶ��̴� Ʈ���� Disable
	_bool		GetIsColAnim() { return m_isColAnim; }

public:
	void		SetAnimIndex(_int _iAnimIndex) { m_iMyAnimIndex = _iAnimIndex; }		// My Anim Index
	void		AddProjectileTrigger(_float _fProjectileTriggerTrackPos, _int eProjectileTriggerOwners);	//

public:
	void		SetIsSoundAnim() { m_isSoundAnim = true; }		// 
	void		AddSound(string _strSoundName, _float _fSoundTrackPos, _float _fVolume);	//
	//For Anim Tool
	void		ResetSound();	//
	void		SetSoundCursor() { m_SoundCursor = -1; }	//

private:
	_float m_fSpeed = { 1.f };					// Json���� ���� �ִϸ��̼� ��� �ӵ��Դϴ�.

private:
	_char m_szName[MAX_PATH] = "";
	_uint m_iNumAnimations = { 0 };

	_double m_Duration = { 0.0 };
	_double m_TickPerSecond = { 0.0 };
	_double m_TrackPosition = { 0.0 };
	_double m_BlendingPosition = { 0.0 };

	_uint m_iNumChannels = { 0 };
	vector<shared_ptr<class CChannel>> m_Channels = {};
	vector<_uint> m_iCurrentKeyFrames;

	_bool	m_isFinished	= { false };	// �ִϸ��̼��� ��������
	_float	m_fAnimCancleRatio = { 0.f };	// �ִϸ��̼� ĵ�� ������ ����
	_bool	m_bCanCancel	= { false };	// �ִϸ��̼� ĵ�� ������ ��������
	_float	m_fAnimRatio = { 0.f };			// �ִϸ��̼� ����� ����

	
	weak_ptr<class CGameObject> m_pOwnerObject; // �� �ִϸ��̼��� ����

	//�������� ���� �����Դϴ�.
	_float	m_fChangingDelay	= { 0.0f };				// ���� ������ ����� �ð�.
	_bool	m_isChanging		= { false };			// ���� ������ ���� ������.
	map<_uint, CChannel::KEYFRAME> m_BlendKeyFrame;		// ���� �ִϸ��̼ǰ� ��ġ�� ������ ���� ���� KEYFRAME������ ����.

	//Collier ����
	_float	m_fColStartTrackPos = {0.f};	// ColliderTrigger�� ���� TrackPos
	_float	m_fColEndTrackPos = {0.f};		// ColliderTrigger�� ���� TrackPos
	_bool	m_isColOn = { false };			// ColliderTrigger�� ���� ON/OFF ����
	_bool	m_isColAnim = { false };		// ColliderTrigger�� �ִ� �ִϸ��̼�����.

	//Projectile ����
	_int			m_iMyAnimIndex = { -1 };	// �� �ִϸ��̼� �ε���
	vector<_float>	m_ProjectileTriggers;		// Projectile�߻� Ű������
	vector<_int>	m_ProjectileTriggerOwners;	// Projectile�߻�Ǵ� ��ġ
	_int			m_TriggerCursor = { -1 };

	//Sound ����
	_bool			m_isSoundAnim = { false };	// ���尡 �ִ� �ִϸ��̼�����
	vector<string>	m_SoundNames;				// ������� ������ �̸�
	vector<_float>	m_SoundTrackPoses;			// ���� ��� TrackPos
	vector<_float>	m_SoundVolumes;				// ���� ����
	_int			m_SoundCursor = { -1 };

public:
	static shared_ptr<CAnimation> Create(ifstream& _ifs, shared_ptr<CModel> _pModel, _float _fSpeed);
	shared_ptr<CAnimation> Clone();

};

END
