#pragma once
#include "../../../../input/CsvReader.h"
#include "../Clothes.h"

class PointSetter
{
public:
	//�R���X�g���N�^
	PointSetter();
	//�f�X�g���N�^
	~PointSetter();
	//csv�ǂݍ���
	void Load(const std::string& fileName);
	//�|�C���g�ݒ�
	void SetLocalPoints(const SPRITE_ID& startSpriteId, float length);
	//
	std::map<CuttingState, std::vector<Vector3>> GetLocalPoints(const SPRITE_ID& spriteId) const;

private:
	//csv���[�_�[
	CsvReader csvReader_;
	//�t�@�C����
	std::string fileName_;
	//���̃X�v���C�gID
	SPRITE_ID spriteId_;
	//���̓����蔻��|�C���g�}�b�v
	std::map<SPRITE_ID, std::map<CuttingState, std::vector<Vector3>>> spritePoints_;
	//���̌��_����x�_�܂ł̒���
	float LENGTH;

};