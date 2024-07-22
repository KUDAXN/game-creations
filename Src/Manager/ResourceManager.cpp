#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	Resource res;

	// �X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome_2.mv1");
	resourcesMap_.emplace(SRC::SKYDOME, res);

	//���X�|�[���n�_
	res = Resource(Resource::TYPE::EFFEKSEER, Application::PATH_EFFECT + "ForceFieldTornado.efkefc");
	resourcesMap_.emplace(SRC::RESET, res);

	// �����o��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "SpeechBalloon.png");
	resourcesMap_.emplace(SRC::SPEECH_BALLOON, res);

	//����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/Shot.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);

	//�X�e�[�W�P
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/H_stage.mv1");
	resourcesMap_.emplace(SRC::STAGE1, res);
	
	//�X�e�[�W2
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/H_stage_2D.mv1");
	resourcesMap_.emplace(SRC::STAGE2, res);

	//�X�e�[�W(�ׂ�����)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/block.mv1");
	resourcesMap_.emplace(SRC::BLOCK, res);

	//�X�e�[�W(�L������)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/plane.mv1");
	resourcesMap_.emplace(SRC::PLANE, res);

	//�X�e�[�W(��)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage/Wall.mv1");
	resourcesMap_.emplace(SRC::WALL, res);
	
	//����L����
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Player/model.mv1");
	resourcesMap_.emplace(SRC::HUMAN, res);

	//�^�C�g��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Title_0.png");
	resourcesMap_.emplace(SRC::TITLE, res);

	//�^�C�g��2
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Title2.png");
	resourcesMap_.emplace(SRC::TITLE2, res);

	//�Q�[���I�[�o�[
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "GameOver.png");
	resourcesMap_.emplace(SRC::GAMEOVER, res);

	//�Q�[���N���A
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Clear.png");
	resourcesMap_.emplace(SRC::GAMECLEAR, res);
}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	resourcesMap_.clear();
	delete instance_;
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
