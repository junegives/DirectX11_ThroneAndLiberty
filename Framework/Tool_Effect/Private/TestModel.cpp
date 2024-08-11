#include "TestModel.h"
#include "Model.h"
#include "Texture.h"

#include "AnimModel.h"

CTestModel::CTestModel()
{
}

CTestModel::~CTestModel()
{
}

HRESULT CTestModel::Initialize(const string& _strModelKey)
{
    __super::Initialize(nullptr);

    m_pDevice = GAMEINSTANCE->GetDeviceInfo();
    m_pContext = GAMEINSTANCE->GetDeviceContextInfo();

    /*Shader Info*/
    m_eShaderType = ESHADER_TYPE::ST_EFFECTMESH;
    m_iShaderPass = 0;

    m_pModelCom = GAMEINSTANCE->GetModel(_strModelKey);

    m_eEffectMeshDesc.fMaskUVScroll = { 0.f, 0.f };

    m_eEffectMeshDesc.bBillBoard = 0;
    m_eEffectMeshDesc.iMaskSamplerType = 0;
    m_eEffectMeshDesc.iMaskUVType = 0;
    m_eEffectMeshDesc.fMaskDiscardValue = 0.5f;

    m_eEffectMeshDesc.fMaskUVScroll = { 0.0f, 0.0f };
    m_eEffectMeshDesc.iMaskUVAtlasCnt = { 0.f, 0.f };
    m_eEffectMeshDesc.fMaskUVAtlasSpeed = { 0.f };

    m_eEffectMeshDesc.iNoiseSamplerType = 0;
    m_eEffectMeshDesc.iNoiseUVType = 0;

    m_eEffectMeshDesc.fNoiseUVScroll = { 0.0f, 0.0f };
    m_eEffectMeshDesc.iNoiseUVAtlasCnt = { 0.f, 0.f };
    m_eEffectMeshDesc.fNoiseUVAtlasSpeed = 0.f;

    /*m_eEffectMeshDesc.vColor3 = { 0.f, 0.f, 0.f, 0.f };
    m_eEffectMeshDesc.vColor4 = { 0.f, 0.f, 0.f, 0.f };*/

    m_eEffectMeshDesc.bUseDiffuse = true;
    //m_pTextureCom[TEX_DIFFUSE] = CTexture::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Resources/Level_Static/Effect/Diffuse/PlaceHolder.png"), 1);

    m_eEffectMeshDesc.fDuration = 3.f;

    m_OriginWorldMat = SimpleMath::Matrix::Identity;

    m_pTransformCom->SetTurnSpeed(1.f);

    return S_OK;
}

void CTestModel::PriorityTick(_float _fTimeDelta)
{
}

void CTestModel::Tick(_float _fTimeDelta)
{
    m_fTimeDelta = _fTimeDelta;

    if (/*m_bLoop &*/ m_eEffectMeshDesc.fDuration <= m_fActiveTime)
    {
        m_fActiveTime = 0.f;
        m_iCurColorSetIdx = 0;
        m_iCurScaleSetIdx = 0;
        m_iCurTranslationSetIdx = 0;
    }

    m_fActiveTime += _fTimeDelta;

    if (m_bScaleFollowBone)
    {
        m_pTransformCom->SetScaling(m_pScaleFollowBoneMat->Right().Length(), m_pScaleFollowBoneMat->Up().Length(), m_pScaleFollowBoneMat->Forward().Length());
    }

    else if (m_bScaleFollowObj)
    {
        m_pTransformCom->SetScaling(m_pScaleFollowObj->GetTransform()->GetScaled());
    }

    if (m_bRotationFollowBone)
    {
        m_pTransformCom->LookAtDir(m_pRotationFollowBoneMat->Forward());
    }

    else if (m_bRotationFollowObj)
    {
        m_pTransformCom->LookAtDir(m_pRotationFollowObj->GetTransform()->GetState(CTransform::STATE_LOOK));
    }

    if (m_bTranslationFollowBone)
    {
        m_pTransformCom->SetState(CTransform::STATE_POSITION, m_pTranslationFollowBoneMat->Translation());
    }

    else if (m_bTranslationFollowObj)
    {
        m_pTransformCom->SetState(CTransform::STATE_POSITION, m_pTranslationFollowObj->GetTransform()->GetState(CTransform::STATE_POSITION));
    }

    /*m_pTransformCom->RotationPlus({ 1.f, 0.f, 0.f }, m_fTurnVel.x * _fTimeDelta * m_fActiveTime);
    m_pTransformCom->RotationPlus({ 0.f, 1.f, 0.f }, m_fTurnVel.y * _fTimeDelta * m_fActiveTime);
    m_pTransformCom->RotationPlus({ 0.f, 0.f, 1.f }, m_fTurnVel.z * _fTimeDelta * m_fActiveTime);*/

    m_pTransformCom->Turn({ 1.f, 0.f, 0.f }, m_fTurnVel.x * _fTimeDelta);
    m_pTransformCom->Turn({ 0.f, 1.f, 0.f }, m_fTurnVel.y * _fTimeDelta);
    m_pTransformCom->Turn({ 0.f, 0.f, 1.f }, m_fTurnVel.z * _fTimeDelta);

    m_pTransformCom->RotationPlus({ 1.f, 0.f, 0.f }, m_fTurnAcc.x * _fTimeDelta * m_fActiveTime);
    m_pTransformCom->RotationPlus({ 0.f, 1.f, 0.f }, m_fTurnAcc.y * _fTimeDelta * m_fActiveTime);
    m_pTransformCom->RotationPlus({ 0.f, 0.f, 1.f }, m_fTurnAcc.z * _fTimeDelta * m_fActiveTime);
}                                            
                                             
