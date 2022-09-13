
#include "../stdafx.h"
#include "MyAudiPluginGUI.h"
#include "../resource.h"

MyAudiPluginGUI::MyAudiPluginGUI()
{
    
}

//Bind non static text UI controls to properties for property view
AK_WWISE_PLUGIN_GUI_WINDOWS_BEGIN_POPULATE_TABLE(DelayProp)
    AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_CHECK_FEEDBACKENABLED, szFeedbackEnabled)
    AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_CHECK_PROCESSLFE, szProcessLFE)
    AK_WWISE_PLUGIN_GUI_WINDOWS_POP_ITEM(IDC_GAIN_SLIDER, szPlaceholder)
AK_WWISE_PLUGIN_GUI_WINDOWS_END_POPULATE_TABLE()

//Take necessary action on property changes.
//Note: user also has the option of catching appropriate message in WindowProc
void MyAudiPluginGUI::NotifyPropertyChanged(
    const GUID& in_guidPlatform,
    const char* in_szPropertyName
) {
    if (!strcmp(in_szPropertyName, szFeedbackEnabled))
    {
        if (m_hwndPropView)
            EnableFeedback();
    }
}

HINSTANCE MyAudiPluginGUI::GetResourceHandle() const
{
    AFX_MANAGE_STATE()(AfxGetStaticModuleState());
    return AfxGetStaticModuleState()->m_hCurrentResourceHandle;
}

bool MyAudiPluginGUI::GetDialog(AK::Wwise::Plugin::eDialog in_eDialog, UINT & out_uiDalogID, AK::Wwise::Plugin::PopulateTableItem*& out_pTable) const
{
    AKASSERT(in_eDialog == AK::Wwise::Plugin::SettingsDialog);

    out_uiDalogID = IDD_DELAY_BIG;
    out_pTable = DelayProp;

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

//Enable/Disable feedback slider based on feedback enable checkbox
void MyAudiPluginGUI::EnableFeedback() 
{
    auto feedbackEnabled = m_propertySet.GetBool(m_host.GetCurrentPlatform(), szFeedbackEnabled);
    HWND hwndItem = GetDlgItem(m_hwndPropView, IDC_RANGE_FEEDBACK);
    AKASSERT(hwndItem);
    ::EnableWindow(hwndItem, MKBOOL(feedbackEnabled));
    hwndItem = GetDlgItem(m_hwndPropView, IDC_STATIC_FEEDBACK);
    AKASSERT(hwndItem);
    ::EnableWindow(hwndItem, MKBOOL(feedbackEnabled));


}

ADD_AUDIOPLUGIN_CLASS_TO_CONTAINER(
    MyAudi,            // Name of the plug-in container for this shared library
    MyAudiPluginGUI,   // Authoring plug-in class to add to the plug-in container
    MyAudiFX           // Corresponding Sound Engine plug-in class
);
