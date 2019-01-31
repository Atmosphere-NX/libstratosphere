/*
 * Copyright (c) 2018 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#include <mutex>
#include <switch.h>
#include <stratosphere.hpp>

static FirmwareVersion g_firmware_version = FirmwareVersion_Min;
static bool g_HasCached = 0;
static Mutex g_Mutex;

static void _CacheValues(void)
{
    if (__atomic_load_n(&g_HasCached, __ATOMIC_SEQ_CST))
        return;

    mutexLock(&g_Mutex);

    if (g_HasCached) {
        mutexUnlock(&g_Mutex);
        return;
    }

    u32 target_fw = 0;
    GetAtmosphereApiVersion(nullptr, nullptr, nullptr, &target_fw, nullptr);
    
    switch (static_cast<AtmosphereTargetFirmware>(target_fw)) {
        case AtmosphereTargetFirmware_100:
            g_firmware_version = FirmwareVersion_100;
            break;
        case AtmosphereTargetFirmware_200:
            g_firmware_version = FirmwareVersion_200;
            break;
        case AtmosphereTargetFirmware_300:
            g_firmware_version = FirmwareVersion_300;
            break;
        case AtmosphereTargetFirmware_400:
            g_firmware_version = FirmwareVersion_400;
            break;
        case AtmosphereTargetFirmware_500:
            g_firmware_version = FirmwareVersion_500;
            break;
        case AtmosphereTargetFirmware_600:
        case AtmosphereTargetFirmware_620:
            g_firmware_version = FirmwareVersion_600;
            break;
        case AtmosphereTargetFirmware_700:
            g_firmware_version = FirmwareVersion_700;
            break;
        default:
            std::abort();
            break;
    }

    __atomic_store_n(&g_HasCached, true, __ATOMIC_SEQ_CST);

    mutexUnlock(&g_Mutex);
}

FirmwareVersion GetRuntimeFirmwareVersion() {
    _CacheValues();
    return g_firmware_version;
}