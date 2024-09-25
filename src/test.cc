#include "SmartCalc/Model/s21_smart_calculator.h"
#include "gtest/gtest.h"

TEST(SmartCalculatorTest, CalculateWithVariable) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "x^2 + 2*x - 3";
  double variable = 5.0;
  double expected = variable * variable + 2 * variable - 3;
  double result = calculator.calculate(expression, variable);
  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, CalculateWithoutVariable) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "2^3 + 4*2 - 3";
  double variable = 0.0;
  double expected = 13.0;
  double result = calculator.calculate(expression, variable);
  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, CalculateStrNegative) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "-2 * (3 + x) - 5 / x";
  double variable = 4.0;

  double expected = -2.0 * (3.0 + variable) - 5.0 / variable;
  double result = calculator.calculate(expression, variable);

  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, CalculateStrPositive) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "+2 * (3 + x) - 5 / x";
  double variable = 4.0;

  double expected = +2.0 * (3.0 + variable) - 5.0 / variable;
  double result = calculator.calculate(expression, variable);

  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, CalculateTopLexemeMinusDigit) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "(-2 * (3 + x)) - 5 / x";
  double variable = 4.0;

  double expected = (-2.0 * (3.0 + variable)) - 5.0 / variable;
  double result = calculator.calculate(expression, variable);

  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, CalculateTopLexemePlus) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "(+2 * (3 + x)) - 5 / x";
  double variable = 4.0;

  double expected = (+2.0 * (3.0 + variable)) - 5.0 / variable;
  double result = calculator.calculate(expression, variable);

  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, DivisionByZero) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "10 / 0";
  double variable = 0.0;
  EXPECT_THROW(calculator.calculate(expression, variable), std::logic_error);
}

TEST(SmartCalculatorTest, CalculateModulo) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "7 % 3";
  double variable = 0.0;

  double expected = 7 % 3;
  double result = calculator.calculate(expression, variable);

  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, CalculateModuloMod) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "7 mod 3";
  double variable = 0.0;

  double expected = 7 % 3;
  double result = calculator.calculate(expression, variable);

  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, TwoOrMorePowersOnTop) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "2 * 3 + 4 / 2 ^ 3 ^ 4 ^ 5";
  double result = calculator.calculate(expression, 0.0);
  EXPECT_EQ(result, 6.0);
}

TEST(SmartCalculatorTest, ProcessPriorityOpenBracket) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "2 * 3 + 4 / 2 ^ 3 ^ 4 ^ 5+ 7 * 1 / 3 + 5)";
  EXPECT_THROW(calculator.calculate(expression, 0), std::logic_error);
}

TEST(SmartCalculatorTest, ModuloByZero) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "7 mod 0";
  double variable = 0.0;
  EXPECT_THROW(calculator.calculate(expression, variable), std::logic_error);
}

TEST(SmartCalculatorTest, CalculateComplexExpression) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "2 + 3 * 4 - (5 mod 2) ^ 2";
  double variable = 0.0;
  double expected = 13.0;
  double result = calculator.calculate(expression, variable);
  EXPECT_EQ(expected, result);
}

TEST(SmartCalculatorTest, UnexpectedLexemeType) {
  s21::SmartCalculatorModel calculator;
  s21::Lexeme lexeme{0, 0, LexemeType::VARIABLE};
  EXPECT_THROW(calculator.calculate("2^3$", 0), std::invalid_argument);
}

TEST(SmartCalculatorTest, InvalidExpression) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "3+++++4";
  double variable = 0.0;
  EXPECT_THROW(calculator.calculate(expression, variable),
               std::invalid_argument);
}

TEST(SmartCalculatorTest, InvalidDecimal) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "3.4 + 3..4";
  double variable = 0.0;
  EXPECT_THROW(calculator.calculate(expression, variable),
               std::invalid_argument);
}

TEST(SmartCalculatorTest, IncorrectDecimal) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "3.4 + 3.n";
  double variable = 0.0;
  EXPECT_THROW(calculator.calculate(expression, variable),
               std::invalid_argument);
}

