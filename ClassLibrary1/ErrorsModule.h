#pragma once

#include "windows.h"
using namespace System;

namespace CompilerCore
{
#define MAX_COMPILER_ERRORS 100

	public enum class ERROR_PHASE
	{
		UNDEFINED,
		LEXIC_ANALYZER,
		SYNTAX_ANALYZER,
		SEMANTIC_ANALYZER,
		CODE_GENERATION,
		SIZE
	};

	public ref class ErrorsModule
	{
	private:
		int m_numErrors;
		cli::array<String ^> ^ m_errorsArray;

		bool addError(ERROR_PHASE errorPhase, int lineNumber, String ^ errorDesc, String ^ errorLine);

	public:

		ErrorsModule();
		~ErrorsModule();

		void reset();
		bool addErrorLex(int lineNum, const char *desc, const char *line);

		property cli::array<String ^> ^ Errors
		{
			cli::array<String ^> ^ get()
			{
				return m_errorsArray;
			}
		}
	};
}