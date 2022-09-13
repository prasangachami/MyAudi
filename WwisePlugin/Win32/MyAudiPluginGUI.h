#pragma once

#include "../MyAudiPlugin.h"
#include "../resource.h"

class MyAudiPluginGUI final
	: public AK::Wwise::Plugin::PluginMFCWindows<>
	, public AK::Wwise::Plugin::GUIWindows
	, public AK::Wwise::Plugin::RequestHost
	, public AK::Wwise::Plugin::RequestPropertySet
{
public:
	MyAudiPluginGUI();

	HINSTANCE GetResourceHandle() const override;

	virtual void NotifyPropertyChanged(
		const GUID & in_guidPlatform,
		const char* in_szPropertyName
	) override;

	bool GetDialog(
		AK::Wwise::Plugin::eDialog in_eDialog,
		UINT& out_uiDialogID,
		AK::Wwise::Plugin::PopulateTableItem*& out_pTable
	) const override;

	bool WindowProc(
		AK::Wwise::Plugin::eDialog in_eDialog,
		HWND in_hWnd,
		uint32_t in_message,
		WPARAM in_wParam,
		LPARAM in_lParam,
		LRESULT& out_iResult
	) override;

	private:
		void EnableFeedback();

		HWND m_hwndPropView = nullptr;
};
