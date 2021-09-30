#include "globals.h"
#include <QtMath>

bool isEqual(double first, double second){
 return fabs(first - second) <= Constants::doubleEpsilon;
}
