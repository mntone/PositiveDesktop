#pragma once

struct ServiceWrapper {
	ServiceWrapper() = default;
	ServiceWrapper(ServiceWrapper const&) = delete;
	ServiceWrapper& operator=(ServiceWrapper const&) = delete;

	virtual void close() noexcept = 0;
};
using service_t = ServiceWrapper;

service_t* CreateVirtualDesktopNotificationService() noexcept;

struct ServiceManager {
	service_t* services_[1];

	~ServiceManager();

	bool Initialize() noexcept;
};
