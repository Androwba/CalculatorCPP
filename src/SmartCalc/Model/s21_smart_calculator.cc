#include "s21_smart_calculator.h"

namespace s21 {

std::stack<Lexeme> SmartCalculatorModel::tokenize(
    const std::string &expression, std::string::size_type *index) {
  std::stack<Lexeme> result;
  std::string::size_type it = *index;
  char currentChar = expression[it];
  switch (currentChar) {
    case 'x':
      if (it + 1 == expression.length() || expression[it + 1] == '+' ||
          expression[it + 1] == '-' || expression[it + 1] == '*' ||
          expression[it + 1] == '/' || expression[it + 1] == '^' ||
          expression[it + 1] == '%' || expression[it + 1] == ')' ||
          expression[it + 1] == ' ' || expression[it + 1] == '.') {
        result.push(Lexeme{0, 0, LexemeType::VARIABLE});
        (*index)++;
      } else {
        throw std::invalid_argument("Invalid variable 'x'");
      }
      break;
    case '+':
      result.push(Lexeme{0, 1, LexemeType::PLUS});
      (*index)++;
      break;
    case '-':
      result.push(Lexeme{0, 1, LexemeType::MINUS});
      (*index)++;
      break;
    case '/':
      result.push(Lexeme{0, 2, LexemeType::DIVIDE});
      (*index)++;
      break;
    case '*':
      result.push(Lexeme{0, 2, LexemeType::MULTIPLY});
      (*index)++;
      break;
    case '^':
      result.push(Lexeme{0, 3, LexemeType::POWER});
      (*index)++;
      break;
    case '%':
      result.push(Lexeme{0, 3, LexemeType::MODULO});
      (*index)++;
      break;
    case 'm':
      if (expression[it + 1] == 'o' && expression[it + 2] == 'd') {
        result.push(Lexeme{0, 3, LexemeType::MODULO});
        (*index) += 3;
      }
      break;
    case 's':
      if (expression[it + 1] == 'i' && expression[it + 2] == 'n') {
        result.push(Lexeme{0, 4, LexemeType::SIN});
        (*index) += 3;
      } else if (expression[it + 1] == 'q' && expression[it + 2] == 'r' &&
                 expression[it + 3] == 't') {
        result.push(Lexeme{0, 4, LexemeType::SQRT});
        (*index) += 4;
      }
      break;
    case 'c':
      if (expression[it + 1] == 'o' && expression[it + 2] == 's') {
        result.push(Lexeme{0, 4, LexemeType::COS});
        (*index) += 3;
      }
      break;
    case 't':
      if (expression[it + 1] == 'a' && expression[it + 2] == 'n') {
        result.push(Lexeme{0, 4, LexemeType::TAN});
        (*index) += 3;
      }
      break;
    case 'l':
      if (expression[it + 1] == 'o' && expression[it + 2] == 'g') {
        result.push(Lexeme{0, 4, LexemeType::LOG});
        (*index) += 3;
      } else if (expression[it + 1] == 'n') {
        result.push(Lexeme{0, 4, LexemeType::LN});
        (*index) += 2;
      }
      break;
    case 'a':
      if (expression[it + 1] == 's' && expression[it + 2] == 'i' &&
          expression[it + 3] == 'n') {
        result.push(Lexeme{0, 4, LexemeType::ASIN});
        (*index) += 4;
      } else if (expression[it + 1] == 'c' && expression[it + 2] == 'o' &&
                 expression[it + 3] == 's') {
        result.push(Lexeme{0, 4, LexemeType::ACOS});
        (*index) += 4;
      } else if (expression[it + 1] == 't' && expression[it + 2] == 'a' &&
                 expression[it + 3] == 'n') {
        result.push(Lexeme{0, 4, LexemeType::ATAN});
        (*index) += 4;
      }
      break;
    case '(':
      result.push(Lexeme{0, 0, LexemeType::LEFT_PARENTHESIS});
      (*index)++;
      break;
    case ')':
      result.push(Lexeme{0, 0, LexemeType::RIGHT_PARENTHESIS});
      (*index)++;
      break;
    default:
      throw std::invalid_argument("Unknown symbol: " +
                                  std::string(1, currentChar));
  }
  return result;
}

void SmartCalculatorModel::processPriority(std::stack<Lexeme> &tmp,
                                           double variable) {
  std::stack<Lexeme> topStack;
  if (!operators_.empty()) {
    topStack.push(operators_.top());
  } else {
    topStack.push({0, -1, LexemeType::DIGIT});
  }
  if (tmp.top().getType() == LexemeType::VARIABLE) {
    operands_.push({variable, 0, LexemeType::DIGIT});
  } else if (tmp.top().getType() == LexemeType::LEFT_PARENTHESIS) {
    operators_.push(tmp.top());
  } else if (tmp.top().getPriority() > topStack.top().getPriority() ||
             (tmp.top().getType() == LexemeType::POWER &&
              topStack.top().getType() != LexemeType::POWER)) {
    operators_.push(tmp.top());
  } else if (tmp.top().getType() == LexemeType::RIGHT_PARENTHESIS) {
    while (!operators_.empty() &&
           operators_.top().getType() != LexemeType::LEFT_PARENTHESIS) {
      evaluate();
    }
    if (!operators_.empty()) {
      operators_.pop();
    }
  } else {
    while (!operators_.empty() &&
           tmp.top().getPriority() <= operators_.top().getPriority() &&
           operators_.top().getType() != LexemeType::LEFT_PARENTHESIS) {
      evaluate();
    }
    operators_.push(tmp.top());
  }
}

void SmartCalculatorModel::evaluate() {
  Lexeme operation = operators_.top();
  operators_.pop();
  std::stack<Lexeme> operandStack1;
  std::stack<Lexeme> operandStack2;
  if (static_cast<int>(operation.getType()) > 8) {
    operandStack2.push(operands_.top());
    operands_.pop();
  } else {
    if (!operands_.empty()) {
      operandStack1.push(operands_.top());
      operands_.pop();
    }
    if (!operands_.empty()) {
      operandStack2.push(operands_.top());
      operands_.pop();
    }
  }
  std::stack<Lexeme> resultStack;
  if (static_cast<int>(operation.getType()) <= 8) {
    resultStack =
        evaluateBinary(operation.getType(), operandStack2.top().getValue(),
                       operandStack1.top().getValue());
  } else {
    resultStack =
        evaluateUnary(operation.getType(), operandStack2.top().getValue());
  }
  operands_.push(resultStack.top());
}

std::stack<Lexeme> SmartCalculatorModel::evaluateBinary(LexemeType op,
                                                        double operand1,
                                                        double operand2) {
  std::stack<Lexeme> result;
  switch (op) {
    case LexemeType::PLUS:
      result.push({operand1 + operand2, 0, LexemeType::DIGIT});
      break;
    case LexemeType::MINUS:
      result.push({operand1 - operand2, 0, LexemeType::DIGIT});
      break;
    case LexemeType::DIVIDE:
      if (operand2 == 0.0) {
        throw std::logic_error("Illegal division by zero");
      }
      result.push({operand1 / operand2, 0, LexemeType::DIGIT});
      break;
    case LexemeType::MULTIPLY:
      result.push({operand1 * operand2, 0, LexemeType::DIGIT});
      break;
    case LexemeType::POWER:
      result.push({std::pow(operand1, operand2), 0, LexemeType::DIGIT});
      break;
    case LexemeType::MODULO:
      if (operand2 == 0.0) {
        throw std::logic_error("Illegal division by zero");
      }
      result.push({std::fmod(operand1, operand2), 0, LexemeType::DIGIT});
      break;
    default:
      throw std::invalid_argument("Unexpected lexeme type in evaluateBinary");
  }
  return result;
}

std::stack<Lexeme> SmartCalculatorModel::evaluateUnary(LexemeType op,
                                                       double operand) {
  std::stack<Lexeme> result;
  result.push({0, 0, LexemeType::DIGIT});
  switch (op) {
    case LexemeType::SIN:
      result.top().setValue(sin(operand));
      break;
    case LexemeType::COS:
      result.top().setValue(cos(operand));
      break;
    case LexemeType::SQRT:
      if (operand < 0) {
        throw std::logic_error("Negative square root");
      }
      result.top().setValue(sqrt(operand));
      break;
    case LexemeType::TAN:
      result.top().setValue(tan(operand));
      break;
    case LexemeType::ASIN:
      result.top().setValue(asin(operand));
      break;
    case LexemeType::ACOS:
      result.top().setValue(acos(operand));
      break;
    case LexemeType::ATAN:
      result.top().setValue(atan(operand));
      break;
    case LexemeType::LN:
      result.top().setValue(log(operand));
      break;
    case LexemeType::LOG:
      result.top().setValue(log10(operand));
      break;
    default:
      throw std::invalid_argument("Unexpected lexeme type in evaluateUnary");
  }
  return result;
}

bool SmartCalculatorModel::isValidExpression(const std::string &expression) {
  std::stack<char> parenthesesStack;
  std::string expressionCopy = removeWhitespace(expression);
  if (expressionCopy.empty()) {
    return false;
  }
  if (expressionCopy.back() == '+' || expressionCopy.back() == '-' ||
      expressionCopy.back() == '*' || expressionCopy.back() == '/' ||
      expressionCopy.back() == '^' || expressionCopy.back() == '(' ||
      expressionCopy.back() == '%' || expressionCopy.front() == ')' ||
      expressionCopy.find("()") != std::string::npos ||
      expressionCopy.find(")(") != std::string::npos) {
    return false;
  }
  for (std::string::size_type i = 0; i < expressionCopy.length() - 1; i++) {
    if ((expressionCopy[i] == '+' || expressionCopy[i] == '-' ||
         expressionCopy[i] == '*' || expressionCopy[i] == '/' ||
         expressionCopy[i] == '^' || expressionCopy[i] == '%') &&
        (expressionCopy[i + 1] == '+' || expressionCopy[i + 1] == '-' ||
         expressionCopy[i + 1] == '*' || expressionCopy[i + 1] == '/' ||
         expressionCopy[i + 1] == '^' || expressionCopy[i + 1] == '%')) {
      return false;
    }
  }
  int decimalPointCount = 0;
  for (std::string::size_type i = 0; i < expressionCopy.length(); i++) {
    if (expressionCopy[i] == '.') {
      decimalPointCount++;
      if (decimalPointCount > 1 ||
          (i == 0 || !isdigit(expressionCopy[i - 1])) ||
          (i == expressionCopy.length() - 1 ||
           !isdigit(expressionCopy[i + 1]))) {
        return false;
      }
    } else {
      decimalPointCount = 0;
    }
  }
  for (char c : expressionCopy) {
    if (c == '(') {
      parenthesesStack.push('(');
    } else if (c == ')') {
      if (parenthesesStack.empty()) {
        return false;
      }
      parenthesesStack.pop();
    }
  }
  return parenthesesStack.empty();
}

std::string SmartCalculatorModel::removeWhitespace(const std::string &str) {
  std::string result;
  for (char c : str) {
    if (!std::isspace(static_cast<unsigned char>(c))) {
      result += c;
    }
  }
  return result;
}

double SmartCalculatorModel::calculate(std::string str, double variable) {
  if (!isValidExpression(str)) {
    throw std::invalid_argument("Invalid expression");
  }
  str = removeWhitespace(str);
  std::stack<Lexeme> tmp;
  std::stack<Lexeme> result;

  if (str[0] == '-' || str[0] == '+') {
    operands_.push({0, 0, LexemeType::DIGIT});
  }
  std::string::size_type i = 0;
  while (i < str.length()) {
    if (isdigit(str[i])) {
      std::string::size_type j = i;
      while (j < str.length() && (isdigit(str[j]) || str[j] == '.')) {
        j++;
      }
      double value = std::stod(str.substr(i, j - i));
      operands_.push({value, 0, LexemeType::DIGIT});
      i = j;
    } else {
      tmp = tokenize(str, &i);
      if (i > 1 && str[i - 2] == '(' &&
          (tmp.top().getType() == LexemeType::MINUS ||
           tmp.top().getType() == LexemeType::PLUS)) {
        operands_.push({0, 0, LexemeType::DIGIT});
      }
      processPriority(tmp, variable);
    }
  }
  while (!operators_.empty() && !operands_.empty()) {
    evaluate();
  }
  if (!operands_.empty()) {
    result.push(operands_.top());
  }
  return result.top().getValue();
}

}  // namespace s21
