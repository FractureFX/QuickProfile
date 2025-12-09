#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/ProfilePage.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto spr = CCSprite::createWithSpriteFrameName("GJ_profileButton_001.png");
		spr->setScale(0.6f);
		auto menu = this->getChildByID("left-button-menu");
		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, menu_selector(MyPauseLayer::onProfileButton)
		);

		menu->addChild(btn, 0);
		menu->updateLayout();
	}

	void onProfileButton(CCObject* sender) {
		const auto gm = GJAccountManager::sharedState();
		int accountID = gm->m_accountID;
		const auto profileLayer = ProfilePage::create(accountID, true);
		profileLayer->show();
	}
};