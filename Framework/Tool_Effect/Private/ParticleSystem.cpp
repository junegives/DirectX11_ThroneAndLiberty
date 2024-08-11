#include "ParticleSystem.h"
#include "VIInstanceParticle.h"

#include "Model.h"

CParticleSystem::CParticleSystem()
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& rhs)
{
}

CParticleSystem::~CParticleSystem()
{
    Safe_Delete_Array(m_pParticles);
}

HRESULT CParticleSystem::Initialize(PARTICLE_SYSTEM* pDesc)
{
    pDesc->iEffectType = EFFECT_PARTICLE;

    __super::Initialize(pDesc);

    m_pDevice = GAMEINSTANCE->GetDeviceInfo();
    m_pContext = GAMEINSTANCE->GetDeviceContextInfo();

    m_pTransformCom->SetTurnSpeed(1.f);
    m_iCurKeyFrameIdx = 0;

    Initialize_Desc(pDesc);

    // ��ƼŬ ���� ����
    m_pParticles = new PARTICLE[m_eParticleSystem.iMaxParticles];

    for (_int i = 0; i < m_eParticleSystem.iMaxParticles; i++)
    {
        m_pParticles[i].bActive = false;
    }

    m_iCurrentParticleCnt = 0;

    // �׽�Ʈ�� �ڵ�
    //SetOwner(GAMEINSTANCE->GetGameObject(LEVEL_LOGO, TEXT("Layer_AnimObject")));

    m_pVIBufferCom = dynamic_pointer_cast<CVIInstanceParticle>(GAMEINSTANCE->GetBuffer("Buffer_InstanceParticle")->Clone(&m_eParticleSystem.iMaxParticles));
    if (m_pVIBufferCom == nullptr)
        return E_FAIL;

    m_bEmitted = false;
    m_bFinished = false;
    m_bCanEmit = true;

    return S_OK;
}

void CParticleSystem::PriorityTick(_float _fTimeDelta)
{
}

void CParticleSystem::Tick(_float _fTimeDelta)
{
    m_ePrevState = m_eState;

    if (!m_bStart)
        return;

    if (m_bFinished)
        return;

    if (!m_bPlaying)
        return;

    m_fSpeedTime = _fTimeDelta * m_fSpeed;
    m_fActiveTime += m_fSpeedTime;

    if (m_bEntered && m_pOwner.lock())
    {
        if (1 == m_eParticleSystem.iPosType)
            m_OwnerWorld = m_pOwner.lock()->GetTransform()->GetWorldMatrix();

        else if (2 == m_eParticleSystem.iPosType)
            m_OwnerWorld = (*m_pFollowBoneMat) * m_pOwner.lock()->GetTransform()->GetWorldMatrix();

        m_bEntered = false;
    }

    // ��ü ��� ������ ��
    if (m_eParticleSystem.fDuration <= m_fActiveTime)
    {
        m_bCanEmit = false;
    }

    // ��ƼŬ �ý��� ���� ����

    // ���� �� �� �ֵ��� bActive = false�� �ٲ��ֱ�
    CheckDeadParticles(m_fSpeedTime);

    // �� �� �߻� ��ƼŬ ����ؼ� �����ϱ�
    if (m_bCanEmit)
        SpawnParticles(m_fSpeedTime);

    // ��ƼŬ ������Ʈ
    UpdateParticles(m_fSpeedTime);

    m_pVIBufferCom->Update(m_pParticles, m_eParticleSystem.iMaxParticles);
}

void CParticleSystem::LateTick(_float _fTimeDelta)
{
    if (!m_bStart)
        return;

    if (m_bFinished)
        return;

    if (!m_bPlaying)
        return;

    __super::LateTick(_fTimeDelta);
}

