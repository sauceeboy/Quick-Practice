#include <Geode/Geode.hpp>
#include <Geode/modify/EditLevelLayer.hpp>
#include "../vars/Shared.hpp"

using namespace geode::prelude;

class $modify(MyEditLevelLayer, EditLevelLayer) {
public:
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;

        if (!Mod::get()->getSettingValue<bool>("enabled")) return true;

        bool autoPractice = Mod::get()->getSettingValue<bool>("auto-practice");
        
        g_forcePractice = autoPractice;

        auto menu = this->getChildByID("level-edit-menu");
        if (!menu) return true;
        
        auto practiceSprite = CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png");
        practiceSprite->setScale(1.3f);
        
        auto practiceButton = CCMenuItemSpriteExtra::create(
            practiceSprite,
            this,
            menu_selector(MyEditLevelLayer::onPractice)
        );
        practiceButton->setID("practice-button"_spr);

        if (!autoPractice) {
            menu->addChild(practiceButton);
        }
        auto playButton = menu->getChildByID("play-button");
        auto editButton = menu->getChildByID("edit-button");
        auto shareButton = menu->getChildByID("share-button");

        if (editButton) editButton->setZOrder(1);
        practiceButton->setZOrder(2);
        if (playButton) playButton->setZOrder(3);
        if (shareButton) shareButton->setZOrder(4);

        menu->updateLayout();
        
        return true;
    }

    void onPractice(CCObject*) {
        g_forcePractice = true;

        auto menu = this->getChildByID("level-edit-menu");
        if (menu) {
            if (auto playButton = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("play-button"))) {
                playButton->activate();
            }
        }
    }
};
