#pragma once

#include <cstdio>
#include <cstring>
#include <ctime>
#include <exception>

class MatrixException : public std::exception {
protected:
  char errinfo[512] = "";

public:
  MatrixException(const char filename[], const size_t line,
                  const char inf[] = "Undefined exception occured!") {
    time_t current_time = time(nullptr);
    auto local_time = localtime(&current_time);
    char timebuf[64] = "";
    strftime(timebuf, sizeof(timebuf), "%c", local_time);

    snprintf(errinfo, sizeof(errinfo),
             "Time: %s File: %s\nLine: %zu Message: %s", timebuf, filename,
             line, inf);
  }

  virtual ~MatrixException() {}

  virtual const char *what() const noexcept override { return errinfo; }
};

class MemoryError : public MatrixException {
public:
  MemoryError(const char filename[], const size_t line, const char inf[] = "")
      : MatrixException(filename, line, "Out of memory!") {}

  virtual ~MemoryError() {}
};

class InitError : public MatrixException {
public:
  InitError(const char filename[], const size_t line, const char inf[] = "")
      : MatrixException(filename, line, "Bad initialisation data!") {}

  virtual ~InitError() {}
};

class IndexOutOfRange : public MatrixException {
public:
  IndexOutOfRange(size_t ind, const char filename[], const size_t line,
                  const char inf[] = "")
      : MatrixException(filename, line, "Index out of range!") {
    char extra[64];
    snprintf(extra, sizeof(extra), " ind = %zu", ind);
    strncat(errinfo, extra, sizeof(errinfo) - strlen(errinfo) - 1);
  }

  virtual ~IndexOutOfRange() {}
};

class SizeError : public MatrixException {
public:
  SizeError(const size_t rows, const size_t cols, const char filename[],
            const size_t line, const char inf[] = "")
      : MatrixException(filename, line, "Inappropriate size!") {
    char extra[64];
    snprintf(extra, sizeof(extra), " rows = %zu cols = %zu", rows, cols);
    strncat(errinfo, extra, sizeof(errinfo) - strlen(errinfo) - 1);
  }

  virtual ~SizeError() {}
};

class ZeroDivMatrix : public MatrixException {
public:
  ZeroDivMatrix(const size_t row, const size_t col, const char filename[],
                const size_t line, const char inf[] = "")
      : MatrixException(filename, line, "Division by zero!") {
    char extra[64];
    snprintf(extra, sizeof(extra), " row = %zu col = %zu", row, col);
    strncat(errinfo, extra, sizeof(errinfo) - strlen(errinfo) - 1);
  }

  virtual ~ZeroDivMatrix() {}
};

class SingularMatrixError : public MatrixException {
public:
  SingularMatrixError(const char filename[], const size_t line,
                      const char inf[] = "")
      : MatrixException(filename, line, "Determinant equals zero!") {}

  virtual ~SingularMatrixError() {}
};

class NotSquareError : public MatrixException {
public:
  NotSquareError(const size_t rows, const size_t cols, const char filename[],
                 const size_t line, const char inf[] = "")
      : MatrixException(filename, line, "Expected square matrix!") {
    char extra[64];
    snprintf(extra, sizeof(extra), " rows = %zu cols = %zu", rows, cols);
    strncat(errinfo, extra, sizeof(errinfo) - strlen(errinfo) - 1);
  }

  virtual ~NotSquareError() {}
};
