#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>
#include "../../stageGenerator/Stage.h"
#include "../../math/Vector3.h"
#include "../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"

class MenuScreen
{
public:
	//�X�e�[�W�p�l��
	struct PanelStruct
	{
		Vector2 position; //�p�l���̍��W
		bool isDraw; //�I���\��
		float alpha; //�p�l���̃A���t�@�l
		PanelStruct() :
			position(Vector2::Zero), isDraw(false), alpha(0.0f) {}
		PanelStruct(Vector2 position, bool isDraw = false, float alpha = 0.0f) :
			position(position), isDraw(isDraw), alpha(alpha) {}
	};

	//��
	struct StarStruct
	{
		Vector2 position_;
		float isAlpha_;
		float timer_;
		float scale_;

	};


	//�R���X�g���N�^
	MenuScreen();
	//�f�X�g���N�^
	~MenuScreen();
	//������
	void Init();
	//�X�V
	void Update();
	//�`��
	void Draw() const;
	//
	void Action();
	//�O�̃X�e�[�W���N���A���Ă��邩�H
	bool CheckPreviousStage(int sNum);
	//���̃X�e�[�W�̉��
	void OpenNextStage(int sNum);
	//���̃X�e�[�W���������Ă��邩�H
	bool CheckNextStage(int sNum);
	//�p�^�[���Q
	void Pattern2Update();
	void Pattern2Draw() const;
	//"��"�����͂��ꂽ��
	bool IsInputUp() const;
	//"��"�����͂��ꂽ��
	bool IsInputDown() const;
	//"��/A"�̂����ꂩ�����͂��ꂽ��
	bool IsInputLeft() const;
	//"��/��/�E"�̂����ꂩ�����͂��ꂽ��
	bool IsInputAny() const;
	//��
	void Star();
	//���ꐯ
	void ShootingStar();
	//�J���X
	void Crow();
	//SE
	void SE();
	//
	void End();


	Stage GetGamePlayStage()const;

	//�O���N���X�p
public:
	bool GetIsBackSelect()const {
		return backSelect_;
	}
	bool GetIsTutorialSelect()const {
		return stageNum_ == 0;
	}
	void InputSelectStage();
	void ResetBG();

private:
	const float Height = WINDOW_HEIGHT / 4.0f * 2.0f; //�p�l���P��y���W
	const float MoveTime = 0.5f;
	const float BetDis = 339.0f; //�ԋ���
	const int StarNum = 10;
	const Vector2 BgPos = Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 475.0f); //�w�i���W
	const Vector2 BuilPos = Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 285.0f); //�r�����W
	const Vector2 WwwPos = Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT); //�����W

	int stageNum_; //�X�e�[�W�ԍ�
	bool backSelect_; //�߂��I�����Ă��邩�H
	Vector2 cursorPos_; //�J�[�\���̍��W
	std::array<PanelStruct, 9> panel; //PanelStruct���X�e�[�W��������

	//
	float dis_;
	Vector2 from_;

	//�w�i�F
	Vector3 color_;

	//��
	float alphaValue_;
	std::array<float, 3> starAlpha_;

	//���ꐯ
	std::array<StarStruct, 10> sStar_;
	std::array<float, 10> waitTime_;
	std::array<Vector2, 10> prevPos_;
	std::array<float, 10> ssAlpha_;

	//�J���X
	std::array<Vector2, 3> crowPos_;
	std::array<float, 3> interval_;
	std::array<float, 3> cTimer_;
	std::array<Vector2, 3> cVelocity_;
	std::array<Vector2, 3> cFrom_;
	std::array<float, 3> cDis_;

	CharacterAnmManager anmManager_;

	std::array<Stage, 9> stageList_;
};