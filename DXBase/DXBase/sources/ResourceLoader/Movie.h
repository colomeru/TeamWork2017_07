#pragma once

#include <unordered_map>
#include <string>
#include"ResourceLoader.h"
class Movie
{
public:
	static Movie& GetInstance();
	///<summary>
	/// 動画を読み込む
	///</summary>
	void Load(const MOVIE_ID& moveID, const std::string& filename);

	///<summary>
	/// 動画を再生する
	///</summary>
	void Play(const MOVIE_ID& moveID);

	///<summary>
	/// 動画を一時停止する
	///</summary>
	void Stop(const MOVIE_ID& moveID);

	///<summary>
	/// 動画の再生位置を変更する(/秒)
	///</summary>
	void Seek(const MOVIE_ID& moveID, float time);

	///<summary>
	/// 動画の現在の再生位置を受け取る(/秒)
	///</summary>
	float IsTime(const MOVIE_ID& moveID);

	///<summary>
	/// 動画が再生されているか否か受け取る
	///</summary>
	bool IsPlay(const MOVIE_ID& moveID);

	///<summary>
	/// 再生状態の動画を表示する
	///</summary>
	void Draw(const MOVIE_ID& id,const Vector2& pos1,const float& scale);

	void Draw(const MOVIE_ID& id);

	void DrawRotaMovie(const MOVIE_ID& id, const Vector2& pos, const float& scale);

	//動画のサイズを取得
	Vector2 GetMovieSize(const MOVIE_ID& id);

	void Clear();
private:

	int GetHandle(const MOVIE_ID& moveID);
	std::unordered_map<const MOVIE_ID, int> m_movies;

};