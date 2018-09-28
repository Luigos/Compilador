#pragma once

#include <string>
using namespace std;

namespace CompilerCore
{
	enum TOKEN_TYPE
	{
		UNDEFINED = 0,
		ID,
		INT,
		KEYWORD,
		FLOAT,
		STRING,
		LOGICAL_OP,
		UNARY_LOGICAL_OP,
		ARIT_OP,
		REL_OP,
		ASSIGN,
		AGROUP_OP,
		DIMEN_OP,
		DEL

	};

	class Token
	{
	private:
		std::string m_Lex;
		TOKEN_TYPE m_Type;
		int m_LineNumber;

	public:
		Token(std::string lex, TOKEN_TYPE type, int lineNum);
		~Token();

		std::string getLex() const { return m_Lex; }
		std::string getTypeStr();
		TOKEN_TYPE getType() const { return m_Type; }
		int getLineNumber() const { return m_LineNumber; }
	};
}
