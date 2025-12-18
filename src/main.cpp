#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include <Geode/modify/EditLevelLayer.hpp>

using namespace geode::prelude;

// Global variable to track state
bool g_forcePractice = false;

class $modify(MyLevelInfoLayer, LevelInfoLayer) {
public:
    bool init(GJGameLevel* level, bool challenge) {
        if (!LevelInfoLayer::init(level, challenge)) return false;

        bool autoPrac = Mod::get()->getSettingValue<bool>("autopractice");
        std::string layout = Mod::get()->getSettingValue<std::string>("layout");

        auto menu = this->getChildByID("play-menu");
        auto playButton = menu->getChildByID("play-button");

        auto practiceButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png"),
            this,
            menu_selector(MyLevelInfoLayer::onPracticeButton)
        );

        practiceButton->setID("practice-button"_spr);

        if (layout == "Small Practice") {
            practiceButton->setPosition({-55, 51});
            practiceButton->setScale(0.75f);
            practiceButton->m_baseScale = 0.75f;
            playButton->setPositionX(12.00f);
        } 
        else if (layout == "Same Size") {
            practiceButton->setPosition({-40, 51});
            practiceButton->setScale(1.20f);
            practiceButton->m_baseScale = 1.20f;
            playButton->setPositionX(39.00f);
        }

        if (autoPrac) {
            g_forcePractice = true;
            practiceButton->setVisible(false); 
            practiceButton->setEnabled(false); 
            playButton->setPositionX(0.00f);
        } else {
            g_forcePractice = false;
            practiceButton->setVisible(true);
            practiceButton->setEnabled(true);
        }

        menu->addChild(practiceButton);
        return true; 
    }

    void onPracticeButton(CCObject*) {
        g_forcePractice = true;

        auto menu = this->getChildByID("play-menu");
        auto playButton = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("play-button"));

        if (playButton) {
            playButton->activate();
        }
    }
}; // <--- Added missing semicolon here

class $modify(MyLevelSelectLayer, LevelSelectLayer) {
public:
    bool init(int page) {
        if (!LevelSelectLayer::init(page)) return false;
        
        if (Mod::get()->getSettingValue<bool>("autopractice")) {
            g_forcePractice = true;
        }
        
        return true;
    }
};

class $modify(MyEditLevelLayer, EditLevelLayer) {
public:
    bool init(GJGameLevel* level) {
        if (!EditLevelLayer::init(level)) return false;

        auto menu = this->getChildByID("level-edit-menu");
    
        auto playButton = menu->getChildByID("play-button");
        auto editButton = menu->getChildByID("edit-button");
        auto shareButton = menu->getChildByID("share-button");


        auto practiceButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_practiceBtn_001.png"),
            this,
            menu_selector(MyEditLevelLayer::onPractice)
        );

        practiceButton->setID("practice-button"_spr);
        practiceButton->setScale(1.300f); // Added 'f' for float correctness
        menu->addChild(practiceButton);

    
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
        auto playButton = static_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("play-button"));

        if (playButton) {
            playButton->activate();
        }
    }
};

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        
        if (g_forcePractice) {
            this->togglePracticeMode(true);
        }
        
        g_forcePractice = false;
        return true;
    }
};