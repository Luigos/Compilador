#include "LexAnalyzer.h"
#include "stdafx.h"

CompilerCore::LexAnalyzer::LexAnalyzer(ErrorsModule ^ errorsModule)
{
	
	m_Keywords.insert(std::make_pair("var", ""));
	m_Keywords.insert(std::make_pair("int", ""));
	m_Keywords.insert(std::make_pair("float", ""));
	m_Keywords.insert(std::make_pair("string", ""));
	m_Keywords.insert(std::make_pair("bool", ""));
	m_Keywords.insert(std::make_pair("void", ""));
	m_Keywords.insert(std::make_pair("function", ""));
	m_Keywords.insert(std::make_pair("if", ""));
	m_Keywords.insert(std::make_pair("else", ""));
	m_Keywords.insert(std::make_pair("while", ""));
	m_Keywords.insert(std::make_pair("for", ""));
	m_Keywords.insert(std::make_pair("inc", ""));
	m_Keywords.insert(std::make_pair("dec", ""));
	m_Keywords.insert(std::make_pair("switch", ""));
	m_Keywords.insert(std::make_pair("", ""));
	m_Keywords.insert(std::make_pair("default", ""));
	m_Keywords.insert(std::make_pair("print", ""));
	m_Keywords.insert(std::make_pair("read", ""));
	m_Keywords.insert(std::make_pair("return", ""));
	m_Keywords.insert(std::make_pair("true", ""));
	m_Keywords.insert(std::make_pair("false", ""));
}

CompilerCore::LexAnalyzer::~LexAnalyzer()
{
	m_Keywords.clear();
	m_Tokens.clear();
}

