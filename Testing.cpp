﻿#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string

#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui
#include <ftxui/screen/screen.hpp>        // for Full, Screen
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>


int main(void) {


    using namespace ftxui;

    auto ExitButton = Container::Horizontal({
      Button(
          "Exit Program", [&] { return 0; }, ButtonOption::Animated(Color::Red)),
    });

    // Define TitleScreen
    Element HomeScreenTitle =
        vbox({
            vbox({
                 text("    ______ __     ______ _____  __  __ _____  _  _____") | bold | color(Color::GreenLight),
                 text("   / ____// /    / ____// ___/ / / / //__  / ( )/ ___/") | bold | color(Color::GreenLight),
                 text("  / __/  / /    / __/   \\__ \\ / /_/ /   / /  |/ \\__ \\ ") | bold | color(Color::GreenLight),
                 text(" / /___ / /___ / /___  ___/ // __  /   / /__   ___/ / ") | bold | color(Color::GreenLight),
                 text("/_____//_____//_____/ /____//_/ /_/   /____/  /____/  ") | bold | color(Color::GreenLight),
            }),
            vbox({
                 text("   ______ ____ __  __ ____  ______ ____   ______ ____   ___     ____   __  ____  __") | bold | color(Color::GreenLight),
                 text("  / ____// __ \\\\ \\/ // __ \\/_  __// __ \\ / ____// __ \\ /   |   / __ \\ / / / /\\ \\/ /") | bold | color(Color::GreenLight),
                 text(" / /    / /_/ / \\  // /_/ / / /  / / / // / __ / /_/ // /| |  / /_/ // /_/ /  \\  / ") | bold | color(Color::GreenLight),
                 text("/ /___ / _, _/  / // ____/ / /  / /_/ // /_/ // _, _// ___ | / ____// __  /   / /  ") | bold | color(Color::GreenLight),
                 text("\\____//_/ |_|  /_//_/     /_/   \\____/ \\____//_/ |_|/_/  |_|/_/    /_/ /_/   /_/   ") | bold | color(Color::GreenLight),
            }),
            vbox({
                 text("                                                   ") | bold | color(Color::GreenLight),
                 text("  ______ __  __ ____ _   __ ______") | bold | color(Color::GreenLight),
                 text(" /_  __// / / //  _// | / // ____/") | bold | color(Color::GreenLight),
                 text("  / /  / /_/ / / / /  |/ // / __  ") | bold | color(Color::GreenLight),
                 text(" / /  / __  /_/ / / /|  // /_/ /  ") | bold | color(Color::GreenLight),
                 text("/_/  /_/ /_//___//_/ |_/ \\____/   ") | bold | color(Color::GreenLight),
            }),

         }) | border;

    

    auto TitleScreen = Screen::Create(
        Dimension::Fit(HomeScreenTitle),/*Width*/Dimension::Fit(HomeScreenTitle) // Height
    );

    Render(TitleScreen, HomeScreenTitle);
    TitleScreen.Print();


    return EXIT_SUCCESS;
}
