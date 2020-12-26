#include <stdio.h>
#include <string.h>

#include "common.h"
#include "scanner.h"

typedef struct {
  const char* start;
  const char* current;
  int line;
} Scanner;

Scanner scanner;


void initScanner(const char* source) {
  scanner.start = source;
  scanner.current = source;
  scanner.line = 1;
}

static bool isAtEnd() {
  return *scanner.current == '\0';
}

static Token makeToken(TokenType type) {
  Token token;
  token.type = type;
  token.start = scanner.start;
  token.length = (int)(scanner.current - scanner.start);
  token.line = scanner.line;

  return token;
}


static Token errorToken(const char* message) {
  Token token;
  token.type = TOKEN_ERROR;
  token.start = message;
  token.length = (int)strlen(message);
  token.line = scanner.line;

  return token;
}

/**
 * Return current character from the source code and increase the current pointer
 */
static char advance() {
  scanner.current++;
  return scanner.current[-1];
}

/**
 * Check if the next char in the source code is the 'expected' argument
 * If it's the expected one, the 'current' pointer will be increased
 */
static bool match(char expected) {
  if (isAtEnd()) return false;
  if (*scanner.current != expected) return false;

  scanner.current++;
  return true;
}

/**
 * Return next character without increasing the pointer
 */
static char peek() {
  return *scanner.current;
}

/**
 * Return character after the next one without increasing the pointer
 */
static char peekNext() {
  if (isAtEnd()) return '\0';
  return scanner.current[1];
}

static void skipWhitespace() {
  for (;;) {
    char c = peek();

    switch (c) {
      // General whitespace
      case ' ':
      case '\r':
      case '\t':
        advance();
        break;

      // On new lines 
      case '\n':
        scanner.line++;
        advance();
        break;

      // Comments
      case '/':
        if (peekNext() == '/') {
          // Comments goes till end of the line
          // Also we don't consume that actual newline char since it will be consumed
          // by the specific case and it will also increase the line count
          while (peek() != '\n' && !isAtEnd()) advance();
        } else {
          // Meaning that '/' wasn't part of a comment
          return;
        }
      default: 
        return;
    }
  }
}

static Token string() {
  while (peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') scanner.line++;
    advance();
  }

  if (isAtEnd()) return errorToken("Unterminated string.");

  // The closing quote
  advance();
  return makeToken(TOKEN_STRING);
}

static bool isDigit(char c) {
  return c >= '0' && c <= '9';
}

static Token number() {
  while (isDigit(peek())) advance();


  // Look for fractional part
  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the '.'
    advance();

    while (isDigit(peek())) advance();
  }

  return makeToken(TOKEN_NUMBER);
}

Token scanToken() {
  skipWhitespace();

  scanner.start = scanner.current;

  if (isAtEnd()) return makeToken(TOKEN_EOF);

  char c = advance();

  if (isDigit(c)) return number();

  switch (c) {
    case '(': return makeToken(TOKEN_LEFT_PAREN);
    case ')': return makeToken(TOKEN_RIGHT_PAREN);
    case '{': return makeToken(TOKEN_LEFT_BRACE);
    case '}': return makeToken(TOKEN_RIGHT_BRACE);
    case ';': return makeToken(TOKEN_SEMICOLON);
    case ',': return makeToken(TOKEN_COMMA);
    case '.': return makeToken(TOKEN_DOT);
    case '-': return makeToken(TOKEN_MINUS);
    case '+': return makeToken(TOKEN_PLUS);
    case '/': return makeToken(TOKEN_SLASH);
    case '*': return makeToken(TOKEN_STAR);

    case '!': return makeToken(match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
    case '=': return makeToken(match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
    case '<': return makeToken(match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
    case '>': return makeToken(match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);

    case '"': return string();
  }
  return errorToken("Unexpected character.");
}
