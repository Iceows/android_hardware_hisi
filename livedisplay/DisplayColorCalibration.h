/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vendor/lineage/livedisplay/2.0/IDisplayColorCalibration.h>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace hisi {

using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

class DisplayColorCalibration : public IDisplayColorCalibration {
  public:
    bool isSupported();

    Return<int32_t> getMaxValue() override;
    Return<int32_t> getMinValue() override;
    Return<void> getCalibration(getCalibration_cb _hidl_cb) override;
    Return<bool> setCalibration(const hidl_vec<int32_t>& rgb) override;

  private:
    std::vector<int32_t> mCachedColors = {32768, 32768, 32768};
    std::mutex mCachedColorsMutex;
};

}  // namespace hisi
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
