#include <iostream>
#include <string>

#pragma once

class Token
{
public:
	enum class Kind {
        Number,
        Identifier,
        Type,
        LeftParen,
        RightParen,
        LeftSquare,
        RightSquare,
        LeftCurly,
        RightCurly,
        LessThan,
        GreaterThan,
        Equal,
        Plus,
        Minus,
        Asterisk,
        Slash,
        Hash,
        Dot,
        Comma,
        Colon,
        Semicolon,
        SingleQuote,
        DoubleQuote,
        Comment,
        Pipe,
        End,
        Unexpected,
	};

private:
	Kind m_kind;
	std::string_view m_lexeme;

public:
	Token(Kind) noexcept;
	Token(Kind, const char*, std::size_t) noexcept;
	Token(Kind kind, const char*, const char*) noexcept;
	~Token() noexcept;

	Kind kind() const noexcept;
	void kind(Kind) noexcept;

	bool is(Kind) const noexcept;
	bool is_not(Kind) const noexcept;
	bool is_one_of(Kind, Kind) const noexcept;

	template <typename... Ts>
	bool is_one_of(Kind, Kind, Ts...) const noexcept;

	std::string_view lexeme() const noexcept;
	void lexeme(std::string_view) noexcept;
};

