#include "screens/Components/UpgradeButton.h"
#include "GameStatsManager.h"
#include "Colors.h"
#include "EntityHandle.h"
#include "Fonts.h"
#include "TextManager.h"

UpgradeButton::UpgradeButton(
    EntityManager& em,
    const std::string& label,
    FPair pos,
    FPair size,
    Texture* texture,
    SDL_Renderer* renderer,
    UpgradeType type,
    uint32_t parent
    ) : Button(em, label, pos, size, texture, renderer, parent), type(type) {
    ClickCallbackComponent callback;
    callback.onClick = [&, type](uint32_t entity) {
        const int level = GameStatsManager::instance().getUpgradeLevel(type);
        const int cost = upgradesCost[static_cast<size_t>(type)][level];
        const int maxLevel = upgradesMaxLevel[static_cast<size_t>(type)];
        if (GameStatsManager::instance().spendCoins(cost) && level < maxLevel) {
            GameStatsManager::instance().getStats().upgrades[type]++;
            GameStatsManager::instance().save();
        }
    };
    em.addComponent(id, ComponentType::ClickCallback);
    em.setComponentData<ClickCallbackComponent>(id, callback);
    //Cost Text
    GUIComponent guiComp;
    RenderComponent render;
    TypeComponent typeComp {EntityType::GUI};
    costId = em.createEntity();
    EntityHandle handle {costId, em};
    handle.add<GUIComponent>();
    handle.add<RenderComponent>();
    handle.add<TooltipComponent>();
    level = GameStatsManager::instance().getUpgradeLevel(type);
    maxLevel = upgradesMaxLevel[static_cast<size_t>(type)];
    lastlevel = level;
    const int cost = level < maxLevel ? upgradesCost[static_cast<size_t>(type)][level] : 0;
    costText = cost != 0 ? std::to_string(cost) + "$" : "MAX";
    costTexture.m_renderer = renderer;
    costTexture.loadFromText(costText, Colors::White, Fonts::Body);
    render.texture = &costTexture;
    guiComp.parent = id;
    guiComp.pos = {100.0f - costTexture.getWidth() / 2.0f,
                   130.0f + costTexture.getHeight() / 2.0f};

    TooltipComponent tip;
    std::string key = to_key(type);
    float value = upgradesValues[static_cast<size_t>(type)][level];
    float nextValue = upgradesValues[static_cast<size_t>(type)][level + 1];
    float diff = nextValue - value;
    if (!(type == UpgradeType::ProjectileCount || type == UpgradeType::PickupRange || type == UpgradeType::HealthRegen || type == UpgradeType::GoldProb)) {
        diff *= 100.0f;
        nextValue *= 100.0f;
    }
    std::string text;
    text = TextManager::instance().format("upgradeDesc." + key, diff, nextValue);
    tip.text = text;
    em.addComponent(id, ComponentType::Tooltip);
    em.setComponentData(id, tip);

    handle.set(guiComp);
    handle.set(render);
    handle.set(typeComp);

    if (level >= maxLevel) {
        level = maxLevel;
        GUIStateComponent state = em.getComponentData<GUIStateComponent>(id);
        state.state = GUIState::Disabled;
        em.setComponentData(id, state);
    }
}

void UpgradeButton::destroy(EntityManager& em) {
    Button::destroy(em);
    em.destroyEntityLater(costId);
}

void UpgradeButton::updateCost(EntityManager& em) {
    level = GameStatsManager::instance().getUpgradeLevel(type);
    if (level != lastlevel) {
        const int cost = level < maxLevel ? upgradesCost[static_cast<size_t>(type)][level] : 0;
        costText = cost != 0 ? std::to_string(cost) + "$" : "MAX";
        costTexture.loadFromText(costText, Colors::White, Fonts::Body);
        lastlevel = level;
    }
    if (level >= maxLevel) {
        level = maxLevel;
        GUIStateComponent state = em.getComponentData<GUIStateComponent>(id);
        state.state = GUIState::Disabled;
        em.setComponentData(id, state);
    }
}
