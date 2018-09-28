#pragma once
// Este .h me lo pasaron porque no llegué a esa clase
#include <string>

using std::string;

namespace compilerCore
{
	enum class SCOPE
	{
		UNDEFINED,
		FUNC,
		GLOBAL_VAR,
		LOCAL_VAR,
		PARAM
	};

	class localNode
	{
	private:
		string m_type;
		SCOPE m_scope;
		int m_dimen;
		void* m_value;
		localNode* m_localNode;
		string m_funcName;

	public:
		localNode();
		~localNode();
	};
}