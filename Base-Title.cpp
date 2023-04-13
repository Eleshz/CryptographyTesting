#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <vector>
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Button, Horizontal, Renderer
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/component/component_options.hpp"   // for ButtonOption
#include "ftxui/dom/elements.hpp"  // for separator, gauge, text, Element, operator|, vbox, border
#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui
#include <ftxui/screen/screen.hpp>        // for Full, Screen
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

std::string TitleText();
std::string TitleInteractives();

const std::vector<std::string> MenuChoice{ "ADFGVX cipher", "Affine Cipher", "Alberti Cipher", "Alphabet Cipher", "Alphabetum Kaldeorum", "Arnold Cipher", "Ary Numeration", "Atbash", "Autokey Cipher",
                                           "Bacon's Cipher", "Beaufort Cipher", "Bifid Cipher", "Book Cipher", "Thomas Brierley Grave Cipher",
                                           "Caesar Cipher", "Chaocipher", "Copiale Cipher",
                                           "DRYAD",
                                           "Four - square cipher",
                                           "Great Cipher", "Grille",
                                           "Hill Cipher",
                                           "M-94", "Mlecchita vikalpa",
                                           "Nihilist Cipher", "Null Cipher",
                                           "Pig Latin", "Pigpen Cipher", "Playfair Cipher", "Poem Code", "Polyalphabetic Cipher", "Polybius Square",
                                           "Rail Fence Cipher", "Rasterschlüssel 44", "Reihenschieber", "Reservehandverfahren", "ROT13", "Running Key Cipher",
                                           "Scytale", "Shackle Code" ,"Sheshach" ,"Straddling Checkerboard" ,"Substitution Cipher",
                                           "Tabula Recta", "Tap Code", "Transposition Cipher", "Trifid Cipher", "Two - Square Cipher",
                                           "VIC Cipher", "Vigenere Cipher",
                                           "Wadsworth's cipher", "Wahlwort"};

std::span choice{ MenuChoice.begin() , MenuChoice.size() };



int main() {

    using namespace ftxui;

    TitleText();
    TitleInteractives();

    return EXIT_SUCCESS;
}

std::string TitleText() {

    using namespace ftxui;

    Element HomeScreenTitle =
        vbox({
            vbox({
                 text("    ______ __     ______ _____  __  __ _____  _  _____") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text("   / ____// /    / ____// ___/ / / / //__  / ( )/ ___/") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text("  / __/  / /    / __/   \\__ \\ / /_/ /   / /  |/ \\__ \\ ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text(" / /___ / /___ / /___  ___/ // __  /   / /__   ___/ / ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text("/_____//_____//_____/ /____//_/ /_/   /____/  /____/  ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
            }),
            vbox({
                 text("   ______ ____ __  __ ____  ______ ____   ______ ____   ___     ____   __  ____  __") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text("  / ____// __ \\\\ \\/ // __ \\/_  __// __ \\ / ____// __ \\ /   |   / __ \\ / / / /\\ \\/ /") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text(" / /    / /_/ / \\  // /_/ / / /  / / / // / __ / /_/ // /| |  / /_/ // /_/ /  \\  / ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text("/ /___ / _, _/  / // ____/ / /  / /_/ // /_/ // _, _// ___ | / ____// __  /   / /  ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text("\\____//_/ |_|  /_//_/     /_/   \\____/ \\____//_/ |_|/_/  |_|/_/    /_/ /_/   /_/   ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
            }),
            vbox({
                 text("  ______ __  __ ____ _   __ ______") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text(" /_  __// / / //  _// | / // ____/") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text("  / /  / /_/ / / / /  |/ // / __  ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text(" / /  / __  /_/ / / /|  // /_/ /  ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
                 text("/_/  /_/ /_//___//_/ |_/ \\____/   ") | color(Color::GreenLight) | bgcolor(Color::Grey19),
            }),

            });

    Screen TitleScreen = Screen::Create(Dimension::Fixed(88), Dimension::Fixed(16));

    Render(TitleScreen, HomeScreenTitle);
    TitleScreen.Print();
    // std::cout << TitleScreen.ResetPosition(true);
    // USE THE ABOVE TO CLEAR A SCREEN PLEASE
    return "OK";
}

std::string TitleInteractives() {

    using namespace ftxui;

    int selected = 0;

    // Make the menu
    auto ChoiceMenu = Menu(&MenuChoice, &selected);

    // Put the stuff into the menu and send render
    auto ChoiceMenuRender = Renderer(ChoiceMenu, [&] {
        return vbox({
            ChoiceMenu->Render() | vscroll_indicator | frame |
            size(HEIGHT, LESS_THAN, 10) | borderStyled(HEAVY) });
        });

    // Make the buttons
    Component TitleButtonsCOM = Container::Vertical({
    Button(
        "Exit Program?", [&] { exit(0); }, ButtonOption::Animated(Color::Red)),
    Button(
        "Scramble now!", [&] { exit(0); }, ButtonOption::Animated(Color::GreenLight)),
        });

    // Put the stuff into the box(?) and send render 
    auto TitleButtonsBOX = Renderer(TitleButtonsCOM, [&] {
        return vbox({ hbox(text("Selected: "), text((MenuChoice[selected]))), TitleButtonsCOM->Render(),}) | borderStyled(HEAVY) | size(WIDTH, EQUAL, 40);
    });

    // Put all together into a component
    auto BothTogether = Container::Horizontal({ TitleButtonsBOX , ChoiceMenuRender});
    // Make new screen the size of the component
    auto InterScreen = ScreenInteractive::FitComponent();

    // Render screen
    InterScreen.Loop(BothTogether);

    return "OK";
}