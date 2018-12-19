// Copyright 2015-2018 Elviss Strazdins. All rights reserved.

#include "core/Setup.h"

#if OUZEL_COMPILE_WASAPI

#include <string>
#include "AudioDeviceWASAPI.hpp"

namespace ouzel
{
    namespace audio
    {
        class NotificationClient: public IMMNotificationClient
        {
            LONG refCount = 1;

        public:
            NotificationClient() {}
            NotificationClient(const NotificationClient&) = delete;
            NotificationClient(NotificationClient&&) = delete;
            NotificationClient& operator=(const NotificationClient&) = delete;
            NotificationClient& operator=(NotificationClient&&) = delete;

            ULONG STDMETHODCALLTYPE AddRef()
            {
                return InterlockedIncrement(&refCount);
            }

            ULONG STDMETHODCALLTYPE Release()
            {
                ULONG newRefCount = InterlockedDecrement(&refCount);
                if (!newRefCount)
                    delete this;

                return newRefCount;
            }

            HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID** ppvInterface)
            {
                if (riid == IID_IUnknown)
                {
                    AddRef();
                    *ppvInterface = this;
                }
                else if (riid == __uuidof(IMMNotificationClient))
                {
                    AddRef();
                    *ppvInterface = this;
                }
                else
                {
                    *ppvInterface = nullptr;
                    return E_NOINTERFACE;
                }
                return S_OK;
            }

            HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId)
            {
                return S_OK;
            };

            HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId)
            {
                return S_OK;
            }

            HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState)
            {
                return S_OK;
            }

            HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDeviceId)
            {
                if (role == eConsole)
                {
                    if (flow == eRender)
                    {
                        // TODO: implement
                    }
                    else if (flow == eCapture)
                    {
                        // TODO: implement
                    }
                }

                return S_OK;
            }

            HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key)
            {
                return S_OK;
            }
        };

        class WASAPIErrorCategory : public std::error_category
        {
        public:
            const char* name() const noexcept override
            {
                return "DirectSound";
            }

            std::string message(int condition) const override
            {
                switch (condition)
                {
                    default: return "Unknown error (" + std::to_string(condition) + ")";
                }
            }
        };

        const WASAPIErrorCategory wasapiErrorCategory{};

        AudioDeviceWASAPI::AudioDeviceWASAPI(Mixer& initMixer):
            AudioDevice(Driver::WASAPI, initMixer)
        {
            IMMDeviceEnumerator* enumerator = nullptr;

            HRESULT hr;
            if (FAILED(hr = CoCreateInstance(CLSID_MMDeviceEnumerator, nullptr, CLSCTX_ALL, IID_IMMDeviceEnumerator, reinterpret_cast<LPVOID*>(&enumerator))))
                throw std::system_error(hr, wasapiErrorCategory, "Failed to create device enumerator");

            IMMDevice* device;
            if (FAILED(hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device)))
                throw std::system_error(hr, wasapiErrorCategory, "Failed to get audio endpoint");

            notificationClient = new NotificationClient();

            if (FAILED(hr = enumerator->RegisterEndpointNotificationCallback(notificationClient)))
                throw std::system_error(hr, wasapiErrorCategory, "Failed to get audio endpoint");

            if (enumerator) enumerator->Release();
        }

        AudioDeviceWASAPI::~AudioDeviceWASAPI()
        {
            if (notificationClient) notificationClient->Release();
        }
    } // namespace audio
} // namespace ouzel

#endif
