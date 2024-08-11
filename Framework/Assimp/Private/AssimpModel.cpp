#include "AssimpModel.h"

#include "Texture.h"

#include <fstream>
#include <iostream>

#include "AssimpBone.h"
#include "AssimpMesh.h"


CAssimpModel::CAssimpModel()
{
}

CAssimpModel::~CAssimpModel()
{
}

/*_iType : 0 = Anim  1 = NonAnim*/
void CAssimpModel::ConvertModel(const string& _strFilePath, _uint _iType, const wstring& _strSaveFileName, const wstring& _strFileName)
{

	m_strObjName = _strFileName;

	wstring strSavePath = TEXT("");

	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;
	Assimp::Importer Importer;

	_bool IsAnim = true;

	/*NonAnim*/
	if (1 == _iType) {
		iFlag |= aiProcess_PreTransformVertices;
		IsAnim = false;
	}

	pAIScene = Importer.ReadFile(_strFilePath.c_str(), iFlag);

	//m_Bones

	if (!pAIScene)
		return;

	ofstream out(_strSaveFileName, ios::binary);

	/*�ִ� ��ִ� ����*/
	out.write((char*)&IsAnim, sizeof(IsAnim));


	if (1 == _iType) {
		ReadyMesh(out);
		ReadyMaterial(out);
	}
	else if (0 == _iType) {

		/*��*/
		ReadBoneData(pAIScene->mRootNode, out, -1);

		size_t iSize = m_Bones.size();
		out.write((char*)&iSize, sizeof(size_t));

		for (auto& iter : m_Bones) {
			iter->ParsingBone(out);
		}


		/*�ִԸ޽�*/

		_int m_iNumMeshes = pAIScene->mNumMeshes;

		for (size_t i = 0; i < m_iNumMeshes; i++) {

			shared_ptr<CAssimpMesh> pMesh = CAssimpMesh::Create(_iType,nullptr, nullptr, pAIScene->mMeshes[i], shared_from_this(), XMMatrixIdentity());

			if (!pMesh)
				return;

			m_Meshes.push_back(pMesh);
		}

		
		size_t MeshSize = m_Meshes.size();
		out.write((char*)&MeshSize, sizeof(size_t));

		for (auto& iter : m_Meshes) {
			iter->ParsingAnimMeshDatat(out);
		}

		ReadyMaterial(out);
		ReadyAnim(out);

	}

	out.close();
}

void CAssimpModel::ReadyBones(ofstream& out)
{

	//aiNode* pNode = pAIScene->mRootNode;
	//_int iParentBoneIdx = -1;
	//
	//ReadBoneData(pNode, out, iParentBoneIdx); 

	///*��ü �� ����*/
	//size_t iSize = m_Bones.size();

	//out.write((char*)&iSize, sizeof(size_t));

	//string strName;

	//char strTemp[MAX_PATH];

	///*���� ���� �̸�, �θ� �� �ε���, Ʈ������ ��Ʈ���� ���*/
	//for (auto& iter : m_Bones) {

	//	strcpy_s(strTemp, iter.strBoneName.c_str());

	//	out.write((char*)strTemp, MAX_PATH * sizeof(char));
	//	out.write((char*)&iter.iParentBoneIndex, sizeof(_int));
	//	out.write((char*)&iter.TransformMatrix, sizeof(_float4x4));

	//}
}

