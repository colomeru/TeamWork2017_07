#pragma once

#include "IScene.h"
#include <map>
#include "../camera/Camera.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../stageGenerator/StageGeneratorManager.h"
#include "../actor/player/Player.h"
#include "GamePlayDefine.h"
#include "addScreen\BackgroundScreen.h"
#include "addScreen/TutorialTextScreen.h"

static const int maxTutorialNum = 2;

class TutorialManager;

class TutorialScene : public IScene
{
public:
	enum UnLockType {
		ChangeLane,
		ChangeLaneUp,
		ChangeLaneFall,
		BiteClothes,
		PlayPendulum,
		KillTapper,
		UseSword,
		StartWind,
		ChangeHead,
		Dummy,
	};
public:
	// �R���X�g���N�^
	TutorialScene();
	// �f�X�g���N�^
	~TutorialScene();

	// ������
	virtual void Initialize() override;
	virtual void SceneInit();
	// �X�V
	virtual void Update() override;
	// �`��
	virtual void Draw() const override;
	// �I�����Ă��邩�H
	virtual bool IsEnd() const override;
	// ���̃V�[����Ԃ�
	virtual Scene Next() const override;
	// �I��������
	virtual void End() override;
	// ���b�Z�[�W����
	void handleMessage(EventMessage message, void* param);

	void addCurrentNum() {currentTutorialNum_++;}
private:
	//�v���C���[�����b�N����
	void SceneLock();
	//�`���[�g���A���N���A��������Atype�ƈ�v�����������������
	void UnLock(UnLockType type);

	//�V�[���̃��b�N�����𖞂����Ă��邩�𒲂ׂ�
	bool IsCanSceneLock()const;

	//��ʃ��b�N�̃��[����ǉ�����
	void SetLockList(int currentTutorial, int tutorialLockNum);

	//�e�`���[�g���A���X�e�[�W�p�̃Z�b�g�֐��A�e�L�X�g���ς�邽�тɌĂԂ���
	void SetLock1(int tutorialLockNum);
	void SetLock2(int tutorialLockNum);

private:
	void ChangeNextTutorial();
private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	using PlayerPtr = std::shared_ptr<Player>;
	// ���[���h
	WorldPtr		world_;
	PlayerPtr		player_;

	BackgroundScreen bgScreen_;

	//�X�e�[�W�쐬�}�l�[�W���[
	StageGenerateManager stageGeneratorManager;

	// ���̃V�[��
	Scene			nextScene_;

	TutorialTextScreen textScreen_;

	//���݂̃`���[�g���A���̃x�[�X�ԍ�
	int currentTutorialNum_;
	//�`���[�g���A���̒�~�ԍ�
	int tutorialLockNum_;

	std::string StageNameList_[maxTutorialNum];
	
	std::string TextAddList_[4];

	float dummy_;

	std::vector<std::pair<UnLockType, bool>> lockList_;

	std::vector<std::function<void(int)>> setLockFuncList_;

	std::shared_ptr<TutorialManager> enemGenerator_;

};