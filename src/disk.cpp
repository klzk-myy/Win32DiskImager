/**********************************************************************
 *  This program is free software; you can redistribute it and/or     *
 *  modify it under the terms of the GNU General Public License       *
 *  as published by the Free Software Foundation; either version 2    *
 *  of the License, or (at your option) any later version.            *
 *                                                                    *
 *  This program is distributed in the hope that it will be useful,   *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of    *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the     *
 *  GNU General Public License for more details.                      *
 *                                                                    *
 *  You should have received a copy of the GNU General Public License *
 *  along with this program; if not, see http://gnu.org/licenses/     *
 *  ---                                                               *
 *  Copyright (C) 2009, Justin Davis <tuxdavis@gmail.com>             *
 *  Copyright (C) 2009-2017 ImageWriter developers                    *
 *                 https://sourceforge.net/projects/win32diskimager/  *
 **********************************************************************/

#ifndef WINVER
#define WINVER 0x0601
#endif

#include <QMessageBox>
#include <QString>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <winioctl.h>
#include "disk.h"
#include "mainwindow.h"

HANDLE getHandleOnFile(LPCWSTR filelocation, DWORD access)
{
    HANDLE hFile;
    hFile = CreateFileW(filelocation, access, (access == GENERIC_READ) ? FILE_SHARE_READ : 0, nullptr, (access == GENERIC_READ) ? OPEN_EXISTING:CREATE_ALWAYS, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        wchar_t *errormessage=nullptr;
        ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0,
                         (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("File Error"), QObject::tr("An error occurred when attempting to get a handle on the file.\n"
                                                              "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return hFile;
}
DWORD getDeviceID(HANDLE hVolume)
{
    VOLUME_DISK_EXTENTS sd = {};
    DWORD bytesreturned;
    if (!DeviceIoControl(hVolume, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, nullptr, 0, &sd, sizeof(sd), &bytesreturned, nullptr))
    {
        wchar_t *errormessage=nullptr;
        ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0,
                         (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Volume Error"),
                              QObject::tr("An error occurred when attempting to get information on volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return sd.Extents[0].DiskNumber;
}

HANDLE getHandleOnDevice(int device, DWORD access)
{
    HANDLE hDevice;
    QString devicename = QString("\\\\.\\PhysicalDrive%1").arg(device);
    std::wstring wdevicename = devicename.toStdWString();
    hDevice = CreateFileW(wdevicename.c_str(), access, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hDevice == INVALID_HANDLE_VALUE)
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Device Error"),
                              QObject::tr("An error occurred when attempting to get a handle on the device.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return hDevice;
}

HANDLE getHandleOnVolume(int volume, DWORD access)
{
    HANDLE hVolume;
    wchar_t volumename[] = L"\\\\.\\A:";
    volumename[4] += volume;
    hVolume = CreateFileW(volumename, access, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hVolume == INVALID_HANDLE_VALUE)
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Volume Error"),
                              QObject::tr("An error occurred when attempting to get a handle on the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return hVolume;
}

bool getLockOnVolume(HANDLE handle)
{
    DWORD bytesreturned;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_LOCK_VOLUME, nullptr, 0, nullptr, 0, &bytesreturned, nullptr);
    if (!bResult)
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Lock Error"),
                              QObject::tr("An error occurred when attempting to lock the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return (bResult);
}

bool removeLockOnVolume(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_UNLOCK_VOLUME, nullptr, 0, nullptr, 0, &junk, nullptr);
    if (!bResult)
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Unlock Error"),
                              QObject::tr("An error occurred when attempting to unlock the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return (bResult);
}

bool unmountVolume(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_DISMOUNT_VOLUME, nullptr, 0, nullptr, 0, &junk, nullptr);
    if (!bResult)
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Dismount Error"),
                              QObject::tr("An error occurred when attempting to dismount the volume.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return (bResult);
}

bool isVolumeUnmounted(HANDLE handle)
{
    DWORD junk;
    BOOL bResult;
    bResult = DeviceIoControl(handle, FSCTL_IS_VOLUME_MOUNTED, nullptr, 0, nullptr, 0, &junk, nullptr);
    return (!bResult);
}

char *readSectorDataFromHandle(HANDLE handle, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
    unsigned long bytesread;
    char *data = new char[sectorsize * numsectors];
    LARGE_INTEGER li;
    li.QuadPart = startsector * sectorsize;
    SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN);
    DWORD bytesToRead = (sectorsize * numsectors > 0xFFFFFFFFULL) ? 0xFFFFFFFF : (DWORD)(sectorsize * numsectors);
    if (!ReadFile(handle, data, bytesToRead, &bytesread, nullptr))
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Read Error"),
                              QObject::tr("An error occurred when attempting to read data from handle.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
        delete[] data;
        data = nullptr;
    }
    if (data && bytesread < bytesToRead)
    {
            memset(data + bytesread, 0, bytesToRead - bytesread);
    }
    return data;
}

bool writeSectorDataToHandle(HANDLE handle, char *data, unsigned long long startsector, unsigned long long numsectors, unsigned long long sectorsize)
{
    unsigned long byteswritten;
    BOOL bResult;
    LARGE_INTEGER li;
    li.QuadPart = startsector * sectorsize;
    SetFilePointer(handle, li.LowPart, &li.HighPart, FILE_BEGIN);
    DWORD bytesToWrite = (sectorsize * numsectors > 0xFFFFFFFFULL) ? 0xFFFFFFFF : (DWORD)(sectorsize * numsectors);
    bResult = WriteFile(handle, data, bytesToWrite, &byteswritten, nullptr);
    if (!bResult)
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Write Error"),
                              QObject::tr("An error occurred when attempting to write data to handle.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
    }
    return (bResult);
}

unsigned long long getNumberOfSectors(HANDLE handle, unsigned long long *sectorsize)
{
    DWORD junk;
    DISK_GEOMETRY_EX diskgeometry;
    BOOL bResult;
    bResult = DeviceIoControl(handle, IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, nullptr, 0, &diskgeometry, sizeof(diskgeometry), &junk, nullptr);
    if (!bResult)
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Device Error"),
                              QObject::tr("An error occurred when attempting to get the device's geometry.\n"
                                          "Error %1: %2").arg(GetLastError()).arg(errText));
        LocalFree(errormessage);
        return 0;
    }
    if (sectorsize != nullptr)
    {
        *sectorsize = (unsigned long long)diskgeometry.Geometry.BytesPerSector;
    }
    return (unsigned long long)diskgeometry.DiskSize.QuadPart / (unsigned long long)diskgeometry.Geometry.BytesPerSector;
}

unsigned long long getFileSizeInSectors(HANDLE handle, unsigned long long sectorsize)
{
    unsigned long long retVal = 0;
    if (sectorsize) // avoid divide by 0
    {
        LARGE_INTEGER filesize;
        if(GetFileSizeEx(handle, &filesize) == 0)
        {
            // error
            wchar_t *errormessage=nullptr;
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
            QString errText = QString::fromWCharArray(errormessage);
            QMessageBox::critical(MainWindow::getInstance(), QObject::tr("File Error"),
                                  QObject::tr("An error occurred while getting the file size.\n"
                                              "Error %1: %2").arg(GetLastError()).arg(errText));
            LocalFree(errormessage);
            retVal = 0;
        }
        else
        {
            retVal = ((unsigned long long)filesize.QuadPart / sectorsize ) + (((unsigned long long)filesize.QuadPart % sectorsize )?1:0);
        }
    }
    return(retVal);
}

bool spaceAvailable(char *location, unsigned long long spaceneeded)
{
    ULARGE_INTEGER freespace;
    BOOL bResult;
    bResult = GetDiskFreeSpaceEx(location, nullptr, nullptr, &freespace);
    if (!bResult)
    {
        wchar_t *errormessage=nullptr;
        FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
        QString errText = QString::fromWCharArray(errormessage);
        QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Free Space Error"),
                              QObject::tr("Failed to get the free space on drive %1.\n"
                                          "Error %2: %3\n"
                                          "Checking of free space will be skipped.").arg(location).arg(GetLastError()).arg(errText));
        return true;
    }
    return (spaceneeded <= freespace.QuadPart);
}

// given a drive letter (ending in a slash), return the label for that drive
// TODO make this more robust by adding input verification
QString getDriveLabel(const char *drv)
{
    QString retVal;
    int szNameBuf = MAX_PATH + 1;
    char *nameBuf = nullptr;
    if( (nameBuf = (char *)calloc(szNameBuf, sizeof(char))) != 0 )
    {
        ::GetVolumeInformationA(drv, nameBuf, szNameBuf, nullptr,
                                        nullptr, nullptr, nullptr, 0);
    }

    // if malloc fails, nameBuf will be nullptr.
    // if GetVolumeInfo fails, nameBuf will contain empty string
    // if all succeeds, nameBuf will contain label
    if(nameBuf == nullptr)
    {
        retVal = QString("");
    }
    else
    {
        retVal = QString(nameBuf);
        free(nameBuf);
    }

    return(retVal);
}

BOOL GetDisksProperty(HANDLE hDevice, PSTORAGE_DEVICE_DESCRIPTOR pDevDesc,
                      DEVICE_NUMBER *devInfo)
{
    STORAGE_PROPERTY_QUERY Query; // input param for query
    DWORD dwOutBytes; // IOCTL output length
    BOOL bResult; // IOCTL return val
    BOOL retVal = true;

    // specify the query type
    Query.PropertyId = StorageDeviceProperty;
    Query.QueryType = PropertyStandardQuery;

    // Query using IOCTL_STORAGE_QUERY_PROPERTY
    bResult = ::DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY,
                &Query, sizeof(STORAGE_PROPERTY_QUERY), pDevDesc,
                pDevDesc->Size, &dwOutBytes, (LPOVERLAPPED)nullptr);
    if (bResult)
    {
        bResult = ::DeviceIoControl(hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER,
                    nullptr, 0, devInfo, sizeof(DEVICE_NUMBER), &dwOutBytes,
                    (LPOVERLAPPED)nullptr);
        if (!bResult)
        {
            retVal = false;
            wchar_t *errormessage=nullptr;
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
            QString errText = QString::fromWCharArray(errormessage);
            QMessageBox::critical(MainWindow::getInstance(), QObject::tr("File Error"),
                                  QObject::tr("An error occurred while getting the device number.\n"
                                              "This usually means something is currently accessing the device;"
                                              "please close all applications and try again.\n\nError %1: %2").arg(GetLastError()).arg(errText));
            LocalFree(errormessage);
        }
    }
    else
    {
        retVal = false;
    }

    return(retVal);
}

// some routines fail if there's no trailing slash in a name,
// 		others fail if there is.  So this routine takes a name (trailing
// 		slash or no), and creates 2 versions - one with the slash, and one w/o
//
// 		CALLER MUST FREE THE 2 RETURNED STRINGS
bool slashify(char *str, char **slash, char **noSlash)
{
    bool retVal = false;
    int strLen = strlen(str);
    if ( strLen > 0 )
    {
        if ( *(str + strLen - 1) == '\\' )
        {
            // trailing slash exists
            if (( (*slash = (char *)calloc( (strLen + 1), sizeof(char))) != nullptr) &&
                    ( (*noSlash = (char *)calloc(strLen, sizeof(char))) != nullptr))
            {
                strncpy(*slash, str, strLen);
                strncpy(*noSlash, *slash, (strLen - 1));
                retVal = true;
            }
        }
        else
        {
            // no trailing slash exists
            if ( ((*slash = (char *)calloc( (strLen + 2), sizeof(char))) != nullptr) &&
                 ((*noSlash = (char *)calloc( (strLen + 1), sizeof(char))) != nullptr) )
            {
                strncpy(*noSlash, str, strLen);
                sprintf(*slash, "%s\\", *noSlash);
                retVal = true;
            }
        }
    }
    return(retVal);
}

bool GetMediaType(HANDLE hDevice)
{
    DISK_GEOMETRY diskGeo;
    DWORD cbBytesReturned;
    if (DeviceIoControl(hDevice, IOCTL_DISK_GET_DRIVE_GEOMETRY,nullptr, 0, &diskGeo, sizeof(diskGeo), &cbBytesReturned, nullptr))
    {
        if ((diskGeo.MediaType == FixedMedia) || (diskGeo.MediaType == RemovableMedia))
        {
            return true; // Not a floppy
        }
    }
    return false;
}

bool checkDriveType(char *name, ULONG *pid)
{
    HANDLE hDevice;
    PSTORAGE_DEVICE_DESCRIPTOR pDevDesc;
    DEVICE_NUMBER deviceInfo;
    bool retVal = false;
    char *nameWithSlash;
    char *nameNoSlash;
    int driveType;
    DWORD cbBytesReturned;

    // some calls require no tailing slash, some require a trailing slash...
    if ( !(slashify(name, &nameWithSlash, &nameNoSlash)) )
    {
        return(retVal);
    }

    driveType = GetDriveType(nameWithSlash);
    switch( driveType )
    {
    case DRIVE_REMOVABLE: // The media can be removed from the drive.
    case DRIVE_FIXED:     // The media cannot be removed from the drive. Some USB drives report as this.
    {
        std::wstring wnameNoSlash = QString(nameNoSlash).toStdWString();
        hDevice = CreateFileW(wnameNoSlash.c_str(), FILE_READ_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
        if (hDevice == INVALID_HANDLE_VALUE)
        {
            wchar_t *errormessage=nullptr;
            FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), 0, (LPWSTR)&errormessage, 0, nullptr);
            QString errText = QString::fromWCharArray(errormessage);
            QMessageBox::critical(MainWindow::getInstance(), QObject::tr("Volume Error"),
                                  QObject::tr("An error occurred when attempting to get a handle on %3.\n"
                                              "Error %1: %2").arg(GetLastError()).arg(errText).arg(nameWithSlash));
            LocalFree(errormessage);
        }
        else
        {
            int arrSz = sizeof(STORAGE_DEVICE_DESCRIPTOR) + 512 - 1;
            pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[arrSz];
            pDevDesc->Size = arrSz;

            // get the device number if the drive is
            // removable or (fixed AND on the usb bus, SD, or MMC (undefined in XP/mingw))
            if(GetMediaType(hDevice) && GetDisksProperty(hDevice, pDevDesc, &deviceInfo) &&
                    ( ((driveType == DRIVE_REMOVABLE) && (pDevDesc->BusType != BusTypeSata))
                      || ( (driveType == DRIVE_FIXED) && ((pDevDesc->BusType == BusTypeUsb)
                      || (pDevDesc->BusType == BusTypeSd ) || (pDevDesc->BusType == BusTypeMmc )) ) ) )
            {
                // ensure that the drive is actually accessible
                // multi-card hubs were reporting "removable" even when empty
                if(DeviceIoControl(hDevice, IOCTL_STORAGE_CHECK_VERIFY2, nullptr, 0, nullptr, 0, &cbBytesReturned, (LPOVERLAPPED) nullptr))
                {
                    *pid = deviceInfo.DeviceNumber;
                    retVal = true;
                }
                else
                // IOCTL_STORAGE_CHECK_VERIFY2 fails on some devices under XP/Vista, try the other (slower) method, just in case.
                {
                    CloseHandle(hDevice);
                    hDevice = CreateFileW(wnameNoSlash.c_str(), FILE_READ_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
                    if(DeviceIoControl(hDevice, IOCTL_STORAGE_CHECK_VERIFY, nullptr, 0, nullptr, 0, &cbBytesReturned, (LPOVERLAPPED) nullptr))
                    {
                        *pid = deviceInfo.DeviceNumber;
                        retVal = true;
                    }
                }
            }

            delete[] pDevDesc;
            CloseHandle(hDevice);
        }
    }
        break;
    default:
        retVal = false;
    }

    // free the strings allocated by slashify
    free(nameWithSlash);
    free(nameNoSlash);

    return(retVal);
}
