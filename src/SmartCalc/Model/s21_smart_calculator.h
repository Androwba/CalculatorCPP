#ifndef S21_SMART_CALCULATOR_H_
#define S21_SMART_CALCULATOR_H_

#include <cmath>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>

enum class LexemeType {
  DIGIT,
  VARIABLE,
  MINUS,
  PLUS,
  DIVIDE,
  MULTIPLY,
  POWER,
  MODULO,
  UNARY_MINUS,
  UNARY_PLUS,
  COS,
  SIN,
  TAN,
  ACOS,
  ASIN,
  ATAN,
  SQRT,
  LN,
  LOG,
  LEFT_PARENTHESIS,
  RIGHT_PARENTHESIS
};

namespace s21 {

class Lexeme {
 public:
  Lexeme(double value, int priority, LexemeType type)
      : value_(value), priority_(priority), type_(type) {}

  double getValue() const { return value_; }
  void setValue(double value) { value_ = value; }
  int getPriority() const { return priority_; }
  LexemeType getType() const { return type_; }

 private:
  double value_;
  int priority_;
  LexemeType type_;
};

class SmartCalculatorModel {
 public:
  SmartCalculatorModel() {}
  ~SmartCalculatorModel() {}

  double calculate(std::string str, double variable);
  bool isValidExpression(const std::string &expression);

 private:
  std::stack<Lexeme> operators_ = std::stack<Lexeme>();
  std::stack<Lexeme> operands_ = std::stack<Lexeme>();

  void evaluate();
  void processPriority(std::stack<Lexeme> &tmp, double variable);
  std::stack<Lexeme> tokenize(const std::string &expression,
                              std::string::size_type *index);
  std::stack<Lexeme> evaluateBinary(LexemeType op, double operand1,
                                    double operand2);
  std::stack<Lexeme> evaluateUnary(LexemeType op, double operand);
  std::string removeWhitespace(const std::string &str);
};

}  // namespace s21

#endif  // S21_SMART_CALCULATOR_H_
