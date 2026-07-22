/* ========================================================================== */
/*                                                                            */
/*   httpclient.cpp                                                               */
/*   (c) 2012 Author                                                          */
/*                                                                            */
/*   Description                                                              */
/*                                                                            */
/* ========================================================================== */
//x86_64-w64-mingw32-g++ httpclient.cpp -o httpclient.exe -lwininet

#include <iostream>
#include <windows.h>
#include <wininet.h>
#include <vector>

int main() {
    // 1. Initialize WinINet
    HINTERNET hInternet = InternetOpenA(
        "MinGW-HTTP-Client", 
        INTERNET_OPEN_TYPE_DIRECT, 
        NULL, 
        NULL, 
        0
    );

    if (!hInternet) {
        std::cerr << "InternetOpenA failed. Error: " << GetLastError() << "\n";
        return 1;
    }

    // 2. Open the URL connection
    // Note: Use an "http" URL. For "https", you must pass INTERNET_FLAG_SECURE.
    const char* url = "http://google.com";
    HINTERNET hConnect = InternetOpenUrlA(
        hInternet, 
        url, 
        NULL, 
        0, 
        INTERNET_FLAG_RELOAD, 
        0
    );

    if (!hConnect) {
        std::cerr << "InternetOpenUrlA failed. Error: " << GetLastError() << "\n";
        InternetCloseHandle(hInternet);
        return 1;
    }

    // 3. Read the data
    std::vector<char> buffer(4096);
    DWORD bytesRead = 0;
    std::string responseData;

    std::cout << "Fetching data from " << url << "...\n\n";

    while (InternetReadFile(hConnect, buffer.data(), buffer.size() - 1, &bytesRead) && bytesRead > 0) {
        buffer[bytesRead] = '\0'; // Null-terminate the chunk
        responseData.append(buffer.data(), bytesRead);
    }

    // 4. Output the response content
    std::cout << responseData << "\n";

    // 5. Clean up handles
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return 0;
}
