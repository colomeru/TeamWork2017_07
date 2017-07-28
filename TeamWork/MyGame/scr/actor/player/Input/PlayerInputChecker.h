#pragma once
#include"../../../input/InputChecker.h"

class PlayerInputChecker {
public:
	explicit PlayerInputChecker(bool isUse);

	//�L�[�������ꂽ����Ԃ�
	bool KeyTriggerDown(InputChecker::Input_Key key)const;
	//�L�[������������Ԃ�
	bool KeyTriggerUp(InputChecker::Input_Key key)const;
	//�L�[�������Ă��邩��Ԃ�
	bool KeyStateDown(InputChecker::Input_Key key)const;
	//�L�[��������Ă��邩��Ԃ�
	bool KeyStateUp(InputChecker::Input_Key key)const;

	//�X�e�B�b�N�̌X����Ԃ�
	Vector2 Stick()const;

	//�w������ɃX�e�B�b�N���|��Ă��邩��Ԃ�
	bool StickStateDown(InputChecker::Input_Stick stick)const;
	//�w������ɃX�e�B�b�N��|��������Ԃ�
	bool StickTriggerDown(InputChecker::Input_Stick stick)const;

	//�L�[�𗘗p�\�����󂯎��
	bool GetUseKey()const;
	//�L�[�𗘗p�\����ݒ肷��
	void SetUseKey(bool isUse);

	//�X�e�B�b�N�̃��b�N��ݒ肷��
	void SetStickLock(bool isLock);
	//����L�[�̃��b�N��ݒ肷��
	void SetKeyLock(InputChecker::Input_Key key, bool isLock);
	//�S�ẴL�[�̃��b�N��ݒ肷��
	void SetKeyLock(bool isLock);
private:
	//�S�̂ɑ΂���L�[���b�N
	bool useKey_;

	//����L�[�ɑ΂���L�[���b�N
	std::map<InputChecker::Input_Key, bool> keyLockList_;
	//�X�e�B�b�N�ɑ΂���L�[���b�N
	bool stickLock_;
};