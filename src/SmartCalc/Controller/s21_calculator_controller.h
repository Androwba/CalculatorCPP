#ifndef CONTROLLER_S21_CALCULATOR_CONTROLLER_H_
#define CONTROLLER_S21_CALCULATOR_CONTROLLER_H_

#include "../Model/s21_smart_calculator.h"

namespace s21 {
class Controller {
 private:
  SmartCalculatorModel model_;

 public:
  Controller() {}
  ~Controller() {}
  double Calculate(const std::string& str, double expression);
  int Validate(const std::string& variable);
};
}  // namespace s21

#endif  // CONTROLLER_S21_CALCULATOR_CONTROLLER_H_
