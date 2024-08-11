#pragma once

namespace physx
{
	enum ECOLLISIONFLAG		/* WORD0, WORD1*/
	{
		COL_NONE			= 0,		/* �������� �浹 ���� �ʴ´�. */
		COL_STATIC			= 1 << 0,	/* ������ �ǹ� ���� Static ��ü */
		COL_PLAYER			= 1 << 1,
		COL_PLAYERWEAPON	= 1 << 2,
		COL_PLAYERSKILL		= 1 << 3,
		COL_PLAYERPROJECTILE	= 1 << 4,
		COL_PLAYERPARRYING		= 1 << 5,
		COL_CHECKBOX			= 1 << 6,
		COL_ROPE				= 1 << 7,

		COL_NPC = 1 << 9,						// NPC HitBox
		COL_MONSTER			= 1 << 10,			// ���� HitBox
		COL_MONSTERWEAPON	= 1 << 11,			// ���� ����
		COL_MONSTERSKILL		= 1 << 12,
		COL_MONSTERPROJECTILE	= 1 << 13,	// Projectile

		COL_TRIGGER = 1 << 14,

		COL_TRAP = 1 << 15,
		COL_AMITOY = 1 << 19,					// �ƹ�����
		COL_AMITOY_ATTACK = 1 << 20,			// �ƹ����� ����

		COL_INTERACTIONOBJ = 1 << 21,
		/* �浹�� �÷��� �߰��� �� */
		COL_END = 1 << 30
	};

	enum EWORD2_FLAGS /* PxFilterData.word2 */
	{
		NONE_WORD = 0,
		ENABLE_COLLISION = 1 << 0,
		IS_TRIGGER = 1 << 1

	};

	enum EGEOMETRY_TYPE
	{
		GT_SPHERE,					// �� ���
		GT_BOX,						// OBB ���
		GT_CAPSULE,					// �˾� ���(�ַ�, �÷��̾�, ���� ��)
		//GT_MODEL_CONVEXMESH,		// ������ �޽� ���(����, ����)
		GT_MODEL_TRIANGLEMESH,		// ������ �޽� ���(����)
		GT_HEIGHTFILED,				// ���̸�(����)
		GT_END
	};
}