#include "s21_calculator_controller.h"

namespace s21 {
double Controller::Calculate(const std::string& str, double expression) {
  return this->model_.calculate(str, expression);
}

int Controller::Validate(const std::string& variable) {
  return this->model_.isValidExpression(variable);
}
}  // namespace s21
