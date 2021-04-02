#include "Token.h"

Token::Token(Kind kind) noexcept : m_kind(kind) {}

Token::Token(Kind kind, const char* beg, std::size_t len) noexcept
    : m_kind{ kind }, m_lexeme(beg, len) {}

Token::Token(Kind kind, const char* begin, const char* end) noexcept
    : m_kind{ kind }, m_lexeme(begin, std::distance(begin, end)) {}

Token::~Token() {}

/* Kind part */

Token::Kind Token::kind() const noexcept 
{ 
    return m_kind; 
}

void Token::kind(Kind kind) noexcept 
{
    m_kind = kind;
}

/* Compare */

bool Token::is(Kind kind) const noexcept 
{ 
    return m_kind == kind; 
}

bool Token::is_not(Kind kind) const noexcept 
{ 
    return m_kind != kind; 
}

bool Token::is_one_of(Kind k1, Kind k2) const noexcept 
{ 
    return is(k1) || is(k2); 
}

template <typename... Ts>
bool Token::is_one_of(Kind k1, Kind k2, Ts... ks) const noexcept {
    return is(k1) || is_one_of(k2, ks...);
}

/* Lexeme */

std::string_view Token::lexeme() const noexcept 
{ 
    return m_lexeme; 
}

void Token::lexeme(std::string_view lexeme) noexcept 
{
    m_lexeme = std::move(lexeme);
}