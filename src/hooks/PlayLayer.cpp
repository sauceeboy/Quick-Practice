#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../vars/Shared.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool dontCreateObjects) {
        if (!PlayLayer::init(level, dontCreateObjects)) return false;
        
        if (!Mod::get()->getSettingValue<bool>("enabled")) return true;
        
        if (g_forcePractice) {
            Loader::get()->queueInMainThread([this] {
                this->togglePracticeMode(true);
            });
        }
        
        g_forcePractice = false;
        return true;
    }
};