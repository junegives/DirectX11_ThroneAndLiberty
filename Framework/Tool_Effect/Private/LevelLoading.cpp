#include "LevelLoading.h"
#include "Loader.h"

CLevelLoading::CLevelLoading()
    :CLevel()
{
}

CLevelLoading::~CLevelLoading()
{
}

HRESULT CLevelLoading::Initialize()
{
    /*����� ����*/
 //   if(FAILED())

    m_pLoader = CLoader::Create();

    return S_OK;
}

void CLevelLoading::Tick(_float _fTimeDelta)
{
    if (m_pLoader->isFinished())
        m_pLoader->StartLevel();

}

void CLevelLoading::LateTick(_float _fTimeDelta)
{
}

HRESULT CLevelLoading::Render()
{
    return S_OK;
}

HRESULT CLevelLoading::ReadyNextLevel(_uint _iNextLevel)
{
    /*���� ����*/
    m_pLoader->StartLoading(Tool_Effect::LEVEL(_iNextLevel));


    return S_OK;
}

shared_ptr<CLevelLoading> CLevelLoading::Create()
{
    shared_ptr<CLevelLoading> pInstance = make_shared<CLevelLoading>();

    if (FAILED(pInstance->Initialize()))
        MSG_BOX("Filed to Create: CLevelLoading");

    return pInstance;
}
