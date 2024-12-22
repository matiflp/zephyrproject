
#include <iostream>
#include <string>
using namespace std;

int main(void)
{
    string board = string(CONFIG_BOARD);

    if (board == "native_posix") {
        cout << "The application is running on the native_posix_64 host board\n";
    } else {
        cout << "Running on " << CONFIG_BOARD << " host board.\n";
    }

    return 0;
}