HRESULT CParticleSystem::Render()
{
    if (m_eParticleSystem.bUsePivot)
    {
        GAMEINSTANCE->ReadyShader(ST_PARTICLE, 2);

        if (m_eEffectDesc.bUseDiffuse)
            GAMEINSTANCE->BindSRV("g_DiffuseTexture", m_strTextureKey[TEX_DIFFUSE]);

        GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());
    }

    else
    {
        GAMEINSTANCE->ReadyShader(ST_PARTICLE, m_iShaderPass);

        if (m_eEffectDesc.bUseDiffuse)
            GAMEINSTANCE->BindSRV("g_DiffuseTexture", m_strTextureKey[TEX_DIFFUSE]);

        GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());
    }

    GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

    _float4 vCamPos = GAMEINSTANCE->GetCamPosition();

    if (GAMEINSTANCE->BindRawValue("g_vCamPosition", &vCamPos, sizeof(_float4)))
        return E_FAIL;

    /*if (GAMEINSTANCE->BindSRV("g_DiffuseTexture", m_eParticleSystem.strTexDiffuseKey))
        return E_FAIL;*/

    if (GAMEINSTANCE->BindRawValue("g_bUseColor", &m_eParticleSystem.bUseColor, sizeof(_bool)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->BeginShader()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CParticleSystem::RenderGlow()
{
    // �����̶� �������� �ƴ϶�, �׳� ���� ���ϰŸ� ���� ������ ���� �ؾߵ�.
    if (CRenderer::RENDER_BLUR == m_eEffectDesc.iRenderGroup)
    {
        if (m_eParticleSystem.bUsePivot)
            GAMEINSTANCE->ReadyShader(ST_PARTICLE, 2);

        else
            GAMEINSTANCE->ReadyShader(ST_PARTICLE, m_eParticleSystem.iShaderPass);
    }

    // �װ� �ƴϸ� ���󰪸� ������ �׷��� �ȴ�. (����ũ, ������? �ϴ� �׷����� ��������)
    else
    {
        _bool bUseBlurColor = m_eParticleSystem.bUseGlowColor;
        _float4 vBlurColor = m_eParticleSystem.vGlowColor;

        GAMEINSTANCE->BindRawValue("g_bUseBlurColor", &bUseBlurColor, sizeof(_bool));
        GAMEINSTANCE->BindRawValue("g_vBlurColor", &vBlurColor, sizeof(_float4));

        if (m_eParticleSystem.bUsePivot)
            GAMEINSTANCE->ReadyShader(ST_PARTICLE, 3);

        else
            GAMEINSTANCE->ReadyShader(ST_PARTICLE, 1);
    }

    GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

    _float4 vCamPos = GAMEINSTANCE->GetCamPosition();

    if (GAMEINSTANCE->BindRawValue("g_vCamPosition", &vCamPos, sizeof(_float4)))
        return E_FAIL;

    if (m_eEffectDesc.bUseDiffuse)
        GAMEINSTANCE->BindSRV("g_DiffuseTexture", m_strTextureKey[TEX_DIFFUSE]);

    if (FAILED(GAMEINSTANCE->BeginShader()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CParticleSystem::Initialize_Desc(PARTICLE_SYSTEM* pDesc)
{
    // Default
    /*m_eParticleSystem.iShaderType = (ESHADER_TYPE)pDesc->iShaderType;
    m_eParticleSystem.iShaderPass = pDesc->iShaderPass;
    m_eParticleSystem.iRenderGroup = pDesc->iRenderGroup;
    m_eParticleSystem.iEffectType = 2;*/

    // Main
    {
        m_eParticleSystem.fDuration = pDesc->fDuration;
        m_eParticleSystem.vLifeTime = pDesc->vLifeTime;
        m_eParticleSystem.bLoop = pDesc->bLoop;
        m_eParticleSystem.iBillBoardType = pDesc->iBillBoardType;
        m_eParticleSystem.bUseDiffuse = true;
        m_eParticleSystem.strTexDiffuseKey = pDesc->strTexDiffuseKey;
        m_eParticleSystem.iSpawnType = pDesc->iSpawnType;
        m_eParticleSystem.fSpawnNumPerSec = (_float)pDesc->fSpawnNumPerSec;
        m_eParticleSystem.StartTrackPos = pDesc->StartTrackPos;
    }

    // Pos
    {
        m_eParticleSystem.iPosType = pDesc->iPosType;
        m_eParticleSystem.bPosFollow = pDesc->bPosFollow;
        m_eParticleSystem.bUsePosKeyFrame = pDesc->bUsePosKeyFrame;

        if (!m_eParticleSystem.bUsePosKeyFrame)
        {
            m_eParticleSystem.vPosMin = pDesc->vPosMin;
            m_eParticleSystem.vPosMax = pDesc->vPosMax;
        }
        m_eParticleSystem.strPosFollowBoneKey = pDesc->strPosFollowBoneKey;
        m_eParticleSystem.iMaxParticles = pDesc->iMaxParticles;
    }

    // Haze
    {
        m_eParticleSystem.bUseHazeEffect = pDesc->bUseHazeEffect;
        m_eParticleSystem.vHazeSpeedMin = pDesc->vHazeSpeedMin;
        m_eParticleSystem.vHazeSpeedMax = pDesc->vHazeSpeedMax;
    }

    // Pivot
    {
        m_eParticleSystem.bUsePivot = pDesc->bUsePivot;
        m_eParticleSystem.vPivotMin = pDesc->vPivotMin;
        m_eParticleSystem.vPivotMax = pDesc->vPivotMax;
    }

    // GlowColor
    {
        m_eParticleSystem.bUseGlowColor = pDesc->bUseGlowColor;
        m_eParticleSystem.vGlowColor = pDesc->vGlowColor;
    }

    // Speed
    {
        m_eParticleSystem.bSpeedMode = pDesc->bSpeedMode;

        // Time Mode
        if (!m_eParticleSystem.bSpeedMode)
        {
            vector<SPEED_KEYFRAME_PARTICLE> speedKeyDesc = {};
            SPEED_KEYFRAME_PARTICLE speedKey = {};

            for (auto& iter : pDesc->KeyFrames.SpeedKeyFrames) {

                speedKey.eEaseType = iter.eEaseType;
                speedKey.fTime = iter.fTime;
                speedKey.vSpeedMin = iter.vSpeedMin;
                speedKey.vSpeedMax = iter.vSpeedMax;

                speedKeyDesc.emplace_back(speedKey);
            }

            m_eParticleSystem.KeyFrames.SpeedKeyFrames = speedKeyDesc;
        }

        // Acc Mode
        else
        {
            m_eParticleSystem.bSpeedMode = pDesc->bSpeedMode;

            m_eParticleSystem.vStartSpeedMin = pDesc->vStartSpeedMin;
            m_eParticleSystem.vStartSpeedMax = pDesc->vStartSpeedMax;

            m_eParticleSystem.vSpeedAccMin = pDesc->vSpeedAccMin;
            m_eParticleSystem.vSpeedAccMax = pDesc->vSpeedAccMax;
        }
    }

    // Scale
    {
        vector<SCALE_KEYFRAME_PARTICLE> scaleKeyDescs = {};
        SCALE_KEYFRAME_PARTICLE scaleKey = {};

        for (auto& iter : pDesc->KeyFrames.ScaleKeyFrames) {

            scaleKey.eEaseType = iter.eEaseType;
            scaleKey.fTime = iter.fTime;

            scaleKey.vScaleMin = iter.vScaleMin;
            scaleKey.vScaleMax = iter.vScaleMax;

            scaleKeyDescs.emplace_back(scaleKey);
        }

        m_eParticleSystem.KeyFrames.ScaleKeyFrames = scaleKeyDescs;
    }

    // Pos

    if (m_eParticleSystem.bUsePosKeyFrame)
    {
        vector<POSITION_KEYFRAME_PARTICLE> PosKeyDescs = {};
        POSITION_KEYFRAME_PARTICLE PosKey = {};

        for (auto& iter : pDesc->KeyFrames.PosKeyFrames) {

            PosKey.eEaseType = iter.eEaseType;
            PosKey.fTime = iter.fTime;

            PosKey.vPosMin = iter.vPosMin;
            PosKey.vPosMax = iter.vPosMax;

            PosKeyDescs.emplace_back(PosKey);
        }

        m_eParticleSystem.KeyFrames.PosKeyFrames = PosKeyDescs;
    }

    // Rotation
    {
        m_eParticleSystem.vRotationMin = pDesc->vRotationMin;
        m_eParticleSystem.vRotationMax = pDesc->vRotationMax;

        m_eParticleSystem.vTurnVelMin = pDesc->vTurnVelMin;
        m_eParticleSystem.vTurnVelMax = pDesc->vTurnVelMax;

        m_eParticleSystem.vTurnAccMin = pDesc->vTurnAccMin;
        m_eParticleSystem.vTurnAccMax = pDesc->vTurnAccMax;
    }

    // Color
    {
        m_eParticleSystem.bUseColor = pDesc->bUseColor;

        if (!m_eParticleSystem.bUseColor)
        {
            m_vStartColorMin = { 0.f, 0.f, 0.f, 0.f };
            m_vStartColorMax = { 0.f, 0.f, 0.f, 0.f };
        }

        else
        {
            vector<CParticleSystem::COLOR_KEYFRAME_PARTICLE> ColorKeyFrames;
            COLOR_KEYFRAME_PARTICLE colorKey = {};


            for (auto& iter : pDesc->KeyFrames.ColorKeyFrames) {

                colorKey.eEaseType = iter.eEaseType;
                colorKey.fTime = iter.fTime;
                colorKey.vColorMin = iter.vColorMin;
                colorKey.vColorMax = iter.vColorMax;

                ColorKeyFrames.emplace_back(colorKey);
            }

            m_eParticleSystem.KeyFrames.ColorKeyFrames = ColorKeyFrames;
        }
    }

    // �ʱⰪ ����
    Reset();

    return S_OK;
}

void CParticleSystem::SpawnParticles(_float _fTimeDelta)
{
    switch (m_eParticleSystem.iSpawnType)
    {
    case 0:
        m_fSpawnTime += _fTimeDelta;

        while (true)
        {
            // ���� �� ���� ����
            // �ƽø� �������� ���� ������ ���� �� ����
            if (m_fSpawnTime >= (1.f / m_eParticleSystem.fSpawnNumPerSec)
                && m_iCurrentParticleCnt < m_eParticleSystem.iMaxParticles)
            {
                m_fSpawnTime -= (1.f / m_eParticleSystem.fSpawnNumPerSec);

                for (_int i = 0; i < m_eParticleSystem.iMaxParticles; i++)
                {
                    // ��ƼŬ ���� �ڵ�
                    if (!m_pParticles[i].bActive)
                    {
                        CreateParticle(i);
                        break;
                    }
                }
            }
            else if (m_iCurrentParticleCnt >= m_eParticleSystem.iMaxParticles)
            {
                m_fSpawnTime = 0.f;
                break;
            }
            else
            {
                break;
            }
        }
        break;
    case 1:
        // ���� ������ 0�� �� ����
        if (0 == m_iCurrentParticleCnt)
        {
            for (_int i = 0; i < m_eParticleSystem.iMaxParticles; i++)
            {
                // ��ƼŬ ���� �ڵ�
                CreateParticle(i);
            }

            if (!m_eParticleSystem.bLoop)
                m_bCanEmit = false;
        }
        break;
    case 2:

        break;
    default:
        break;
    }
}

void CParticleSystem::UpdateParticles(_float _fTimeDelta)
{
    _float3 vNextSpeed;
    _float3 vNextScale;
    _float4 vNextColor;

    for (_int i = 0; i < m_eParticleSystem.iMaxParticles; i++)
    {
        if (!m_pParticles[i].bActive)
            continue;

        m_pParticles[i].fLifeTime -= _fTimeDelta;

        // 0. Pivot
        if (m_eParticleSystem.bUsePivot && !m_eParticleSystem.iBillBoardType)
        {
            m_pParticles[i].vPivot = _float4(m_pParticles[i].vPosition.x, m_pParticles[i].vPosition.y, m_pParticles[i].vPosition.z, 1.f);
        }

        // 1. Speed
        // Ű�����ӿ��� �� �� �� �� ���ǵ� �޾ƿ��� ���
        if (!m_eParticleSystem.bSpeedMode)
        {
            vNextSpeed = m_eParticleSystem.KeyFrames.GetNextSpeed(m_pParticles[i].iSpeedIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime);

            _int iKeyFramesCnt = m_eParticleSystem.KeyFrames.SpeedKeyFrames.size();

            if (0 == iKeyFramesCnt || 1 == iKeyFramesCnt)
            {
            }

            // ������ Ű�������� �� (Ű�������� �ϳ��� ���� �׻� �����)
            else if (vNextSpeed.x == -101.f)
            {
                m_pParticles[i].vCurSpeed = m_eParticleSystem.KeyFrames.GetCurSpeed(m_pParticles[i].iSpeedIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime, m_pParticles[i].vSpeed, m_pParticles[i].vNextSpeed);
            }

            // ���� Ű���������� ���� �ȳѾ�� ��
            else if (vNextSpeed.y == -101.f)
            {
                m_pParticles[i].vCurSpeed = m_eParticleSystem.KeyFrames.GetCurSpeed(m_pParticles[i].iSpeedIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime, m_pParticles[i].vSpeed, m_pParticles[i].vNextSpeed);
            }

            // �� Ű�������� ó�� �Ѿ ����
            else
            {
                m_pParticles[i].iSpeedIdx++;
                m_pParticles[i].vSpeed = m_pParticles[i].vNextSpeed;
                m_pParticles[i].vCurSpeed = m_pParticles[i].vSpeed;
                m_pParticles[i].vNextSpeed = vNextSpeed;
            }
        }

        else
        {
            m_pParticles[i].vCurSpeed += m_pParticles[i].vSpeedAcc * _fTimeDelta;
        }

        // Haze Effect
        if (m_eParticleSystem.bUseHazeEffect)
        {
            m_pParticles[i].fHazeTime += _fTimeDelta;

            if (m_pParticles[i].fHazeTime >= m_pParticles[i].fLifeTime)
            {
                m_pParticles[i].fHazeTime = 0.f;
                m_pParticles[i].vHazeSpeed.x = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vHazeSpeedMin.x, m_eParticleSystem.vHazeSpeedMax.x, true);
                m_pParticles[i].vHazeSpeed.y = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vHazeSpeedMin.y, m_eParticleSystem.vHazeSpeedMax.y, true);
                m_pParticles[i].vHazeSpeed.z = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vHazeSpeedMin.z, m_eParticleSystem.vHazeSpeedMax.z, true);
            }
        }

        m_vCurSpeed = (m_pParticles[i].vCurSpeed + m_pParticles[i].vHazeSpeed) * _fTimeDelta;

        if (m_eParticleSystem.bRelativeOwner)
        {
            m_vCurSpeedMatrix = SimpleMath::Matrix::CreateTranslation(m_vCurSpeed);
            m_vCurSpeed = (m_vCurSpeedMatrix * m_pParticles[i].OwnerMat).Translation();
        }

        // Last. Implosion�� ��� Dir�� �缳���ؼ� �������� m_vCurSpeed�� ���� �����Ͽ� Dir�� �����Ѵ�.
        if (m_eParticleSystem.iBillBoardType == 1)
        {
            m_vCurDir = m_pParticles[i].vPivot - m_pParticles[i].vPosition;
            m_vCurDir.Normalize();

            m_vCurSpeed = _float3(m_vCurDir.x * fabs(m_vCurSpeed.x), m_vCurDir.y * fabs(m_vCurSpeed.x), m_vCurDir.z * fabs(m_vCurSpeed.x));

            m_pParticles[i].vPosition += m_vCurSpeed;

            _float fDist = 0.f;
            _float3 vDist = m_pParticles[i].vPosition - _float3(m_pParticles[i].vPivot.x, m_pParticles[i].vPivot.y, m_pParticles[i].vPivot.z);

            fDist = vDist.Length();

            if (fDist < m_vCurSpeed.Length() || m_vCurSpeed.Length() <= 0.0f || fDist <= (m_pParticles[i].vCurScale.y) / 8.f)
            {
                m_pParticles[i].fLifeTime = 0.f;
            }
        }

        else
            m_pParticles[i].vPosition += m_vCurSpeed;

        // 2. Scale
        vNextScale = m_eParticleSystem.KeyFrames.GetNextScale(m_pParticles[i].iScaleIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime);

        _int iKeyFramesCnt = m_eParticleSystem.KeyFrames.ScaleKeyFrames.size();

        if (0 == iKeyFramesCnt || 1 == iKeyFramesCnt)
        {
        }

        // ������ Ű�������� �� (Ű�������� �ϳ��� ���� �׻� �����)
        else if (vNextScale.x == -101.f)
        {
            m_pParticles[i].vCurScale = m_eParticleSystem.KeyFrames.GetCurScale(m_pParticles[i].iScaleIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime, m_pParticles[i].vScale, m_pParticles[i].vNextScale);
        }

        // ���� Ű���������� ���� �ȳѾ�� ��
        else if (vNextScale.y == -101.f)
        {
            m_pParticles[i].vCurScale = m_eParticleSystem.KeyFrames.GetCurScale(m_pParticles[i].iScaleIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime, m_pParticles[i].vScale, m_pParticles[i].vNextScale);
        }

        // �� Ű�������� ó�� �Ѿ ����
        else
        {
            m_pParticles[i].iScaleIdx++;
            m_pParticles[i].vScale = m_pParticles[i].vNextScale;
            m_pParticles[i].vCurScale = m_pParticles[i].vScale;
            m_pParticles[i].vNextScale = vNextScale;
        }

        // 3. Color
        if (m_eParticleSystem.bUseColor)
        {
            vNextColor = m_eParticleSystem.KeyFrames.GetNextColor(m_pParticles[i].iColorIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime);

            _int iKeyFramesCnt = m_eParticleSystem.KeyFrames.ColorKeyFrames.size();

            if (0 == iKeyFramesCnt || 1 == iKeyFramesCnt)
            {
            }

            // ������ Ű�������� �� (Ű�������� �ϳ��� ���� �׻� �����)
            else if (vNextColor.x == -101.f)
            {
                m_pParticles[i].vCurColor = m_eParticleSystem.KeyFrames.GetCurColor(m_pParticles[i].iColorIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime, m_pParticles[i].vColor, m_pParticles[i].vNextColor);
            }

            // ���� Ű���������� ���� �ȳѾ�� ��
            else if (vNextColor.y == -101.f)
            {
                m_pParticles[i].vCurColor = m_eParticleSystem.KeyFrames.GetCurColor(m_pParticles[i].iColorIdx, m_pParticles[i].fStartLifeTime - m_pParticles[i].fLifeTime, m_pParticles[i].vColor, m_pParticles[i].vNextColor);
            }

            // �� Ű�������� ó�� �Ѿ ����
            else
            {
                m_pParticles[i].iColorIdx++;
                m_pParticles[i].vColor = m_pParticles[i].vNextColor;
                m_pParticles[i].vCurColor = m_pParticles[i].vColor;
                m_pParticles[i].vNextColor = vNextColor;
            }
        }
    }
}

void CParticleSystem::CheckDeadParticles(_float _fTimeDelta)
{
    for (_int i = 0; i < m_eParticleSystem.iMaxParticles; i++)
    {
        if (m_pParticles[i].bActive && 0.f >= m_pParticles[i].fLifeTime)
        {
            m_pParticles[i].bActive = false;
            m_iCurrentParticleCnt -= 1;
        }
    }

    // ��� ��ƼŬ�� �� �Ҹ�Ǿ��� �� ����
    if (!m_bCanEmit)
    {
        if (m_iCurrentParticleCnt <= 0)
        {
            m_bFinished = true;

            // ResetParticles
            m_fActiveTime = 0.f;
            m_bEmitted = false;
            m_bFinished = true;

            // ����
            m_eState = EFFECT_STATE_FINISH;
        }
    }

    //// �ð��� �� ������ ��� ��ƼŬ�� �� �Ҹ�Ǿ��� ��
    //if (m_bFinished)
    //{
    //    if (m_eParticleSystem.bLoop)
    //    {
    //        // ResetParticles
    //        m_fActiveTime = 0.f;
    //        m_bEmitted = false;
    //        m_bFinished = false;
    //        m_bCanEmit = true;

    //        m_eState = EFFECT_STATE_START;
    //    }

    //    else
    //    {
    //        // ResetParticles
    //        m_fActiveTime = 0.f;
    //        m_bEmitted = false;
    //        m_bFinished = true;
    //        m_bCanEmit = true;

    //        // ����
    //        m_eState = EFFECT_STATE_FINISH;
    //    }
    //}
}

void CParticleSystem::CreateParticle(_int _iIndex)
{
    // ��ƼŬ ���� �� ����� �� ��
    // ��ƼŬ ó�� ������ �� ��ġ �������ֱ�
    //if (0 != m_eParticleSystem.iPosType && !m_eParticleSystem.bPosFollow)
    //{
    //    if (!m_pOwner.lock() || !m_pOwner.lock()->IsEnabled())
    //    {
    //        // ���� ��ü�� ������� �� ���� ����
    //        return;
    //    }

    //    m_pTransformCom->SetState(CTransform::STATE_POSITION, m_pOwner.lock()->GetTransform()->GetState(CTransform::STATE_POSITION));
    //}

    // 0. ��ƼŬ ��ġ ����
    if (0 == m_eParticleSystem.iPosType)
    {
        if (m_eParticleSystem.bUsePosKeyFrame)
        {
            _float3 vCurMinPos = m_eParticleSystem.KeyFrames.GetCurMinPos(m_fActiveTime);
            _float3 vCurMaxPos = m_eParticleSystem.KeyFrames.GetCurMaxPos(m_fActiveTime);

            m_pParticles[_iIndex].vPosition.x = GAMEINSTANCE->PickRandomFloat(vCurMinPos.x, vCurMaxPos.x, true);
            m_pParticles[_iIndex].vPosition.y = GAMEINSTANCE->PickRandomFloat(vCurMinPos.y, vCurMaxPos.y, true);
            m_pParticles[_iIndex].vPosition.z = GAMEINSTANCE->PickRandomFloat(vCurMinPos.z, vCurMaxPos.z, true);
        }

        else
        {
            m_pParticles[_iIndex].vPosition.x = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPosMin.x, m_eParticleSystem.vPosMax.x, true);
            m_pParticles[_iIndex].vPosition.y = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPosMin.y, m_eParticleSystem.vPosMax.y, true);
            m_pParticles[_iIndex].vPosition.z = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPosMin.z, m_eParticleSystem.vPosMax.z, true);
        }
    }

    else
    {
        if (m_eParticleSystem.bPosFollow)
        {
            // ���� ��ü�� ������� �� ���� ����
            if (!m_pOwner.lock() || !m_pOwner.lock()->IsEnabled())
                return;

            if (1 == m_eParticleSystem.iPosType)
                m_OwnerWorld = m_pOwner.lock()->GetTransform()->GetWorldMatrix();

            else if (2 == m_eParticleSystem.iPosType)
                m_OwnerWorld = *m_pFollowBoneMat * m_pOwner.lock()->GetTransform()->GetWorldMatrix();
        }

        if (m_eParticleSystem.bUsePosKeyFrame)
        {
            _float3 vCurMinPos = m_eParticleSystem.KeyFrames.GetCurMinPos(m_fActiveTime);
            _float3 vCurMaxPos = m_eParticleSystem.KeyFrames.GetCurMaxPos(m_fActiveTime);

            m_vCurPos.x = GAMEINSTANCE->PickRandomFloat(vCurMinPos.x, vCurMaxPos.x, true);
            m_vCurPos.y = GAMEINSTANCE->PickRandomFloat(vCurMinPos.y, vCurMaxPos.y, true);
            m_vCurPos.z = GAMEINSTANCE->PickRandomFloat(vCurMinPos.z, vCurMaxPos.z, true);
        }

        else
        {
            m_vCurPos.x = (GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPosMin.x, m_eParticleSystem.vPosMax.x, true));
            m_vCurPos.y = (GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPosMin.y, m_eParticleSystem.vPosMax.y, true));
            m_vCurPos.z = (GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPosMin.z, m_eParticleSystem.vPosMax.z, true));
        }

        m_vCurPosMatrix = SimpleMath::Matrix::CreateTranslation(m_vCurPos);
        m_vCurPos = (m_vCurPosMatrix * m_OwnerWorld).Translation();

        m_pParticles[_iIndex].vPosition = m_vCurPos;

        if (m_eParticleSystem.bUsePivot)
        {
            m_vCurPivot.x = (GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPivotMin.x, m_eParticleSystem.vPivotMax.x, true));
            m_vCurPivot.y = (GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPivotMin.y, m_eParticleSystem.vPivotMax.y, true));
            m_vCurPivot.z = (GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vPivotMin.z, m_eParticleSystem.vPivotMax.z, true));

            m_vCurPivotMatrix = SimpleMath::Matrix::CreateTranslation(m_vCurPivot);
            m_vCurPivot = (m_vCurPivotMatrix * m_OwnerWorld).Translation();

            m_pParticles[_iIndex].vPivot = _float4(m_vCurPivot.x, m_vCurPivot.y, m_vCurPivot.z, 1.f);
        }
    }

    m_pParticles[_iIndex].OwnerMat = m_OwnerWorld;
    m_pParticles[_iIndex].OwnerMat.Translation(_float3::Zero);

    // 1. ���� ���� �� ����
    m_pParticles[_iIndex].fLifeTime = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vLifeTime.x, m_eParticleSystem.vLifeTime.y, true);
    m_pParticles[_iIndex].fStartLifeTime = m_pParticles[_iIndex].fLifeTime;

    m_pParticles[_iIndex].iColorIdx = 0;
    m_pParticles[_iIndex].iSpeedIdx = 0;
    m_pParticles[_iIndex].iScaleIdx = 0;

    // 2. �ӵ� ��忡 ����
    if (!m_eParticleSystem.bSpeedMode)
    {
        // 2-0-0. �ӵ� ù��° Ű������ �о �ӵ� ����
        // 2-0-1. �ӵ� ���� �� ����
        m_pParticles[_iIndex].vSpeed.x = GAMEINSTANCE->PickRandomFloat(m_vStartSpeedMin.x, m_vStartSpeedMax.x, true);
        m_pParticles[_iIndex].vSpeed.y = GAMEINSTANCE->PickRandomFloat(m_vStartSpeedMin.y, m_vStartSpeedMax.y, true);
        m_pParticles[_iIndex].vSpeed.z = GAMEINSTANCE->PickRandomFloat(m_vStartSpeedMin.x, m_vStartSpeedMax.z, true);
    }
    else
    {
        // 2-1-0. ��ƼŬ �ӵ� ���� �� ����
        m_pParticles[_iIndex].vSpeed.x = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vStartSpeedMin.x, m_eParticleSystem.vStartSpeedMax.x, true);
        m_pParticles[_iIndex].vSpeed.y = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vStartSpeedMin.y, m_eParticleSystem.vStartSpeedMax.y, true);
        m_pParticles[_iIndex].vSpeed.z = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vStartSpeedMin.z, m_eParticleSystem.vStartSpeedMax.z, true);

        // 2-1-1. ��ƼŬ ���ӵ� ���� �� ����
        m_pParticles[_iIndex].vSpeedAcc.x = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vSpeedAccMin.x, m_eParticleSystem.vSpeedAccMax.x, true);
        m_pParticles[_iIndex].vSpeedAcc.y = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vSpeedAccMin.y, m_eParticleSystem.vSpeedAccMax.y, true);
        m_pParticles[_iIndex].vSpeedAcc.z = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vSpeedAccMin.z, m_eParticleSystem.vSpeedAccMax.z, true);
    }

    m_pParticles[_iIndex].vCurSpeed.x = m_pParticles[_iIndex].vSpeed.x;
    m_pParticles[_iIndex].vCurSpeed.y = m_pParticles[_iIndex].vSpeed.y;
    m_pParticles[_iIndex].vCurSpeed.z = m_pParticles[_iIndex].vSpeed.z;

    m_pParticles[_iIndex].vNextSpeed.x = m_pParticles[_iIndex].vSpeed.x;
    m_pParticles[_iIndex].vNextSpeed.y = m_pParticles[_iIndex].vSpeed.y;
    m_pParticles[_iIndex].vNextSpeed.z = m_pParticles[_iIndex].vSpeed.z;

    // 3. ũ�� ù��° Ű������ �о ũ�� ����
    m_pParticles[_iIndex].vScale.x = GAMEINSTANCE->PickRandomFloat(m_vStartScaleMin.x, m_vStartScaleMax.x, true);
    m_pParticles[_iIndex].vScale.y = GAMEINSTANCE->PickRandomFloat(m_vStartScaleMin.y, m_vStartScaleMax.y, true);
    m_pParticles[_iIndex].vScale.z = GAMEINSTANCE->PickRandomFloat(m_vStartScaleMin.z, m_vStartScaleMax.z, true);

    m_pParticles[_iIndex].vCurScale.x = m_pParticles[_iIndex].vScale.x;
    m_pParticles[_iIndex].vCurScale.y = m_pParticles[_iIndex].vScale.y;
    m_pParticles[_iIndex].vCurScale.z = m_pParticles[_iIndex].vScale.z;

    m_pParticles[_iIndex].vNextScale.x = m_pParticles[_iIndex].vScale.x;
    m_pParticles[_iIndex].vNextScale.y = m_pParticles[_iIndex].vScale.y;
    m_pParticles[_iIndex].vNextScale.z = m_pParticles[_iIndex].vScale.z;

    // 4. ȸ�� ���� �� ����
    /*m_pParticles[_iIndex].vRotation.x = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vRotationMin.x, m_eParticleSystem.vRotationMax.x);
    m_pParticles[_iIndex].vRotation.y = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vRotationMin.y, m_eParticleSystem.vRotationMax.y);
    m_pParticles[_iIndex].vRotation.z = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vRotationMin.z, m_eParticleSystem.vRotationMax.z);*/

    // 5. ȸ�� �ӵ� ���� �� ����
    m_pParticles[_iIndex].vTurnVel.x = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vTurnVelMin.x, m_eParticleSystem.vTurnVelMax.x, true);
    m_pParticles[_iIndex].vTurnVel.y = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vTurnVelMin.y, m_eParticleSystem.vTurnVelMax.y, true);
    m_pParticles[_iIndex].vTurnVel.z = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vTurnVelMin.z, m_eParticleSystem.vTurnVelMax.z, true);

    // 5. ȸ�� ���ӵ� ���� �� ����
    m_pParticles[_iIndex].vTurnAcc.x = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vTurnAccMin.x, m_eParticleSystem.vTurnAccMax.x, true);
    m_pParticles[_iIndex].vTurnAcc.y = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vTurnAccMin.y, m_eParticleSystem.vTurnAccMax.y, true);
    m_pParticles[_iIndex].vTurnAcc.z = GAMEINSTANCE->PickRandomFloat(m_eParticleSystem.vTurnAccMin.z, m_eParticleSystem.vTurnAccMax.z, true);

    // 6. ���� ���� �� ����
    if (m_eParticleSystem.bUseColor)
    {
        // 6. ���� ���� �� ����
        _int iRand = rand() % 2;

        if (0 == iRand)
        {
            m_pParticles[_iIndex].vColor.x = GAMEINSTANCE->PickRandomFloat(m_vStartColorMin.x - 0.01f, m_vStartColorMin.x + 0.01f, true);
            m_pParticles[_iIndex].vColor.y = GAMEINSTANCE->PickRandomFloat(m_vStartColorMin.y - 0.01f, m_vStartColorMin.y + 0.01f, true);
            m_pParticles[_iIndex].vColor.z = GAMEINSTANCE->PickRandomFloat(m_vStartColorMin.z - 0.01f, m_vStartColorMin.z + 0.01f, true);
            m_pParticles[_iIndex].vColor.w = GAMEINSTANCE->PickRandomFloat(m_vStartColorMin.w - 0.01f, m_vStartColorMin.w + 0.01f, true);
        }
        else
        {
            m_pParticles[_iIndex].vColor.x = GAMEINSTANCE->PickRandomFloat(m_vStartColorMax.x - 0.01f, m_vStartColorMax.x + 0.01f, true);
            m_pParticles[_iIndex].vColor.y = GAMEINSTANCE->PickRandomFloat(m_vStartColorMax.y - 0.01f, m_vStartColorMax.y + 0.01f, true);
            m_pParticles[_iIndex].vColor.z = GAMEINSTANCE->PickRandomFloat(m_vStartColorMax.z - 0.01f, m_vStartColorMax.z + 0.01f, true);
            m_pParticles[_iIndex].vColor.w = GAMEINSTANCE->PickRandomFloat(m_vStartColorMax.w - 0.01f, m_vStartColorMax.w + 0.01f, true);
        }

        m_pParticles[_iIndex].vCurColor.x = m_pParticles[_iIndex].vColor.x;
        m_pParticles[_iIndex].vCurColor.y = m_pParticles[_iIndex].vColor.y;
        m_pParticles[_iIndex].vCurColor.z = m_pParticles[_iIndex].vColor.z;
        m_pParticles[_iIndex].vCurColor.w = m_pParticles[_iIndex].vColor.w;

        m_pParticles[_iIndex].vNextColor.x = m_pParticles[_iIndex].vColor.x;
        m_pParticles[_iIndex].vNextColor.y = m_pParticles[_iIndex].vColor.y;
        m_pParticles[_iIndex].vNextColor.z = m_pParticles[_iIndex].vColor.z;
        m_pParticles[_iIndex].vNextColor.w = m_pParticles[_iIndex].vColor.w;
    }

    m_pParticles[_iIndex].bActive = true;
    m_iCurrentParticleCnt++;

    m_bEmitted = true;
}

