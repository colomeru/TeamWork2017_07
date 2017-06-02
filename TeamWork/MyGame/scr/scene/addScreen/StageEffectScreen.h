#pragma once
#include <vector>
#include<map>
#include"../Scene.h"
#include"../../stageGenerator/Stage.h"
#include"../../math/Vector3.h"

class StageEffectScreen
{
	struct RGBAColorF {
		float r;
		float g;
		float b;
		float a;


		RGBAColorF(float r = 0.0f,float g=0.0f,float b=0.0f,float a=0.5f) :r(r),g(g),b(b),a(a){
			
		}
		int GetRedColor255I() const{
			return (int)roundf(r * 255.0f);
		}
		int GetGreenColor255I() const{
			return (int)roundf(g * 255.0f);
		}
		int GetBlueColor255I() const{
			return (int)roundf(b * 255.0f);
		}
		int GetAlpha255I() const{
			return (int)roundf(a * 255.0f);
		}
		//static const RGBAColorF Zero;

	};
public:
	// コンストラクタ
	StageEffectScreen();

	void Init(Stage stage=Stage::Stage1);
	void Update();
	// 描画
	void Draw() const;
	void SetIsPlayEffect(bool isplay);
	void StartEffect();
private:
	void drawUpdate();
private:
	int sinCount_;
	std::vector<Scene> changeSceneList_;
	std::vector<float> textAlphaList_;
	std::vector<float> textSizeList_;
	std::map <Stage, RGBAColorF> EffectColor_;
	Stage stage_;

	bool isPlayEffect_;


};