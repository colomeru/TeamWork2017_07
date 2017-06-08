#pragma once
#include "Clothes.h"
#include "../MyGame/scr/stageGenerator/StageGeneratorManager.h"

//���̉摜�̃p�^�[��
enum BaseDrawPattern
{
	Orange,
	SkyBlue,
};

class BaseClothes : public Clothes, public std::enable_shared_from_this<BaseClothes>
{
public:
	//�R���X�g���N�^
	BaseClothes(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos, float weight, bool is_Pin = false);
	//�f�X�g���N�^
	~BaseClothes();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;

private:
	void GraphicPattern();
	void PatternDraw() const;

private:
	StageGenerateManager* stageManager_;
	BaseDrawPattern pattern_;
};