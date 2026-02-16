#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/CCLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto leftMenu = this->getChildByID("left-button-menu");
		auto rightMenu = this->getChildByID("right-button-menu");
		auto settingPos = Mod::get()->getSettingValue<std::string>("position");
		auto targetMenu = (settingPos == "Left") ? leftMenu : rightMenu;

		if (!targetMenu) {
			targetMenu = leftMenu;
			return;
		}

		auto spr = CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png");
		spr->setScale(0.6f);
		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(MyPauseLayer::onProfileButton)
		);

		targetMenu->addChild(btn, 0);
		targetMenu->updateLayout();
	}

	void onProfileButton(CCObject* sender) {
		const auto gm = GJAccountManager::sharedState();
		int accountID = gm->m_accountID;
		auto profileLayer = ProfilePage::create(accountID, true);
		profileLayer->show();
		profileLayer->setZOrder(999);
	}
};

class $modify(LevelInfoLayer) {
    void onPlay(CCObject* sender) {
        if (PlayLayer::get()) {
            FLAlertLayer::create(
                "Warning",
                "You can't play a level when you are already in one!",
                "OK"
            )->show();
            return;
        }
        LevelInfoLayer::onPlay(sender);
    }

    void tryCloneLevel(CCObject* sender) {
        if (PlayLayer::get()) {
            FLAlertLayer::create(
                "Warning",
                "You can't clone a level when you are already in one!",
                "OK"
            )->show();
            return;
        }
        LevelInfoLayer::tryCloneLevel(sender);
    }
};

// CREDITS GO TO DANKMEME FOR THIS PART OF THE CODE
class $modify(FixedPlayLayer, PlayLayer) {
	bool isCurrentPlayLayer(){
		auto playLayer = cocos2d::CCScene::get()->getChildByType<PlayLayer>(0);
		return playLayer == this;
	}
	bool isPaused(bool checkCurrent){
		if(checkCurrent && !isCurrentPlayLayer()) return false;

		for(CCNode* child : CCArrayExt<CCNode*>(this->getParent()->getChildren())) {
			if(typeinfo_cast<PauseLayer*>(child)) {
				return true;
			}
		}

		return false;
	}
	void onEnterH(){
		auto weRunningScene = this->getParent() == CCScene::get();

		if(weRunningScene){
			CCLayer::onEnter();
			return;
		}

		Loader::get()->queueInMainThread([self = Ref(this)] {
		        if (!self->isPaused(false)) {
		            self->CCLayer::onEnter();
		        }
		    });
	}
};
class $modify(MyCCLayer, CCLayer){
	void onEnter(){
		if(reinterpret_cast<void*>(PlayLayer::get()) == reinterpret_cast<void*>(this)){
				auto pl = reinterpret_cast<FixedPlayLayer*>(static_cast<CCLayer*>(this));
				pl->onEnterH();
		} else {
			CCLayer::onEnter();
		}
	}
};