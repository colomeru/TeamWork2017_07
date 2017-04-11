#ifndef _GAME_DEVICE_H_
#define _GAME_DEVICE_H_

#include "DxLib.h"

class GameDevice{
public:
	GameDevice(int width, int height, float rate, int windowFlag, float fps, int colorBitDepth = 32);
	~GameDevice();
	//バッファをフリップ
	void flip();
private:
	// コピー禁止
	GameDevice(const GameDevice& other) = delete;
	GameDevice& operator = (const GameDevice& other) = delete;
};

#endif