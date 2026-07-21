// Copyright © 2026 CCP ehf.

#include "semver.h"

namespace pdm {
    bool ParseSemanticVersion(std::string_view val, SemanticVersion& result)
    {
        result.major = result.minor = result.patch = 0;

        enum {
            CORE,
            PRE_RELEASE,
            BUILD,
        };

        auto state = CORE;
        unsigned int* version_core = &result.major;

        for(auto c : val)
        {
            if (state == CORE)
            {
                if (isdigit(c)) {
                    *version_core *= 10;
                    *version_core += c - '0';
                    continue;
                } else if (c == '.') {
                    if (version_core == &result.major) {
                        version_core = &result.minor;
                        continue;
                    } else if (version_core == &result.minor) {
                        version_core = &result.patch;
                        continue;
                    } else {
                        return false;
                    }
                } else if ( c == '+' && version_core == &result.patch) {
                    state = BUILD;
                    continue;
                } else if ( c == '-' && version_core == &result.patch ) {
                    state = PRE_RELEASE;
                    continue;
                } else {
                    return false;
                }
            } else if (state == PRE_RELEASE) {
                if (isalnum(c) || c == '-' || c == '.')
                {
                    result.pre_release += c;
                    continue;
                } else if ( c == '+' ) {
                    state = BUILD;
                    continue;
                } else {
                    return false;
                }
            } else {
                if (isalnum(c) || c == '-' || c == '.') {
                    result.build += c;
                    continue;
                } else {
                    return false;
                }
            }
        }

        return true;
    }
}
