#pragma once
#include "../Clothes.h"
#include "../MyGame/scr/actor/player/Player.h"

//�n���K�[
class Hanger : public Clothes, public std::enable_shared_from_this<Hanger>
{
public:
	//�R���X�g���N�^
	Hanger(IWorld* world, CLOTHES_ID clothes, int laneNum, Vector2 pos);
	//�f�X�g���N�^
	~Hanger();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;
	//�󓮍X�V
	virtual void OnUpdate() override;

private:

};