void CAssimpModel::ReadBoneData(aiNode* _pNode, ofstream& out , _int _iParentBoneIdx)
{

	shared_ptr<CAssimpBone> pBone = CAssimpBone::Create(_pNode, _iParentBoneIdx);

	if (!pBone)
		return;

	m_Bones.push_back(pBone);

	_uint iParentIndex = (_uint)(m_Bones.size() - 1);

	//����Լ��� ���� ���ϴ� ������ ��ȸ�Ѵ�
	for (size_t i = 0; i < _pNode->mNumChildren; i++) {
		ReadBoneData(_pNode->mChildren[i], out, iParentIndex);
	}


	//BoneDesc bone;
	//bone.strBoneName = _pNode->mName.data;
	//bone.iParentBoneIndex = _iParentBoneIdx;
	//bone.iBoneIndex = m_Bones.size();
	//
	//_float4x4 TransformMat;
	//
	//memcpy(&TransformMat, &_pNode->mTransformation, sizeof(_float4x4));
	//bone.TransformMatrix = XMMatrixTranspose(TransformMat);

	//_float4x4 test1 = XMMatrixTranspose(TransformMat);
	//_float4x4 test2 = TransformMat.Transpose();
	////XMMatrixTranspose(

	//_bool IsSameBone = false;

	//for (auto& iter : m_Bones) {
	//	if (iter.strBoneName == bone.strBoneName) {

	//		IsSameBone = true;
	//		break;
	//	}
	//}

	//if (!IsSameBone) {
	//	m_Bones.push_back(bone);
	//}

	//_uint iParentIndex = (_uint)(m_Bones.size() - 1);

	//for (size_t i = 0; i < _pNode->mNumChildren; i++)
	//{
	//	ReadBoneData(_pNode->mChildren[i], out, iParentIndex);
	//}

}

void CAssimpModel::ReadyMaterial(ofstream& out)
{
	/*���͸��� ����*/
	size_t iMaterialNum = pAIScene->mNumMaterials;
	out.write((char*)&iMaterialNum, sizeof(size_t));

	for (size_t i = 0; i < iMaterialNum; i++) {

		aiMaterial* pAIMateral = pAIScene->mMaterials[i];
		               
	//	pAIMateral->
		size_t TextureNum = 0;

		/*��ü ���׸��� �ؽ��� ���� �̸� ��� */
		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++) {
			aiString strTextureFilePath;

			if (FAILED(pAIMateral->GetTexture(aiTextureType(j), 0, &strTextureFilePath))) {
				continue;
			}

			++TextureNum;

		}

		/*���׸��� �ִ� �ؽ��� ���� */
		out.write((char*)&TextureNum, sizeof(size_t));

		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; j++) {

			size_t TextureNum = pAIMateral->GetTextureCount(aiTextureType(j));

			aiString strTextureFilePath;

			if (FAILED(pAIMateral->GetTexture(aiTextureType(j), 0, &strTextureFilePath)))
				continue;

			_char szFileName[MAX_PATH] = "";
			_char szExt[MAX_PATH] = "";

			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);


			out.write((char*)&szFileName, MAX_PATH * sizeof(char));
			
			_uint iTexType = aiTextureType(j);

			out.write((char*)&iTexType, sizeof(_uint));

		}
	}
	

}

void CAssimpModel::ReadyMesh(ofstream& out)
{
	size_t MeshSize = pAIScene->mNumMeshes;
	out.write((char*)&MeshSize, sizeof(size_t));

	for (size_t i = 0; i < MeshSize; ++i) {

		_char m_szName[MAX_PATH] = "";
		strcpy_s(m_szName, pAIScene->mMeshes[i]->mName.data);


		out.write((char*)m_szName, sizeof(char) * MAX_PATH);
		out.write((char*)&pAIScene->mMeshes[i]->mMaterialIndex, sizeof(_uint));

		out.write((char*)&pAIScene->mMeshes[i]->mNumVertices, sizeof(_uint));

		_uint iIndices = pAIScene->mMeshes[i]->mNumFaces * 3;

		out.write((char*)&iIndices, sizeof(_uint));

		for (size_t j = 0; j < pAIScene->mMeshes[i]->mNumVertices; j++) {

			out.write((char*)&pAIScene->mMeshes[i]->mVertices[j], sizeof(_float3));
			out.write((char*)&pAIScene->mMeshes[i]->mNormals[j], sizeof(_float3));
			out.write((char*)&pAIScene->mMeshes[i]->mTextureCoords[0][j], sizeof(_float2));
			out.write((char*)&pAIScene->mMeshes[i]->mTangents[j], sizeof(_float3));
		}

		_uint mFace = pAIScene->mMeshes[i]->mNumFaces;
		out.write((char*)&mFace, sizeof(mFace));


		for (size_t j = 0; j < mFace; j++) {

			_uint ind1 = pAIScene->mMeshes[i]->mFaces[j].mIndices[0];
			_uint ind2 = pAIScene->mMeshes[i]->mFaces[j].mIndices[1];
			_uint ind3 = pAIScene->mMeshes[i]->mFaces[j].mIndices[2];

			out.write((char*)&ind1, sizeof(_uint));
			out.write((char*)&ind2, sizeof(_uint));
			out.write((char*)&ind3, sizeof(_uint));
		}
	}
}

