#include "UIInstanceBossAbnormal.h"
#include "UIMgr.h"

#include "VIInstanceUI.h"

CUIInstanceBossAbnormal::CUIInstanceBossAbnormal()
{

}

CUIInstanceBossAbnormal::~CUIInstanceBossAbnormal()
{

}

HRESULT CUIInstanceBossAbnormal::Initialize(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CUIBase::UIDesc& _UIDesc, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize)
{
    if (FAILED(Super::Initialize()))
        return E_FAIL;

    m_vUIPos = _vUIPos;
    m_vUISize = _vUISize;

    m_UIDesc = _UIDesc;

    if (FAILED(AddComponent(_strTextureTags, _strMaskTextureTag, _strNoiseTextureTag, _InstanceDesc, _vUIPos, _vUISize)))
        return E_FAIL;

    m_fX = _InstanceDesc.vCenter.x;
    m_fY = _InstanceDesc.vCenter.y;
    m_fSizeX = _InstanceDesc.vSize.x;
    m_fSizeY = _InstanceDesc.vSize.y;

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_UIINSTANCE;
    m_iShaderPass = m_UIDesc.iShaderPassIdx;

    m_vSlotUVRatio.resize(m_vUIPos.size());

    return S_OK;
}

void CUIInstanceBossAbnormal::PriorityTick(_float _fTimeDelta)
{

}

void CUIInstanceBossAbnormal::Tick(_float _fTimeDelta)
{
    KeyInput();

    /* 부모를 따라다니게끔 */
    if (!m_pUIParent.expired() && (m_pUIParent.lock()->GETPOS != m_vPrevParentPos) && (m_vPrevParentPos != _float3(FLT_MAX, FLT_MAX, FLT_MAX)))
    {
        _float3 moveDir = m_pUIParent.lock()->GETPOS - m_vPrevParentPos;
        moveDir.Normalize();
        _float moveDist = SimpleMath::Vector3::Distance(m_pUIParent.lock()->GETPOS, m_vPrevParentPos);

        _float3 newPos = _float3(m_fX, m_fY, 0.f) + (moveDir * moveDist);
        SetUIPos(newPos.x, newPos.y);

        for (auto& texPos : m_vUIPos)
        {
            _float3 newTexPos = _float3(texPos.x, texPos.y, 0.f) + (moveDir * moveDist);
            texPos = _float2(newTexPos.x, newTexPos.y);
        }

        m_BindWorldMat = m_pTransformCom->GetWorldMatrix();
    }
    else
    {
        m_BindWorldMat = m_pTransformCom->GetWorldMatrix();
    }

    if (!m_pUIChildren.empty())
    {
        for (auto& childPair : m_pUIChildren)
        {
            if (childPair.second.lock() != nullptr)
                childPair.second.lock()->Tick(_fTimeDelta);
        }
    }

    if (!m_pUIParent.expired())
        m_vPrevParentPos = m_pUIParent.lock()->GETPOS;
}

void CUIInstanceBossAbnormal::LateTick(_float _fTimeDelta)
{
    if (m_isOnce && m_isWorldUI && !m_pUIChildren.empty())
    {
        m_isOnce = false;

        for (auto& childPair : m_pUIChildren)
        {
            if (childPair.second.lock() != nullptr)
                childPair.second.lock()->SetIsWorldUI(true);
        }
    }

    if (m_isWorldUI)
    {
        if (FAILED(GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_WORLDUI, shared_from_this())))
            return;
    }

    else
    {
        if (FAILED(GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_UI, shared_from_this())))
            return;
    }

    if (!m_pUIChildren.empty())
    {
        for (auto& childPair : m_pUIChildren)
        {
            if (childPair.second.lock() != nullptr)
                childPair.second.lock()->LateTick(_fTimeDelta);
        }
    }

    if (!m_strTexKeys.empty())
    {
        m_iInstanceRenderState = 0;
    }

    if (m_UIDesc.isMasked)
    {
        m_iInstanceRenderState = 1;
    }

    if (m_UIDesc.isNoised)
    {
        m_iInstanceRenderState = 2;
    }

    if (m_strTexKeys.empty() && !m_UIDesc.isMasked && !m_UIDesc.isNoised)
    {
        m_iInstanceRenderState = 3;
    }

    UpdateTexture();
}

