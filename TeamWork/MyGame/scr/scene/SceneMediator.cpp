#include "SceneMediator.h"

void SceneMediator::Initiallize()
{
}

void SceneMediator::Add(ACTOR_ID id, SymbolParameter symbol)
{
	symbolParam_[id].push_back(symbol);
}

void SceneMediator::Clear()
{
	symbolParam_.clear();
}

std::vector<SymbolParameter> SceneMediator::GetList(ACTOR_ID id) const
{
	if (symbolParam_.at(id).size() != 0)
		return symbolParam_.at(id);

	return std::vector<SymbolParameter>();
}

SymbolParameter SceneMediator::GetEncountSymbol() const
{
	return encountSymbol_;
}

void SceneMediator::SetEncountSymbol(SymbolParameter symbol)
{
	encountSymbol_ = symbol;
}

