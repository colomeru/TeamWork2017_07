#pragma once
#include"../../../input/InputChecker.h"

class PlayerInputChecker {
public:
	PlayerInputChecker(bool isUse);

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
private:
	bool useKey_;
};