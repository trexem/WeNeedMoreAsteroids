#include "EntityManager.h"
#include "SettingsScreen.h"
#include "texture.hpp"
#include "SettingsManager.h"
#include "GameStateManager.h"
#include "GUI.h"
#include "TextureManager.h"
#include "TextManager.h"

void SettingsScreen::create(EntityManager& eManager, SDL_Renderer* renderer) {
    checkBoxFalseTexture = TextureManager::instance().get("gui/checkBoxFalse");
    checkBoxTrueTexture = TextureManager::instance().get("gui/checkBoxTrue");
    leftButtonTexture = TextureManager::instance().get("gui/leftButton");
    rightButtonTexture = TextureManager::instance().get("gui/rightButton");
    settings = SettingsManager::instance().get();
    // BackButton
    backButtonTexture = TextureManager::instance().get("gui/backButton");
    FPair size {backButtonTexture->getWidth(), backButtonTexture->getHeight()};
    FPair pos {75.0f, 75.0f};
    backButton = std::make_shared<Button>(eManager, "", pos, size, backButtonTexture, renderer);
    ClickCallbackComponent callback;
    callback.onClick = [&] (uint32_t entity) {
        GameStateManager::instance().setState(GameStateManager::instance().getLastState());
    };
    eManager.addComponent(backButton->id, ComponentType::ClickCallback);
    eManager.setComponentData<ClickCallbackComponent>(backButton->id, callback);
#if not defined (__ANDROID__)
    //** Resolution **
    // Left Button
    pos.x = GUI::screenWidth / 2.0f - 500.0f;
    pos.y = GUI::screenHeight / 2.0f - 300.0f;
    size = 75.0f;
    leftResolutionButton = std::make_shared<Button>(eManager, "", pos, size, leftButtonTexture, renderer);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().backResolution();
    };
    eManager.addComponent(leftResolutionButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(leftResolutionButton->id, callback);
    // Label
    pos.x = GUI::screenWidth / 2.0f - 200.0f;
    size = 400.0f;
    std::string resText = SettingsManager::instance().getCurrentResolutionName();
    resolutionText = TextManager::instance().get("label.resolution") + resText;
    resolutionLabel = std::make_shared<Label>(eManager, resolutionText, pos, size, renderer, 0, Fonts::Subtitle);
    // Right Button
    pos.x += 700.0f;
    size = 75.0f;
    rightResolutionButton = std::make_shared<Button>(eManager, "", pos, size, rightButtonTexture, renderer);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().nextResolution();
    };
    eManager.addComponent(rightResolutionButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(rightResolutionButton->id, callback);

    // ** FullScreen **
    //Label
    pos.y += 150;
    pos.x = GUI::screenWidth / 2.0f - 200.0f;
    size = 400.0f;
    std::string text = TextManager::instance().get("label.fullscreen");
    fullscreenLabel = std::make_shared<Label>(eManager, text, pos, size, renderer, 0, Fonts::Subtitle);
    // Button
    pos.x += 700.0f;
    size = 75.0f;
    fullScreenButton = std::make_shared<Box>(eManager, pos, size, checkBoxFalseTexture);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().get().fullscreen = !SettingsManager::instance().get().fullscreen;
        SettingsManager::instance().updateResolution();
    };
    eManager.addComponent(fullScreenButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(fullScreenButton->id, callback);
#endif
    // ** VSync **
    //Label
    pos.y += 150;
    pos.x = GUI::screenWidth / 2.0f - 200.0f;
    size = 400.0f;
    vsyncLabel = std::make_shared<Label>(eManager, "VSync", pos, size, renderer, 0, Fonts::Subtitle);
    // Button
    pos.x += 700.0f;
    size = 75.0f;
    vsyncButton = std::make_shared<Box>(eManager, pos, size, checkBoxFalseTexture);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().get().vsync = !SettingsManager::instance().get().vsync;
        SettingsManager::instance().updateResolution();
    };
    eManager.addComponent(vsyncButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(vsyncButton->id, callback);

    //** Master Volume **
    // Left Button
    pos.x = GUI::screenWidth / 2.0f - 500.0f;
    pos.y += 150;
    size = 75.0f;
    leftMasterVolume = std::make_shared<Button>(eManager, "", pos, size, leftButtonTexture, renderer);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().decreaseVolume(VolumeSource::MasterVolume);
    };
    eManager.addComponent(leftMasterVolume->id, ComponentType::ClickCallback);
    eManager.setComponentData(leftMasterVolume->id, callback);
    // Label
    pos.x = GUI::screenWidth / 2.0f - 200.0f;
    size = 400.0f;
    int vol = SettingsManager::instance().getVolume(VolumeSource::MasterVolume);
    masterText = TextManager::instance().format("label.masterVolume", vol);
    masterLabel = std::make_shared<Label>(eManager, masterText, pos, size, renderer, 0, Fonts::Subtitle);
    // Right Button
    pos.x += 700.0f;
    size = 75.0f;
    rightMasterVolume = std::make_shared<Button>(eManager, "", pos, size, rightButtonTexture, renderer);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().increaseVolume(VolumeSource::MasterVolume);
    };
    eManager.addComponent(rightMasterVolume->id, ComponentType::ClickCallback);
    eManager.setComponentData(rightMasterVolume->id, callback);

    //** Music Volume **
    // Left Button
    pos.x = GUI::screenWidth / 2.0f - 500.0f;
    pos.y += 150;
    size = 75.0f;
    leftMusicVolume = std::make_shared<Button>(eManager, "", pos, size, leftButtonTexture, renderer);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().decreaseVolume(VolumeSource::MusicVolume);
    };
    eManager.addComponent(leftMusicVolume->id, ComponentType::ClickCallback);
    eManager.setComponentData(leftMusicVolume->id, callback);
    // Label
    pos.x = GUI::screenWidth / 2.0f - 200.0f;
    size = 400.0f;
    vol = SettingsManager::instance().getVolume(VolumeSource::MusicVolume);
    musicText = TextManager::instance().format("label.musicVolume", vol);
    musicLabel = std::make_shared<Label>(eManager, musicText, pos, size, renderer, 0, Fonts::Subtitle);
    // Right Button
    pos.x += 700.0f;
    size = 75.0f;
    rightMusicVolume = std::make_shared<Button>(eManager, "", pos, size, rightButtonTexture, renderer);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().increaseVolume(VolumeSource::MusicVolume);
    };
    eManager.addComponent(rightMusicVolume->id, ComponentType::ClickCallback);
    eManager.setComponentData(rightMusicVolume->id, callback);

    //** SFX Volume **
    // Left Button
    pos.x = GUI::screenWidth / 2.0f - 500.0f;
    pos.y += 150;
    size = 75.0f;
    leftSfxVolume = std::make_shared<Button>(eManager, "", pos, size, leftButtonTexture, renderer);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().decreaseVolume(VolumeSource::SFXVolume);
    };
    eManager.addComponent(leftSfxVolume->id, ComponentType::ClickCallback);
    eManager.setComponentData(leftSfxVolume->id, callback);
    // Label
    pos.x = GUI::screenWidth / 2.0f - 200.0f;
    size = 400.0f;
    vol = SettingsManager::instance().getVolume(VolumeSource::SFXVolume);
    sfxText = TextManager::instance().format("label.sfxVolume", vol);
    sfxLabel = std::make_shared<Label>(eManager, sfxText, pos, size, renderer, 0, Fonts::Subtitle);
    // Right Button
    pos.x += 700.0f;
    size = 75.0f;
    rightSfxVolume = std::make_shared<Button>(eManager, "", pos, size, rightButtonTexture, renderer);
    callback.onClick = [&] (uint32_t entity) {
        SettingsManager::instance().increaseVolume(VolumeSource::SFXVolume);
    };
    eManager.addComponent(rightSfxVolume->id, ComponentType::ClickCallback);
    eManager.setComponentData(rightSfxVolume->id, callback);
}

