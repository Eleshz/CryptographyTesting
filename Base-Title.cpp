#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <map>
#include <fstream>
#include <csignal>

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
using namespace std;

bool ContinueRender = true;

std::string Title();
void clear();


std::string EncodeStarter(std::string EncodeSelection);

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
                                           "Wadsworth's cipher", "Wahlwort" }; // List of all the ciphers that can be used, used for the menu component.

std::span choice{ MenuChoice.begin() , MenuChoice.size() };




int main() {

    using namespace ftxui;

    Title();


    return EXIT_SUCCESS;
}

std::string Title() {

    using namespace ftxui;

    auto InterScreen = ScreenInteractive::TerminalOutput();

    int selected = 0;

    auto Title = Renderer([&] {
        return vbox({

               vbox({
                 text("    ______ __     ______ _____  __  __ _____  _  _____") | color(Color::GreenLight),
                 text("   / ____// /    / ____// ___/ / / / //__  / ( )/ ___/") | color(Color::GreenLight),
                 text("  / __/  / /    / __/   \\__ \\ / /_/ /   / /  |/ \\__ \\ ") | color(Color::GreenLight),
                 text(" / /___ / /___ / /___  ___/ // __  /   / /__   ___/ / ") | color(Color::GreenLight),
                 text("/_____//_____//_____/ /____//_/ /_/   /____/  /____/  ") | color(Color::GreenLight),
            }),
            vbox({
                 text("   ______ ____ __  __ ____  ______ ____   ______ ____   ___     ____   __  ____  __") | color(Color::GreenLight),
                 text("  / ____// __ \\\\ \\/ // __ \\/_  __// __ \\ / ____// __ \\ /   |   / __ \\ / / / /\\ \\/ /") | color(Color::GreenLight),
                 text(" / /    / /_/ / \\  // /_/ / / /  / / / // / __ / /_/ // /| |  / /_/ // /_/ /  \\  / ") | color(Color::GreenLight),
                 text("/ /___ / _, _/  / // ____/ / /  / /_/ // /_/ // _, _// ___ | / ____// __  /   / /  ") | color(Color::GreenLight),
                 text("\\____//_/ |_|  /_//_/     /_/   \\____/ \\____//_/ |_|/_/  |_|/_/    /_/ /_/   /_/   ") | color(Color::GreenLight),
            }),
            vbox({
                 text("  ______ __  __ ____ _   __ ______") | color(Color::GreenLight),
                 text(" /_  __// / / //  _// | / // ____/") | color(Color::GreenLight),
                 text("  / /  / /_/ / / / /  |/ // / __  (Click a blank area before clicking any buttons lol)") | color(Color::GreenLight),
                 text(" / /  / __  /_/ / / /|  // /_/ /  ") | color(Color::GreenLight),
                 text("/_/  /_/ /_//___//_/ |_/ \\____/   ") | color(Color::GreenLight),
            }) });
        });

    // Make the menu
    auto ChoiceMenu = Menu(&MenuChoice, &selected);

    // Put the stuff into the menu and send render
    auto ChoiceMenuRender = Renderer(ChoiceMenu, [&] {
        return vbox({
            ChoiceMenu->Render()

            | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 10) | borderStyled(HEAVY) });
        }) | Maybe(&ContinueRender);


    // Make the buttons
    Component TitleTouchies = Container::Vertical({ Button(
        "Exit Program?", [&] {
            exit(0);






        },

            ButtonOption::Animated(Color::Red)),
    Button(
        "Scramble now!", [&] {

         std::string Test;


         ContinueRender = false;
            
         raise(SIGINT);


           

         system("cls");


         EncodeStarter(MenuChoice[selected]);



         std::cout << "HAS THIS WORKED???" << endl;
         std::cin >> Test;
         std::cout << Test;

        },


            ButtonOption::Animated(Color::GreenLight)),
     Button(
        "Decode", [&] { exit(0); return; },
         
            ButtonOption::Animated(Color::Blue3)),
        }) | Maybe(&ContinueRender);


    // Put the stuff into the box(?) and send render 
    auto TitleButtonsBOX = Renderer(TitleTouchies, [&] {
        return vbox({ hbox(text("Selected: "), text((MenuChoice[selected]))),

        TitleTouchies->Render(), }) | borderStyled(HEAVY) | size(WIDTH, EQUAL, 40);
        });

    auto Instructions = Renderer([&] {
        return vbox({

               vbox({
                    text("Paste/Write what you want to encode into the"),
                    text("Input.txt file") | color(Color::YellowLight),
               }),

               separator(),
               vbox({
                    text("Then get your encoded text in the"),
                    text("Output.txt file") | color(Color::YellowLight),
               }),

               separator(),
               vbox({
                    text("You will be provided with a key"),
                    text("At the bottom of the text") | color(Color::BlueViolet),
               }),

               separator(),
               vbox({
                    text("Use the key to decode the text"),
                    text("Put it into the Key.txt file") | color(Color::Red),
               }),

            }) | borderDouble ;
        });

    

    // Put all together into a component
    

    auto BothTogether = Container::Horizontal({ TitleButtonsBOX , ChoiceMenuRender, Instructions }) | Maybe(&ContinueRender); // Puts everything together into one component that can be rendered

    auto FinalTitle = Container::Vertical({ BothTogether, Title }) | Maybe(&ContinueRender);
    // Make new screen the size of the component

   

    InterScreen.Loop(FinalTitle);

    

    return "OK";
}

std::string EncodeStarter(std::string EncodeSelection) {


    



    return "OK";
}

void clear() { // Clear the screen

    cin.sync();
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}