void CAssimpModel::ReadyAnimMesh(ofstream& out)
{
	size_t MeshSize = pAIScene->mNumMeshes;
	/*�޽� ����*/
	out.write((char*)&MeshSize, sizeof(size_t));

	/*������ŭ �ݺ��� �����༭ �����ϱ�*/
	for (size_t i = 0; i < MeshSize; ++i) {

		aiMesh* pAIMesh = pAIScene->mMeshes[i];

		_char m_szName[MAX_PATH] = "";
		strcpy_s(m_szName, pAIMesh->mName.data);

		/*�޽� �̸�*/
		out.write((char*)m_szName, sizeof(char) * MAX_PATH);
		/*���׸��� �ε���*/
		out.write((char*)&pAIMesh->mMaterialIndex, sizeof(_uint));

		/*vertice ����*/
		out.write((char*)&pAIMesh->mNumVertices, sizeof(_uint));

		_uint iIndices = pAIMesh->mNumFaces * 3;

		VTXANIMMESH* Vertices = new VTXANIMMESH[pAIMesh->mNumVertices];
		ZeroMemory(Vertices, sizeof(VTXANIMMESH) * pAIMesh->mNumVertices);

		/*Indices����*/
		out.write((char*)&iIndices, sizeof(_uint));

		for (size_t j = 0; j < pAIMesh->mNumVertices; j++) {

			memcpy(&Vertices[j].vPosition, &pAIMesh->mVertices[j], sizeof(_float3));
			memcpy(&Vertices[j].vNormal, &pAIMesh->mNormals[j], sizeof(_float3));
			memcpy(&Vertices[j].vTexcoord, &pAIMesh->mTextureCoords[0][j], sizeof(_float2));
			memcpy(&Vertices[j].vTangent, &pAIMesh->mTangents[j], sizeof(_float3));


			//out.write((char*)&pAIMesh->mVertices[j], sizeof(_float3));
			//out.write((char*)&pAIMesh->mNormals[j], sizeof(_float3));
			//out.write((char*)&pAIMesh->mTextureCoords[0][j], sizeof(_float2));
			//out.write((char*)&pAIMesh->mTangents[j], sizeof(_float3));
		}


		/*�ִԸ޽� �� ����*/
		_uint iNumBones = pAIMesh->mNumBones;
		/*�޽� �������� ���¿� ������ �ִ� ����*/
		out.write((char*)&iNumBones, sizeof(_uint));

		//���� ������
		if (0 == iNumBones){
			_int iBoneIndex = 0;

			auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](shared_ptr<CAssimpBone> pBone) {

				iBoneIndex++;

				if (!strcmp(m_szName, pBone->GetBoneName()))
					return true;
				return false;
				});

			_float4x4 OffsetMatrix;

			OffsetMatrix = XMMatrixIdentity();

			out.write((char*)&OffsetMatrix, sizeof(_float4x4));
			out.write((char*)&iBoneIndex, sizeof(_int));

		}
		else {

			for (UINT k = 0; k < iNumBones; k++) {

				aiBone* pBone = pAIMesh->mBones[k];

				_char m_szName2[MAX_PATH] = "";
				strcpy_s(m_szName2, pBone->mName.data);


				_int iBoneIndex = { -1 };

				auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](shared_ptr<CAssimpBone> pBone) {

					iBoneIndex++;

					if (!strcmp(m_szName2, pBone->GetBoneName()))
						return true;
					return false;
					});

				


			/*	_int idxNum = find_if(m_Bones.begin(), m_Bones.end(), [m_szName2](shared_ptr<CAssimpBone> bone) {
					return bone->GetBoneName() == m_szName2;
					})->iBoneIndex;*/

				_float4x4 BeforeMat;

				memcpy(&BeforeMat, &pBone->mOffsetMatrix, sizeof(_float4x4));

				_float4x4 OffsetMatrix = XMMatrixTranspose(BeforeMat);

				//XMMatrixTranspose(
				out.write((char*)&OffsetMatrix, sizeof(_float4x4));
				out.write((char*)&iBoneIndex, sizeof(_int));


				for (UINT m = 0; m < pBone->mNumWeights; m++)
				{
					if (0.f == Vertices[pBone->mWeights[m].mVertexId].vBlendWeights.x)
					{
						Vertices[pBone->mWeights[m].mVertexId].vBlendIndices.x = k;
						Vertices[pBone->mWeights[m].mVertexId].vBlendWeights.x = pBone->mWeights[m].mWeight;
					}

					else if (0.f == Vertices[pBone->mWeights[m].mVertexId].vBlendWeights.y)
					{
						Vertices[pBone->mWeights[m].mVertexId].vBlendIndices.y = k;
						Vertices[pBone->mWeights[m].mVertexId].vBlendWeights.y = pBone->mWeights[m].mWeight;
					}

					else if (0.f == Vertices[pBone->mWeights[m].mVertexId].vBlendWeights.z)
					{
						Vertices[pBone->mWeights[m].mVertexId].vBlendIndices.z = k;
						Vertices[pBone->mWeights[m].mVertexId].vBlendWeights.z = pBone->mWeights[m].mWeight;
					}

					else if (0.f == Vertices[pBone->mWeights[m].mVertexId].vBlendWeights.w)
					{
						Vertices[pBone->mWeights[m].mVertexId].vBlendIndices.w = k;
						Vertices[pBone->mWeights[m].mVertexId].vBlendWeights.w = pBone->mWeights[m].mWeight;
					}
				}

			}

		}

		for (size_t z = 0; z < pAIMesh->mNumVertices; z++)
		{
			out.write((char*)&Vertices[z], sizeof(VTXANIMMESH));
		}

		_uint mFace = pAIMesh->mNumFaces;
		out.write((char*)&mFace, sizeof(mFace));


		for (size_t x = 0; x < mFace; x++) {

			_uint ind1 = pAIMesh->mFaces[x].mIndices[0];
			_uint ind2 = pAIMesh->mFaces[x].mIndices[1];
			_uint ind3 = pAIMesh->mFaces[x].mIndices[2];

			out.write((char*)&ind1, sizeof(_uint));
			out.write((char*)&ind2, sizeof(_uint));
			out.write((char*)&ind3, sizeof(_uint));
		}
	}


}

