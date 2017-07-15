#include "PlayerInputChecker.h"

PlayerInputChecker::PlayerInputChecker(bool isUse) :useKey_(isUse), stickLock_(false)
{
	for (int i = 0; i < (int)InputChecker::Input_Key::Key_Count; i++) {
		keyLockList_[(InputChecker::Input_Key)i] = false;
	}
}

bool PlayerInputChecker::KeyTriggerDown(InputChecker::Input_Key key) const
{
	if (!useKey_)return false;
	if (keyLockList_.at(key))return false;

	return InputChecker::GetInstance().KeyTriggerDown(key);
}

bool PlayerInputChecker::KeyTriggerUp(InputChecker::Input_Key key) const
{
	if (!useKey_)return false;
	if (keyLockList_.at(key))return false;

	return InputChecker::GetInstance().KeyTriggerUp(key);
}

bool PlayerInputChecker::KeyStateDown(InputChecker::Input_Key key) const
{
	if (!useKey_)return false;
	if (keyLockList_.at(key))return false;

	return InputChecker::GetInstance().KeyStateDown(key);
}

bool PlayerInputChecker::KeyStateUp(InputChecker::Input_Key key) const
{
	if (!useKey_)return false;
	if (keyLockList_.at(key))return false;

	return InputChecker::GetInstance().KeyStateUp(key);
}

Vector2 PlayerInputChecker::Stick() const
{
	if (!useKey_)return Vector2::Zero;
	if (stickLock_)return Vector2::Zero;

	return InputChecker::GetInstance().Stick();
}

bool PlayerInputChecker::StickStateDown(InputChecker::Input_Stick stick) const
{
	if (!useKey_)return false;
	if (stickLock_)return false;

	return InputChecker::GetInstance().StickStateDown(stick);
}

bool PlayerInputChecker::StickTriggerDown(InputChecker::Input_Stick stick) const
{
	if (!useKey_)return false;
	if (stickLock_)return false;

	return InputChecker::GetInstance().StickTriggerDown(stick);
}

bool PlayerInputChecker::GetUseKey() const
{
	return useKey_;
}

void PlayerInputChecker::SetUseKey(bool isUse)
{
	useKey_ = isUse;
}

void PlayerInputChecker::SetStickLock(bool isLock)
{
	stickLock_ = isLock;
}

void PlayerInputChecker::SetKeyLock(InputChecker::Input_Key key, bool isLock)
{
	keyLockList_[key] = isLock;
}

void PlayerInputChecker::SetKeyLock(bool isLock)
{
	for (auto& kll : keyLockList_) {
		kll.second = isLock;
	}
}
