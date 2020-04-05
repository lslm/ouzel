// Copyright 2015-2020 Elviss Strazdins. All rights reserved.

#ifndef OUZEL_XCODE_PBXBUILDPHASE_HPP
#define OUZEL_XCODE_PBXBUILDPHASE_HPP

#include "PBXObject.hpp"

namespace ouzel
{
    namespace xcode
    {
        class PBXBuildPhase: public PBXObject
        {
        public:
            PBXBuildPhase() = default;

            std::string getIsa() const override { return "PBXBuildPhase"; }

            plist::Value encode() const override
            {
                auto result = PBXObject::encode();
                result["buildActionMask"] = 2147483647;
                result["runOnlyForDeploymentPostprocessing"] = 0;
                return result;
            }
        };

        using PBXBuildPhaseRef = std::reference_wrapper<const PBXBuildPhase>;
    }
}

#endif // OUZEL_XCODE_PBXBUILDPHASE_HPP
