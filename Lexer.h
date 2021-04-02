#include "stdafx.h"
#include "Token.h"

#pragma once

class Lexer
{
public:
	Lexer(const char*) noexcept;
	Token next() noexcept;

private:
	const char* m_beg = nullptr;
	std::vector<char> spec_pos;
	Token prev_token;

protected:
	bool is_space(char) noexcept;
	bool is_digit(char) noexcept;
	bool is_identifier_char(char) noexcept;
	bool is_type(std::string) noexcept;

private:
	Token identifier() noexcept;
	Token type(std::string &) noexcept;
	Token paren() noexcept;
	Token atom(Token::Kind) noexcept;
	Token error(Token::Kind, std::string ="") noexcept;

	char peek() const noexcept;
	char get() noexcept;
};

