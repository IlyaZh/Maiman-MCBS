#include "constants.h"
#include <QtMath>

bool isEqual(double first, double second){
 return fabs(first - second) <= Const::DoubleEpsilon;
}
