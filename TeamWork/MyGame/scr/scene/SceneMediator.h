#pragma once
#include "../game/ID.h"
#include "../actor/ActorParameter.h"
#include <memory>
#include <functional>
#include <unordered_map>

class SceneMediator
{
private:
	SceneMediator() = default;
	~SceneMediator() = default;

public:
	// �C���X�^���X
	static SceneMediator& GetInstance()
	{
		static SceneMediator s;
		return s;
	}

	// ������
	void Initiallize();
	// �ǉ�
	void Add(ACTOR_ID id, SymbolParameter symbol);
	// �N���A
	void Clear();
	void SetEncountSymbol(SymbolParameter symbol);
	// 
	std::vector<SymbolParameter> GetList(ACTOR_ID id) const;
	// �G���J�E���g�G�l�~�[�̎擾
	SymbolParameter GetEncountSymbol() const;
	// �G���J�E���g�G�l�~�[�̃Z�b�g

private:
	// �X�e�[�W�̃G�l�~�[�f�[�^
	std::unordered_map<ACTOR_ID, std::vector<SymbolParameter>>	symbolParam_;
	// �G���J�E���g�G�l�~�[�f�[�^
	SymbolParameter		encountSymbol_;
};