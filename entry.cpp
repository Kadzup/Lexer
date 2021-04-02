#include <iomanip>
#include <iostream>
#include "Lexer.h"

std::ostream& operator<<(std::ostream& os, const Token::Kind& kind) {
    static const char* const names[]{
        "Identifier", "Type", "LeftParen",  "RightParen", "Comma",
        "Start", "End", "Unexpected", "UnknownDataType", "UnnecessaryTypeDeclaration", 
        "UnknownIdentifier", "MissedIdentifier", "MissedLeftParen", 
        "MissedRightParen", "MissedBothParen", "MissedTypeBeforeIdentifier", 
        "MissedComma", "UnexpectedComma", "UnhandledError", "Fine"
    };
    return os << names[static_cast<int>(kind)];
}

int main() {
    auto code = "int a(int a)";

    Lexer lex(code);
    for (auto token = lex.next();
        not token.is_one_of(Token::Kind::End, Token::Kind::Unexpected);
        token = lex.next()) {
        std::cout << std::setw(12) << token.kind() << " |" << token.lexeme()
            << "|\n";
    }
}