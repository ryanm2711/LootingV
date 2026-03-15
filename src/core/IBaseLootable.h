#include <types.h>
#include <string>
#include <natives.h>

#pragma once

enum class LootTier {
    POOR,
    WORKING_CLASS,
    RICH
};

class IBaseLootable {
public:
    // The "Common" Constructor
    IBaseLootable(int& ID, std::string helpMsg, LootTier tier) : m_ID(ID), m_HelpMessage(std::move(helpMsg)), m_LootTier(tier) {}
    virtual ~IBaseLootable() = default;
    virtual bool Loot() = 0;
    
    inline Vector3 GetPosition() const {
        return ENTITY::GET_ENTITY_COORDS(GetID(), false);
    }

    inline bool ExistsInWorld() const {
        return ENTITY::DOES_ENTITY_EXIST(GetID());
    }

    // Added to the very top base class
    int GetID() const {
        return m_ID;
    }

    inline std::string GetHelpMessage() const {
        return m_HelpMessage;
    }

    inline LootTier GetTier() const {
        return m_LootTier;
    }
protected:
    int m_ID;
    LootTier m_LootTier;
    std::string m_HelpMessage{};
};