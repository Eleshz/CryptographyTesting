#include <memory>  // for shared_ptr, __shared_ptr_access
#include <string>  // for operator+, to_string
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <map>
#include <fstream>
#include <csignal>
#include <filesystem>
#include "EleshzAlgorithms.h"

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
std::string FileExist(std::string GivenName);


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

    std::cout << "\n\n\n" << QuickSearch("./", "Input.txt", 6) << "\n\n\n";

    using namespace ftxui;

    //clear();
    //Title();


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
    Component TitleTouchies = Container::Vertical({

    Button( // This button is for exiting the program
        "Exit Program?", [&] {


            exit(0);






        },

            ButtonOption::Animated(Color::Red)),
    Button( // This button is for scrambling the given text
        "Scramble now!", InterScreen.WithRestoredIO([&] {

         system("cls");

         EncodeStarter(MenuChoice[selected]);





        }),


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
                    text("Inside Key.txt") | color(Color::BlueViolet),
               }),

               separator(),
               vbox({
                    text("Use the key to decode the text"),
                    text("Either keep or put a new key into Key.txt") | color(Color::Red),
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

    std::string ResponseChecker;

    std::string Yes = "y"; //Lol why do I have to do this // Probably cause you're an idiot

    std::cout << EncodeSelection << "\n\n";
    std::cout << "Is this your selected? " << "(y/n)\n";

    getline(cin, ResponseChecker);

    if (!(ResponseChecker == "y")) {
        return "IDK TEST FAIL LOL";
    }
    else;

    std::cout << std::endl;
    //USE THIS FOR WAITING A CERTAIN AMOUNT OF TIME
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "Perfect...\n\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));


    try {

        std::string InputDirectory;
        std::string OutputDirectory;
        std::string KeyDirectory;

        std::string FileName;


        FileName = "Input.txt";
        InputDirectory = FileExist(FileName);
        std::cout << "I have found Input.txt here -> '" << InputDirectory << "'\n";
        std::cout << "-----------------------------------------\n";




        FileName = "Output.txt";
        OutputDirectory = FileExist(FileName);
        std::cout << "I have found Output.txt here -> '" << OutputDirectory << "'\n";
        std::cout << "-----------------------------------------\n";




        FileName = "Key.txt";
        KeyDirectory = FileExist(FileName);
        std::cout << "I have found Key.txt here -> '" << KeyDirectory << "'\n";
        std::cout << "-----------------------------------------\n";


    }
    catch (const std::exception& e) {
        // handle the exception
        std::cerr << "Error: " << e.what() << "\n";
    }

    system("pause");
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

std::string FileExist(std::string GivenName)
{
    namespace fs = std::filesystem;

    bool HasBeenFound = false;

    std::vector<std::string> FolderList{};
    std::vector<std::string> FileList;

    FolderList.push_back("./");

    while (true) {

        std::cout << "I will be checking the " << FolderList[0] << " folder now.\n\n\n\n";


            for (const auto& file : fs::directory_iterator(fs::path(FolderList[0]))) {

                if (fs::is_directory(file)) {

                    std::string Temp;

                    std::cout << file << " <--- Folder \n";

                    Temp = (file.path().string());

                    Temp = Temp.substr(Temp.rfind("\\") + 1);

                    FolderList.push_back(Temp);
                }
                else {
                    std::cout << file << " <--- File \n";
                    FileList.push_back(file.path().string());
                }
            }


            std::cout << "\n\n";

            for (const auto& File : FileList) {
                if (File == GivenName) {
                    return File + "/" + GivenName;
                }
                else {
                    std::cout << File << " Was not equal to: " << GivenName << "\n";
                }
            }

            std::cout << "\n" << FolderList.size() << "\n\n";
            std::cout << "\n" << FileList.size() << "\n\n";

            FolderList.erase(FolderList.begin());

            for (const auto& File : FolderList) {
                std::cout << File << "\n";
            }



            FileList.clear();
            FileList.shrink_to_fit();

            if (FolderList.empty()) {
                std::cout << "\n\nNo more folders!! Couldn't find!!!\n";
                return "Fail";
            }

    }
}
