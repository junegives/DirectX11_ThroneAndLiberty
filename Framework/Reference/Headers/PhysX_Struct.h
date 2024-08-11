#pragma once

namespace Engine
{
	class CModel;
	class CVITerrain;
}

namespace physx
{
	struct ENGINE_DLL Geometry abstract
	{
	protected:
		Geometry() = delete;
		Geometry(EGEOMETRY_TYPE _eType) :m_eGeometryType(_eType) {};
		EGEOMETRY_TYPE m_eGeometryType = GT_END;
	public:
		EGEOMETRY_TYPE GetType() { return m_eGeometryType; };
		const char* strShapeTag = "ShapeTag";	// �� ����� ���� �±�
		_float3 vOffSetPosition = { 0.0f,0.0f,0.0f };	// �� ����� ������ ��ǥ
		_float3 vOffSetRotation = { 0.0f,0.0f,0.0f };	// �� ����� ������ ȸ��(Degree)
	};

	struct ENGINE_DLL GeometrySphere final : public Geometry
	{
		GeometrySphere() : Geometry(GT_SPHERE) {}
		float fRadius = 0.0f;	// ��ü�� ������(���� ������)
	};

	struct ENGINE_DLL GeometryBox final : public Geometry
	{
		GeometryBox() : Geometry(GT_BOX) {}
		_float3 vSize = { 0.0f,0.0f,0.0f };	// ��ü�� x,y,z ũ��(���� ������)
	};

	struct ENGINE_DLL GeometryCapsule final : public Geometry
	{
		GeometryCapsule() : Geometry(GT_CAPSULE) {}
		float fRadius = 0.0f;	// �� ������(���� ������)
		float fHeight = 0.0f;	// ����(���� ������)
	};

	//struct ENGINE_DLL GeometryConvexMesh final : public Geometry
	//{
	//	GeometryConvexMesh() : Geometry(GT_MODEL_CONVEXMESH), pModel(nullptr) {}
	//	shared_ptr<CModel> pModel = nullptr; // ��ü�� �� ������Ʈ
	//};

	struct ENGINE_DLL GeometryTriangleMesh final : public Geometry
	{
		GeometryTriangleMesh() : Geometry(GT_MODEL_TRIANGLEMESH), pModel(nullptr) {}
		shared_ptr<CModel> pModel = nullptr; // ��ü�� �� ������Ʈ
	};

	struct ENGINE_DLL GeometryHeightField final : public Geometry
	{
		GeometryHeightField() : Geometry(GT_HEIGHTFILED), pVITerrain(nullptr) {}
		shared_ptr<CVITerrain> pVITerrain = nullptr; // ������ ����
	};

	inline PxFilterFlags MySimulationFilterShader(
		PxFilterObjectAttributes /* attributes0 */, PxFilterData filterData0,
		PxFilterObjectAttributes /* attributes1 */, PxFilterData filterData1,
		PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		PX_UNUSED(filterData0);
		PX_UNUSED(filterData1);
		PX_UNUSED(constantBlockSize);
		PX_UNUSED(constantBlock);

		PxU32 ColFlag0	= filterData0.word0;	/* A ��ü�� �浹 �÷��� */
		PxU32 ColEnums0	= filterData0.word1;	/* A ��ü�� �浹�Ǵ� �÷��׵� */
		PxU32 Flags0	= filterData0.word2;	/* A ��ü�� ��Ÿ �÷��׵�, PhysX_Enums.h�� EWORD2_FLAGS ���� */

		PxU32 ColFlag1	= filterData1.word0;	/* B ��ü�� �浹 �÷��� */
		PxU32 ColEnums1 = filterData1.word1;	/* B ��ü�� �浹�Ǵ� �÷��׵� */
		PxU32 Flags1	= filterData1.word2;	/* B ��ü�� ��Ÿ �÷��׵�, PhysX_Enums.h�� EWORD2_FLAGS ���� */

		// �� ��ü�� �������� �ʴ´�.
		// �Ǵ�, �� ��ü �߿� ������ �浹�� ��Ȱ��ȭ �س���
		if ((ColFlag0 & ColEnums1) == COL_NONE || (ColFlag1 & ColEnums0) == COL_NONE || (Flags0 & ENABLE_COLLISION) == 0 || (Flags1 & ENABLE_COLLISION) == 0)
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlag::eSUPPRESS;
		}

		// �� �Ʒ��� �� �� �浹 Ȱ��ȭ �س���

		pairFlags = PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST | PxPairFlag::eDETECT_DISCRETE_CONTACT;

		// �� �� Ʈ���Ű� �ƴϴ�
		// �������� �浹 �÷��׵� �߰�
		if ((Flags0 & IS_TRIGGER) == 0 && (Flags1 & IS_TRIGGER) == 0)
		{
			pairFlags |= PxPairFlag::eSOLVE_CONTACT; //| PxPairFlag::eDETECT_CCD_CONTACT;
		}

		return PxFilterFlag::eDEFAULT;
	}
}
