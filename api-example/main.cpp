#include "ezcheats/api/API.h"
#include <iostream>

int main()
{
    setlocale(LC_ALL, "Russian");
    ezcheats::API apiClient("http://127.0.0.1:5000", "60859d3a6ab355d3234812bd", "qweqweqwe");
    try {
        UINT count = apiClient.onlineCount();
        std::cout << "online: " << count << "\n";
        apiClient.updateOnline();
        count = apiClient.onlineCount();
        std::cout << "online new: " << count << "\n";
        std::cout << apiClient.userTimeLeft() << "\n";
    } catch (std::runtime_error err) {
        std::cout << "Runtime_error: " << err.what() << "\n";
        std::cout << "Last error msg: " << apiClient.lastError() << "\n";
    }
}
