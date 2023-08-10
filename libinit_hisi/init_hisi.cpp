/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "vendor_init.h"

#include <libinit_connectivity.h>
#include <libinit_variants.h>

void vendor_load_properties() {
    load_connectivity_properties();
    load_variants_properties();
}
