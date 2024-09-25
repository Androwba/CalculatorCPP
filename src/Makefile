CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -Werror
CFLAGS=-c
LIN_FLAGS  := -pthread -lm
TESTSRC=test.cc
LIBSRC=$(filter-out $(TESTSRC), $(wildcard SmartCalc/Model/*.cc))
TESTOBJ=$(TESTSRC:.cc=.o)
LIBOBJ=$(LIBSRC:.cc=.o)
EXECUTABLE=s21_Smart_Calc
TARGET=s21_test
STATICLIB=smart_calc.a
DIRECTORIES := . SmartCalc SmartCalc/Controller SmartCalc/View SmartCalc/Model
FIND&CHECK := $(wildcard $(addsuffix /*.cc,$(DIRECTORIES))) $(wildcard $(addsuffix /*.h,$(DIRECTORIES)))
INSTALLDIR=build
SUPPRESS=--suppress=*:*
SUPPRESS_LIST = SmartCalc/qcustomplot.h test.cc SmartCalc/View/* SmartCalc/Controller/*
TO_SUPPRESS=$(foreach file,$(SUPPRESS_LIST),$(SUPPRESS)$(file))
LCOVEXEC=$(EXECUTABLE).info
REPORTDIR=report
LEAKS_REPORT_FILE=leak_report.txt
RM=rm -rf
UNAME_S=$(shell uname -s)

ifeq ($(UNAME_S), Linux)
	OPEN=xdg-open
	LDFLAGS += $(LIN_FLAGS) -lgtest --coverage
	CALC=cd build && xdg-open .
	LEAKS=valgrind --leak-check=full --verbose --log-file=$(REPORTDIR)/$(LEAKS_REPORT_FILE)
	CAT=cat report/leak_report.txt | tail -7
	MANUAL = manual
else ifeq ($(UNAME_S), Darwin)
	GTEST_PREFIX := $(shell brew --prefix googletest)
	LDFLAGS = -L$(GTEST_PREFIX)/lib -lgtest --coverage
	CPPFLAGS = -I$(GTEST_PREFIX)/include
	OPEN=open
	CALC=open build/SmartCalc.app/Contents/MacOS/SmartCalc
	LEAKS=CK_FORK=no leaks -atExit --
	MANUAL = manual_html
endif

.PHONY: all build install uninstall dist dvi clean test gcov_report lint leaks cppcheck rebuild git

all: install

%.o: %.c
	$(CXX) $(CFLAGS) $^ -o $@

$(STATICLIB): $(LIBOBJ)
	ar rcs $@ $^

install: uninstall
	mkdir $(INSTALLDIR) && cd $(INSTALLDIR) && qmake6 ../SmartCalc/SmartCalc.pro && make && make clean && rm -f Makefile .qmake.stash
	$(CALC)
uninstall:
	$(RM) $(INSTALLDIR)

dvi:
	makeinfo --html --force ./manual.texi
	@$(OPEN) $(MANUAL)/index.html

dist:
	$(RM) $(INSTALLDIR)
	mkdir $(INSTALLDIR)
	cp Makefile manual.texi test.cc $(INSTALLDIR)
	cp -R SmartCalc $(INSTALLDIR)
	tar -czvf ./SmartCalc_v2.0.tar.tgz ./$(INSTALLDIR)
	$(RM) $(INSTALLDIR)

build: clean $(TESTOBJ) $(STATICLIB)
	$(CXX) $(CXXFLAGS) $(TESTOBJ) $(STATICLIB) -o $(EXECUTABLE) $(LDFLAGS)

clean:
	@$(RM) .clang-format
	$(RM) $(EXECUTABLE) $(STATICLIB)
	@$(RM) build-SmartCalc-Desktop-Debug
	@$(RM) build-SmartCalc-Ubuntu-Debug
	$(RM) *.o SmartCalc/Model/*.o *.gcno *.gcda *.gcov s21_test build 
	$(RM) $(LCOVEXEC) $(REPORTDIR)
	@$(RM) ../build/*
	@$(RM) $(MANUAL)
	@$(RM) SmartCalc_v2.0.tar.tgz

test: build
	./$(EXECUTABLE)

google:
ifeq ("","$(wildcard ./.clang-format)")
	@cp -f ../materials/linters/.clang-format .
endif
	clang-format -i $(FIND&CHECK)

style:	
	@echo "\033[96m◄----------------------- Style Test -----------------------►\033[0m"
ifeq ("","$(wildcard ./.clang-format)")
	@cp -f ../materials/linters/.clang-format .
endif
	@clang-format -n $(FIND&CHECK) 
	@echo "\033[92mTest Done\033[0m"

cppcheck:
	@echo "\033[92m◄----------------------- CPP ------------------------------►\033[0m"
	@cppcheck --language=c++ --enable=all --suppress=missingIncludeSystem $(FIND&CHECK) $(TO_SUPPRESS)

leaks: build
	@echo "\033[92m◄----------------------- Leaks ----------------------------►\033[0m"
	@mkdir $(REPORTDIR)
	@$(LEAKS) ./$(EXECUTABLE)
ifeq ($(shell uname), Linux)
	@$(CAT)
endif

check: style cppcheck leaks

gcov_report: build
ifeq ($(UNAME_S), Linux)
	$(CXX) $(CXXFLAGS) --coverage -o $(TARGET) test.cc SmartCalc/Model/s21_smart_calculator.cc smart_calc.a $(LDFLAGS)
	./$(TARGET)
	lcov --capture --initial --directory . --output-file $(LCOVEXEC)
	lcov --no-external --directory . --capture --output-file $(LCOVEXEC)
	lcov --remove $(LCOVEXEC) "/usr/*" --output-file $(LCOVEXEC)
	genhtml $(LCOVEXEC) --output-directory $(REPORTDIR)
	@$(OPEN) $(REPORTDIR)/index.html
else ifeq ($(UNAME_S), Darwin)
	$(CXX) $(CXXFLAGS) --coverage -o $(TARGET) test.cc SmartCalc/Model/s21_smart_calculator.cc smart_calc.a $(CPPFLAGS) -L$(GTEST_PREFIX)/lib -lgtest --coverage
	./$(TARGET)
	@mkdir -p $(REPORTDIR)
	gcovr --object-directory . --output $(LCOVEXEC)
	gcovr --output $(REPORTDIR)/lcov-report.html --html --html-details
	@$(OPEN) $(REPORTDIR)/lcov-report.html
endif

rebuild:
	$(MAKE) clean
	$(MAKE) all 

# make git m="your message"
git:
	git add -u
	git commit -m "$m"
	git push origin develop
