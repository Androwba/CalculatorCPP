#include <QApplication>

#include "View/s21_calculator_view.h"

namespace s21 {
class MainWindow;
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::MainWindow w;  // Create an instance of s21::MainWindow
  w.show();

  return a.exec();
}
