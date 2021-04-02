#include "stdafx.h"

#pragma once

class Token
{
public:
	enum class Kind {
        Identifier,
        Type,
        LeftParen,
        RightParen,
        Comma,
		Start,
        End,
        Unexpected,
		UnknownDataType,
		UnnecessaryTypeDeclaration,
		UnknownIdentifier,
		MissedIdentifier,
		MissedLeftParen,
		MissedRightParen,
		MissedBothParen,
		MissedTypeBeforeIdentifier,
		MissedComma,
		UnexpectedComma,
		UnhandledError,
		Fine
	};

private:
	Kind m_kind;
	std::string m_lexeme;

public:
    Token() noexcept;
	Token(Kind) noexcept;
	Token(Kind, const std::string) noexcept;
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

	std::string lexeme() const noexcept;
	void lexeme(std::string) noexcept;
};

