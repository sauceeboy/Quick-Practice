#include <Geode/Geode.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>

using namespace geode::prelude;

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

		if (layout == "Small Practice") {
		practiceButton->setPosition( {-55, 51} );
		practiceButton->setID("practice-button"_spr);
		practiceButton->setScale(0.75);
		practiceButton->m_baseScale = 0.75;
		playButton->setPositionX(12.00);
	} else if (layout == "Same Size") {
		practiceButton->setPosition( {-40, 51} );
		practiceButton->setID("practice-button"_spr);
		practiceButton->setScale(1.200);
		practiceButton->m_baseScale = 1.200;
		playButton->setPositionX(39.00);
	}

		if (autoPrac) {
		g_forcePractice = true;
		practiceButton->m_bEnabled = false;
		practiceButton->m_bVisible = false;
		playButton->setPositionX(0.00);
	} else {
		g_forcePractice = false;
		practiceButton->setID("practice-button"_spr);
		practiceButton->m_bEnabled = true;
		practiceButton->m_bVisible = true;
	}

		menu->addChild(practiceButton);
		return true;
	}

	void onPracticeButton(CCObject*) {
		g_forcePractice = true;

		auto menu = this->getChildByID("play-menu");
		auto playButton = menu->getChildByType<CCMenuItemSpriteExtra>(0);

		playButton->activate();
	};
};

class $modify(MyLevelSelectLayer, LevelSelectLayer) {
	public:

	bool init(int page) {
		if (!LevelSelectLayer::init(page)) return false;
		
		bool autoPractice = Mod::get()->getSettingValue<bool>("autopractice");

		if (autoPractice) {
			g_forcePractice = true;
		}
		return true;
	}
};

class $modify(MyPlayLayer, PlayLayer) {
	bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
		if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
		
		if (g_forcePractice) {
			this->togglePracticeMode(true);
		} else {
			this->togglePracticeMode(false);
		}

		g_forcePractice = true;
		return true;
	}
};
