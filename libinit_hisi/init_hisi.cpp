/*
 * Copyright (C) 2024 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "vendor_init.h"

#include <libinit_dalvik.h>
#include <libinit_variants.h>

void vendor_load_properties() {
    load_dalvik();
    load_variants();
}
