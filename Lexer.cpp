#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cctype> // for std::isalpha, std::isdigit

enum TokenType {
	Number,
	Identifier,
	Equals,
	OpenParen,
	CloseParen,
	BinaryOperator,
	Let,
    EndOfFile, // Signifies end of file
};

const std::unordered_map<std::string, TokenType> KEYWORDS = {
    { "let", TokenType::Let },
};

struct Token {
	std::string value;
	TokenType type;
};

//  Token creation
Token token(const std::string& value, TokenType type) {
	return { value, type };
}


bool isalpha(char c) {
    return std::isalpha(c);
}

bool isskippable(char c) {
    return std::isspace(c) || c == '\n' || c == '\t';
}

bool isint(char c) {
    return std::isdigit(c);
}

// Tokenize the source code string into a vector of Tokens
std::vector<Token> tokenize(const std::string& sourceCode) {
    std::vector<Token> tokens;
    size_t i = 0; // Use an index to iterate through the source code

    // Loop through source code until the end
    while (i < sourceCode.length()) {
        char c = sourceCode[i];

        // Single-character tokens
        switch (c) {
            case '(':
                tokens.push_back(token("(", TokenType::OpenParen));
                ++i;
                continue;
            case ')':
                tokens.push_back(token(")", TokenType::CloseParen));
                ++i;
                continue;
            case '+': case '-': case '*': case '/':
                tokens.push_back(token(std::string(1, c), TokenType::BinaryOperator));
                ++i;
                continue;
            case '=':
                tokens.push_back(token("=", TokenType::Equals));
                ++i;
                continue;
        }

        // Multicharacter tokens: Numbers
        if (std::isdigit(c)) {
            std::string num;
            while (i < sourceCode.length() && std::isdigit(sourceCode[i])) {
                num += sourceCode[i++];
            }

            tokens.push_back(token(num, TokenType::Number));
            continue;
        }

        // Multicharacter tokens: Identifiers or Keywords
        if (std::isalpha(c)) {
            std::string ident;
            while (i < sourceCode.length() && std::isalnum(sourceCode[i])) { // Use std::isalnum to also consider number in identifiers
                ident += sourceCode[i++];
            }

            // Check if the identifier matches any reserved keyword
            auto reserved = KEYWORDS.find(ident);
            if (reserved != KEYWORDS.end()) {
                tokens.push_back(token(ident, reserved->second)); // Reserved Keyword
            } else {
                tokens.push_back(token(ident, TokenType::Identifier)); // User-defined Identifier
            }
            continue;
        }

        // Skipping whitespace and unrecognized characters
        if (std::isspace(c)) {
            ++i;
        } else {
            std::cerr << "Unrecognized character found in source: " << c << std::endl;
            exit(1);
        }
    }

    // Append EndOfFile token at the end
    tokens.push_back(token("", TokenType::EndOfFile));
    return tokens;
}

int main() {
    // Read the content of the file into a string
    std::ifstream file("example.txt");
    std::string sourceCode((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // Tokenize the source code
    std::vector<Token> tokens = tokenize(sourceCode);

    // Output the tokens
    for (const auto& token : tokens) {
        std::cout << "Token: " << token.value << ", Type: " << token.type << std::endl;
    }
}