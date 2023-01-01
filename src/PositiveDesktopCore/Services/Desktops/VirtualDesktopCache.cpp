#include "pch.h"
#include "VirtualDesktopCache.h"

#include "Private/VirtualDesktopDelegate.h"

using namespace winrt;

using namespace app::desktop;

VirtualDesktopCache::~VirtualDesktopCache() {
	for (auto cache : cache_) {
		IVirtualDesktopDelegate* delegate = std::exchange(cache.second, nullptr);
		WINRT_ASSERT(delegate);
		delete delegate;
	}
}

HRESULT VirtualDesktopCache::CreateDelegate(IVirtualDesktop* iface, int index, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	HRESULT hr = iface->GetID(&id);
	if (FAILED(hr)) return hr;
	WINRT_ASSERT(cache_.find(id) == cache_.end());

	IVirtualDesktopDelegate* delegate = new VirtualDesktopDelegate10240(iface, index);
	cache_.emplace(id, delegate);
	*ppDelegate = delegate;
	return hr;
}

HRESULT VirtualDesktopCache::CreateDelegate(IVirtualDesktop2* iface, int index, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	HRESULT hr = iface->GetID(&id);
	if (FAILED(hr)) return hr;
	WINRT_ASSERT(cache_.find(id) == cache_.end());

	IVirtualDesktopDelegate* delegate = new VirtualDesktopDelegate19041(iface, index);
	cache_.emplace(id, delegate);
	*ppDelegate = delegate;
	return hr;
}

HRESULT VirtualDesktopCache::CreateDelegate(IVirtualDesktop20231* iface, int index, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	HRESULT hr = iface->GetID(&id);
	if (FAILED(hr)) return hr;
	WINRT_ASSERT(cache_.find(id) == cache_.end());

	IVirtualDesktopDelegate* delegate = new VirtualDesktopDelegate20231(iface, index);
	cache_.emplace(id, delegate);
	*ppDelegate = delegate;
	return hr;
}

HRESULT VirtualDesktopCache::CreateDelegateIfNeeded(IVirtualDesktop* iface, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	HRESULT hr = iface->GetID(&id);
	if (FAILED(hr)) return hr;

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		IVirtualDesktopDelegate* delegate = new VirtualDesktopDelegate10240(iface, static_cast<int>(cache_.size()));
		cache_.emplace(id, delegate);
		*ppDelegate = delegate;
		return hr;
	}
	*ppDelegate = itr->second;
	return hr;
}

HRESULT VirtualDesktopCache::CreateDelegateIfNeeded(IVirtualDesktop2* iface, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	HRESULT hr = iface->GetID(&id);
	if (FAILED(hr)) return hr;

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		IVirtualDesktopDelegate* delegate = new VirtualDesktopDelegate19041(iface, static_cast<int>(cache_.size()));
		cache_.emplace(id, delegate);
		*ppDelegate = delegate;
		return hr;
	}
	*ppDelegate = itr->second;
	return hr;
}

HRESULT VirtualDesktopCache::CreateDelegateIfNeeded(IVirtualDesktop20231* iface, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	HRESULT hr = iface->GetID(&id);
	if (FAILED(hr)) return hr;

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		IVirtualDesktopDelegate* delegate = new VirtualDesktopDelegate20231(iface, static_cast<int>(cache_.size()));
		cache_.emplace(id, delegate);
		*ppDelegate = delegate;
		return hr;
	}
	*ppDelegate = itr->second;
	return hr;
}

HRESULT VirtualDesktopCache::FromInterface(IVirtualDesktop* iface, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	check_hresult(iface->GetID(&id));

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		return E_INVALIDARG;
	}
	*ppDelegate = itr->second;
	return S_OK;
}

HRESULT VirtualDesktopCache::FromInterface(IVirtualDesktop2* iface, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	check_hresult(iface->GetID(&id));

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		return E_INVALIDARG;
	}
	*ppDelegate = itr->second;
	return S_OK;
}

HRESULT VirtualDesktopCache::FromInterface(IVirtualDesktop20231* iface, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	check_hresult(iface->GetID(&id));

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		return E_INVALIDARG;
	}
	*ppDelegate = itr->second;
	return S_OK;
}

HRESULT VirtualDesktopCache::MoveDelegate(IVirtualDesktop20231* iface, int nFromIndex, int nToIndex, IVirtualDesktopDelegate** ppDelegate) noexcept {
	guid id;
	check_hresult(iface->GetID(&id));

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		return E_INVALIDARG;
	}
	itr->second->Index(nToIndex);

	// e.g. [A, B, C, D]
	if (nFromIndex > nToIndex) { // 3 to 1, expected: [A, D, B, C]
		for (auto& cache : cache_) {
			int const nCacheIndex { cache.second->Index() };
			if (nFromIndex <= nCacheIndex && nCacheIndex < nToIndex) {
				cache.second->Index(nCacheIndex + 1);
			}
		}
	} else if (nFromIndex < nToIndex) { // 1 to 3, expected: [A, C, D, B]
		for (auto& cache : cache_) {
			int const nCacheIndex { cache.second->Index() };
			if (nFromIndex < nCacheIndex && nCacheIndex <= nToIndex) {
				cache.second->Index(nCacheIndex - 1);
			}
		}
	} else {
		WINRT_ASSERT(false);
		return TYPE_E_OUTOFBOUNDS;
	}

	*ppDelegate = itr->second;
	return S_OK;
}

IVirtualDesktopDelegate* VirtualDesktopCache::DeleteDelegate(IVirtualDesktop* iface) {
	guid id;
	check_hresult(iface->GetID(&id));

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		throw hresult_invalid_argument();
	}

	IVirtualDesktopDelegate* delegate = itr->second;
	cache_.erase(itr);

	// -1 if index
	int const targetIndex = delegate->Index();
	for (auto& cache : cache_) {
		int const cacheIndex = cache.second->Index();
		if (cacheIndex > targetIndex) {
			cache.second->Index(cacheIndex - 1);
		}
	}
	return delegate;
}

IVirtualDesktopDelegate* VirtualDesktopCache::DeleteDelegate(IVirtualDesktop2* iface) {
	guid id;
	check_hresult(iface->GetID(&id));

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		throw hresult_invalid_argument();
	}

	IVirtualDesktopDelegate* delegate = itr->second;
	cache_.erase(itr);

	// -1 if index
	int const targetIndex = delegate->Index();
	for (auto& cache : cache_) {
		int const cacheIndex = cache.second->Index();
		if (cacheIndex > targetIndex) {
			cache.second->Index(cacheIndex - 1);
		}
	}
	return delegate;
}

IVirtualDesktopDelegate* VirtualDesktopCache::DeleteDelegate(IVirtualDesktop20231* iface) {
	guid id;
	check_hresult(iface->GetID(&id));

	auto itr = cache_.find(id);
	if (itr == cache_.end()) {
		throw hresult_invalid_argument();
	}

	IVirtualDesktopDelegate* delegate = itr->second;
	cache_.erase(itr);

	// -1 if index
	int const targetIndex = delegate->Index();
	for (auto& cache : cache_) {
		int const cacheIndex = cache.second->Index();
		if (cacheIndex > targetIndex) {
			cache.second->Index(cacheIndex - 1);
		}
	}
	return delegate;
}
