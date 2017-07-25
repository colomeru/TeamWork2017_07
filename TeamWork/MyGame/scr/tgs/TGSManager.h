#pragma once


//TGS�p�̃Q�[�������I���N���X
class TGSManager {
private:
	TGSManager();
public:
	static TGSManager& GetInstance();

	//�o�ߎ��ԁA�^�C�}�[�̗L��������������
	void Initialize();
	//���ԍX�V����A�I������A�����܂ł��s��
	bool Update();

	//�Q�[�����I��������
	void GameEnd();
//GetSet�֐�
public:
	void SetActive(bool isActive);

//����֐�
private:
	bool CheckExit_Input()const;
	bool CheckExit_Timer()const;

//�X�V�����֐�
	//���Ԃ��X�V����
	void AddTime();
	//�^�C�}�[�����Z�b�g���邩���f
	void CheckIsReset();
	//
	void ResetTime();
	//�����I���^�C�}�[
	float timeCount_;
	//�^�C�}�[�̗L������
	bool isActive_;

private:
	const float EXIT_TIME_{ 120.0f };
};