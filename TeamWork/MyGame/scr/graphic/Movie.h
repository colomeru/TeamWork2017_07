#pragma once

#include <unordered_map>
#include <string>

class Movie
{
public:
	Movie() = default;
	~Movie() = default;

	///<summary>
	/// 動画を読み込む
	///</summary>
	void Load(const std::string& filename);

	///<summary>
	/// 動画を再生する
	///</summary>
	void Play(const std::string& handle);

	///<summary>
	/// 動画を一時停止する
	///</summary>
	void Stop(const std::string& handle);

	///<summary>
	/// 動画の再生位置を変更する(/秒)
	///</summary>
	void Seek(const std::string& handle, float time);

	///<summary>
	/// 動画の現在の再生位置を受け取る(/秒)
	///</summary>
	float IsTime(const std::string& handle) const;

	///<summary>
	/// 動画が再生されているか否か受け取る
	///</summary>
	bool IsPlay(const std::string& handle) const;

	///<summary>
	/// 再生状態の動画を表示する
	///</summary>
	void Draw() const;

private:
	///<summary>
	/// ファイル名の'.'から後を取り除く
	///</summary>
	std::string Split(std::string filename);

	///<summary>
	/// 指定したハンドルがセットされていなければ、スローする
	///</summary>
	void IsGlow(const std::string& handle) const;

	std::unordered_map<std::string, int> m_movies;

};