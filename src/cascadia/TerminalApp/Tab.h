// Copyright (c) Microsoft Corporation.
// Licensed under the MIT license.

#pragma once
#include "Pane.h"
#include "ColorPickupFlyout.h"
#include "Tab.g.h"

// fwdecl unittest classes
namespace TerminalAppLocalTests
{
    class TabTests;
};

namespace winrt::TerminalApp::implementation
{
    struct Tab : public TabT<Tab>
    {
    public:
        Tab() = delete;
        Tab(const GUID& profile, const Microsoft::Terminal::TerminalControl::TermControl& control);

        // Called after construction to perform the necessary setup, which relies on weak_ptr
        void Initialize(const Microsoft::Terminal::TerminalControl::TermControl& control);

        Microsoft::UI::Xaml::Controls::TabViewItem GetTabViewItem();
        Windows::UI::Xaml::UIElement GetRootElement();
        Microsoft::Terminal::TerminalControl::TermControl GetActiveTerminalControl() const;
        std::optional<GUID> GetFocusedProfile() const noexcept;

        bool IsFocused() const noexcept;
        void SetFocused(const bool focused);

        winrt::fire_and_forget Scroll(const int delta);

        bool CanSplitPane(Microsoft::Terminal::Settings::SplitState splitType);
        void SplitPane(Microsoft::Terminal::Settings::SplitState splitType, const GUID& profile, Microsoft::Terminal::TerminalControl::TermControl& control);

        winrt::fire_and_forget UpdateIcon(const winrt::hstring iconPath);

        float CalcSnappedDimension(const bool widthOrHeight, const float dimension) const;
        Microsoft::Terminal::Settings::SplitState PreCalculateAutoSplit(Windows::Foundation::Size rootSize) const;

        void ResizeContent(const Windows::Foundation::Size& newSize);
        void ResizePane(const Microsoft::Terminal::Settings::Direction& direction);
        void NavigateFocus(const Microsoft::Terminal::Settings::Direction& direction);

        void UpdateSettings(const Microsoft::Terminal::Settings::TerminalSettings& settings, const GUID& profile);
        winrt::hstring GetActiveTitle() const;
        winrt::fire_and_forget SetTabText(const winrt::hstring text);

        void Shutdown();
        void ClosePane();

        std::optional<Windows::UI::Color> GetTabColor();

        WINRT_CALLBACK(Closed, Windows::Foundation::EventHandler<Windows::Foundation::IInspectable>);
        WINRT_CALLBACK(PropertyChanged, Windows::UI::Xaml::Data::PropertyChangedEventHandler);
        DECLARE_EVENT(ActivePaneChanged, _ActivePaneChangedHandlers, winrt::delegate<>);
        DECLARE_EVENT(ColorSelected, _colorSelected, winrt::delegate<Windows::UI::Color>);
        DECLARE_EVENT(ColorCleared, _colorCleared, winrt::delegate<>);

        OBSERVABLE_GETSET_PROPERTY(winrt::hstring, Title, _PropertyChangedHandlers);
        OBSERVABLE_GETSET_PROPERTY(winrt::hstring, IconPath, _PropertyChangedHandlers);

    private:
        std::shared_ptr<Pane> _rootPane{ nullptr };
        std::shared_ptr<Pane> _activePane{ nullptr };
        winrt::hstring _lastIconPath{};
        TerminalApp::ColorPickupFlyout _tabColorPickup{};
        std::optional<Windows::UI::Color> _tabColor{};

        bool _focused{ false };
        Microsoft::UI::Xaml::Controls::TabViewItem _tabViewItem{ nullptr };

        void _MakeTabViewItem();
        void _Focus();

        void _CreateContextMenu();
        void _SetTabColor(const Windows::UI::Color& color);
        void _ResetTabColor();
        void _RefreshVisualState();

        void _BindEventHandlers(const Microsoft::Terminal::TerminalControl::TermControl& control) noexcept;

        void _AttachEventHandlersToControl(const Microsoft::Terminal::TerminalControl::TermControl& control);
        void _AttachEventHandlersToPane(std::shared_ptr<Pane> pane);

        int _GetLeafPaneCount() const noexcept;
        void _UpdateActivePane(std::shared_ptr<Pane> pane);

        friend class ::TerminalAppLocalTests::TabTests;
    };
}
