#include <Geode/Geode.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include "Shared.hpp"

using namespace geode::prelude;

class $modify(MyLevelSelectLayer, LevelSelectLayer) {
public:
    bool init(int page) {
        if (!LevelSelectLayer::init(page)) return false;
        
        if (!Mod::get()->getSettingValue<bool>("enabled")) return true;
        
        if (Mod::get()->getSettingValue<bool>("auto-practice")) {
            g_forcePractice = true;
        }
        
        return true;
    }
};