void SettingsScreen::destroy(EntityManager& eManager) {
    SettingsManager::instance().save();
    backButton->destroy(eManager);
#if not defined(__ANDROID__)
    leftResolutionButton->destroy(eManager);
    resolutionLabel->destroy(eManager);
    rightResolutionButton->destroy(eManager);
    fullscreenLabel->destroy(eManager);
    fullScreenButton->destroy(eManager);
#endif
    vsyncLabel->destroy(eManager);
    vsyncButton->destroy(eManager);
    leftMasterVolume->destroy(eManager);
    masterLabel->destroy(eManager);
    rightMasterVolume->destroy(eManager);

    leftMusicVolume->destroy(eManager);
    musicLabel->destroy(eManager);
    rightMusicVolume->destroy(eManager);

    leftSfxVolume->destroy(eManager);
    sfxLabel->destroy(eManager);
    rightSfxVolume->destroy(eManager);
}

void SettingsScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {

}

void SettingsScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {

}

void SettingsScreen::update(EntityManager& eManager, SDL_Renderer* renderer) {
    backButton->updateState(eManager);
#if not defined(__ANDROID__)
    leftResolutionButton->updateState(eManager);
    rightResolutionButton->updateState(eManager);
    std::string newResText = TextManager::instance().get("label.resolution") + SettingsManager::instance().getCurrentResolutionName();
    if (resolutionText != newResText) {
        resolutionText = newResText;
        resolutionLabel->setText(resolutionText);
    }
    fullScreenButton->updateState(eManager);
    RenderComponent* fullScreenCheck = eManager.getComponentDataPtr<RenderComponent>(fullScreenButton->id);
    fullScreenCheck->texture = SettingsManager::instance().get().fullscreen ? checkBoxTrueTexture : checkBoxFalseTexture;
#endif
    vsyncButton->updateState(eManager);
    RenderComponent* vsyncCheck = eManager.getComponentDataPtr<RenderComponent>(vsyncButton->id);
    vsyncCheck->texture = SettingsManager::instance().get().vsync ? checkBoxTrueTexture : checkBoxFalseTexture;

    leftMasterVolume->updateState(eManager);
    rightMasterVolume->updateState(eManager);
    int vol = SettingsManager::instance().getVolume(VolumeSource::MasterVolume);
    std::string newMasterText = TextManager::instance().format("label.masterVolume", vol);
    if (masterText != newMasterText) {
        masterText = newMasterText;
        masterLabel->setText(masterText);
    }

    leftMusicVolume->updateState(eManager);
    rightMusicVolume->updateState(eManager);
    vol = SettingsManager::instance().getVolume(VolumeSource::MusicVolume);
    std::string newMusicText = TextManager::instance().format("label.musicVolume", vol);
    if (musicText != newMusicText) {
        musicText = newMusicText;
        musicLabel->setText(musicText);
    }

    leftSfxVolume->updateState(eManager);
    rightSfxVolume->updateState(eManager);
    vol = SettingsManager::instance().getVolume(VolumeSource::SFXVolume);
    std::string newSfxText = TextManager::instance().format("label.sfxVolume", vol);
    if (sfxText != newSfxText) {
        sfxText = newSfxText;
        sfxLabel->setText(sfxText);
    }
}
