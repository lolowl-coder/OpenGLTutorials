#include "MathUtils.h"

#include <cstdlib>
#include <cmath>

float MathUtils::rnd()
{
   return rand() / (float)RAND_MAX;
}

float MathUtils::rnd(const float mn, const float mx)
{
   return rand() / (float)RAND_MAX * (mx - mn) + mn;
}