void CAssimpModel::ReadyAnim(ofstream& out)
{
	/*Json Parsing ���� */

	/*�ִϸ��̼� ����*/
	size_t iAnimNum = pAIScene->mNumAnimations;
	out.write((char*)&iAnimNum, sizeof(size_t));

	Json::Value root;


	for (size_t i = 0; i < iAnimNum; ++i) {

		Json::Value Animation;

		_char m_szName[MAX_PATH] = "";
		strcpy_s(m_szName, pAIScene->mAnimations[i]->mName.data);

		/*Json Data*/
		Animation["AnimName"] = m_szName;
		Animation["Speed"] = 1.f;

		_uint iNumChannel = pAIScene->mAnimations[i]->mNumChannels;

		string index = to_string(i);
		root[index] = Animation;

		out.write(m_szName, sizeof(char) * MAX_PATH);
		out.write((char*)&pAIScene->mAnimations[i]->mDuration, sizeof(_double));
		out.write((char*)&pAIScene->mAnimations[i]->mTicksPerSecond, sizeof(_double));
		out.write((char*)&iNumChannel, sizeof(_uint));

		for(UINT k = 0; k < iNumChannel; k++) {
			_char m_szName2[MAX_PATH] = "";
			strcpy_s(m_szName2, pAIScene->mAnimations[i]->mChannels[k]->mNodeName.data);

			
			out.write(m_szName2, sizeof(char) * MAX_PATH);

			_uint iPosKeyNum = pAIScene->mAnimations[i]->mChannels[k]->mNumPositionKeys;
			_uint iScaleKeyNum = pAIScene->mAnimations[i]->mChannels[k]->mNumScalingKeys;
			_uint iRotKeyNum = pAIScene->mAnimations[i]->mChannels[k]->mNumRotationKeys;

			_uint MaxKeyNum = max(iPosKeyNum, iScaleKeyNum);
			MaxKeyNum = max(MaxKeyNum, iRotKeyNum);

			out.write((char*)&MaxKeyNum, sizeof(_uint));

			for (_uint j = 0; j < MaxKeyNum; j++) {

				KEYFRAME KeyFrame = {};

				_float3 vScale;
				_float4 vRotation;
				_float3 vPosition;

				//Ű�� �߰��� ������� ���� ���������� ���� + ��� �� Ű�� ��ü ������ Ű��ŭ �����ϱ⿡ �̷��� for���� �ۼ��ص� ������
				if (pAIScene->mAnimations[i]->mChannels[k]->mNumScalingKeys > j) {

					memcpy(&vScale, &pAIScene->mAnimations[i]->mChannels[k]->mScalingKeys[j].mValue, sizeof(_float3));
					KeyFrame.Time = pAIScene->mAnimations[i]->mChannels[k]->mScalingKeys[j].mTime;
				}

				if (pAIScene->mAnimations[i]->mChannels[k]->mNumRotationKeys > j) {
					//wxyz ������ ����־ ���� ��Ī�������

					vRotation.x = pAIScene->mAnimations[i]->mChannels[k]->mRotationKeys[j].mValue.x;
					vRotation.y = pAIScene->mAnimations[i]->mChannels[k]->mRotationKeys[j].mValue.y;
					vRotation.z = pAIScene->mAnimations[i]->mChannels[k]->mRotationKeys[j].mValue.z;
					vRotation.w = pAIScene->mAnimations[i]->mChannels[k]->mRotationKeys[j].mValue.w;
					KeyFrame.Time = pAIScene->mAnimations[i]->mChannels[k]->mRotationKeys[j].mTime;

				}

				if (pAIScene->mAnimations[i]->mChannels[k]->mNumPositionKeys > j) {

					memcpy(&vPosition, &pAIScene->mAnimations[i]->mChannels[k]->mPositionKeys[j].mValue, sizeof(_float3));
					KeyFrame.Time = pAIScene->mAnimations[i]->mChannels[k]->mPositionKeys[j].mTime;
				}


				KeyFrame.vScale = vScale;
				KeyFrame.vRotation = vRotation;
				KeyFrame.vPosition = vPosition;

				out.write((char*)&KeyFrame, sizeof(KEYFRAME));
		
			}
		}
	}


	wstring strCompletePath = m_strAnimSavePath + m_strObjName + TEXT(".json");

	WriteJson(root, strCompletePath);
}

