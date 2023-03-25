#include <QtMath>

#include "constants.h"

bool isEqual(double first, double second) {
  return fabs(first - second) <= Const::kDoubleEpsilon;
}
