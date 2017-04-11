#include "ResourceLoader.h"

ResourceLoader::ResourceLoader() {
}

ResourceLoader::~ResourceLoader() {
	models_.clear();
	textures_.clear();
	sounds_.clear();
	animations_.clear();
}

ResourceLoader & ResourceLoader::GetInstance() {
	static ResourceLoader instance;
	return instance;
}

void ResourceLoader::loadModel(const ModelID& id, const char* file_name) {
	int handle = MV1LoadModel(file_name);

	if (handle == -1) throw std::string("ファイルは存在しません");

	//if (MV1LoadModel(file_name) == -1)DxLib_End();
	models_[id] = handle;
}

void ResourceLoader::loadTexture(const TextureID& id, const char* file_name) {
	int handle = LoadGraph(file_name);

	if (handle == -1) throw std::string("ファイルは存在しません");

	textures_[id] = handle;
	//if (LoadGraph(file_name) == -1)DxLib_End();
	//textures_[id] = LoadGraph(file_name);
}

void ResourceLoader::loadAnimation(const AnimationID & id, const char * file_name, const Vector2 & size, const int & row, const int & column, const int & surplus){
	if (LoadGraph(file_name) == -1) throw std::string("ファイルは存在しません");

	animationsSize_[id] = size;

	int handle[500];
	LoadDivGraph(file_name, (row*column) - surplus, row,column, size.x, size.y, handle);
	for (int i = 0; i < (row*column) - surplus; i++) {
		animations_[id].push_back(handle[i]);
	}
}

void ResourceLoader::loadAnimation(const AnimationID& id, const char * file_name, const int & size, const int & row, const int & column, const int & surplus){
	loadAnimation(id, file_name, Vector2::One * size, row, column, surplus);
}

void ResourceLoader::loadSound(const SoundID& id, const char * file_name){
	int handle = LoadSoundMem(file_name);

	if (handle == -1) throw std::string("ファイルは存在しません");

	sounds_[id] = handle;
}

int ResourceLoader::getModelID(const ModelID& id) {
	return models_[id];
}

int ResourceLoader::getTextureID(const TextureID& id) {
	return textures_[id];
}

int ResourceLoader::getSoundID(const SoundID& id){
	return sounds_[id];
}

std::vector<int> ResourceLoader::getAnimationIDs(const AnimationID& id){
	return animations_[id];
}

Vector2 ResourceLoader::GetTextureSize(TextureID id)
{
	int x, y, flag;
	//テクスチャのサイズ取得
	flag = GetGraphSize(ResourceLoader::GetInstance().getTextureID(id), &x, &y);
	//エラーだ
	if (flag < 0) return Vector2::Zero;
	//成功だ
		return Vector2(static_cast<float>(x), static_cast<float>(y));
}

Vector2 ResourceLoader::GetTextureSize(AnimationID id)
{
	return animationsSize_[id];
}

void ResourceLoader::clearResources()
{
	for (auto model : models_) {
		MV1DeleteModel(model.second);
	}
	models_.clear();
	for (auto texture : textures_) {
		MV1DeleteModel(texture.second);
	}
	textures_.clear();
}
