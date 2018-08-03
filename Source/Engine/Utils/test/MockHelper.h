//
// Created by bentoo on 13.05.18.
//

#ifndef YAGE_MOCKHELPER_H
#define YAGE_MOCKHELPER_H

#include <trompeloeil.hpp>

using TDestructionReqVec = std::vector<decltype(
        trompeloeil::lifetime_monitor_releaser{} +
        trompeloeil::lifetime_monitor_modifier<false>{})>;

#endif //YAGE_MOCKHELPER_H
