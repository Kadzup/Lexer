#include "Token.h"

#pragma once

class Lexer
{
public:
	Lexer(const char*) noexcept;
	Token next() noexcept;

private:
	const char* m_beg = nullptr;

public:
	bool is_space(char) noexcept;
	bool is_digit(char) noexcept;
	bool is_identifier_char(char) noexcept;

private:
	Token identifier() noexcept;
	Token number() noexcept;
	Token slash_or_comment() noexcept;
	Token atom(Token::Kind) noexcept;

	char peek() const noexcept;
	char get() noexcept;
};

