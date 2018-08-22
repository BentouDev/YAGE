//
// Created by bentoo on 11.05.18.
//

#ifndef YAGE_DISPOSE_H
#define YAGE_DISPOSE_H

#include "MacroDefines.h"

#define YAGE_DISPOSE auto YAGE_MAKE_UNIQUE(scope_exit_) /*[[maybe_unused]]*/ = [&]()

#endif // YAGE_DISPOSE_H