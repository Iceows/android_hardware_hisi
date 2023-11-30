/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "hisi_nve"

#include "include/hisi_nve.h"

#include <android-base/logging.h>
#include <android-base/properties.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>


// Write MAC Adress
template <typename T>
static void set(const std::string& path, const T& value) {
    std::ofstream file(path);
    file << value << std::endl;
}


std::string load_nve_path() {
    // Loop over all the possible paths and use
    // the first one that exists and can be read.
    for (const auto& path : kNvePaths) {
        // Make sure the path is accessible for us.
        if (access(path.c_str(), R_OK) == 0) return path;
    }

    // If we reached this point, we couldn't find
    // a suitable path. Return an empty string and
    // let the caller handle the situation.
    return "";
}

std::streampos find_start_offset(std::ifstream& file) {
    if (!file.is_open()) return std::streampos(-1);

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    const size_t bufferSize = 4096;
    char buffer[bufferSize];

    for (std::streampos i = 0; i < file_size; i += bufferSize - 7) {
        file.seekg(i);
        file.read(buffer, bufferSize);
        for (size_t j = 0; j < bufferSize - 7; ++j) {
            if (std::memcmp(buffer + j, "SWVERSI", 7) == 0) {
                return i + static_cast<std::streampos>(j);
            }
        }
    }
    return std::streampos(-1);
}

bool is_hex(std::string str) {
    return str.find_first_not_of("0123456789ABCDEF") == std::string::npos;
}

std::string hex_to_mac(std::string hex) {
    std::stringstream ss;
    size_t split = 2;

    ss << hex.substr(0, split);

    for (size_t loc = split; loc < hex.size(); loc += split) {
        ss << ':' << hex.substr(loc, split);
    }

    return ss.str();
}

std::string nve_read(const std::string& mac_name, const std::string& path) {
    char buf[1]; // TODO: improve
    unsigned long mac_index = 0;

    // The caller is supposed to pass the path to the NVE
    // partition block and the name of the entry to read.
    std::ifstream nvme(path, std::ios::binary);

    // This shouldn't be required but make sure the path
    // the caller passed is valid and the file can be read.
    if (!nvme.is_open()) {
        // If we can't open the file, return an empty string
        // and let the caller handle the situation.
        LOG(ERROR) << "Unable to open " << path;
        return "";
    }

    LOG(INFO) << "Searching MAC entry for " << mac_name.c_str();

    // The next step is to find the start offset of the NVE
    // partition. This is done by searching for the "SWVERSI"
    // string and then going back 4 bytes.
    std::streampos start_offset = find_start_offset(nvme);
    if (start_offset == std::streampos(-1)) {
        // If we can't find the start offset, return an empty
        // string and let the caller handle the situation.
        LOG(ERROR) << "Unable to find the start offset of the NVE partition";
        return "";
    }
    
    while (!nvme.eof()) {
        nvme.read(buf, sizeof(buf));

	if (buf[0] == mac_name[mac_index]) {
            mac_index++;

            if (mac_index == (strlen(mac_name.c_str()) - 1)) {
                LOG(INFO) << "Found MAC entry for " << mac_name.c_str();
                std::string mac;

                for(size_t i = 0; i < MAC_ENTRY_MAX_LEN; i++) {
                    char mac_buff[1];

                    nvme.read(mac_buff, sizeof(mac_buff));

                    std::string tmp_mac(1, mac_buff[0]);

                    if (is_hex(tmp_mac)) {
                        mac += tmp_mac;
                    }

                    if (strlen(mac.c_str()) == MAC_LEN) { // full mac read
                        nvme.close();
                        return hex_to_mac(mac);
                    }
                }

                LOG(ERROR) << "Found a MAC entry but couldn't fully read MAC!";
                LOG(ERROR) << "This is probably a bug!";
            }
        } else {
            mac_index = 0;
        }
    }

    nvme.close();

    LOG(WARNING) << "MAC don't found";
    return "";
}


int LoadNveProperties() {
    std::string path = load_nve_path();
    if (path.empty()) {
        LOG(ERROR) << "Unable to find a suitable path for the NVE partition";
        return -1;
    }

    for (const auto& pair : kNveMacMap) {
        if (std::string mac_value = nve_read(pair.first, path); mac_value.empty()) {
            LOG(WARNING) << "Unable to read " << pair.first << " from NVE partition";
        } else {
            LOG(INFO) << "MAC found : " << mac_value;
            set(pair.second, mac_value);
        }
    }

    android::base::SetProperty(kPropMacsAreReady, "1");
    return 0;
}

void load_hisi_nve() {
    if (LoadNveProperties() < 0) LOG(WARNING) << "Unable to load nve properties";
}
