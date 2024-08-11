#include "UIInstanceIndicator.h"

#include "GameInstance.h"
#include "VIInstanceUI.h"

CUIInstanceIndicator::CUIInstanceIndicator()
{

}

CUIInstanceIndicator::~CUIInstanceIndicator()
{

}

HRESULT CUIInstanceIndicator::Initialize(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CUIBase::UIDesc& _UIDesc, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize)
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
    m_fSizeY = _InstanceDesc.fSizeY;

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_UIINSTANCE;
    m_iShaderPass = m_UIDesc.iShaderPassIdx;

    m_vSlotUVRatio.resize(m_vUIPos.size());

    PrepareUVs(6.f, 6.f);

    {
        D3D11_MAPPED_SUBRESOURCE subResource{};

        ComPtr<ID3D11Buffer> instanceBuffer = m_pInstanceBuffer->GetInstanceBuffer();

        GAMEINSTANCE->GetDeviceContextInfo()->Map(instanceBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);

        VTXUI* vertices = reinterpret_cast<VTXUI*>(subResource.pData);

        _uint iNumSlots = static_cast<_uint>(m_vUIPos.size());

        vertices[0].iSlotUV = 1; // UV
        vertices[0].vRight = _float4(m_vUISize[0].x, 0.f, 0.f, 0.f);
        vertices[0].vUp = _float4(0.f, m_vUISize[0].y, 0.f, 0.f);
        vertices[1].iSlotUV = 2; // Text
        vertices[2].iSlotUV = 0; // Icon
        vertices[2].vRight = _float4(m_vUISize[2].x, 0.f, 0.f, 0.f);
        vertices[2].vUp = _float4(0.f, m_vUISize[2].y, 0.f, 0.f);

        GAMEINSTANCE->GetDeviceContextInfo()->Unmap(instanceBuffer.Get(), 0);
    }

    SetIsWorldUI(true);

    return S_OK;
}

void CUIInstanceIndicator::PriorityTick(_float _fTimeDelta)
{

}

void CUIInstanceIndicator::Tick(_float _fTimeDelta)
{
    KeyInput();

    m_fTimeAcc += _fTimeDelta;
    if (m_fTimeAcc > 0.02f)
    {
        m_iUVIndex++;
        if (m_iUVIndex >= m_UVs.size())
            m_iUVIndex = 0;

        m_fTimeAcc = 0.f;
    }

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

void CUIInstanceIndicator::LateTick(_float _fTimeDelta)
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

    //UpdateTexture();
}

HRESULT CUIInstanceIndicator::Render()
{
    if (FAILED(GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->BindWVPMatrixOrthoGraphic(m_BindWorldMat)))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->BindRawValue("g_fDiscardAlpha", &m_UIDesc.fDiscardAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(GAMEINSTANCE->BindRawValue("g_iInstanceRenderState", &m_iInstanceRenderState, sizeof(_uint))))
        return E_FAIL;

    _float2 uvTL = _float2(m_UVs[m_iUVIndex].x, m_UVs[m_iUVIndex].y);
    if (FAILED(GAMEINSTANCE->BindRawValue("g_vTexcoordTL", &uvTL, sizeof(_float2))))
        return E_FAIL;

    _float2 uvBR = _float2(m_UVs[m_iUVIndex].z, m_UVs[m_iUVIndex].w);
    if (FAILED(GAMEINSTANCE->BindRawValue("g_vTexcoordBR", &uvBR, sizeof(_float2))))
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

    if (m_UIDesc.strTag == "SubQuestIndicatorUV0" || m_UIDesc.strTag == "SubQuestIndicatorUV1" || m_UIDesc.strTag == "SubQuestIndicatorUV2" || m_UIDesc.strTag == "SubQuestIndicatorUV3")
    {
        if (FAILED(GAMEINSTANCE->RenderFont("Font_SuiteL10", m_strDist, _float2((m_vUIPos[1].x - (m_vUISize[1].x * 0.5f)) + (g_iWinSizeX * 0.5f), (-m_vUIPos[1].y - (m_vUISize[1].y * 0.5f)) + (g_iWinSizeY * 0.5f)), CCustomFont::FA_END, _float4(0.28f, 0.61f, 1.f, 1.f))))
            return E_FAIL;
    }

    else if (m_UIDesc.strTag == "MainQuestIndicatorUV" || m_UIDesc.strTag == "MainQuestIndicatorUV_0")
    {
        if (FAILED(GAMEINSTANCE->RenderFont("Font_SuiteL10", m_strDist, _float2((m_vUIPos[1].x - (m_vUISize[1].x * 0.5f)) + (g_iWinSizeX * 0.5f), (-m_vUIPos[1].y - (m_vUISize[1].y * 0.5f)) + (g_iWinSizeY * 0.5f)), CCustomFont::FA_END, _float4(1.f, 0.45f, 0.93f, 1.f))))
            return E_FAIL;
    }

    return S_OK;
}

void CUIInstanceIndicator::UpdateDistance(_float _fDistance)
{
    wstring strDistance/* = to_wstring(_fDistance) + L" m"*/;

    wstringstream ss;
    ss << std::fixed << std::setprecision(1) << _fDistance;

    strDistance = ss.str() + L" m";

    m_strDist = strDistance;
}

void CUIInstanceIndicator::UpdateTexture()
{
    D3D11_MAPPED_SUBRESOURCE subResource{};

    ComPtr<ID3D11Buffer> instanceBuffer = m_pInstanceBuffer->GetInstanceBuffer();

    GAMEINSTANCE->GetDeviceContextInfo()->Map(instanceBuffer.Get(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &subResource);

    VTXUI* vertices = reinterpret_cast<VTXUI*>(subResource.pData);

    _uint iNumSlots = static_cast<_uint>(m_vUIPos.size());

    GAMEINSTANCE->GetDeviceContextInfo()->Unmap(instanceBuffer.Get(), 0);
}

HRESULT CUIInstanceIndicator::AddComponent(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize)
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

shared_ptr<CUIInstanceIndicator> CUIInstanceIndicator::Create(vector<string>& _strTextureTags, const string& _strMaskTextureTag, const string& _strNoiseTextureTag, CUIBase::UIDesc& _UIDesc, CVIInstancing::InstanceDesc& _InstanceDesc, vector<_float2>& _vUIPos, vector<_float2>& _vUISize)
{
    shared_ptr<CUIInstanceIndicator> pInstance = make_shared<CUIInstanceIndicator>();

    if (FAILED(pInstance->Initialize(_strTextureTags, _strMaskTextureTag, _strNoiseTextureTag, _UIDesc, _InstanceDesc, _vUIPos, _vUISize)))
    {
        MessageBox(nullptr, L"Initialize Failed", L"CUIInstanceIndicator::Create", MB_OK);

        pInstance.reset();

        return nullptr;
    }

    return pInstance;
}
