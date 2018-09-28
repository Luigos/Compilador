#pragma once

#include "windows.h"
#include "ErrorsModule.h"
#include "Token.h"
#include "vcclr.h"

using namespace System;

#include <map>
#include <vector>
#include <stack>
using namespace std;

namespace CompilerCore
{
#define LEX_ERROR_INVALID_CHARACTER         "Invalid character"
#define LEX_ERROR_STRING_NOT_CLOSED         "String constant not closed"
#define LEX_ERROR_COMMENT_NOT_CLOSED        "Comment not closed"
#define LEX_ERROR_INVALID_FLOAT             "Invalid floating point number. Digit expected"
#define LEX_ERROR_INVALID_LOGICAL_OP_AND    "Invalid logical 'AND' operator. "
#define LEX_ERROR_INVALID_LOGICAL_OP_OR     "Invalid logical 'OR' operator."
#define LEX_ERROR_INVALID_STATE             "Internal error. Invalid state."

	enum class LEX_STATE
	{
		START = 0,
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
		DEL,
		COMMENT
	};

	// LEX analyzer class
	class LexAnalyzer
	{
	private:
		msclr::gcroot<ErrorsModule ^> managedRef_errorsModule;

		std::map<std::string, std::string> m_Keywords;
		std::vector<Token *> m_Tokens;

		// ...
		// ...

	public:

		LexAnalyzer(ErrorsModule ^errorsModule);
		~LexAnalyzer();

		bool ParseSourceCode(const char* sourceCode);
		void reset();
		void getTokens(std::vector<Token *> *tokensVec) const;
	};
}