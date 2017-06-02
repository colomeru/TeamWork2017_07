#pragma once
#include <list>
#include <map>
#include <functional>
#include <memory>
#include "TweenObject.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Easing.h"

enum EaseType
{
	Linear,
	EaseInQuad,
	EaseOutQuad,
	EaseInOutQuad,
	EaseInCubic,
	EaseOutCubic,
	EaseInOutCubic,
	EaseInQuart,
	EaseOutQuart,
	EaseInOutQuart,
	EaseInQuint,
	EaseOutQuint,
	EaseInOutQuint,
	EaseInSine,
	EaseOutSine,
	EaseInOutSine,
	EaseInExpo,
	EaseOutExpo,
	EaseInOutExpo,
	EaseInCirc,
	EaseOutCirc,
	EaseInOutCirc,
	EaseInElastic,
	EaseOutElastic,
	EaseInOutElastic,
	EaseInBack,
	EaseOutBack,
	EaseInOutBack,
	EaseInBounce,
	EaseOutBounce,
	EaseInOutBounce
};

// �C�[�W���O�}�l�[�W���[�N���X
class TweenManager
{
private:
	// �R���X�g���N�^
	TweenManager();
	// �f�X�g���N�^
	~TweenManager();

	/* �R�s�[�֎~ */
	TweenManager(const TweenManager&) = delete;
	TweenManager& operator =(const TweenManager&) = delete;

public:
	static TweenManager &GetInstance() {
		static TweenManager t;
		return t;
	}

	// ������
	void Initialize();
	// �X�V
	void Update(const float deltaTime);
	// �ǉ�
	void Add(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// �ǉ�
	void Add(Vector2* value, const EaseType& type, const Vector2& b, const Vector2& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// �ǉ�
	void Add(Vector3* value, const EaseType& type, const Vector3& b, const Vector3& c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// �ǉ�
	void Add(const EaseType& type, float* from, const float to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// �ǉ�
	void Add(const EaseType& type, Vector2* from, const Vector2 to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// �ǉ�
	void Add(const EaseType& type, Vector3* from, const Vector3& to, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// �f�B���C�Ăяo��
	void Delay(const float delay, const std::function<void()>& func, float* debug = nullptr);
	// ���[�v�����icallback�Ăяo����1���[�v�� / �K���f�X�g���N�^�Ȃ�� Cancel ���Ăԁj
	void Loop(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ���ꃋ�[�v�����icallback�Ăяo����1���[�v�� / �K���f�X�g���N�^�Ȃ�� Cancel ���Ăԁj
	void LoopPingPong(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, const std::function<void()>& callback = nullptr, const float s = 1.0f);
	// ���[�v�����i��x�̂݁j
	void LoopOnce(float* value, const EaseType& type, const float b, const float c, const float d = 1.0f, std::function<void()> callback = nullptr, const float s = 1.0f);
	// �N���A
	void Clear();
	// �폜
	void Remove();
	// �L�����Z���i���������ꍇtrue��Ԃ��j
	bool Cancel(const float* value);
	// �L�����Z���i���������ꍇtrue��Ԃ��j
	bool Cancel(const Vector2* value);
	// �L�����Z���i���������ꍇtrue��Ԃ��j
	bool Cancel(const Vector3* value);
	// �f�B���C�L�����Z���i���������ꍇtrue��Ԃ��j
	bool DelayCancel(const float* value = nullptr);

private:
	// �C�[�W���O�p�V�F�A�h�|�C���^
	using TweenPtr = std::shared_ptr<TweenObject>;
	// �C�[�Y���X�g
	std::list<TweenPtr> easeList_;
	//
	using TweenFuncParam = std::function<float(float, float, float, float, float)>;
	//
	std::map<EaseType, TweenFuncParam>	easeFuncMap_;
	//
	Easing	ease_;
	// �X�V�^�C�v
	UpdateType	updateType;
};