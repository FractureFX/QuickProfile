#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto leftMenu = this->getChildByID("left-button-menu");
		auto rightMenu = this->getChildByID("right-button-menu");
		auto settingPos = Mod::get()->getSettingValue<std::string>("position");
		auto targetMenu = (settingPos == "Left") ? leftMenu : rightMenu;
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
		const auto profileLayer = ProfilePage::create(accountID, true);
		profileLayer->show();
	}
};

