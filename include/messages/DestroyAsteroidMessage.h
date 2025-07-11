#pragma once

#include <cstdint>

#include "weaponAbilities.h"
#include "message.h"

class DestroyAsteroidMessage : public Message {
public:
    uint32_t id;
    bool playerDestroyed;
    DestroyAsteroidMessage(uint32_t id, bool pD = true) : id(id), playerDestroyed(pD) {}
};
