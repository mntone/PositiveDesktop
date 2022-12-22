#include "pch.h"
#include "Services.h"

#define INIT_SERVICE(__I, __S) service = Create##__S##(); \
	if (nullptr == service) { \
		return true; \
	} \
	services_[__I] = service;

bool ServiceManager::Initialize() noexcept {
	service_t* service { nullptr };
	INIT_SERVICE(0, VirtualDesktopNotificationService);
	return false;
}

ServiceManager::~ServiceManager() {
	for (service_t*& service : services_) {
		service_t* temp = std::exchange(service, nullptr);
		if (temp) {
			temp->close();
		}
	}
}
