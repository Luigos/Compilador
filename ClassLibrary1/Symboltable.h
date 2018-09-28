#pragma once

#include <map>
#include <string>
#include "GlobalNode.h"

using std::string;
using std::map;

namespace compilerCore
{
	class symTable
	{
	private:
		map<string, globalNode> m_symbols;
	public:
		symTable();
		~symTable();
		bool addSymbol();
		bool symbolExists();
		string getSymbolName();
		string getSymbolScope();
		bool updateSymbolType();
		int getSymbolDimension();
	};
}