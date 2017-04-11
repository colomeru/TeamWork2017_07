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
	/// 画像を読み込む
	///</summary>
	void LoadSprite(Sprite& sprite, Model& model);

	///<summary>
	/// ３Ｄモデルを読み込む
	///</summary>
	void LoadModel(Model& model, bool async);

	///<summary>
	/// 音楽を読み込む
	///</summary>
	void LoadSound(Sound& sound);

	///<summary>
	/// 非同期読み込みを有効化
	///</summary>
	void EnableASync();
	///<summary>
	/// 非同期読み込みを無効化
	///</summary>
	void DisableASync();
	///<summary>
	/// 現在非同期で読み込まれている数
	///</summary>
	template <typename T>
	int GetASyncLoadCount(T& content);
	///<summary>
	/// サウンド用特殊化読み込まれている数
	///</summary>
	//template <>
	//int GetASyncLoadCount(Sound& content);

	///<summary>
	/// 現在非同期で読み込まれているすべての数
	///</summary>
	//int GetASyncLoadAllCount(Model& model, Sprite& sprite, Sound& sound);

	///<summary>
	/// 非同期で読み込みを行っているか
	///</summary>
	bool IsASync();
};