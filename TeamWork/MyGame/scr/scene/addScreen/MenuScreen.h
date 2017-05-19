#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>

class MenuScreen
{
public:
	struct PanelStruct
	{
		Vector2 position; //�p�l���̍��W
		bool isDraw; //�I���\��
		float alpha; //�p�l���̃A���t�@�l
		PanelStruct():
			position(Vector2::Zero),isDraw(false),alpha(0.0f){}
		PanelStruct(Vector2 position, bool isDraw = false, float alpha = 0.0f) :
			position(position), isDraw(isDraw), alpha(alpha) {}
	};

	//�R���X�g���N�^
	MenuScreen();
	//�f�X�g���N�^
	~MenuScreen();
	//�X�V
	void Update();
	//�`��
	void Draw() const;
	//
	void Action();
	//
	void OpenNextStage();
	
private:
	int stageNum = 0; //�X�e�[�W�ԍ�
	const float height = WINDOW_HEIGHT / 4.0f * 3.0f; //�p�l���P��y���W
	Vector2 backPos; //�߂�p�l���̍��W
	Vector2 cursorPos; //�J�[�\���̍��W
	int panelNum, cursorNum; //�p�l���ԍ��A�J�[�\���ԍ�
	std::array<PanelStruct, 9> panel; //PanelStruct���X�e�[�W��������

	//
	Vector2 pos;
	Vector2 gPos;
	Vector2 dir;
	float dis;
	float disN;
};