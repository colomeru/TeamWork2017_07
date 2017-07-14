#pragma once
#include "../../Def.h"
#include "../../math/Vector2.h"
#include <array>
#include "../../stageGenerator/Stage.h"
#include "../../math/Vector3.h"
#include "../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"
#include "screenSupport/MenuCrow.h"

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
		Vector2 position_;	//���W
		float isAlpha_;		//�A���t�@�l
		float timer_;		//�^�C�}
		float scale_;		//�g��l
		float waitTime_;	//�ҋ@����
		Vector2 prevPos_;	//�߂���W
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
	//���̃X�e�[�W�̉��
	void OpenNextStage(int sNum);
	//���̃X�e�[�W���������Ă��邩�H
	bool CheckNextStage(int sNum);
	//"��"�����͂��ꂽ��
	bool IsInputUp() const;
	//"��"�����͂��ꂽ��
	bool IsInputDown() const;
	//"��/A"�̂����ꂩ�����͂��ꂽ��
	bool IsInputLeft() const;
	//"��/��/�E"�̂����ꂩ�����͂��ꂽ��
	bool IsInputAny() const;
	//�`�F�b�N�{�^���������ꂽ��
	bool IsInputCheck() const;
	//��
	void Star();
	//���ꐯ
	void ShootingStar();
	//�J���X
	void Crow();
	//SE
	void SE();
	//�I��
	void End();
	//�X�e�[�W���擾
	Stage GetGamePlayStage()const;

	//�O���N���X�p
public:
	//�u�߂�v���I������Ă��邩�H
	bool GetIsBackSelect()const {
		return backSelect_;
	}
	//�`���[�g���A���X�e�[�W���I������Ă��邩�H
	bool GetIsTutorialSelect()const {
		return stageNum_ == 0;
	}
	//�X�e�[�W���Z�b�g
	void InputSelectStage();
	//�w�i���Z�b�g
	void ResetBG();
	//�X�e�[�W�ԍ����擾
	int GetStageNumber();

private:
	const float Height = WINDOW_HEIGHT / 4.0f * 2.0f;	//�p�l���P��y���W
	const float MoveTime = 0.5f;						//�ړ��ɂ����鎞��
	const float BetDis = 339.0f;						//�ԋ���
	const int StarNum = 10;								//���̌�
	const Vector2 BgPos = Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 475.0f);		//�w�i���W
	const Vector2 BuilPos = Vector2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 285.0f);	//�r�����W
	const Vector2 WwwPos = Vector2(WINDOW_WIDTH / 2, WINDOW_HEIGHT);						//�����W
	const float OffSet = 300.0f;

	int stageNum_;						//�X�e�[�W�ԍ�
	bool backSelect_;					//�߂��I�����Ă��邩�H
	Vector2 cursorPos_;					//�J�[�\���̍��W
	std::array<PanelStruct, 9> panel;	//PanelStruct���X�e�[�W��������

	//
	float dis_;		//�ړI�l
	Vector2 from_;	//�ړ����W

	Vector3 color_;	//�w�i�F

	//��
	float alphaValue_;					//�A���t�@�ύX�l
	std::array<float, 3> starAlpha_;	//�A���t�@�l

	//���ꐯ
	std::array<StarStruct, 10> sStar_;	//���ꐯ�\����

	//�J���X
	std::array<Vector2, 3> crowPos_;	//�J���X���W
	std::array<float, 3> interval_;		//�J��Ԃ�����
	std::array<float, 3> cTimer_;		//�J���X�^�C�}
	std::array<Vector2, 3> cVelocity_;	//�J���X�ړ���
	std::array<Vector2, 3> cFrom_;		//�ڕW�l
	std::array<float, 3> cDis_;			//�ړ����W
	std::array<MenuCrow, 3> crow_;		//�J���X

	CharacterAnmManager anmManager_;	//�A�j���[�V����

	std::array<Stage, 9> stageList_;	//�X�e�[�W���X�g
};