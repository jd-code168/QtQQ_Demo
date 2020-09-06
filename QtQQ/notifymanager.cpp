#include "notifymanager.h"
#include "commonutils.h"

NotifyManager *NotifyManager::instance = nullptr;

NotifyManager::NotifyManager()
	:QObject(nullptr)
{
}

NotifyManager::~NotifyManager()
{
}

NotifyManager *NotifyManager::getInstance() {
	if (nullptr == instance) {
		instance = new NotifyManager;
	}
	return instance;
}

void NotifyManager::notifyOtherWindowChangeSkin(const QColor &color) {
	emit signalSkinChanged(color);
	CommonUtils::setDefaultSkinColor(color); 
}