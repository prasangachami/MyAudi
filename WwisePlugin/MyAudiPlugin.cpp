/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2021 Audiokinetic Inc.
*******************************************************************************/


#include "MyAudiPlugin.h"
#include "resource.h"
#include "../SoundEnginePlugin/MyAudiFXFactory.h"

#include <AK/Tools/Common/AkAssert.h>

MyAudiPlugin::MyAudiPlugin() 
{
}

MyAudiPlugin::~MyAudiPlugin()
{
}

bool MyAudiPlugin::GetBankParameters(const GUID & in_guidPlatform, AK::Wwise::Plugin::DataWriter& in_dataWriter) const
{
    // Write bank data here 11
    in_dataWriter.WriteReal32(m_propertySet.GetReal32(in_guidPlatform, "Placeholder"));

    return true;
}

AK_WWISE_PLUGIN_GUI_WINDOWS_BEGIN_POPULATE_TABLE(MyAudiProperties)
AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(
    IDC_GAIN_SLIDER,
    "Placeholder"
    )

AK_WWISE_PLUGIN_GUI_WINDOWS_END_POPULATE_TABLE()

HINSTANCE MyAudiPlugin::GetResourceHandle() const
{
    return HINSTANCE();
}

bool MyAudiPlugin::GetDialog(
    AK::Wwise::Plugin::eDialog in_eDialog,
    UINT& out_uiDialogID,
    AK::Wwise::Plugin::PopulateTableItem*& out_pTable
) const
{
    switch (in_eDialog)
    {
    case AK::Wwise::Plugin::eDialog::SettingsDialog:
    {
        out_uiDialogID = IDD_PROPPAGE_LARGE;
        out_pTable = MyAudiProperties;
        return true;
    }
    case AK::Wwise::Plugin::eDialog::ContentsEditorDialog:
    default:
    {
        return false;
    }
    }
}


DEFINE_AUDIOPLUGIN_CONTAINER(MyAudi);											// Create a PluginContainer structure that contains the info for our plugin
EXPORT_AUDIOPLUGIN_CONTAINER(MyAudi);											// This is a DLL, we want to have a standardized name
ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(                                             // Add our CLI class to the PluginContainer
    MyAudi,        // Name of the plug-in container for this shared library
    MyAudiPlugin,  // Authoring plug-in class to add to the plug-in container
    MyAudiFX       // Corresponding Sound Engine plug-in class
);
DEFINE_PLUGIN_REGISTER_HOOK

DEFINEDUMMYASSERTHOOK;							// Placeholder assert hook for Wwise plug-ins using AKASSERT (cassert used by default)
