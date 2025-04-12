#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <X11/Xlib.h>

constexpr int _width = 2560;
constexpr int _height = 1414;

struct GameWindow
{
    pid_t pid;
    std::string name;
    std::string window_id;
};

std::string get_process_name(pid_t pid)
{
    std::ifstream cmdline("/proc/" + std::to_string(pid) + "/comm");
    std::string name;
    std::getline(cmdline, name);
    return name;
}

std::vector<GameWindow> list_game_windows()
{
    std::vector<GameWindow> games;

    FILE* fp = popen("wmctrl -l", "r");
    if (!fp)
    {
        std::cerr << "wmctrl execution failed.\n";
        return games;
    }
    std::cout << "wmctrl execution success.\n";

    char buffer[512];
    while (fgets(buffer, sizeof(buffer), fp)) {
        pid_t pid;
        std::string name;
        std::string window_id;

        std::stringstream ss(buffer);
        ss >> window_id;
        ss >> pid;
        std::getline(ss, name);

        if (window_id != "0") {
            games.push_back({pid, name, window_id});
        }
    }
    if (pclose(fp) != 0)
    {
        std::cerr << "Failed to close FD for wmctrl.\n";
    }

    return games;
}

void resize_and_center_window(const std::string& window_id, int width, int height)
{
    Display* display = XOpenDisplay(nullptr);
    if (!display)
    {
        std::cerr << "Unable to open x11 and aquire display\n";
        return;
    }

    Screen* screen = DefaultScreenOfDisplay(display);
    int screen_width = screen->width;
    int screen_height = screen->height;

    int x = (screen_width - width) / 2;
    int y = (screen_height - height) / 2;

    std::string command_unmaximize = "wmctrl -i -r " + window_id + " -b remove,maximized_vert,maximized_horz";
    if (system(command_unmaximize.c_str()))
    {
        std::cerr << std::endl;
    }

    std::string command = "wmctrl -i -r " + window_id + " -e 0," + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(width) + "," + std::to_string(height);
    if (system(command.c_str()) != 0)
    {
        std::cerr << "Resize command has failed.\n";
    }
    XCloseDisplay(display);
}

void remove_headerbar(const std::string &window_id)
{
    std::string command = "xprop -id " + window_id + " -f _MOTIF_WM_HINTS 32c -set _MOTIF_WM_HINTS \"0x2, 0x0, 0x0, 0x0, 0x0\"";
    if(system(command.c_str()) != 0)
    {
        std::cerr << "(headerbar) couldnt be removed.\n";
    }
    else
    {
        std::cout << "(headerbar) removed with success " << window_id << ".\n";
    }
}

int main() {
    std::vector<GameWindow> games = list_game_windows();
    if (games.empty()) {
        std::cout << "No game window found.\n";
        return 0;
    }

    std::cout << "Window detected :\n";
    for (size_t i = 0; i < games.size(); ++i) {
        std::cout << i << ") [" << games[i].pid << "] " << games[i].name << "\n";
    }

    std::cout << "Please select the window id : ";
    int choice;
    std::cin >> choice;

    if (choice < 0 || static_cast<size_t>(choice) >= games.size())
    {
        std::cerr << "Invalid choice.\n";
        return 1;
    }

    int width = 0, height = 0;
    std::cout << "New width (default = " << _width << ") : ";
    std::cin >> width;
    std::cout << "New height (default = " << _height << ") : ";
    std::cin >> height;

    if (width <= 100) { width = _width; }
    if (height <= 100) { height = _height; }

    resize_and_center_window(games[choice].window_id, width, height);
    remove_headerbar(games[choice].window_id);
    std::cout << "Window resized and moved !\n";

    return 0;
}
