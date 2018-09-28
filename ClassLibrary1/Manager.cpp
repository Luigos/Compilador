#include "stdafx.h"
#include "ErrorsModule.h"
#include "Manager.h"

/*
*/
CompilerCore::Manager::Manager()
{
	managed_errorsModule = gcnew ErrorsModule();
	cpp_lexAnalyzer = new LexAnalyzer(managed_errorsModule);
}

/*
*/
CompilerCore::Manager::~Manager()
{
	// Free memory of LEX analyzer
	if (cpp_lexAnalyzer != nullptr)
	{
		delete cpp_lexAnalyzer;
		cpp_lexAnalyzer = nullptr;
	}

	// Memory of ERRORS MODULE is automatically collected (allocated with gcnew)
}

/*
* Main entry point into the compiler logic
*/
cli::array<String ^> ^ CompilerCore::Manager::compileProgram(String ^ sourceCode)
{
	cli::array<String ^> ^ compilationDetails;

	// Clear state if all pointers are valid
	if (managed_errorsModule && cpp_lexAnalyzer != nullptr)
	{
		managed_errorsModule->reset();
		cpp_lexAnalyzer->reset();
	}
	else
	{
		// invalid pointers, return error...
	}

	lexAnalysis(sourceCode);

	compilationDetails = getCompilationDetails();

	return compilationDetails;
}

/*
*/
void CompilerCore::Manager::lexAnalysis(String ^ sourceCode)
{
	if (cpp_lexAnalyzer != nullptr)
	{
		cpp_lexAnalyzer->parseSourceCode((const char *)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(sourceCode).ToPointer());
	}
}

// ===========================================================================================
// EL CODIGO DE getCompilationDetails() SE LOS DEJO A USTEDES (hint: lean el libro que les dije para que vean como manejar arrays en CLI)
//
// La clase Manager solo tiene un punto de entrada (publico), que es compileProgram(). PERO, ese metodo debe regresar los ERRORES (si hubo alguno) y TAMBIEN los TOKENS.
// Por eso el metodo se llama getCompilationDetails() y no getCompilationErrors().
// Es decir, el array que van a regresar a la Windows Form va a ir en este formato (es un ejemplo)
//
// compilationDetails[0] = "<LEXIC_ANALYZER> Error, simbolo ilegal '@'. Linea 1. Linea --> 'var @'";
// compilationDetails[1] = "<LEXIC_ANALYZER> Error, comentario sin cerrar. Linea 3. Linea --> '/* Este es un comentario'";
// compilationDetails[2] = "@"; // Dado que en la gramatica no permitimos la '@', les recomiendo usarla como marcador para que la Windows Form, al leer el array de compilation details,
//                              // sepa donde inician los errores y donde inician los tokens.
// compilationDetails[3] = "var  KEYWORD";
// compilationDetails[4] = "a    ID";
// compilationDetails[5] = ",    DELIMITADOR";
// etc...


cli::array<String^>^ compilerCore::Manager::getCompilationDetails()
{
	vector<token*>* tokens = new vector<token*>();
	m_lexAnalyzer->getTokens(tokens);
	cli::array<String^>^ tokenArray = gcnew cli::array<String^>(tokens->size());
	int i = 0;
	for (vector<token*>::iterator it = tokens->begin(); it != tokens->end(); ++it)
	{
		String^ lex = gcnew String((*it)->getLex().c_str());
		String^ type = gcnew String((*it)->getTypeStr().c_str());
		tokenArray->SetValue(String::Format("{0} {1} {2}", lex, type, (*it)->getLineNumber()), i);
		i++;
	}
	int k = 0;
	for (int i = 0; i < m_errorModule->Errors->Length; i++)
	{
		if (m_errorModule->Errors[i] != "")
		{
			k++;
		}
	}
	cli::array<String^>^ compDetails = gcnew cli::array<String^>(k + tokens->size());
	for (i = 0; i < m_errorModule->Errors->Length; i++)
	{
		if (m_errorModule->Errors[i] != "")
		{
			compDetails[i] = m_errorModule->Errors[i];
		}
	}
	for (int j = 0; j < tokens->size(); j++)
	{
		compDetails[j + k] = tokenArray[j];
	}
	return compDetails;
}

cli::array<cli::array<String^>^, 2>^ compilerCore::Manager::compile(String^ sourceCode)
{
	//cli::array<String^>^ compilationDetails = gcnew cli::array<String^>(1);
	//compilationDetails[0] = gcnew String("Hola mundo.");
	//return compilationDetails;
	cli::array<cli::array<String^>^, 2>^ compilationDetails;

	// Clear state if all pointers are valid
	if (m_errorModule && m_lexAnalyzer != nullptr)
	{
		m_errorModule->reset();
		m_lexAnalyzer->reset();
	}
	else
	{
		// invalid pointers, return error...
	}

	lexAnalysis(sourceCode);

	compilationDetails = getCompDetails();

	return compilationDetails;
}

cli::array<cli::array<String^>^, 2>^ compilerCore::Manager::getCompDetails()
{
	cli::array<cli::array<String^>^, 2>^ compDetails = gcnew cli::array<cli::array<String^>^, 2>(1, 1);
	return compDetails;
}
