#pragma once
#include "../../../actor/Actor.h"
#include <vector>


class CreditTextGenerator : public Actor {
public:
	//�R���X�g���N�^
	CreditTextGenerator(IWorld* world, Vector2 position);
	//�f�X�g���N�^
	~CreditTextGenerator();
	//�X�V
	virtual void Update() override;
	//�`��
	virtual void Draw() const override;

private:
	//����
	void ClothAdd();

private:
	int frame_;							//�t���[����
	std::vector<SPRITE_ID> id_;			//���\�[�XID
	int idCount_;						//ID�ύX�p�J�E���g
	float loopHandle_;					//

	std::vector<SPRITE_ID> postId_;		//���\�[�XID
	std::vector<Vector2> postSize_;		//���\�[�X�T�C�Y

};
