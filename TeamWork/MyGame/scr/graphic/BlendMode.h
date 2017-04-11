#pragma once

#include <Dxlib.h>

namespace BLEND_MODE
{
	//ブレンドなし
	const int NoBlend	= DX_BLENDMODE_NOBLEND;
	//アルファブレンド
	const int Alpha		= DX_BLENDMODE_ALPHA;
	//加算合成
	const int Add		= DX_BLENDMODE_ADD;
	//減算合成
	const int Sub		= DX_BLENDMODE_SUB;
	//乗算合成
	const int Mul		= DX_BLENDMODE_MULA;
	//反転
	const int Inverse	= DX_BLENDMODE_INVSRC;
	//乗算済みアルファ用
	const int PMA_Alpha = DX_BLENDMODE_PMA_ALPHA;
	//乗算済みアルファ用
	const int PMA_Add	= DX_BLENDMODE_PMA_ADD;
	//乗算済みアルファ用
	const int PMA_Sub	= DX_BLENDMODE_PMA_SUB;
	//乗算済みアルファ用
	const int PMA_Inverse = DX_BLENDMODE_PMA_INVSRC;
}