/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay@2.1-service.hisi"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>

#include "DisplayColorCalibration.h"

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using ::vendor::lineage::livedisplay::V2_0::IDisplayColorCalibration;
using ::vendor::lineage::livedisplay::V2_0::hisi::DisplayColorCalibration;

int main() {
    android::sp<IDisplayColorCalibration> dcc = new DisplayColorCalibration();

    configureRpcThreadpool(1, true /*callerWillJoin*/);

    if (dcc->registerAsService() != android::OK) {
        LOG(ERROR) << "Cannot register display color calibration HAL service.";
        return 1;
    }

    LOG(INFO) << "LiveDisplay HAL service is ready.";

    joinRpcThreadpool();

    LOG(ERROR) << "LiveDisplay HAL service failed to join thread pool.";
    return 1;
}
