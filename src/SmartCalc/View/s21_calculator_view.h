#ifndef VIEW_S21_CALCULATOR_VIEW_H_
#define VIEW_S21_CALCULATOR_VIEW_H_

#include <QKeyEvent>
#include <QLineEdit>
#include <QMainWindow>
#include <QRegularExpression>
#include <QVector>

#include "../qcustomplot.h"
#include "Controller/s21_calculator_controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

namespace s21 {

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 protected:
  void keyPressEvent(QKeyEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void onVariableXTextChanged(const QString &text);

 private slots:
  void onNumberButtonClicked();
  void on_pushButton_plus_clicked();
  void on_pushButton_minus_clicked();
  void on_pushButton_equals_clicked();
  void on_pushButton_multiply_clicked();
  void on_pushButton_division_clicked();
  void on_pushButton_dot_clicked();
  void on_pushButton_clear_clicked();
  void on_pushButton_backspace_clicked();
  void on_pushButton_power_clicked();
  void on_pushButton_mod_clicked();
  void on_pushButton_openBracket_clicked();
  void on_pushButton_closeBracket_clicked();
  void on_pushButton_x_clicked();
  void on_pushButton_sin_clicked();
  void on_pushButton_cos_clicked();
  void on_pushButton_tan_clicked();
  void on_pushButton_asin_clicked();
  void on_pushButton_acos_clicked();
  void on_pushButton_atan_clicked();
  void on_pushButton_sqrt_clicked();
  void on_pushButton_log_clicked();
  void on_pushButton_ln_clicked();
  void on_pushButton_graph_clicked();
  void on_pushButton_plot_clicked();
  void on_pushButton_clear_graph_clicked();
  void updateStarMarkerColor();

 private:
  Ui::MainWindow *ui;
  QString currentInput;
  int currentValue;
  bool checkToAvoidRepetitiveOperators();
  void removeLastWord();
  void updateLineEditText(const QString &errorMessage = "");
  QLineEdit *variable_x;
  Controller controller_;
  QVector<double> x_, y_;
  QTimer *colorTimer;
};

}  // namespace s21

#endif  // VIEW_S21_CALCULATOR_VIEW_H_
