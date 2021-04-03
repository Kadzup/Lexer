#include "Lexer.h"

Lexer::Lexer(const char* beg) noexcept : m_beg{ beg } {
    // temp
    m_prev_token = Token(Token::Kind::Start);
    m_spec_pos.push_back('#');
}

/* Navigation */

Token Lexer::next() noexcept {
    while (is_space(peek())) get();

    switch (peek()) {
    case '\0': 
    {
        // check for ',' '(' ')'
        if(!m_prev_token.is(Token::Kind::MissedBothParen) && !m_prev_token.is(Token::Kind::MissedLeftParen) && !m_prev_token.is(Token::Kind::MissedRightParen))
        {
            return m_prev_token = paren();
        }

        return m_prev_token = Token(Token::Kind::End, m_beg, 1);
    }
    default: 
    {
        std::string tempStr = "";
        
        // is type?
        Token typeToken = type(tempStr);

        if (m_prev_token.is(Token::Kind::Type) || m_prev_token.is(Token::Kind::UnnecessaryTypeDeclaration))
        {
            if (typeToken.is(Token::Kind::Type))
            {
                return m_prev_token = error(Token::Kind::UnnecessaryTypeDeclaration, tempStr);
            }
            else
            {
                return m_prev_token = error(Token::Kind::UnknownIdentifier, tempStr);
            }
        }
        else if(m_prev_token.is(Token::Kind::Identifier))
        { 
            if (std::find(m_spec_pos.begin(), m_spec_pos.end(), '(') != m_spec_pos.end())
            {
                return m_prev_token = error(Token::Kind::MissedComma, tempStr);
            }
            else if (typeToken.is(Token::Kind::Type))
            {
                return m_prev_token = error(Token::Kind::UnnecessaryTypeDeclaration, tempStr);
            }
            else if (typeToken.is(Token::Kind::UnknownDataType))
            {
                return m_prev_token = typeToken;
            }
        }
        else
        {
            if (typeToken.is(Token::Kind::Type))
            {
                // is '(' exist?
                if(std::find(m_spec_pos.begin(), m_spec_pos.end(), '(') != m_spec_pos.end())
                {
                    // is ident before?
                    if (m_prev_token.is(Token::Kind::Comma))
                    {
                        return m_prev_token = typeToken;
                    }
                    else if (m_prev_token.is(Token::Kind::Identifier)) {
                        return m_prev_token = error(Token::Kind::MissedComma);
                    }
                    else
                    {
                        return m_prev_token = typeToken;
                    }
                }
                else
                {
                    return m_prev_token = typeToken;
                }
            }
            else if (typeToken.is(Token::Kind::UnknownDataType))
            {
                return m_prev_token = typeToken;
            }
        }

        return m_prev_token = error(Token::Kind::UnhandledError);
    }
    case 'a':
    case 'b':
    case 'c':
    case 'A':
    case 'B':
    case 'C':
        return m_prev_token = identifier();
    case '(': 
    {
        m_spec_pos.push_back(peek());
        return m_prev_token = atom(Token::Kind::LeftParen);
    }
    case ')':
        m_spec_pos.push_back(peek());

        if(m_prev_token.is(Token::Kind::Type))
        {
            return m_prev_token = error(Token::Kind::MissedIdentifier);
        }
        else if (m_prev_token.is(Token::Kind::Comma))
        {
            return m_prev_token = error(Token::Kind::UnexpectedComma);
        }

        return m_prev_token = atom(Token::Kind::RightParen);
    case ',':
        if (m_prev_token.is(Token::Kind::LeftParen))
        {
            get();
            return m_prev_token = error(Token::Kind::UnexpectedComma);
        }
        else if (m_prev_token.is(Token::Kind::Comma))
        {
            get();
            return m_prev_token = error(Token::Kind::UnexpectedComma);
        }
        else if (m_prev_token.is(Token::Kind::UnexpectedComma))
        {
            get();
            return m_prev_token = error(Token::Kind::UnexpectedComma);
        }
        else 
        {
            return m_prev_token = atom(Token::Kind::Comma);
        }
    }
}

char Lexer::peek() const noexcept
{ 
    return *m_beg; 
}

char Lexer::get() noexcept 
{ 
    return *m_beg++; 
}

/* Compare */

bool Lexer::is_space(char c) noexcept 
{
    switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
        return true;
    default:
        return false;
    }
}

bool Lexer::is_spec_char(char c) noexcept
{
    switch (c)
    {
    case '(':
    case ')':
    case ',':
    case '\0':
        return true;
    default:
        return false;
    }
}

bool Lexer::is_identifier_char(char c) noexcept {
    switch (c) {
    case 'a':
    case 'b':
    case 'c':
    case 'A':
    case 'B':
    case 'C':
        return true;
    default:
        return false;
    }
}

