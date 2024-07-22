#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		SKYDOME,			//�X�J�C�h�[��
		RESET,		//�Z�[�u�n�_�̌֒�
		SPEECH_BALLOON,		//�����o��
		PLAYER,				//����
		STAGE1,				//�X�e�[�W���f��
		STAGE2,				//�X�e�[�W���f��(2D�p)
		BLOCK,				//�X�e�[�W(�ׂ�����)
		PLANE,				//�X�e�[�W(�L������)
		WALL,				//�X�e�[�W(��)
		HUMAN,					//����L����
		HUMAN_IDLE,				//����L����(�ҋ@)
		HUMAN_WALK,				//����L����(���s)
		HUMAN_JUMP,				//����L����(�W�����v)
		TITLE,					//�^�C�g��
		TITLE2,					//�^�C�g��2
		TL_TAB,					//�؂�ւ�����
		TL_BSRASH,				//�W�����v����
		TL_WASD,				//�ړ�����
		TL_RE,					//���Z�b�g����
		TL_LR,					//�n�_�؂�ւ�����
		GAMEOVER,				//�Q�[���I�[�o�[
		GAMECLEAR,				//�Q�[���N���A
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void) = default;

	// �������[�h
	Resource* _Load(SRC src);

};

