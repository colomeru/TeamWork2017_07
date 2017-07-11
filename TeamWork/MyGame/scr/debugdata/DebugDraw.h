#pragma once
#include<DxLib.h>
#include<string>

class DebugDraw {
public:
#ifdef NDEBUG
	template<class ...T>
	//デバッグ専用の文字列描画
	static void DebugDrawFormatString(int x,int y,unsigned int color,const std::string& text, T ...args){}

	static void DebugDrawLine(int x1, int y1, int x2, int y2, unsigned int color, int thickness = 1) {}
#else
	template<class ...T>
	//デバッグ用文字列描画
	static void DebugDrawFormatString(int x, int y, unsigned int color, const std::string& text, T ...args) {
		DrawFormatString(x, y, color, text.c_str(), args...);
	}
	//デバッグ用線分描画
	static void DebugDrawLine(int x1,int y1,int x2,int y2,unsigned int color,int thickness=1) {
		DrawLine(x1,y1,x2,y2,color,thickness);
	}

#endif

};