void CAssimpModel::SetSavePath()
{
}

_int CAssimpModel::GetBoneIndex(const _char* _pBoneName) const
{
	_int iBoneIndex = { -1 };

	auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](shared_ptr<CAssimpBone> pBone) {

		iBoneIndex++;

		if (!strcmp(_pBoneName, pBone->GetBoneName()))
			return true;
		return false;
		});


	if (iter == m_Bones.end())
		return -1;


	return iBoneIndex;
}

void CAssimpModel::WriteJson(Json::Value _value, const wstring& _strSavePath)
{
	if (_value.empty() || TEXT("") == _strSavePath) {
		MSG_BOX("JSON root is Empty or Wrong PathName");
		return;
	}

	Json::StreamWriterBuilder builder;
	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	std::ofstream ofStream;
	ofStream.open(_strSavePath, ios_base::out);

	if (ofStream.is_open()) {
		writer->write(_value, &ofStream);
	}
	else {
		return;
	}

	ofStream.close();

}

shared_ptr<CAssimpModel> CAssimpModel::Create(const string& _strFilePath, _uint _iType, const wstring& _strSaveFileName, const wstring& _strFileName)
{
	shared_ptr<CAssimpModel> pInstance = make_shared<CAssimpModel>();
	pInstance->ConvertModel(_strFilePath, _iType, _strSaveFileName, _strFileName);

	return pInstance;
}
