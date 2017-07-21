#pragma once
#include "../../../actor/Field/Enemys/EnemyCharas/CharacterAnmManager.h"


class MenuCrow {
public:
	enum class State {
		Move,	//�ړ�
		Idle,	//�ҋ@
	};
	//�R���X�g���N�^
	MenuCrow();
	//�f�X�g���N�^
	~MenuCrow();
	//�X�V
	void Update(const int stageNum);
	//�`��
	void Draw() const;
	//�ړI�n���Z
	void AddDistance(float betDis);
	//������
	void Initialize(Vector2 position, float interval);

private:
	void Move();	//�ړ�
	void Idle();	//�ҋ@

private:
	const float MoveTime = 0.5f;	//�ړ��ɂ����鎞��
	const float OffSet = 300.0f;	//��ʊO

	Vector2 position_;	//���W
	float interval_;	//�J��Ԃ�����
	float timer_;		//�^�C�}
	Vector2 velocity_;	//�ړ���
	Vector2 from_;		//�ڕW�l
	float dis_;			//�ړI�n

	CharacterAnmManager anmManager_;	//�A�j���[�V����
	State state_;						//���
};