HRESULT CUIInstanceBossAbnormal::Render()
{
    if (FAILED(GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->BindWVPMatrixOrthoGraphic(m_BindWorldMat)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->BindRawValue("g_fDiscardAlpha", &m_UIDesc.fDiscardAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->BindRawValue("g_iInstanceRenderState", &m_iInstanceRenderState, sizeof(_uint))))
        return E_FAIL;

    if (!m_strTexKeys.empty())
    {
        _uint iNumTexKeys = static_cast<_uint>(m_strTexKeys.size());
        for (_uint i = 0; i < iNumTexKeys; ++i)
        {
            string shaderKey = "g_DiffuseTexture" + to_string(i);

            if (FAILED(GAMEINSTANCE->BindSRV(shaderKey.c_str(), m_strTexKeys[i])))
                return E_FAIL;
        }
    }

    if (m_UIDesc.isMasked)
    {
        if (FAILED(GAMEINSTANCE->BindSRV("g_MaskTexture", m_strMaskKey)))
            return E_FAIL;
    }

    if (m_UIDesc.isNoised)
    {
        if (FAILED(GAMEINSTANCE->BindSRV("g_NoiseTexture", m_strNoiseKey)))
            return E_FAIL;
    }

    if (FAILED(GAMEINSTANCE->BeginShader()))
        return E_FAIL;

    if (FAILED(m_pInstanceBuffer->BindBuffers()))
        return E_FAIL;

    if (FAILED(m_pInstanceBuffer->Render()))
        return E_FAIL;

    return S_OK;
}

void CUIInstanceBossAbnormal::ResetStates()
{
    m_iGobState = 0;
    m_iOrcState = 0;
    m_iFinalBossState = 0;
}

void CUIInstanceBossAbnormal::UpdateTexture()
{
    D3D11_MAPPED_SUBRESOURCE subResource{};

    ComPtr<ID3D11Buffer> instanceBuffer = m_pInstanceBuffer->GetInstanceBuffer();

    GAMEINSTANCE->GetDeviceContextInfo()->Map(instanceBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);

    VTXUI* vertices = reinterpret_cast<VTXUI*>(subResource.pData);

    _uint iNumSlots = static_cast<_uint>(m_vUIPos.size());

    _int iPickedNum = IsInstanceMouseOn();

    for (_uint i = 0; i < iNumSlots; ++i)
    {
        vertices[i].iSlotUV = 0;

        if (iPickedNum != -1 && iPickedNum == i)
        {
            vertices[i].vRight = _float4(m_vUISize[i].x * 1.1f, 0.f, 0.f, 0.f);
            vertices[i].vUp = _float4(0.f, m_vUISize[i].y * 1.1f, 0.f, 0.f);
        }
        else
        {
            vertices[i].vRight = _float4(m_vUISize[i].x, 0.f, 0.f, 0.f);
            vertices[i].vUp = _float4(0.f, m_vUISize[i].y, 0.f, 0.f);
        }
    }

    for (_uint i = 0; i < iNumSlots; ++i)
    {
        if (m_iGobState == 10 || m_iGobState == 11 || m_iGobState == 12 || m_iOrcState == 9 || m_iOrcState == 10 ||
            m_iOrcState == 11 || m_iFinalBossState == 29 || m_iFinalBossState == 30 || m_iFinalBossState == 31)
        {
            if (vertices[i].iSlotUV != 1)
            {
                vertices[i].iTexIndex = 0; // Down
                vertices[i].iSlotUV = 1;

                break;
            }
        }
    }

    for (_uint i = 0; i < iNumSlots; ++i)
    {
        if (m_iGobState == 26 || m_iOrcState == 25 || m_iFinalBossState == 42)
        {
            if (vertices[i].iSlotUV != 1)
            {
                vertices[i].iTexIndex = 1; // Shock
                vertices[i].iSlotUV = 1;

                break;
            }
        }
    }

    for (_uint i = 0; i < iNumSlots; ++i)
    {
        if (m_iGobState == 61 || m_iGobState == 62 || m_iGobState == 63 || m_iOrcState == 64 || m_iFinalBossState == 74)
        {
            if (vertices[i].iSlotUV != 1)
            {
                vertices[i].iTexIndex = 2; // Stun
                vertices[i].iSlotUV = 1;

                break;
            }
        }
    }

    GAMEINSTANCE->GetDeviceContextInfo()->Unmap(instanceBuffer.Get(), 0);
}

HRESULT CUIInstanceBossAbnormal::AddComponent(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize)
{
    m_pInstanceBuffer = CVIInstanceUI::Create(GAMEINSTANCE->GetDeviceInfo(), GAMEINSTANCE->GetDeviceContextInfo(), _InstanceDesc, _vUIPos);
    if (m_pInstanceBuffer == nullptr)
        return E_FAIL;

    m_strTexKeys = _strTextureTags;

    if (m_UIDesc.isMasked)
        m_strMaskKey = _strMaskTextureTag;

    if (m_UIDesc.isNoised)
        m_strNoiseKey = _strNoiseTextureTag;

    return S_OK;
}

shared_ptr<CUIInstanceBossAbnormal> CUIInstanceBossAbnormal::Create(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CUIBase::UIDesc& _UIDesc, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize)
{
    shared_ptr<CUIInstanceBossAbnormal> pInstance = make_shared<CUIInstanceBossAbnormal>();

    if (FAILED(pInstance->Initialize(_strTextureTags, _strMaskTextureTag, _strNoiseTextureTag, _UIDesc, _InstanceDesc, _vUIPos, _vUISize)))
    {
        MessageBox(nullptr, L"Initialize Failed", L"CUIInstanceBossAbnormal::Create", MB_OK);

        pInstance.reset();

        return nullptr;
    }

    return pInstance;
}
