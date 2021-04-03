#include <iomanip>
#include <iostream>
#include <windows.h>
#include "Lexer.h"

std::ostream& operator<<(std::ostream& os, const Token::Kind& kind) {
    static const char* const names[]{
        "Identifier", "Type", "Left Paren",  "Right Paren", "Comma",
        "Start", "End", "Unexpected", "Unknown Data Type", "Unnecessary Type Declaration", 
        "Unknown Identifier", "Missed Identifier", "Missed Left Paren", 
        "Missed Right Paren", "Missed Both Paren", "Missed Type Before Identifier", 
        "Missed Comma", "Unexpected Comma", "Unhandled Error", "Fine"
    };
    return os << names[static_cast<int>(kind)];
}

int main() {
    auto code = "test a(int bac, error cab, void aabc)";

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    Lexer lex(code);
    for (auto token = lex.next(); not token.is_one_of(Token::Kind::End, Token::Kind::Unexpected); token = lex.next()) 
    {
        SetConsoleTextAttribute(hConsole, 2);
        if(token.lexeme().find("Error") != -1)
        {
            SetConsoleTextAttribute(hConsole, 12);
        }
        std::cout << std::setw(30) << token.kind() << " |" << token.lexeme()
            << "|\n";
    }
    SetConsoleTextAttribute(hConsole, 7);
}