bool CompilerCore::LexAnalyzer::ParseSourceCode(const char * sourceCode)
{
	int linenumber = 1;
	const char* CurrChar = sourceCode;
	std::string buffer;
	std::string LineBuffer;
	LEX_STATE state = LEX_STATE::START;
	Token* token;

	while (*CurrChar != '\0')
	{	//y si te callas a la verga por favor
		
		switch (state)
		{
		case CompilerCore::LEX_STATE::START:

			if (isalpha(static_cast<unsigned char>(*CurrChar)) || '_')
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::ID;
			}
			
			if (isdigit(static_cast<unsigned char>(*CurrChar)))
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::INT;
			}

			// Espaciador
			if (*CurrChar == (' ' || '\t' || '\n' || '\r'))
			{
				if (*CurrChar == '\n')
				{
					linenumber++;
				}
				CurrChar++;
			}
			// Logico
			if (*CurrChar == ('&' || '|'))
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::LOGICAL_OP;
			}
			// Aritmetico
			if (*CurrChar == ('+' || '-' || '/' || '*' || '%' || '^'))
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::ARIT_OP;
			}
			// Relacional
			if (*CurrChar == ('<' || '>' || '!' || '='))
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::REL_OP;
			}
			// Delimitador
			if (*CurrChar == ('.' || ',' || ':' || ';'))
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::DEL;
			}
			// Agrupador
			if (*CurrChar == ('{' || '}' || '(' || ')'))
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::AGROUP_OP;
			}
			// Dimensional
			if (*CurrChar == ('[' || ']'))
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::DIMEN_OP;
			}

			if (*CurrChar == '"')
			{
				buffer.clear();
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::STRING;
			}

			break;

		case CompilerCore::LEX_STATE::ID:
			
			// checar si es alphanumeric
			if (isalnum(static_cast<unsigned char>(*CurrChar)) || *CurrChar == '_')
			{
				buffer.append(CurrChar, 1);
				CurrChar++;
			}

			// Una vez que recibe el espacio no entra en el if anterior por lo tanto
			//	entra aca en el cual comparamos con las keywords que tenemos y hacemos el token adecuado
			else
			{
				//keyword
				if (m_Keywords.find(buffer) != m_Keywords.end())
				{
					token = new Token(buffer, CompilerCore::TOKEN_TYPE::KEYWORD, linenumber);
					m_Tokens.push_back(token);
					buffer.clear();
				}

				// Id
				else
				{
					token = new Token(buffer, CompilerCore::TOKEN_TYPE::ID, linenumber);
					m_Tokens.push_back(token);
					buffer.clear();
				}

				//regresamos al estado de inicio
				state = LEX_STATE::START;
			}
			break;

		case CompilerCore::LEX_STATE::INT:
			
			//Checamos por numero
			if (isdigit(static_cast<unsigned char>(*CurrChar)))
			{
				buffer.append(CurrChar, 1);
				CurrChar++;
			}

			//Checamos por punto y encaso de encontrar mandamos a flotantes
			else if (*CurrChar == '.')
			{
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::FLOAT;
			}

			//Terminamos con un entero
			else if (*CurrChar == (' ' || '\t' || '\n' || '\r'))
			{
				token = new Token(buffer, CompilerCore::TOKEN_TYPE::INT, linenumber);
				m_Tokens.push_back(token);
				buffer.clear();

				state = LEX_STATE::START;
			}

			//Error
			else
			{
				if (!managedRef_errorsModule->addErrorLex(linenumber, "Numero invalido", buffer.c_str()))
				{
					return false;
				}
				CurrChar++;

				state = LEX_STATE::START;
			}

			break;

		case CompilerCore::LEX_STATE::LOGICAL_OP:
			
			//Siempre que no esté cerrada la cadena entrará eaqui
			if (*CurrChar != '"')
			{
				//Checamos que el caracter sea enter y damos error en casode que sea
				if (*CurrChar == '\r')
				{
					buffer.append(CurrChar, 1);
					CurrChar++;

					if (!managedRef_errorsModule->addErrorLex(linenumber, LEX_ERROR_STRING_NOT_CLOSED, buffer.c_str()))
					{
						return false;
					}
					
					state = LEX_STATE::START;
				}

				//Mientras solo avanzamos 
				else
				{
					buffer.append(CurrChar, 1);
					CurrChar++;
					
				}
			}

			//Si encontramos las otras comillas terminamosla cadena
			else
			{
				buffer.append(CurrChar, 1);
				CurrChar++;
				token = new Token(buffer, CompilerCore::TOKEN_TYPE::STRING, linenumber);
				m_Tokens.push_back(token);
				buffer.clear();
				state = LEX_STATE::START;
			}

			break;

		case CompilerCore::LEX_STATE::ARIT_OP:
			 //Buscamos para posible numero con signo negativo, ya sea entero o flotante
			if (buffer.back() == '-')
			{
				//Entero
				if (isdigit(static_cast<unsigned char>(*CurrChar)))
				{
					buffer.append(CurrChar, 1);
					CurrChar++;
					state = LEX_STATE::INT;
				}
				//Flotante
				else if (*CurrChar == '.')
				{
					buffer.append(CurrChar, 1);
					CurrChar++;
					state = LEX_STATE::FLOAT;
				}
			}
			//Buscamos por un inicio de comentario
			else if (buffer.back() == '/'  && *CurrChar == '*')
			{
				CurrChar++;
				state = LEX_STATE::COMMENT;
			}

			//Es un operador Aritmetico
			else
			{
				token = new Token(buffer, CompilerCore::TOKEN_TYPE::ARIT_OP, linenumber);
				m_Tokens.push_back(token);
				buffer.clear();
				state = LEX_STATE::START;
			}
			break;

		case CompilerCore::LEX_STATE::REL_OP:
			
			if (*CurrChar != '=')
			{
				if (*CurrChar == ('<' || '>' || '!'))
				{
					if (!managedRef_errorsModule->addErrorLex(linenumber, "Operator Invalido", buffer.c_str()))
					{
						return false;
					}
					CurrChar++;
					state = LEX_STATE::START;
				}

				else if (buffer.back() == '=' || '!')
				{
					if (buffer.back() == '=')
					{

						token = new Token(buffer, CompilerCore::TOKEN_TYPE::ASSIGN, linenumber);
						m_Tokens.push_back(token);
						CurrChar++;
						buffer.clear();
					}
					
					else
					{
						token = new Token(buffer, CompilerCore::TOKEN_TYPE::UNARY_LOGICAL_OP, linenumber);
						m_Tokens.push_back(token);
						buffer.clear();
					}
				}
				
				else
				{
					token = new Token(buffer, CompilerCore::TOKEN_TYPE::REL_OP, linenumber);
					m_Tokens.push_back(token);
					buffer.clear();
				}
				state = LEX_STATE::START;
			
			}

			else
			{
				buffer.append(CurrChar, 1);
				CurrChar++;
				token = new Token(buffer, CompilerCore::TOKEN_TYPE::REL_OP, linenumber);
				m_Tokens.push_back(token);
				buffer.clear();
				state = LEX_STATE::START;
			}
			
			break;

		case CompilerCore::LEX_STATE::DEL:
			
			if (isdigit(static_cast<unsigned char>(*CurrChar)) && buffer.back() == '.')
			{
				buffer.append(CurrChar, 1);
				CurrChar++;
				state = LEX_STATE::FLOAT;
			}
			else
			{
				token = new Token(buffer, CompilerCore::TOKEN_TYPE::DEL, linenumber);
				m_Tokens.push_back(token);
				buffer.clear();
				state = LEX_STATE::START;
			}

			break;

		case CompilerCore::LEX_STATE::AGROUP_OP:
			
			token = new Token(buffer, CompilerCore::TOKEN_TYPE::AGROUP_OP, linenumber);
			m_Tokens.push_back(token);
			buffer.clear();
			state = LEX_STATE::START;

			break;

		case CompilerCore::LEX_STATE::DIMEN_OP:
			
			token = new Token(buffer, CompilerCore::TOKEN_TYPE::DIMEN_OP, linenumber);
			m_Tokens.push_back(token);
			buffer.clear();
			state = LEX_STATE::START;

			break;

		case CompilerCore::LEX_STATE::COMMENT:
			
			if (buffer.back() == '*' && *CurrChar == '/')
			{
				CurrChar++;
				state = LEX_STATE::START;
			}

			if (*CurrChar == '\n' || *CurrChar == '\0')
			{
				if (*CurrChar == '\n')
				{
					if (!managedRef_errorsModule->addErrorLex(linenumber, LEX_ERROR_COMMENT_NOT_CLOSED, buffer.c_str()))
					{
						return false;
					}
					CurrChar++;
					buffer.clear();
				}
				else
				{
					
					if (!managedRef_errorsModule->addErrorLex(linenumber, LEX_ERROR_COMMENT_NOT_CLOSED, buffer.c_str()))
					{
						return false;
					}
					CurrChar++;
					buffer.clear();
				}
			}

			break;

		default:

			if (!managedRef_errorsModule->addErrorLex(linenumber, LEX_ERROR_INVALID_STATE, buffer.c_str()))
			{
				return false;
			}
			CurrChar++;
			buffer.clear();
			
			break;
		

	}


	return false;
}

void CompilerCore::LexAnalyzer::reset()
{
	m_Tokens.clear();
}

void CompilerCore::LexAnalyzer::getTokens(std::vector<Token*>* tokensVec) const
{
}


