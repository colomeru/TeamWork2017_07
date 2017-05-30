#pragma once
#include <list>
#include <functional>
#include <memory>

enum EaseType
{
	Linear
};


class EaseNode
{
public:
	// �R���X�g���N�^
	EaseNode(float* value, EaseType type, float b, float c, float d = 1.0f, std::function<void()> callback = nullptr);
	// �f�X�g���N�^
	~EaseNode();
	// �X�V
	void Update(float deltaTime);
	// �I�����Ă��邩�H
	bool IsEnd() const;

private:
	//
	float* value_;
	//
	std::function<void()> callback_;
	// �I���t���O
	bool isEnd_;
	//
	float b_;
	//
	float c_;
	//
	float d_;
	//
	float timer_;
};

//
using EasePtr = std::shared_ptr<EaseNode>;

//
class EasingManager
{
private:
	// �R���X�g���N�^
	EasingManager();
	// �f�X�g���N�^
	~EasingManager();

	/* �R�s�[�֎~ */
	EasingManager(const EasingManager&) = delete;
	EasingManager& operator =(const EasingManager&) = delete;

public:
	static EasingManager &GetInstance() {
		static EasingManager e;
		return e;
	}

	// ������
	void Initialize();
	// �X�V
	void Update(float deltaTime);
	// �ǉ�
	void Add(EasePtr ease);
	// �N���A
	void Clear();
	// �폜
	void Remove();

private:

	//
	std::list<EasePtr> easeList_;

};