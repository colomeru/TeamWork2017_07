#pragma once

#include "../../Math/Vector2.h"
#include "../../ResourceLoader/ResourceLoader.h"

#include <string>
#include <unordered_map>

class IWorld;

enum ContentsEnum {
	NUMBER = 0,
	POSITION = 1,
	SPEEKER = 2,
	TEXT_FIRST = 3,
	TEXT_SECOND = 4
};

class TutorialManager {
	struct Elements {
		int posX;
		std::string speaker;
		std::string text1;
		std::string text2;
	};
public:
	TutorialManager();
	~TutorialManager();
	void load_csv(const std::string& file_name);
	void init();
	void update(IWorld* world, float deltaTime);
	void draw()const;
	void changeTutorial(IWorld* world);
	void changeTurn(IWorld* world);
	bool isTutorialEnd();
	void stepText(float deltaTime);

	void tutoStateUpdate(IWorld* world, float deltaTime);
	void popUp(IWorld* world, float deltaTime);
	void popBack(IWorld* world, float deltaTime);
	void change(IWorld* world, float deltaTime);
	void move(IWorld* world, float deltaTime);
private:
	enum TutoState {
		POP_UP,
		TEXT,
		POP_BACK,
		CHANGE,
		MOVE
	};

	Vector2 FUKIDASHI_SIZE = ResourceLoader::GetInstance().GetTextureSize(TextureID::HUKIDASI_TEX);
private:
	TutoState state_;
	float timer_;
	int textIndex_;

	typedef std::unordered_map<int, std::vector<Elements>> TutorialContents;
	TutorialContents contents_;

	Vector2 drawPos_;

	float drawY_;
	float scale_;
	int color_;
	int currentTutorial_;
	int currentTurn_;
	std::string text1_;
	std::string text2_;
	Vector2 position_;
	Vector2 cameraPos_;

	int sound_;

};
