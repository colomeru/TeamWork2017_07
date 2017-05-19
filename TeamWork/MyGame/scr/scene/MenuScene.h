#pragma once
#include "IScene.h"
#include <map>
#include "../Def.h"

class MenuScene : public IScene
{
public:
	// �R���X�g���N�^
	MenuScene();
	// �f�X�g���N�^
	~MenuScene();

	// ������
	virtual void Initialize() override;
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


	struct PanelStruct
	{
		Vector2 position;
		bool isDraw;
		float alpha;
	};

private:
	int stageNum = 0;
	const float height = WINDOW_HEIGHT / 4.0f * 3.0f;
	std::array<Vector2, 9> stagePos;

	//
	Vector2 pos;
	Vector2 gPos;
	Vector2 dir;
	float dis;
	float disN;

private:
	// ���[���h�p�V�F�A�h�|�C���^
	using WorldPtr = std::shared_ptr<World>;
	// ���[���h
	WorldPtr		world_;

	// ���̃V�[��
	Scene			nextScene_;
};