HRESULT CParticleSystem::SetOwner(shared_ptr<CGameObject> _pOwner)
{
    m_pOwner = _pOwner;

    if (0 != m_eParticleSystem.iPosType)
    {
        if (2 == m_eParticleSystem.iPosType)
        {
            if ("" == m_eParticleSystem.strPosFollowBoneKey)
            {
                m_eParticleSystem.iPosType = 1;
            }

            m_pFollowBoneMat = dynamic_pointer_cast<CModel>(m_pOwner.lock()->GetComponent(L"Com_Model"))->GetCombinedBoneMatrixPtr(m_eParticleSystem.strPosFollowBoneKey.c_str());

            if (!m_pFollowBoneMat)
            {
                m_eParticleSystem.iPosType = 1;
            }
        }
    }

    return S_OK;
}

void CParticleSystem::Reset()
{
    __super::Reset();

    /*if (m_pOwner.lock())
    {
        if (!m_eParticleSystem.bPosFollow)
        {
            m_pTransformCom->SetState(CTransform::STATE_POSITION, m_pOwner.lock()->GetTransform()->GetState(CTransform::STATE_POSITION));
        }

        else
        {
            m_pTransformCom->SetState(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));
        }
    }*/
    m_pTransformCom->SetState(CTransform::STATE_POSITION, _float3(0.f, 0.f, 0.f));

    m_iCurrentParticleCnt = 0;

    if (!m_eParticleSystem.bSpeedMode)
    {
        m_vStartSpeedMin = m_eParticleSystem.KeyFrames.SpeedKeyFrames.begin()->vSpeedMin;
        m_vStartSpeedMax = m_eParticleSystem.KeyFrames.SpeedKeyFrames.begin()->vSpeedMax;
    }

    m_vStartScaleMin = m_eParticleSystem.KeyFrames.ScaleKeyFrames.begin()->vScaleMin;
    m_vStartScaleMax = m_eParticleSystem.KeyFrames.ScaleKeyFrames.begin()->vScaleMax;

    if (m_eParticleSystem.bUsePosKeyFrame)
    {
        m_vStartPosMin = m_eParticleSystem.KeyFrames.PosKeyFrames.begin()->vPosMin;
        m_vStartPosMax = m_eParticleSystem.KeyFrames.PosKeyFrames.begin()->vPosMax;
    }

    if (!m_eParticleSystem.bUseColor)
    {
        m_vStartColorMin = { 0.f, 0.f, 0.f, 0.f };
        m_vStartColorMax = { 0.f, 0.f, 0.f, 0.f };
    }
    else
    {
        m_vStartColorMin = m_eParticleSystem.KeyFrames.ColorKeyFrames.begin()->vColorMin;
        m_vStartColorMax = m_eParticleSystem.KeyFrames.ColorKeyFrames.begin()->vColorMax;
    }

    m_bEmitted = false;
    m_bFinished = false;
    m_bPlaying = true;
    m_bCanEmit = true;
    m_fSpawnTime = 0.f;
    m_bEntered = true;
}

shared_ptr<CParticleSystem> CParticleSystem::Create(PARTICLE_SYSTEM* pDesc)
{
    shared_ptr<CParticleSystem> pInstance = make_shared<CParticleSystem>();

    if (FAILED(pInstance->Initialize(pDesc)))
        MSG_BOX("Failed to Create : CParticleSystem");

    return pInstance;
}