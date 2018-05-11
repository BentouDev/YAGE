//
// Created by bentoo on 11.05.18.
//

#ifndef YAGE_DISPOSE_H
#define YAGE_DISPOSE_H

#define YAGE_CONCATENATE_DETAIL(x, y) x##y
#define YAGE_CONCATENATE(x, y) YAGE_CONCATENATE_DETAIL(x, y)
#define YAGE_MAKE_UNIQUE(x) YAGE_CONCATENATE(x, __COUNTER__)

#define YAGE_DISPOSE auto YAGE_MAKE_UNIQUE(scope_exit_) /*[[maybe_unused]]*/ = [&]()

#endif // YAGE_DISPOSE_H