bool Lexer::is_type(std::string_view type) noexcept
{
    if (type.empty())
        return false;

    return type == "float" || type == "int" || type == "void";
}

/* Tokens */

Token Lexer::atom(Token::Kind kind) noexcept 
{ 
    return Token(kind, m_beg++, 1); 
}

Token Lexer::identifier() noexcept 
{
    const char* start = m_beg;
    bool isValid = true;
    get();

    while (!is_space(peek()) && !is_spec_char(peek()))
    {
        if (!is_identifier_char(peek()))
        {
            isValid = false;
        }

        get();
    }

    if (!isValid)
    {
        return error(Token::Kind::UnknownIdentifier, std::string(start, m_beg));
    }

    if (m_prev_token.is_not(Token::Kind::Type) && m_prev_token.is_not(Token::Kind::UnnecessaryTypeDeclaration)) {
        return error(Token::Kind::MissedTypeBeforeIdentifier, std::string(start, m_beg));
    }

    return Token(Token::Kind::Identifier, start, m_beg);
}

Token Lexer::type(std::string &str) noexcept
{
    const char* start = m_beg;
    std::string type = "";

    while (!is_space(peek())  && !is_spec_char(peek()))
    {
        type += peek();
        get();
    }

    str = type;

    if(is_type(type))
        return Token(Token::Kind::Type, start, m_beg);
    return error(Token::Kind::UnknownDataType, type);
}

Token Lexer::paren() noexcept
{
    bool lp = std::find(m_spec_pos.begin(), m_spec_pos.end(), '(') != m_spec_pos.end();
    bool rp = std::find(m_spec_pos.begin(), m_spec_pos.end(), ')') != m_spec_pos.end();

    if(!lp && !rp)
    {
        return error(Token::Kind::MissedBothParen);
    }
    else if (!lp) 
    {
        return error(Token::Kind::MissedLeftParen);
    }
    else if(!rp)
    {
        return error(Token::Kind::MissedRightParen);
    }

    return Token(Token::Kind::End);
}

Token Lexer::error(Token::Kind kind, std::string name) noexcept
{
    Token error;
    std::string errorMessage = "[Error]: ";
    switch (kind) {
    case Token::Kind::UnknownDataType:
    {
        errorMessage += "Unknown Data Type";
        if (name != "") 
        {
            errorMessage += " '" + name + "'\0";
        }

        error = Token(Token::Kind::UnknownDataType, errorMessage);

        break;
    }
    case Token::Kind::UnknownIdentifier:
    {
        errorMessage += "Unknown Identifier";
        if (name != "")
        {
            errorMessage += " '" + name + "'";
        }

        error = Token(Token::Kind::UnknownIdentifier, errorMessage);

        break;
    }
    case Token::Kind::UnnecessaryTypeDeclaration:
    {
        errorMessage += "Unnecessary Type Declaration";
        if (name != "")
        {
            errorMessage += " '" + name + "'";
        }

        error = Token(Token::Kind::UnnecessaryTypeDeclaration, errorMessage);

        break;
    }
    case Token::Kind::MissedTypeBeforeIdentifier:
    {
        errorMessage += "Missed Type Before Identifier";
        if (name != "")
        {
            errorMessage += " '" + name + "'";
        }

        error = Token(Token::Kind::MissedTypeBeforeIdentifier, errorMessage);

        break;
    }
    case Token::Kind::MissedIdentifier:
    {
        errorMessage += "Missed Identifier";
        if (name != "")
        {
            errorMessage += " '" + name + "'";
        }

        error = Token(Token::Kind::MissedIdentifier, errorMessage);

        break;
    }
    case Token::Kind::Unexpected:
    {
        errorMessage += "Unexpected";
        if (name != "")
        {
            errorMessage += " '" + name + "'";
        }

        error = Token(Token::Kind::Unexpected, errorMessage);

        break;
    }
    case Token::Kind::MissedComma:
    {
        errorMessage += "Missed Comma";

        error = Token(Token::Kind::MissedComma, errorMessage);

        break;
    }
    case Token::Kind::UnexpectedComma:
    {
        errorMessage += "Unexpected Comma";

        error = Token(Token::Kind::UnexpectedComma, errorMessage);

        break;
    }
    case Token::Kind::MissedLeftParen:
    {
        errorMessage += "Missed Left Paren";

        error = Token(Token::Kind::MissedLeftParen, errorMessage);

        break;
    }
    case Token::Kind::MissedRightParen:
    {
        errorMessage += "Missed Right Paren";

        error = Token(Token::Kind::MissedRightParen, errorMessage);

        break;
    }
    case Token::Kind::MissedBothParen:
    {
        errorMessage += "Missed Both Paren";

        error = Token(Token::Kind::MissedBothParen, errorMessage);

        break;
    }
    default:
    {
        errorMessage += "Unhandled Error";

        error = Token(Token::Kind::UnhandledError, errorMessage);

        break;
    }
    }

    return error;
}