TEST(SmartCalculatorTest, XvariableTest) {
  s21::SmartCalculatorModel calculator;

  std::string expression = "x + 10";
  double result = calculator.calculate(expression, 5.0);
  EXPECT_EQ(result, 15.0);

  expression = "x * 3";
  result = calculator.calculate(expression, -2.0);
  EXPECT_EQ(result, -6.0);

  expression = "x ^ 2";
  result = calculator.calculate(expression, 0.0);
  EXPECT_EQ(result, 0.0);

  expression = "x + (3 - 2";
  EXPECT_THROW(calculator.calculate(expression, 3.0), std::invalid_argument);

  expression = "x + 5)";
  EXPECT_THROW(calculator.calculate(expression, 5.0), std::invalid_argument);

  expression = "x / 0";
  EXPECT_THROW(calculator.calculate(expression, 10.0), std::logic_error);

  expression = "sqrt(x)";
  EXPECT_THROW(calculator.calculate(expression, -9.0), std::logic_error);

  expression = "2 x 2";
  EXPECT_THROW(calculator.calculate(expression, 7.0), std::logic_error);

  expression = "x + xx";
  EXPECT_THROW(calculator.calculate(expression, 7.0), std::logic_error);
}

TEST(SmartCalculatorTest, IncompleteExpressionMult) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "2 + x *";
  EXPECT_THROW(calculator.calculate(expression, 1.0), std::invalid_argument);
}

TEST(SmartCalculatorTest, EmptyExpression) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "";
  EXPECT_THROW(calculator.calculate(expression, 0), std::invalid_argument);
}

TEST(IsValidExpressionTest, MissingClosingParenthesis) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "(1 + 2 * (3 - 4)";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, MissingOpenParenthesis) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "1 + 2 * 3 - 4)";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, NestedMissingClosingParenthesis) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "((1 + 2) * (3 - 4)";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, NestedMissingOpenParenthesis) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "(1 + 2) * 3 - 4)";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, ValidExpression) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "((1 + 2) * (3 - 4))";
  EXPECT_TRUE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, EmptyExpression) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, InvalidExpressionEndsWithOperator) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "1 + 2 -";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, InvalidExpressionConsecutiveOperators) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "1 + + 2";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, ValidExpressionWithVariables) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "x + 2 * (3 - x)";
  EXPECT_TRUE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, OnlyBrackets) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "()";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, OpenBracket) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "(";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

TEST(IsValidExpressionTest, ClosedBracket) {
  s21::SmartCalculatorModel calculator;
  std::string expression = ")";
  EXPECT_FALSE(calculator.isValidExpression(expression));
}

// Unary

TEST(SmartCalculatorTest, SinSimple) {
  s21::SmartCalculatorModel calculator;
  std::string expression1 = "sin(x)";
  double variable1 = 0.5;
  double expected1 = sin(variable1);
  double result1 = calculator.calculate(expression1, variable1);
  EXPECT_DOUBLE_EQ(expected1, result1);

  std::string expression2 = "sin(6)";
  double expected2 = sin(6);
  double result2 = calculator.calculate(expression2, 0);
  EXPECT_DOUBLE_EQ(expected2, result2);

  std::string expression3 = "sin6";
  double expected3 = sin(6);
  double result3 = calculator.calculate(expression3, 0);
  EXPECT_DOUBLE_EQ(expected3, result3);

  std::string expression4 = "sin  6";
  double expected4 = sin(6);
  double result4 = calculator.calculate(expression4, 0);
  EXPECT_DOUBLE_EQ(expected4, result4);
}

TEST(SmartCalculatorTest, SinComplex) {
  s21::SmartCalculatorModel calculator;
  std::string expression1 = "sin(x) + sin(3)";
  double variable1 = 1.2;
  double expected1 = sin(variable1) + sin(3);
  double result1 = calculator.calculate(expression1, variable1);
  EXPECT_DOUBLE_EQ(expected1, result1);

  std::string expression2 = "sin(x * sin(2)) - sin(1)";
  double variable2 = 0.8;
  double expected2 = sin(variable2 * sin(2)) - sin(1);
  double result2 = calculator.calculate(expression2, variable2);
  EXPECT_DOUBLE_EQ(expected2, result2);
}

TEST(SmartCalculatorTest, SqrtVariable) {
  s21::SmartCalculatorModel calculator;
  std::string expression1 = "sqrt(x)";
  double variable1 = 9.0;
  double expected1 = sqrt(variable1);
  double result1 = calculator.calculate(expression1, variable1);
  EXPECT_DOUBLE_EQ(expected1, result1);

  std::string expression2 = "sqrt(x)";
  double variable2 = 0.0;
  double expected2 = sqrt(variable2);
  double result2 = calculator.calculate(expression2, variable2);
  EXPECT_DOUBLE_EQ(expected2, result2);
}

