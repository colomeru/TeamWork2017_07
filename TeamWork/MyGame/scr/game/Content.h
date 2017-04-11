#pragma once

class Sprite;
class Model;
class Sound;
class File;

class Content
{
public:
	Content() = default;
	~Content() = default;

	///<summary>
	/// �摜��ǂݍ���
	///</summary>
	void LoadSprite(Sprite& sprite, Model& model);

	///<summary>
	/// �R�c���f����ǂݍ���
	///</summary>
	void LoadModel(Model& model, bool async);

	///<summary>
	/// ���y��ǂݍ���
	///</summary>
	void LoadSound(Sound& sound);

	///<summary>
	/// �񓯊��ǂݍ��݂�L����
	///</summary>
	void EnableASync();
	///<summary>
	/// �񓯊��ǂݍ��݂𖳌���
	///</summary>
	void DisableASync();
	///<summary>
	/// ���ݔ񓯊��œǂݍ��܂�Ă��鐔
	///</summary>
	template <typename T>
	int GetASyncLoadCount(T& content);
	///<summary>
	/// �T�E���h�p���ꉻ�ǂݍ��܂�Ă��鐔
	///</summary>
	//template <>
	//int GetASyncLoadCount(Sound& content);

	///<summary>
	/// ���ݔ񓯊��œǂݍ��܂�Ă��邷�ׂĂ̐�
	///</summary>
	//int GetASyncLoadAllCount(Model& model, Sprite& sprite, Sound& sound);

	///<summary>
	/// �񓯊��œǂݍ��݂��s���Ă��邩
	///</summary>
	bool IsASync();
};