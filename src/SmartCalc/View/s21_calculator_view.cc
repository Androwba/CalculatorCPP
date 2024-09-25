#include "s21_calculator_view.h"

#include "Model/s21_smart_calculator.h"
#include "ui_view.h"

namespace s21 {
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle("SmartCalc");
  this->setFixedSize(665, 380);
  variable_x = ui->variable_x;
  variable_x->setText("0");
  connect(variable_x, &QLineEdit::textChanged, this,
          &MainWindow::onVariableXTextChanged);
  connect(ui->pushButton_zero, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_one, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_two, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_three, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_four, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_five, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_six, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_seven, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_eight, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  connect(ui->pushButton_nine, SIGNAL(clicked()), this,
          SLOT(onNumberButtonClicked()));
  ui->pushButton_graph->setCheckable(true);
  colorTimer = new QTimer(this);
  connect(colorTimer, SIGNAL(timeout()), this, SLOT(updateStarMarkerColor()));
  setFocus();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onNumberButtonClicked() {
  QPushButton *button = qobject_cast<QPushButton *>(sender());
  if (button) {
    QString buttonValue = button->text();
    currentInput.append(buttonValue);
    updateLineEditText("");
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Backspace) {
    if (!currentInput.isEmpty()) {
      removeLastWord();
    }
  } else if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
    QString keyValue = event->text();
    currentInput += keyValue;
    updateLineEditText("");
  } else if (event->key() == Qt::Key_Plus) {
    on_pushButton_plus_clicked();
  } else if (event->key() == Qt::Key_Minus) {
    on_pushButton_minus_clicked();
  } else if (event->key() == Qt::Key_Asterisk) {
    on_pushButton_multiply_clicked();
  } else if (event->key() == Qt::Key_Equal || event->key() == Qt::Key_Enter) {
    on_pushButton_equals_clicked();
  } else if (event->key() == Qt::Key_C) {
    on_pushButton_clear_clicked();
  } else if (event->key() == Qt::Key_Slash) {
    on_pushButton_division_clicked();
  } else if (event->key() == Qt::Key_Period || event->key() == Qt::Key_Comma) {
    on_pushButton_dot_clicked();
  } else if (event->key() == Qt::Key_Percent) {
    on_pushButton_mod_clicked();
  } else if (event->key() == Qt::Key_AsciiCircum) {
    on_pushButton_power_clicked();
  } else if (event->key() == Qt::Key_ParenLeft) {
    on_pushButton_openBracket_clicked();
  } else if (event->key() == Qt::Key_ParenRight) {
    on_pushButton_closeBracket_clicked();
  } else if (event->key() == Qt::Key_X) {
    on_pushButton_x_clicked();
  }
}

bool MainWindow::checkToAvoidRepetitiveOperators() {
  QString lastChar = currentInput.right(1);
  QString operators = "+-*/.^mod";
  QString openParenthesis = "(";
  if (operators.contains(lastChar) || lastChar == openParenthesis) {
    return false;
  }
  QString lastTrigFunction;
  QString trigFunctions = "sin cos tan asin acos atan sqrt log ln";
  QStringList trigFunctionList = trigFunctions.split(' ');
  for (const QString &trigFunction : trigFunctionList) {
    if (currentInput.endsWith(trigFunction)) {
      lastTrigFunction = trigFunction;
      break;
    }
  }
  if (!lastTrigFunction.isEmpty() &&
      currentInput.endsWith(lastTrigFunction + lastTrigFunction)) {
    return false;
  }
  return true;
}

void MainWindow::removeLastWord() {
  if (currentInput.endsWith("sqrt") || currentInput.endsWith("atan") ||
      currentInput.endsWith("acos") || currentInput.endsWith("asin")) {
    currentInput.chop(4);
  } else if (currentInput.endsWith("ln")) {
    currentInput.chop(2);
  } else if (currentInput.endsWith("mod") || currentInput.endsWith("tan") ||
             currentInput.endsWith("cos") || currentInput.endsWith("sin") ||
             currentInput.endsWith("log")) {
    currentInput.chop(3);
  } else {
    currentInput.chop(1);
  }
  updateLineEditText("");
}

void MainWindow::updateLineEditText(const QString &errorMessage) {
  if (!errorMessage.isEmpty()) {
    ui->lineEdit->setStyleSheet("color: red");
  } else {
    ui->lineEdit->setStyleSheet("");
  }
  ui->lineEdit->setText(currentInput + errorMessage);
  currentValue = currentInput.toDouble();
}

void MainWindow::on_pushButton_plus_clicked() {
  currentInput.append("+");
  updateLineEditText("");
}

void MainWindow::on_pushButton_minus_clicked() {
  currentInput.append("-");
  updateLineEditText("");
}

void MainWindow::on_pushButton_multiply_clicked() {
  if (!currentInput.isEmpty() && checkToAvoidRepetitiveOperators()) {
    currentInput.append("*");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_division_clicked() {
  if (!currentInput.isEmpty() && checkToAvoidRepetitiveOperators()) {
    currentInput.append("/");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_dot_clicked() {
  if (!checkToAvoidRepetitiveOperators() || currentInput.endsWith('x')) {
    return;
  }
  int lastOperatorIndex = -1;
  QString operators = "+-*/%^)(";
  for (int i = currentInput.length() - 1; i >= 0; --i) {
    if (operators.contains(currentInput[i])) {
      lastOperatorIndex = i;
      break;
    }
  }
  QString currentNumber = currentInput.mid(lastOperatorIndex + 1);
  if (currentNumber.contains(".")) {
    return;
  }
  if (currentInput.isEmpty() || operators.contains(currentInput[0])) {
    currentInput.append("0");
  }
  currentInput.append(".");
  updateLineEditText("");
}

void MainWindow::on_pushButton_clear_clicked() {
  currentInput.clear();
  updateLineEditText();
}

void MainWindow::on_pushButton_backspace_clicked() {
  if (!currentInput.isEmpty()) {
    removeLastWord();
  }
}

void MainWindow::on_pushButton_power_clicked() {
  if (!currentInput.isEmpty() && checkToAvoidRepetitiveOperators()) {
    currentInput.append('^');
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_mod_clicked() {
  if (!currentInput.isEmpty() && checkToAvoidRepetitiveOperators()) {
    currentInput.append("mod");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_openBracket_clicked() {
  if (!currentInput.endsWith('(') && !currentInput.endsWith(')')) {
    currentInput.append("(");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_closeBracket_clicked() {
  if (currentInput.endsWith("sin") || currentInput.endsWith("cos") ||
      currentInput.endsWith("tan") || currentInput.endsWith("asin") ||
      currentInput.endsWith("acos") || currentInput.endsWith("atan") ||
      currentInput.endsWith("log") || currentInput.endsWith("ln") ||
      currentInput.endsWith("sqrt")) {
    return;
  } else {
    int openBracketCount = currentInput.count('(');
    int closedBracketCount = currentInput.count(')');
    if (openBracketCount > closedBracketCount) {
      currentInput.append(")");
      updateLineEditText("");
    }
  }
}

void MainWindow::on_pushButton_x_clicked() {
  if (currentInput.endsWith(".")) {
    return;
  }
  if (!currentInput.isEmpty() && checkToAvoidRepetitiveOperators()) {
    QChar lastChar = currentInput.right(1)[0];
    QString lastCharStr = QString(lastChar);
    if (lastChar.isDigit() || lastCharStr == "x") {
      return;
    }
  }
  if (!currentInput.isEmpty() && checkToAvoidRepetitiveOperators() &&
      currentInput.endsWith(")")) {
    currentInput.append("*");
  }
  currentInput.append("x");
  updateLineEditText("");
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    if (!variable_x->underMouse()) {
      variable_x->clearFocus();
    }
  }
  QMainWindow::mousePressEvent(event);
}

void MainWindow::onVariableXTextChanged(const QString &text) {
  QString trimmedText = text.trimmed();
  if (trimmedText.isEmpty() || trimmedText == ".") {
    variable_x->setText(trimmedText);
    return;
  }
  if (trimmedText.startsWith("0") && trimmedText.length() > 1 &&
      trimmedText[1] != '.') {
    trimmedText.remove(0, 1);
  }
  variable_x->setText(trimmedText);
}

void MainWindow::on_pushButton_sin_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("sin");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_cos_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("cos");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_tan_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("tan");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_asin_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("asin");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_acos_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("acos");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_atan_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("atan");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_sqrt_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("sqrt");
    updateLineEditText("");
  }
}
void MainWindow::on_pushButton_log_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("log");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_ln_clicked() {
  if (!checkToAvoidRepetitiveOperators()) {
    currentInput.append("ln");
    updateLineEditText("");
  }
}

void MainWindow::on_pushButton_equals_clicked() {
  QString expression = currentInput;
  QString variableXValue = variable_x->text().trimmed();
  bool isValidXValue;
  variableXValue.toDouble(&isValidXValue);
  if (!isValidXValue) {
    ui->lineEdit->setStyleSheet("color: red");
    ui->lineEdit->setText(" Incorrect value of X");
    return;
  }
  expression.replace("x", variableXValue);
  if (expression.trimmed().isEmpty() || expression.trimmed() == "0") {
    updateLineEditText(" Empty expression");
    return;
  }

  std::string expressionStr = expression.toStdString();
  if (!controller_.Validate(expressionStr)) {
    updateLineEditText(" Invalid expression");
    return;
  }
  if (expression.contains("/0") || expression.contains("mod0")) {
    updateLineEditText(" Illegal division by zero");
    return;
  }
  if (expression.contains("sqrt(-")) {
    updateLineEditText(" Negative root");
    return;
  }
  QString trigFunctions = "sin cos tan asin acos atan sqrt log ln";
  QStringList trigFunctionList = trigFunctions.split(' ');
  for (const QString &trigFunction : trigFunctionList) {
    if (expression.endsWith(trigFunction)) {
      updateLineEditText(" Incomplete expression");
      return;
    }
  }
  static QRegularExpression operatorsRegex("[+\\-*/.^mod]");
  static QRegularExpression trigFunctionsRegex(
      "sin|cos|tan|asin|acos|atan|sqrt|log|ln");

  if (!expression.contains(operatorsRegex) &&
      !expression.contains(trigFunctionsRegex)) {
    updateLineEditText(" Missing operators or trigonometric functions");
    return;
  }
  static QRegularExpression problematicPattern(
      "(sin|cos|tan|asin|acos|atan|sqrt|log|ln)[+\\-*/.^mod]");
  QString trigexpression = currentInput;
  if (trigexpression.contains(problematicPattern)) {
    updateLineEditText(" Invalid expression");
    return;
  }
  double result =
      controller_.Calculate(expressionStr, variable_x->text().toDouble());
  if (std::isnan(result)) {
    updateLineEditText(" = Nan");
    return;
  }
  QString resultString = QString::number(result, 'f', 7);
  static QRegularExpression trailingZerosRegex("\\.?0+$");
  resultString.remove(trailingZerosRegex);
  updateLineEditText("");
  ui->lineEdit->setText(resultString);
  currentInput = resultString;
}

void MainWindow::on_pushButton_graph_clicked() {
  QPushButton *button = (QPushButton *)sender();
  if (button->isChecked()) {
    this->setFixedWidth(1178);
    setWindowTitle("SmartCalc & Graph");

    button->setChecked(true);
    ui->pushButton_equals->setEnabled(0);
    ui->lineEdit->setPlaceholderText("Enter a trigonometric function with x:");

    ui->x_max->setEnabled(true);
    ui->x_min->setEnabled(true);
    ui->y_max->setEnabled(true);
    ui->y_min->setEnabled(true);
  } else {
    this->setFixedWidth(665);
    setWindowTitle("SmartCalc");

    button->setChecked(false);
    ui->pushButton_equals->setEnabled(1);
    ui->lineEdit->setPlaceholderText("");

    ui->x_max->setEnabled(false);
    ui->x_min->setEnabled(false);
    ui->y_max->setEnabled(false);
    ui->y_min->setEnabled(false);

    ui->qcustomplot->replot();
    x_.clear();
    y_.clear();
    ui->lineEdit->setText("");
  }
}

void MainWindow::updateStarMarkerColor() {
  QColor newColor =
      QColor::fromRgb(arc4random_uniform(256), arc4random_uniform(256),
                      arc4random_uniform(256));
  ui->qcustomplot->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssStar, newColor, 3));
  ui->qcustomplot->replot();
}

void MainWindow::on_pushButton_plot_clicked() {
  QString str = ui->lineEdit->text();
  if (str.contains("x") == false) {
    ui->lineEdit->setStyleSheet("color: red");
    ui->lineEdit->setPlaceholderText("Expression does not contain X");
  } else {
    double x_min_num = ui->x_min->value();
    double x_max_num = ui->x_max->value();
    double y_min_num = ui->y_min->value();
    double y_max_num = ui->y_max->value();

    if (str.contains("sqrt")) {
      x_min_num = std::max(0.0, x_min_num);
      y_min_num = std::max(0.0, y_min_num);
    }
    if (str.contains("sqrt(-") || str.contains("/0") || str.contains("mod0")) {
      if (str.contains("sqrt(-")) {
        updateLineEditText("Negative root");
      } else if (str.contains("/0") || str.contains("mod0")) {
        updateLineEditText("Illegal division by zero");
      }
      return;
    }
    std::string stdString = str.toStdString();
    if (!controller_.Validate(stdString)) {
      updateLineEditText("Invalid expression");
      return;
    }
    double step = 0.001 * (qFabs(x_min_num) + qFabs(x_max_num));
    double Y = 0;
    double X = (double)x_min_num;
    while (X < (double)x_max_num) {
      x_.push_back(X);
      std::string stdString = str.toStdString();
      Y = controller_.Calculate(stdString, X);
      y_.push_back(Y);
      X += step;
    }
    ui->qcustomplot->addGraph();
    QCPGraph *graph = ui->qcustomplot->graph(0);
    graph->setLineStyle(QCPGraph::lsLine);
    graph->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssStar, Qt::black, 3));
    ui->qcustomplot->graph(0)->addData(x_, y_);
    ui->qcustomplot->xAxis->setLabel("X Axis");
    ui->qcustomplot->yAxis->setLabel("Y Axis");
    ui->qcustomplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom |
                                     QCP::iSelectPlottables);
    ui->qcustomplot->xAxis->setRange(x_min_num, x_max_num);
    ui->qcustomplot->yAxis->setRange(y_min_num, y_max_num);
    ui->qcustomplot->replot();
    x_.clear();
    y_.clear();
    ui->qcustomplot->update();
    colorTimer->start(3000);
  }
}

void MainWindow::on_pushButton_clear_graph_clicked() {
  if (ui->qcustomplot->graphCount() > 0) {
    ui->qcustomplot->graph(0)->data()->clear();
    ui->qcustomplot->replot();
    ui->qcustomplot->update();
  }
}
}  // namespace s21
