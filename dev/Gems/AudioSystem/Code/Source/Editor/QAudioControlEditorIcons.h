/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
// Original file Copyright Crytek GMBH or its affiliates, used under license.

#pragma once

#include <QIcon>
#include <AudioControl.h>

namespace AudioControls
{
    //-------------------------------------------------------------------------------------------//
    inline QIcon GetControlTypeIcon(EACEControlType type)
    {
        QString iconFile;

        switch (type)
        {
        case AudioControls::eACET_TRIGGER:
            iconFile = ":/Editor/Icons/Trigger_Icon.svg";
            break;
        case AudioControls::eACET_RTPC:
            iconFile = ":/Editor/Icons/RTPC_Icon.svg";
            break;
        case AudioControls::eACET_SWITCH:
            iconFile = ":/Editor/Icons/Switch_Icon.svg";
            break;
        case AudioControls::eACET_SWITCH_STATE:
            iconFile = ":/Editor/Icons/Property_Icon.svg";
            break;
        case AudioControls::eACET_ENVIRONMENT:
            iconFile = ":/Editor/Icons/Environment_Icon.svg";
            break;
        case AudioControls::eACET_PRELOAD:
            iconFile = ":/Editor/Icons/Preload_Icon.svg";
            break;
        default:
            // should make a "default"/empty icon...
            iconFile = ":/Editor/Icons/RTPC_Icon.svg";
        }

        QIcon icon(iconFile);
        icon.addFile(iconFile, QSize(), QIcon::Selected);
        return icon;
    }

    //-------------------------------------------------------------------------------------------//
    inline QIcon GetFolderIcon()
    {
        QIcon icon = QIcon(":/Editor/Icons/Folder_Icon.svg");
        icon.addFile(":/Editor/Icons/Folder_Icon_Selected.svg", QSize(), QIcon::Selected);
        return icon;
    }

    //-------------------------------------------------------------------------------------------//
    inline QIcon GetSoundBankIcon()
    {
        QIcon icon = QIcon(":/Editor/Icons/Preload_Icon.svg");
        icon.addFile(":/Editor/Icons/Preload_Icon.svg", QSize(), QIcon::Selected);
        return icon;
    }

    //-------------------------------------------------------------------------------------------//
    inline QIcon GetGroupIcon(int group)
    {
        const int numberOfGroups = 5;
        group = group % numberOfGroups;
        QString path;
        switch (group)
        {
        case 0:
            path = ":/Editor/Icons/folder purple.svg";
            break;
        case 1:
            path = ":/Editor/Icons/folder blue.svg";
            break;
        case 2:
            path = ":/Editor/Icons/folder green.svg";
            break;
        case 3:
            path = ":/Editor/Icons/folder red.svg";
            break;
        case 4:
            path = ":/Editor/Icons/folder yellow.svg";
            break;
        default:
            path = "Editor/Icons/folder red.svg";
            break;
        }

        QIcon icon;
        icon.addFile(path);
        icon.addFile(path, QSize(), QIcon::Selected);
        return icon;
    }
} // namespace AudioControls