TEST(SmartCalculatorTest, NegativeRoot) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "sqrt(x)";
  double variable = -4.0;

  EXPECT_THROW(calculator.calculate(expression, variable), std::logic_error);
}

TEST(SmartCalculatorTest, SqrtComplexExpression) {
  s21::SmartCalculatorModel calculator;
  std::string expression =
      "(sin(x) + cos(x))^2 + sqrt(2*x + 1) - log(10*x) + sqrt(sqrt(x))";
  double variable = 1.5;
  double expected = pow(sin(variable) + cos(variable), 2) +
                    sqrt(2 * variable + 1) - log10(10 * variable) +
                    sqrt(sqrt(variable));
  double result = calculator.calculate(expression, variable);
  EXPECT_DOUBLE_EQ(expected, result);
  EXPECT_DOUBLE_EQ(expected, 3.0717106687045073);
  double roundedResult = round(result * 10000.0) / 10000.0;
  EXPECT_DOUBLE_EQ(roundedResult, 3.0717);
}

TEST(SmartCalculatorTest, UnknownSymbol) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "3$4";
  double variable = 0.0;
  EXPECT_THROW(calculator.calculate(expression, variable),
               std::invalid_argument);
}

TEST(SmartCalculatorTest, InvalidParentheses) {
  s21::SmartCalculatorModel calculator;
  std::string expression1 = "3 * (4 + 2";
  EXPECT_THROW(calculator.calculate(expression1, 0.0), std::invalid_argument);

  std::string expression2 = "3 * 4 + 2)";
  EXPECT_THROW(calculator.calculate(expression2, 0.0), std::invalid_argument);

  std::string expression3 = "(3 * (4 + 2)";
  EXPECT_THROW(calculator.calculate(expression3, 0.0), std::invalid_argument);
}

TEST(SmartCalculatorTest, LNTest) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "ln(10)";
  double result = calculator.calculate(expression, 0);
  EXPECT_NEAR(result, 2.30259, 0.00001);
}

TEST(SmartCalculatorTest, LNComparison) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "ln(100)";
  double expected = std::log(100);
  double result = calculator.calculate(expression, 0);
  EXPECT_NEAR(result, expected, 0.00000001);
}

TEST(SmartCalculatorTest, TANTest) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "tan(1.5)";
  double expected = std::tan(1.5);
  double result = calculator.calculate(expression, 0);
  EXPECT_EQ(result, expected);
}

TEST(SmartCalculatorTest, ATANTest) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "atan(0.5)";
  double expected = std::atan(0.5);
  double result = calculator.calculate(expression, 0);
  EXPECT_NEAR(result, expected, 0.00001);
}

TEST(SmartCalculatorTest, ACOSTest) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "acos(0.3)";

  double expected = std::acos(0.3);
  double result = calculator.calculate(expression, 0);
  EXPECT_NEAR(result, expected, 0.00001);
}

TEST(SmartCalculatorTest, ASINTest) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "asin(0.7)";
  double expected = std::asin(0.7);
  double result = calculator.calculate(expression, 0);
  EXPECT_NEAR(result, expected, 0.00001);
}

TEST(SmartCalculatorTest, ComplexExpressionTest) {
  s21::SmartCalculatorModel calculator;
  std::string expression =
      "(4 + 2 * (8 / 2)) ^ 2 - sqrt(16) + ln(9.4) + "
      "cos(2.2) - sin(2.5) + "
      "tan(4) + acos(0.5) - asin(0.7) + atan((1) % 3) mod 2";

  double expected = std::pow((4 + 2 * (8 / 2)), 2) - std::sqrt(16) +
                    std::log(9.4) + std::cos(2.2) - std::sin(2.5) +
                    std::tan(4) + std::acos(0.5) - std::asin(0.7) +
                    std::atan(((1) % 3) % 2);
  double result = calculator.calculate(expression, 0);
  EXPECT_EQ(result, expected);
}

TEST(SmartCalculatorTest, Addition) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "1234567 + 1234567";
  double result = calculator.calculate(expression, 0);
  EXPECT_EQ(result, 2469134);
}

TEST(SmartCalculatorTest, OneOperand) {
  s21::SmartCalculatorModel calculator;
  std::string expression = "1";
  double result = calculator.calculate(expression, 0);
  EXPECT_EQ(result, 1);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
