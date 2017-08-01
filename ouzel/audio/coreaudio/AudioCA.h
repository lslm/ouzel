// Copyright (C) 2017 Elviss Strazdins
// This file is part of the Ouzel engine.

#pragma once

#include "core/CompileConfig.h"

#if OUZEL_SUPPORTS_COREAUDIO

#if OUZEL_PLATFORM_MACOS
#include <CoreAudio/CoreAudio.h>
#endif

#include <AudioUnit/AudioUnit.h>

#include "audio/Audio.h"

namespace ouzel
{
    namespace audio
    {
        class AudioCA: public Audio
        {
            friend Engine;
        public:
            virtual ~AudioCA();

            bool outputCallback(AudioBufferList* ioData);

        protected:
            AudioCA();
            virtual bool init(bool debugAudio) override;

#if OUZEL_PLATFORM_MACOS
            AudioDeviceID deviceId = 0;
#endif
            AudioComponent audioComponent = nullptr;
            AudioComponentInstance audioUnit = nullptr;

            std::vector<uint8_t> data;
        };
    } // namespace audio
} // namespace ouzel

#endif
