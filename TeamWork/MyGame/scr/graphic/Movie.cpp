#include "Movie.h"
#include <DxLib.h>
#include <sstream>
#include "../game/WorkFolder.h"
#include "../Def.h"

// 動画を読み込む
void Movie::Load(const std::string& filename)
{
	std::string name = WorkFolder::Name + filename;
	int handle = OpenMovieToGraph(name.c_str());

	if (handle == -1)
		throw std::string(filename + "という動画ファイルは存在しません");
	
	m_movies[Split(filename)] = handle;
}

// 動画を再生する
void Movie::Play(const std::string& handle)
{
	IsGlow(handle);

	PlayMovieToGraph(m_movies.at(handle));
}

// 動画を一時停止する
void Movie::Stop(const std::string& handle)
{
	IsGlow(handle);

	PauseMovieToGraph(m_movies.at(handle));
}

// 動画の再生位置を変更する(/秒)
void Movie::Seek(const std::string& handle, float time)
{
	IsGlow(handle);

	SeekMovieToGraph(m_movies.at(handle), static_cast<int>(time * 1000));
}

// 動画の現在の再生位置を受け取る(/秒)
float Movie::IsTime(const std::string& handle) const
{
	IsGlow(handle);

	return TellMovieToGraph(m_movies.at(handle)) / 1000.0f;
}

// 動画が再生されているか否か受け取る
bool Movie::IsPlay(const std::string& handle) const
{
	IsGlow(handle);

	return GetMovieStateToGraph(m_movies.at(handle)) == 1;
}

// 再生状態の動画を表示する
void Movie::Draw() const
{
	for (const auto& movie : m_movies)
	{
		if (IsPlay(movie.first))
		{
			DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, movie.second, FALSE);
		}
	}
}

// ファイル名の'.'から後を取り除く
std::string Movie::Split(std::string filename)
{
	std::string assetname;
	std::getline(
		std::stringstream(filename),
		assetname,
		'.'
		);
	return assetname;
}

// 指定したハンドルがセットされていなければ、スローする
void Movie::IsGlow(const std::string& handle) const
{
	if (m_movies.find(handle) == m_movies.end())
		throw std::string(handle + "という動画ハンドルは読み込まれていません");
}
void Movie::Delete()
{
	for (const auto& movie : m_movies)
	{
		DeleteGraph(movie.second);
	}
}