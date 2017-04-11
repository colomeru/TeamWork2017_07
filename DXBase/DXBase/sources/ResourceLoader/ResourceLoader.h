#pragma once

#include <DxLib.h>

#include <unordered_map>
#include <string>
#include "../Math/Vector2.h"

#include "ResourceID.h"

class ResourceLoader {
private:
	ResourceLoader();
	~ResourceLoader();
public:
	static ResourceLoader& GetInstance();

	void loadModel(const ModelID& id, const char* file_name);
	void loadTexture(const TextureID& id, const char* file_name);
	void loadAnimation(const AnimationID& id, const char* file_name, const Vector2 & size, const int & row, const int & column, const int & surplus = 0);
	void loadAnimation(const AnimationID& id, const char* file_name, const int & size, const int & row, const int & column, const int & surplus = 0);
	void loadSound(const SoundID& id, const char* file_name);

	int getModelID(const ModelID& id);
	int getTextureID(const TextureID& id);
	int getSoundID(const SoundID& id);
	std::vector<int> getAnimationIDs(const AnimationID& id);
	//テクスチャのサイズを取得する(引数:テクスチャハンドル,戻り値:Vector2)
	Vector2 GetTextureSize(TextureID id);
	Vector2 GetTextureSize(AnimationID id);
	void clearResources();
private:
	std::unordered_map<ModelID, int> models_;
	std::unordered_map<TextureID, int> textures_;
	std::unordered_map<SoundID, int> sounds_;
	std::unordered_map<AnimationID, std::vector<int>> animations_;
	std::unordered_map <AnimationID, Vector2> animationsSize_;

};