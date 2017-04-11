#pragma once
#include "../game/ID.h"
#include "../actor/ActorParameter.h"
#include <memory>
#include <functional>
#include <unordered_map>

class SceneMediator
{
private:
	SceneMediator() = default;
	~SceneMediator() = default;

public:
	// インスタンス
	static SceneMediator& GetInstance()
	{
		static SceneMediator s;
		return s;
	}

	// 初期化
	void Initiallize();
	// 追加
	void Add(ACTOR_ID id, SymbolParameter symbol);
	// クリア
	void Clear();
	void SetEncountSymbol(SymbolParameter symbol);
	// 
	std::vector<SymbolParameter> GetList(ACTOR_ID id) const;
	// エンカウントエネミーの取得
	SymbolParameter GetEncountSymbol() const;
	// エンカウントエネミーのセット

private:
	// ステージのエネミーデータ
	std::unordered_map<ACTOR_ID, std::vector<SymbolParameter>>	symbolParam_;
	// エンカウントエネミーデータ
	SymbolParameter		encountSymbol_;
};