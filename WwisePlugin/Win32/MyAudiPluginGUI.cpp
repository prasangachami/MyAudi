
#include "MyAudiPluginGUI.h"

MyAudiPluginGUI::MyAudiPluginGUI()
{
    
}

HINSTANCE MyAudiPluginGUI::GetResourceHandle() const
{
    AFX_MANAGE_STATE()(AfxGetStaticModuleState());
    return AfxGetStaticModuleState()->m_hCurrentResourceHandle;
}

bool MyAudiPluginGUI::GetDialog(AK::Wwise::Plugin::eDialog in_eDialog, UINT & out_uiDalogID, AK::Wwise::Plugin::PopulateTableItem*& out_pTable) const
{
    AKASSERT(in_eDialog == AK::Wwise::Plugin::SettingsDialog);

    out_uiDalogID = IDD_PROPPAGE_LARGE;
    out_pTable = nullptr;

    return true;

}

bool MyAudiPluginGUI::WindowProc(
    AK::Wwise::Plugin::eDialog in_eDialog,
    HWND in_hWnd,
    uint32_t in_message,
    WPARAM in_wParam,
    LPARAM in_lParam,
    LRESULT& out_iResult)
{
    switch (in_message)
    {
        case WM_INITDIALOG:
            m_hwndPropView = in_hWnd;
            break;
        case WM_DESTROY:
            m_hwndPropView = NULL;
            break;
    }

    out_iResult = 0;
    return false;
}

ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
    MyAudi,            // Name of the plug-in container for this shared library
    MyAudiPluginGUI,   // Authoring plug-in class to add to the plug-in container
    MyAudiFX           // Corresponding Sound Engine plug-in class
);
