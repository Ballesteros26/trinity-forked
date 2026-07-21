// Copyright © 2026 CCP ehf.

#include "../src/semver.h"
#include "../include/pdm/protobuf.h"
#include "../generated/eve_launcher/pdm.pb.h"
#include "../generated/eve_public/app/platform.pb.h"
#include <gtest/gtest.h>

struct test_row {
    std::string test_data;
    bool expected_ret;
    pdm::SemanticVersion expected_result;
};

test_row test_rows[] = {
    {"..", true, { 0, 0, 0, "", ""}},
    {"1.2.3", true, { 1, 2, 3, "", ""}},
    {"..42", true, { 0, 0, 42, "", ""}},
    {".989.", true, { 0, 989, 0, "", ""}},
    {"123.45.6789", true, { 123, 45, 6789, "", ""}},
    {"1.0.0-alpha.1", true, {1,0,0,"alpha.1", ""}},
    {"1.0.0-alpha.1+internal-build", true, {1,0,0,"alpha.1", "internal-build"}},
    {"1.0.0+internal-build.4711", true, {1,0,0,"", "internal-build.4711"}},
    {"-invalid", false, {0,0,0,"",""}},
    {"+invalid", false, {0,0,0,"",""}},
    {"-invalid+meta", false, {0,0,0,"",""}},
    {".-also+invalid", false, {0,0,0,"",""}},
    {"v1.2.3", false, { 0, 0, 0, "", ""}},
};

TEST(SemanticVersion, CanConvertSemverData) {
    for (int i = 0; i < (sizeof(test_rows)/sizeof(test_row)); ++i) {
        auto& row = test_rows[i];
        pdm::SemanticVersion test_result;
        EXPECT_EQ(row.expected_ret, pdm::ParseSemanticVersion(row.test_data, test_result));
        if (row.expected_ret) {
            EXPECT_EQ(row.expected_result, test_result);
        }
    }
}

TEST(PdmProto, CanGetEVEPublicProtobufData)
{
        eve_public::app::platform::Information msg;
        std::stringstream out;
        ASSERT_TRUE(pdm_proto::GetEVEPublicData(&out));
        ASSERT_TRUE(msg.ParseFromIstream(&out));
}

TEST(PdmProto, CanGetData)
{
        eve_public::app::platform::Information msg;
        std::stringstream out;
        ASSERT_TRUE(pdm_proto::GetEVEPublicData(&out));
        ASSERT_TRUE(msg.ParseFromIstream(&out));
}

TEST(PdmProto, CanGetEVELauncherProtobufData)
{
        platform::Information msg;
        std::stringstream out;
	ASSERT_TRUE(pdm_proto::GetEVELauncherData(&out));
        ASSERT_TRUE(msg.ParseFromIstream(&out));
}
