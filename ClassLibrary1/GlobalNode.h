#pragma once
// Este .h me lo pasaron porque no llegué a esa clase
#include "LocalNode.h"

namespace compilerCore
{
	class globalNode
	{
	private:
		string m_name;
		string m_type;
		SCOPE m_scope;
		int m_dimen;
		void* m_value;
		localNode* m_localNode;

	public:
		globalNode();
		~globalNode();
	};
}