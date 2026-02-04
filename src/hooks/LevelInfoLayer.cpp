#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "../vars/Shared.hpp"

using namespace geode::prelude;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
public:
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;
        if (!Mod::get()->getSettingValue<bool>("enabled")) return true;

        bool isDaily = level->m_dailyID > 0 && !level->m_demon;
        bool isWeekly = level->m_dailyID > 0 && level->m_demon;

        bool autoPrac = Mod::get()->getSettingValue<bool>("auto-practice");
        bool autoPracDaily = Mod::get()->getSettingValue<bool>("auto-practice-daily");
        bool autoPracWeekly = Mod::get()->getSettingValue<bool>("auto-practice-weekly");
        float offsetX = Mod::get()->getSettingValue<float>("offset-x");
        float offsetY = Mod::get()->getSettingValue<float>("offset-y");
        float playOffsetX = Mod::get()->getSettingValue<float>("play-offset-x");
        float playOffsetY = Mod::get()->getSettingValue<float>("play-offset-y");
        float scaleOffset = Mod::get()->getSettingValue<float>("scale-offset");
        float playScaleOffset = Mod::get()->getSettingValue<float>("play-scale-offset");
        std::string layout = Mod::get()->getSettingValue<std::string>("layout");

        
        auto menu = this->getChildByID("play-menu");
        if (!menu) return true; 
        
        auto playButton = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("play-button"));
        
        auto practiceButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png"),
            this,
            menu_selector(MyLevelInfoLayer::onPracticeButton)
        );
        practiceButton->setID("practice-button"_spr);

        auto leftMenu = this->getChildByID("left-side-menu");
        auto practiceOptSprite = CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png");
        practiceOptSprite->setScale(0.80f);

        auto practiceOptionsButton = CCMenuItemSpriteExtra::create(
            practiceOptSprite,
            this,
            menu_selector(MyLevelInfoLayer::onPracticeOptionsButton)
        );
        practiceOptionsButton->setID("practice-options-button"_spr);

        if (leftMenu) {
            leftMenu->addChild(practiceOptionsButton);
            leftMenu->updateLayout();
        }

        if (playButton) {
            if (layout == "Small Practice Left") {
                practiceButton->setPosition({-45 + offsetX, 51 + offsetY});
                practiceButton->setScale(0.75f + scaleOffset);
                
                playButton->setPosition({23.00f + playOffsetX, 51 + playOffsetY});
                playButton->setScale(1.0f + playScaleOffset);
            } 
            else if (layout == "Small Practice Right") {
                practiceButton->setPosition({47 + offsetX, 51 + offsetY});
                practiceButton->setScale(0.75f + scaleOffset);

                playButton->setPosition({-23.00f + playOffsetX, 51 + playOffsetY});
                playButton->setScale(1.0f + playScaleOffset);
            }
            else if (layout == "Same Size") {
                practiceButton->setPosition({-40 + offsetX, 51 + offsetY});
                practiceButton->setScale(1.20f + scaleOffset);

                playButton->setPosition({39.00f + playOffsetX, 51 + playOffsetY});
                playButton->setScale(1.0f + playScaleOffset);
            }

            practiceButton->m_baseScale = practiceButton->getScale();
            playButton->m_baseScale = playButton->getScale();
        }

        if (autoPrac) {
            g_forcePractice = true;
            practiceButton->setVisible(false); 

            if (playButton) {
                playButton->setPosition({0.00f + playOffsetX, 51 + playOffsetY});
            }
        }
        else if (autoPracDaily && isDaily) {
            g_forcePractice = true;
            practiceButton->setVisible(false);

            if (playButton) {
                playButton->setPosition({0.00f + playOffsetX, 51 + playOffsetY});
            }
        }
        else if (autoPracWeekly && isWeekly) {
            g_forcePractice = true;
            practiceButton->setVisible(false);

            if (playButton) {
                playButton->setPosition({0.00f + playOffsetX, 51 + playOffsetY});
            }
        }
        else {
            g_forcePractice = false;
            practiceButton->setVisible(true);
        }

        menu->addChild(practiceButton);
        return true; 
    }

    void onPracticeButton(CCObject*) {
        g_forcePractice = true;

        auto menu = this->getChildByID("play-menu");
        if (menu) {
            if (auto playButton = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("play-button"))) {
                playButton->activate();
            }
        }
    }

    void onPracticeOptionsButton(CCObject*) {
        geode::openSettingsPopup(Mod::get());
    }
};