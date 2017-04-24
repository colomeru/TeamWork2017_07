#include "Sound.h"
#include "../game/WorkFolder.h"
#include "../Def.h"
#include "../math/Math.h"
#include <sstream>

Sound::Sound():
	m_bgm_volume(BGMVOLUME), m_se_volume(SEVOLUME)
{
	
}

Sound::~Sound()
{
	InitSoundMem();
}

// 初期化処理
void Sound::Initialize()
{

	InitSoundMem();
	m_BGMs.clear();
	m_SEs.clear();

	//dupSE.clear();
	for (auto& se : dupSE)
	{
		se = 0;
	}
}

// BGMを読み込む
void Sound::LoadBGM(const std::string& filename, const BGM_ID& id, float volume)
{
	m_BGMs[id] = LoadSoundMem((WorkFolder::Name + filename).c_str());
	m_BGM_Volumes[id] = volume;
	SettingBGM(id);
}

// SEを読み込む
void Sound::LoadSE(const std::string& filename, const SE_ID& id, float volume)
{
	m_SEs[id] = LoadSoundMem((WorkFolder::Name + filename).c_str());
	m_SE_Volumes[id] = volume;
	SettingSE(id);
}

// 3D(立体音響)で再生したいSEを読み込む
void Sound::Load3DSE(const std::string& filename, const SE_ID& id, float volume)
{
	SetCreate3DSoundFlag(TRUE);
	m_SEs[id] = LoadSoundMem((WorkFolder::Name + filename).c_str());
	m_SE_Volumes[id] = volume;
	SetCreate3DSoundFlag(FALSE);

	SettingSE(id);
}

void Sound::DeleteAll()
{
	std::unordered_map<BGM_ID, int>::iterator bgmitr = m_BGMs.begin();
	while (bgmitr != m_BGMs.end()) {
		DeleteSoundMem(bgmitr->first);
		++bgmitr;
	}
	m_BGMs.clear();
	
	std::unordered_map<SE_ID, int>::iterator seitr = m_SEs.begin();
	while (seitr != m_SEs.end()) {
		DeleteSoundMem(seitr->first);
		++seitr;
	}
	m_SEs.clear();

	m_BGM_Volumes.clear();
	m_SE_Volumes.clear();
}

// ファイル名の'.'から後を取り除く
std::string Sound::Split(const std::string& filename)
{
	std::string assetname;
	std::getline(
		std::stringstream(filename),
		assetname,
		'.'
	);
	return assetname;
}

// BGMを鳴らす
void Sound::PlayBGM(const BGM_ID& id, int playtype)
{
	PlaySoundMem(m_BGMs[id], playtype);
}

// SEを鳴らす
void Sound::PlaySE(const SE_ID& id, int playtype)
{
	if (IsPlaySE(id) && playtype == DX_PLAYTYPE_LOOP)
		return;

	ChangeVolumeSoundMem((int)((m_se_volume * m_SE_Volumes[id]) * 255), m_SEs[id]);
	PlaySoundMem(m_SEs[id], playtype);
}

void Sound::PlaySEDuplicate(const SE_ID& id, int playtype)
{
	if (IsPlaySE(id) && playtype == DX_PLAYTYPE_LOOP)
		return;

	ChangeVolumeSoundMem((int)((m_se_volume * m_SE_Volumes[id]) * 255), m_SEs[id]);
	int dupHandle = DuplicateSoundMem(m_SEs[id]);
	PlaySoundMem(dupHandle, playtype);

	for (auto& se : dupSE)
	{
		if (se == 0)
		{
			se = dupHandle;
			return;
		}
	}
}

// BGMを止める
void Sound::StopBGM()
{
	for (auto i : m_BGMs)
	{
		StopSoundMem(i.second);
	}
}

// SEを止める
void Sound::StopSE(const SE_ID& id)
{
	StopSoundMem(m_SEs[id]);
}

// BGMが鳴っているかどうか調べる
bool Sound::IsPlayBGM()
{
	for (auto i : m_BGMs)
	{
		if (CheckSoundMem(i.second))
		{
			return true;
		}
	}
	return false;
}

// SEが鳴っているかどうか調べる
bool Sound::IsPlaySE(const SE_ID& id)
{
	return CheckSoundMem(m_SEs[id]) == 1;
}

// 各BGMのボリュームを設定する
void Sound::SetBGMVolume(const BGM_ID& id, float volume)
{
	m_BGM_Volumes[id] = volume;
	SettingBGM(id);
}

// 各SEのボリュームを設定する
void Sound::SetSEVolume(const SE_ID& id, float volume)
{
	m_SE_Volumes[id] = volume;
	SettingSE(id);
}

// 各BGMのボリュームを受け取る
float Sound::IsBGMVolume(const BGM_ID& id)
{
	return m_BGM_Volumes[id];
}

// 各SEのボリュームを受け取る
float Sound::IsSEVolume(const SE_ID& id)
{
	return m_SE_Volumes[id];
}

// 全てのBGMのボリュームを設定する
void Sound::SetAllBGMVolume(float volume)
{
	m_bgm_volume = volume;
	for (auto& i : m_BGMs)
		SettingBGM(i.first);
	m_bgm_volume = Math::Clamp(m_bgm_volume, 0.0f, 1.0f);
}

// 全てのSEのボリュームを設定する
void Sound::SetAllSEVolume(float volume)
{
	m_se_volume = volume;
	for (auto& i : m_SEs)
		SettingSE(i.first);
	m_se_volume = Math::Clamp(m_se_volume, 0.0f, 1.0f);
}

// 全てのBGMのボリュームを受け取る
float Sound::IsAllBGMVolume()
{
	return m_bgm_volume;
}

// 全てのSEのボリュームを受け取る
float Sound::IsAllSEVolume()
{
	return m_se_volume;
}

void Sound::Update()
{
	for (auto& se : dupSE)
	{
		if (CheckSoundMem(se) == 0 && se != 0)
		{
			DeleteSoundMem(se);
			se = 0;
		}
	}
}

void Sound::SettingBGM(const BGM_ID& id)
{
	ChangeVolumeSoundMem((int)((m_bgm_volume * m_BGM_Volumes[id]) * 255), m_BGMs[id]);
}

void Sound::SettingSE(const SE_ID& id)
{
	ChangeVolumeSoundMem((int)((m_se_volume * m_SE_Volumes[id]) * 255), m_SEs[id]);
}