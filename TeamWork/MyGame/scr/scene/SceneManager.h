#pragma once

#include "ISceneMediator.h"
#include "IScenePtr.h"
#include "Scene.h"
#include <unordered_map>

class SceneManager : public ISceneMediator
{
public:
	// �R���X�g���N�^
	SceneManager();
	// ����������
	void Initialize();
	// �X�V����
	void Update();
	// �`�揈��
	void Draw() const;
	// �I��������
	void End();
	// �V�[���`�F���W
	void Change();

	// �V�[���̒ǉ�
	void Add(Scene name, const IScenePtr& scene);
	void SetScene(Scene name);
	template<typename T, typename... Args>
	void Add(Scene name, Args&&... arg)
	{
		Add(name, std::make_shared<T>(arg...));
	}
	// �V�[���̕ύX
	virtual void Change(Scene name) override;
	// ���������w�肷��
	virtual void Init(Scene name) override;
	// �I���������w�肷��
	virtual void Final(Scene name) override;

private:
	// �R�s�[�֎~
	SceneManager(const SceneManager& other) = default;
	SceneManager& operator = (const SceneManager& other) = default;

	// �V�[�������I��
	void SceneReset();

private:
	// �V�[���}�b�v
	std::unordered_map<Scene, IScenePtr> sceneMap_;
	// ���݂̃V�[��
	IScenePtr	currentScene_;

	/* �V�[�������I���p */
	// �I���t���O
	bool		isEnd_;
	// �F���n���h�����f�o�b�O�p�J�E���g
	float		timer_;
	//
	float		freeTime_;
};