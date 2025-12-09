#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(MyPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();
		auto spr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
		auto menu = this->getChildByID("left-button-menu");
		auto btn = CCMenuItemSpriteExtra::create(
			spr, this, nullptr
		);

		menu->addChild(btn);
		spr->setScale(0.65f);
		menu->updateLayout();
	}
};