void CTestModel::LateTick(_float _fTimeDelta)
{
    //GAMEINSTANCE->AddRenderGroup(CRenderer::RENDER_NONLIGHT, shared_from_this());

}

HRESULT CTestModel::Render()
{
    GAMEINSTANCE->ReadyShader(m_eShaderType, m_iShaderPass);

    _float4x4   OriginWorldMat = m_pTransformCom->GetWorldMatrix();

    if (m_bScaleChange)
    {
        if (m_iCurScaleSetIdx < m_iScaleSetNum - 1 && m_fActiveTime >= m_fScaleTime[m_iCurScaleSetIdx + 1])
            m_iCurScaleSetIdx += 1;

        _float3 vCurScale;

        vCurScale.x = GAMEINSTANCE->Ease((EASE_TYPE)m_iScaleEasing[m_iCurScaleSetIdx], m_fActiveTime - m_fScaleTime[m_iCurScaleSetIdx], m_fScaleTime[m_iCurScaleSetIdx + 1] - m_fScaleTime[m_iCurScaleSetIdx], m_fScale[m_iCurScaleSetIdx].x, m_fScale[m_iCurScaleSetIdx + 1].x), sizeof(_float);
        vCurScale.y = GAMEINSTANCE->Ease((EASE_TYPE)m_iScaleEasing[m_iCurScaleSetIdx], m_fActiveTime - m_fScaleTime[m_iCurScaleSetIdx], m_fScaleTime[m_iCurScaleSetIdx + 1] - m_fScaleTime[m_iCurScaleSetIdx], m_fScale[m_iCurScaleSetIdx].y, m_fScale[m_iCurScaleSetIdx + 1].y), sizeof(_float);
        vCurScale.z = GAMEINSTANCE->Ease((EASE_TYPE)m_iScaleEasing[m_iCurScaleSetIdx], m_fActiveTime - m_fScaleTime[m_iCurScaleSetIdx], m_fScaleTime[m_iCurScaleSetIdx + 1] - m_fScaleTime[m_iCurScaleSetIdx], m_fScale[m_iCurScaleSetIdx].z, m_fScale[m_iCurScaleSetIdx + 1].z), sizeof(_float);

        m_pTransformCom->SetScaling(m_pTransformCom->GetScaled() + vCurScale);
    }

    

    if (m_bTranslationChange)
    {
        if (m_iCurTranslationSetIdx < m_iTranslationSetNum - 1 && m_fActiveTime >= m_fTranslationTime[m_iCurTranslationSetIdx + 1])
            m_iCurTranslationSetIdx += 1;

        _float3 vCurTranslation;

        vCurTranslation.x = GAMEINSTANCE->Ease((EASE_TYPE)m_iTranslationEasing[m_iCurTranslationSetIdx], m_fActiveTime - m_fTranslationTime[m_iCurTranslationSetIdx], m_fTranslationTime[m_iCurTranslationSetIdx + 1] - m_fTranslationTime[m_iCurTranslationSetIdx], m_fTranslation[m_iCurTranslationSetIdx].x, m_fTranslation[m_iCurTranslationSetIdx + 1].x), sizeof(_float);
        vCurTranslation.y = GAMEINSTANCE->Ease((EASE_TYPE)m_iTranslationEasing[m_iCurTranslationSetIdx], m_fActiveTime - m_fTranslationTime[m_iCurTranslationSetIdx], m_fTranslationTime[m_iCurTranslationSetIdx + 1] - m_fTranslationTime[m_iCurTranslationSetIdx], m_fTranslation[m_iCurTranslationSetIdx].y, m_fTranslation[m_iCurTranslationSetIdx + 1].y), sizeof(_float);
        vCurTranslation.z = GAMEINSTANCE->Ease((EASE_TYPE)m_iTranslationEasing[m_iCurTranslationSetIdx], m_fActiveTime - m_fTranslationTime[m_iCurTranslationSetIdx], m_fTranslationTime[m_iCurTranslationSetIdx + 1] - m_fTranslationTime[m_iCurTranslationSetIdx], m_fTranslation[m_iCurTranslationSetIdx].z, m_fTranslation[m_iCurTranslationSetIdx + 1].z), sizeof(_float);

        m_pTransformCom->SetState(CTransform::STATE_POSITION, m_pTransformCom->GetState(CTransform::STATE_POSITION) + vCurTranslation);
    }

    GAMEINSTANCE->BindWVPMatrixPerspective(m_pTransformCom->GetWorldMatrix());

    m_pTransformCom->SetWorldMatrix(OriginWorldMat);

    _uint iNumMeshes = m_pModelCom->GetNumMeshes();

    if (m_eEffectMeshDesc.bUseMask)
    {
        if (1 == m_eEffectMeshDesc.iMaskUVType)
        {
            // ������ ����ؼ� �ѱ��
            m_eEffectMeshDesc.fMaskUVScroll.x += GAMEINSTANCE->PickRandomFloat(m_vMaskScrollRangeX.x, m_vMaskScrollRangeX.y);
            m_eEffectMeshDesc.fMaskUVScroll.y += GAMEINSTANCE->PickRandomFloat(m_vMaskScrollRangeY.x, m_vMaskScrollRangeY.y);

            /*if (1.f <= m_eEffectMeshDesc.fMaskUVScroll.x)
                m_eEffectMeshDesc.fMaskUVScroll.x -= 1.f;

            if (1.f <= m_eEffectMeshDesc.fMaskUVScroll.y)
                m_eEffectMeshDesc.fMaskUVScroll.y -= 1.f;*/
        }
    }

    if (m_eEffectMeshDesc.bUseNoise)
    {
        if (1 == m_eEffectMeshDesc.iNoiseUVType)
        {
            // ������ ����ؼ� �ѱ��
            m_eEffectMeshDesc.fNoiseUVScroll.x += GAMEINSTANCE->PickRandomFloat(m_vNoiseScrollRangeX.x, m_vNoiseScrollRangeX.y);
            m_eEffectMeshDesc.fNoiseUVScroll.y += GAMEINSTANCE->PickRandomFloat(m_vNoiseScrollRangeY.x, m_vNoiseScrollRangeY.y);

            /*if (1.f <= m_eEffectMeshDesc.fNoiseUVScroll.x)
                m_eEffectMeshDesc.fNoiseUVScroll.x -= 1.f;

            if (1.f <= m_eEffectMeshDesc.fNoiseUVScroll.y)
                m_eEffectMeshDesc.fNoiseUVScroll.y -= 1.f;*/
        }

        if (2 == m_eEffectMeshDesc.iNoiseUVType)
        {
            // ���� ���� �ִ� ������
            // UV ������ UV ���ǵ�

            // FrameTime, �� �� ������ �Ѿ�� �ð��� ����������
            // ActiveTime�� FrameTime * FrameIdx�� �Ѿ�� �� ���� ���������� �Ѱ��ش�.
            // x �������� ���� �Ѱܺ��� 1�� �Ѿ�� x�� 0���� �ٲ��ְ� y�������� ������Ų��.

            if (m_fActiveTime > 1.f / m_eEffectMeshDesc.fNoiseUVAtlasSpeed * m_fTimeDelta * m_iNoiseUVIdx)
            {
                m_iNoiseUVIdx++;
                m_vNoiseUVPivot.x += (1.f / m_eEffectMeshDesc.iNoiseUVAtlasCnt.x);

                if (1 <= m_vNoiseUVPivot.x)
                {
                    m_vNoiseUVPivot.x = 0;
                    m_vNoiseUVPivot.y += (1.f / m_eEffectMeshDesc.iNoiseUVAtlasCnt.y);
                }
            }
        }
    }

    if (m_eEffectMeshDesc.bUseColor)
    {
        // ���� ��� �ð��� ���� Color Set�� �ð��� �Ѿ����� Ȯ��
        /*while (m_fActiveTime <= m_fColorTime[m_iCurColorSetIdx + 1])
        {*/
        //// ������ �÷� ��Ʈ������?
        //if (m_iColorSetNum <= m_iCurColorSetIdx)
        //{
        //    // ���� �ڵ�� �� ���� ������??
        //    // �ֳĸ� ������ �÷� ��Ʈ�� ��ȭ �ð��� ����Ʈ ���� �ð��̱� ����
        //    int a = 3;
        //    m_fActiveTime = 0;
        //    return;
        //}

        // �Ѿ����� Color Set Idx �Ѱ��ֱ�
        if (m_iCurColorSetIdx < m_iColorSetNum - 1 && m_fActiveTime >= m_fColorTime[m_iCurColorSetIdx + 1])
            m_iCurColorSetIdx += 1;

        // ������ ���� ���� �����ֱ�
        m_eEffectMeshDesc.iColorSplitNum = m_iColorSplitAreaNum;

        // ������ ���ذ��� ������ ���� ���� �����ֱ�
        // �ñ����� 1. ������ ���ذ��� 4����, ������ ���ذ��� ����� �� ���� �������ϱ�..
        // �ֳĸ�, ������ �÷��� ���ذ� ~ 1 ������ �Ǿ������ʳ�? �ƴϸ� �׷� 
        // �ƹ����� ���Ե��� �ʴ� ������ discard ���ѹ����� �ǰڴ�. (�Ǵ� a���� 0���� �ϸ� �ǰڴ�. �׷� Ÿ�� ��Ʈ ���� ������ ���ذ��� �޶� �ǰ���?
        // ������ ������ ���̴����� ������ϳ�..? �ٵ� r���� �������� �ڸ��� �׷����� �� ���⵵ ��.....
        // �ϴ� �غ���
        m_eEffectMeshDesc.vColorSplitter.x = m_vColorSplitter[0];
        m_eEffectMeshDesc.vColorSplitter.y = m_vColorSplitter[1];
        m_eEffectMeshDesc.vColorSplitter.z = m_vColorSplitter[2];
        m_eEffectMeshDesc.vColorSplitter.w = m_vColorSplitter[3];
        //memcpy(m_eEffectMeshDesc.vCurColorSplitter, m_vColorSplitter, sizeof(_float) * 4);

        // NextColorSet�� �����ϸ�, �����ؼ� m_vCurColor ����ؼ� �����ֱ�
        if (m_iCurColorSetIdx < m_iColorSetNum - 1)
        {
            for (int iColorSplitArea = 0; iColorSplitArea < m_iColorSplitAreaNum; iColorSplitArea++)
            {
                if (0 == iColorSplitArea)
                {
                    m_eEffectMeshDesc.vColor1.x = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].x, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].x), sizeof(_float);
                    m_eEffectMeshDesc.vColor1.y = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].y, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].y), sizeof(_float);
                    m_eEffectMeshDesc.vColor1.z = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].z, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].z), sizeof(_float);
                    m_eEffectMeshDesc.vColor1.w = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].w, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].w), sizeof(_float);
                }
                else if (1 == iColorSplitArea)
                {
                    m_eEffectMeshDesc.vColor2.x = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].x, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].x), sizeof(_float);
                    m_eEffectMeshDesc.vColor2.y = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].y, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].y), sizeof(_float);
                    m_eEffectMeshDesc.vColor2.z = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].z, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].z), sizeof(_float);
                    m_eEffectMeshDesc.vColor2.w = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].w, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].w), sizeof(_float);
                }
                else if (2 == iColorSplitArea)
                {
                    m_eEffectMeshDesc.vColor3.x = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].x, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].x), sizeof(_float);
                    m_eEffectMeshDesc.vColor3.y = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].y, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].y), sizeof(_float);
                    m_eEffectMeshDesc.vColor3.z = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].z, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].z), sizeof(_float);
                    m_eEffectMeshDesc.vColor3.w = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].w, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].w), sizeof(_float);
                }
                else if (3 == iColorSplitArea)
                {
                    m_eEffectMeshDesc.vColor4.x = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].x, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].x), sizeof(_float);
                    m_eEffectMeshDesc.vColor4.y = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].y, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].y), sizeof(_float);
                    m_eEffectMeshDesc.vColor4.z = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].z, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].z), sizeof(_float);
                    m_eEffectMeshDesc.vColor4.w = GAMEINSTANCE->Ease(m_eColorEasingType[m_iCurColorSetIdx], m_fActiveTime - m_fColorTime[m_iCurColorSetIdx], m_fColorTime[m_iCurColorSetIdx + 1] - m_fColorTime[m_iCurColorSetIdx], m_vColor[m_iCurColorSetIdx][iColorSplitArea].w, m_vColor[m_iCurColorSetIdx + 1][iColorSplitArea].w), sizeof(_float);
                }
            }
        }

        else
        {
            for (int iColorSplitArea = 0; iColorSplitArea < m_iColorSplitAreaNum; iColorSplitArea++)
            {
                if (0 == iColorSplitArea)
                {
                    m_eEffectMeshDesc.vColor1.x = m_vColor[m_iCurColorSetIdx][iColorSplitArea].x;
                    m_eEffectMeshDesc.vColor1.y = m_vColor[m_iCurColorSetIdx][iColorSplitArea].y;
                    m_eEffectMeshDesc.vColor1.z = m_vColor[m_iCurColorSetIdx][iColorSplitArea].z;
                    m_eEffectMeshDesc.vColor1.w = m_vColor[m_iCurColorSetIdx][iColorSplitArea].w;
                }
                else if (1 == iColorSplitArea)
                {
                    m_eEffectMeshDesc.vColor2.x = m_vColor[m_iCurColorSetIdx][iColorSplitArea].x;
                    m_eEffectMeshDesc.vColor2.y = m_vColor[m_iCurColorSetIdx][iColorSplitArea].y;
                    m_eEffectMeshDesc.vColor2.z = m_vColor[m_iCurColorSetIdx][iColorSplitArea].z;
                    m_eEffectMeshDesc.vColor2.w = m_vColor[m_iCurColorSetIdx][iColorSplitArea].w;
                }
                else if (2 == iColorSplitArea)
                {
                    m_eEffectMeshDesc.vColor3.x = m_vColor[m_iCurColorSetIdx][iColorSplitArea].x;
                    m_eEffectMeshDesc.vColor3.y = m_vColor[m_iCurColorSetIdx][iColorSplitArea].y;
                    m_eEffectMeshDesc.vColor3.z = m_vColor[m_iCurColorSetIdx][iColorSplitArea].z;
                    m_eEffectMeshDesc.vColor3.w = m_vColor[m_iCurColorSetIdx][iColorSplitArea].w;
                }
                else if (3 == iColorSplitArea)
                {
                    m_eEffectMeshDesc.vColor4.x = m_vColor[m_iCurColorSetIdx][iColorSplitArea].x;
                    m_eEffectMeshDesc.vColor4.y = m_vColor[m_iCurColorSetIdx][iColorSplitArea].y;
                    m_eEffectMeshDesc.vColor4.z = m_vColor[m_iCurColorSetIdx][iColorSplitArea].z;
                    m_eEffectMeshDesc.vColor4.w = m_vColor[m_iCurColorSetIdx][iColorSplitArea].w;
                }
            }
        }
    }
    
    if (m_eEffectMeshDesc.bUseDiffuse)
        GAMEINSTANCE->BindSRV("g_DiffuseTexture", m_pTextureKey[TEX_DIFFUSE]);
        //m_pTextureCom[TEX_DIFFUSE]->BindShaderResource("g_DiffuseTexture", 0);

    if (m_eEffectMeshDesc.bUseMask)
        GAMEINSTANCE->BindSRV("g_MaskTexture", m_pTextureKey[TEX_MASK]);
    //m_pTextureCom[TEX_MASK]->BindShaderResource("g_MaskTexture", 0);

    if (m_eEffectMeshDesc.bUseNoise)
        GAMEINSTANCE->BindSRV("g_NoiseTexture", m_pTextureKey[TEX_NOISE]);
    //m_pTextureCom[TEX_NOISE]->BindShaderResource("g_NoiseTexture", 0);

    if (m_eEffectMeshDesc.bUseColor)
        GAMEINSTANCE->BindRawValue("g_fActiveTime", &m_fActiveTime, sizeof(_float));

    if (2 == m_eEffectMeshDesc.iNoiseUVType)
    {
        GAMEINSTANCE->BindRawValue("g_vNoiseUVPivot", &m_vNoiseUVPivot, sizeof(_float2));
    }

    GAMEINSTANCE->BindRawValue("g_EffectDesc", &m_eEffectMeshDesc, sizeof(EffectMeshDesc));

    for (size_t i = 0; i < iNumMeshes; i++) {
        GAMEINSTANCE->BeginNonAnimModel(m_pModelCom, (_uint)i);
    }

    return S_OK;
}

