// Copyright © 2026 CCP ehf.

#pragma once

#include <string>
#include <string_view>

namespace pdm {
    struct SemanticVersion {
        unsigned int major;
        unsigned int minor;
        unsigned int patch;
        std::string pre_release;
        std::string build;

        bool operator==(const SemanticVersion& _rhs) const {
            return (
                this->major == _rhs.major
                && this->minor == _rhs.minor
                && this->patch == _rhs.patch
                && this->pre_release == _rhs.pre_release
                && this->build == _rhs.build
            );
        }
    };

    bool ParseSemanticVersion(std::string_view val, SemanticVersion& result);
}