HRESULT CTestModel::SetUseTexture(TEX_TYPE _eTexType, _bool _bUseTexture)
{
    _bool bUseTexture = _bUseTexture;

    if (bUseTexture && m_pTextureKey[_eTexType] == "")
        bUseTexture = false;

    switch (_eTexType)
    {
    case Tool_Effect::TEX_DIFFUSE:
        m_eEffectMeshDesc.bUseDiffuse = bUseTexture;
        break;
    case Tool_Effect::TEX_MASK:
        m_eEffectMeshDesc.bUseMask = bUseTexture;
        break;
    case Tool_Effect::TEX_NOISE:
        m_eEffectMeshDesc.bUseNoise = bUseTexture;
        break;
    case Tool_Effect::TEX_END:
        m_eEffectMeshDesc.bUseColor = _bUseTexture;
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetTexture(TEX_TYPE _eTexType, const string& _strTextureTag)
{
    //m_pTextureCom[_eTexType].reset();
    m_pTextureKey[_eTexType] = "";
    m_pTextureKey[_eTexType] = _strTextureTag;
    //m_pTextureCom[_eTexType] = GAMEINSTANCE->GetTexture(_strTextureTag);

    return S_OK;
}

HRESULT CTestModel::SetTexSampler(TEX_TYPE _eTexType, SAMPLE_TYPE _eSampleType)
{
    switch (_eTexType)
    {
    case Tool_Effect::TEX_DIFFUSE:
        break;
    case Tool_Effect::TEX_MASK:
        m_eEffectMeshDesc.iMaskSamplerType = (_int)_eSampleType;
        break;
    case Tool_Effect::TEX_NOISE:
        m_eEffectMeshDesc.iNoiseSamplerType = (_int)_eSampleType;
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetTexUV(TEX_TYPE _eTexType, UV_TYPE _eUVType)
{
    switch (_eTexType)
    {
    case Tool_Effect::TEX_DIFFUSE:
        break;
    case Tool_Effect::TEX_MASK:
        m_eEffectMeshDesc.iMaskUVType = (_int)_eUVType;
        break;
    case Tool_Effect::TEX_NOISE:
        m_eEffectMeshDesc.iNoiseUVType = (_int)_eUVType;
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetTexScrollOpt(TEX_TYPE _eTexType, _float2 _vScrollX, _float2 _vScrollY)
{
    switch (_eTexType)
    {
    case Tool_Effect::TEX_DIFFUSE:
        break;
    case Tool_Effect::TEX_MASK:
        m_vMaskScrollRangeX = _vScrollX;
        m_vMaskScrollRangeY = _vScrollY;
        m_eEffectMeshDesc.fMaskUVScroll.x = 0;
        m_eEffectMeshDesc.fMaskUVScroll.y = 0;
        break;
    case Tool_Effect::TEX_NOISE:
        m_vNoiseScrollRangeX = _vScrollX;
        m_vNoiseScrollRangeY = _vScrollY;
        m_eEffectMeshDesc.fNoiseUVScroll.x = 0;
        m_eEffectMeshDesc.fNoiseUVScroll.y = 0;
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetTexAtlasOpt(TEX_TYPE _eTexType, _float2 _vAtlasCnt, _float _fAtalsSpeed)
{
    switch (_eTexType)
    {
    case Tool_Effect::TEX_DIFFUSE:
        break;
    case Tool_Effect::TEX_MASK:
        m_eEffectMeshDesc.iMaskUVAtlasCnt = _vAtlasCnt;
        m_eEffectMeshDesc.fMaskUVAtlasSpeed = _fAtalsSpeed;
        break;
    case Tool_Effect::TEX_NOISE:
        m_eEffectMeshDesc.iNoiseUVAtlasCnt = _vAtlasCnt;
        m_eEffectMeshDesc.fNoiseUVAtlasSpeed = _fAtalsSpeed;
        m_iNoiseUVIdx = 0;
        m_vNoiseUVPivot = { 0.f, 0.f };
        m_fActiveTime = 0.f;
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetTexDiscardOpt(TEX_TYPE _eTexType, _float _fDiscardValue, _bool _bLessThan)
{
    switch (_eTexType)
    {
    case Tool_Effect::TEX_DIFFUSE:
        break;
    case Tool_Effect::TEX_MASK:
        m_eEffectMeshDesc.fMaskDiscardValue = _fDiscardValue;
        m_eEffectMeshDesc.bMaskDiscardArea = _bLessThan;
        break;
    case Tool_Effect::TEX_NOISE:
        m_eEffectMeshDesc.fNoiseDiscardValue = _fDiscardValue;
        m_eEffectMeshDesc.bNoiseDiscardArea = _bLessThan;
        break;
    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetColorSetNum(_bool _bIncrase)
{
    m_iCurColorSetIdx = 0;
    m_fActiveTime = 0.f;

    if (_bIncrase)
        m_iColorSetNum++;

    else
        m_iColorSetNum--;

    return S_OK;
}

HRESULT CTestModel::SetColorChangeTime(_int _iColorSetIdx, _float _fColorChangeTime)
{
    m_iCurColorSetIdx = 0;
    m_fActiveTime = 0.f;

    m_fColorTime[_iColorSetIdx] = _fColorChangeTime;
    //m_eEffectMeshDesc.fDuration = m_fColorTime[m_iColorSetNum - 1];

    return S_OK;
}

HRESULT CTestModel::SetColorSetEasing(_int _iColorSetIdx, _int _iEaseType)
{
    m_iCurColorSetIdx = 0;
    m_fActiveTime = 0.f;

    m_eColorEasingType[_iColorSetIdx] = (EASE_TYPE)_iEaseType;

    return S_OK;
}

HRESULT CTestModel::SetColorSplitNum(_int _iColorSplitNum)
{
    m_iCurColorSetIdx = 0;

    m_iColorSplitAreaNum = _iColorSplitNum;

    return S_OK;
}

HRESULT CTestModel::SetColorSplitter(_float _vColorSplitter[4])
{
    m_iCurColorSetIdx = 0;

    memcpy(m_vColorSplitter, _vColorSplitter, sizeof(_float) * 4);

    return S_OK;
}

HRESULT CTestModel::SetColor(_int _iColorSetIdx, _int _iColorTimeIdx, _float4 _vColor)
{
    m_iCurColorSetIdx = 0;

    m_vColor[_iColorSetIdx][_iColorTimeIdx] = _vColor;

    return S_OK;
}

HRESULT CTestModel::SetTransformSetNum(_int _iTransformType, _int _iSetNum)
{
    switch (_iTransformType)
    {
        // Scale
    case 0:
        m_iScaleSetNum = _iSetNum;
        if (1 < m_iScaleSetNum)
            m_bScaleChange = true;
        else m_bScaleChange = false;
        break;

        // Rotation
    case 1:
        m_iRotationSetNum = _iSetNum;
        if (1 < m_iRotationSetNum)
            m_bRotationChange = true;
        else m_bRotationChange = false;
        break;

        // Translation
    case 2:
        m_iTranslationSetNum = _iSetNum;
        if (1 < m_iTranslationSetNum)
            m_bTranslationChange = true;
        else m_bTranslationChange = false;
        break;

    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetTransformTime(_int _iTransformType, _int iIdx, _float _fTransformTime)
{
    switch (_iTransformType)
    {
        // Scale
    case 0:
        m_fScaleTime[iIdx] = _fTransformTime;
        break;

        // Rotation
    case 1:
        break;

        // Translation
    case 2:
        m_fTranslationTime[iIdx] = _fTransformTime;
        break;

    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetTransformEasing(_int _iTransformType, _int iIdx, _int _iTransformEasing)
{
    switch (_iTransformType)
    {
        // Scale
    case 0:
        m_iScaleEasing[iIdx] = _iTransformEasing;
        break;

        // Rotation
    case 1:
        break;

        // Translation
    case 2:
        m_iTranslationEasing[iIdx] = _iTransformEasing;
        break;

    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetTransformValue(_int _iTransformType, _int iIdx, _float* _fTransformValue)
{
    switch (_iTransformType)
    {
        // Scale
    case 0:
        m_fScale[iIdx].x = _fTransformValue[0];
        m_fScale[iIdx].x = _fTransformValue[1];
        m_fScale[iIdx].x = _fTransformValue[2];
        break;

        // Rotation
    case 1:
        if (0 == iIdx)
        {
            m_fRotation.x = XMConvertToRadians(_fTransformValue[0]);
            m_fRotation.y = XMConvertToRadians(_fTransformValue[1]);
            m_fRotation.z = XMConvertToRadians(_fTransformValue[2]);

            m_pTransformCom->RotationAll(m_fRotation.x, m_fRotation.y, m_fRotation.z);
        }

        else if (1 == iIdx)
        {
            m_fTurnVel.x = XMConvertToRadians(_fTransformValue[0]);
            m_fTurnVel.y = XMConvertToRadians(_fTransformValue[1]);
            m_fTurnVel.z = XMConvertToRadians(_fTransformValue[2]);
        }

        else
        {
            m_fTurnAcc.x = XMConvertToRadians(_fTransformValue[0]);
            m_fTurnAcc.y = XMConvertToRadians(_fTransformValue[1]);
            m_fTurnAcc.z = XMConvertToRadians(_fTransformValue[2]);
        }
        break;

        // Translation
    case 2:
        m_fTranslation[iIdx].x = _fTransformValue[0];
        m_fTranslation[iIdx].x = _fTransformValue[1];
        m_fTranslation[iIdx].x = _fTransformValue[2];
        break;

    default:
        break;
    }

    return S_OK;
}

HRESULT CTestModel::SetFollowObj(_int _iTransformType, _int _iFollowType, shared_ptr<class CAnimModel> _pAnimModel, _float4x4* _pBoneMatrix)
{
    // Follow ���
    switch (_iTransformType)
    {
    // Scale
    case 0:
        if (-1 == _iFollowType)
        {
            m_bScaleFollowObj = false;
            m_bScaleFollowBone = false;
            m_pScaleFollowObj = nullptr;
            m_pScaleFollowBoneMat = nullptr;
            break;
        }
        // Object
        else if (0 == _iFollowType)
        {
            m_bScaleFollowObj = true;
            m_bScaleFollowBone = false;
            m_pScaleFollowObj = _pAnimModel;
            m_pScaleFollowBoneMat = nullptr;
        }

        else
        {
            m_bScaleFollowObj = false;
            m_bScaleFollowBone = true;
            m_pScaleFollowObj = _pAnimModel;
            m_pScaleFollowBoneMat = _pBoneMatrix;
        }

        break;

    // Rotation
    case 1:
        if (-1 == _iFollowType)
        {
            m_bRotationFollowObj = false;
            m_bRotationFollowBone = false;
            m_pRotationFollowObj = nullptr;
            m_pRotationFollowBoneMat = nullptr;
            break;
        }
        // Object
        else if (0 == _iFollowType)
        {
            m_bRotationFollowObj = true;
            m_bRotationFollowBone = false;
            m_pRotationFollowObj = _pAnimModel;
            m_pRotationFollowBoneMat = nullptr;
        }

        else
        {
            m_bRotationFollowObj = false;
            m_bRotationFollowBone = true;
            m_pRotationFollowObj = _pAnimModel;
            m_pRotationFollowBoneMat = _pBoneMatrix;
        }
        break;

    // Translation
    case 2:
        if (-1 == _iFollowType)
        {
            m_bTranslationFollowObj = false;
            m_bTranslationFollowBone = false;
            m_pTranslationFollowObj = nullptr;
            m_pTranslationFollowBoneMat = nullptr;
            break;
        }
        // Object
        else if (0 == _iFollowType)
        {
            m_bTranslationFollowObj = true;
            m_bTranslationFollowBone = false;
            m_pTranslationFollowObj = _pAnimModel;
            m_pTranslationFollowBoneMat = nullptr;
        }

        else
        {
            m_bTranslationFollowObj = false;
            m_bTranslationFollowBone = true;
            m_pTranslationFollowObj = _pAnimModel;
            m_pTranslationFollowBoneMat = _pBoneMatrix;
        }
        break;

    default:
        break;
    }


    return S_OK;
}

shared_ptr<CTestModel> CTestModel::Create(const string& _strModelKey)
{
    shared_ptr<CTestModel> pInstance = make_shared<CTestModel>();

    if (FAILED(pInstance->Initialize(_strModelKey)))
        MSG_BOX("Failed to Create : CTestModel");

    return pInstance;
}
