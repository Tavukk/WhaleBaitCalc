#include <iostream>
#include <string>
#include <regex>
#include <D:\curl-8.9.1_1-win64-mingw\include\curl/curl.h>
#include <sstream>
#include <cmath> // Include for rounding functions

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        return 0;  // handle memory problem
    }
    return newLength;
}

std::string convertKToNumber(const std::string& str) {
    size_t kPos = str.find('k');
    if (kPos != std::string::npos) {
        double value = std::stod(str.substr(0, kPos));
        std::ostringstream oss;
        oss << value * 1000;
        return oss.str();
    }
    return str;
}

double fetchData(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Initialize curl
    curl = curl_easy_init();
    if(curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Disable SSL verification for testing (not recommended for production)
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // Specify the write callback function and the string to store the response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);

        // Clean up curl
        curl_easy_cleanup(curl);

        if(res == CURLE_OK) {
            // Locate the first instance of the specific class containing the price
            std::regex pattern("<div class=\"LabelValue_value__5rNfm\">(.*?)</div>");
            std::smatch matches;

            if (std::regex_search(readBuffer, matches, pattern)) {
                std::string content = matches[1].str();
                if (content.find("coins") != std::string::npos) {
                    content.erase(content.find_last_not_of(" \n\r\t") + 1);
                    std::string convertedContent = convertKToNumber(content);
                    return std::stod(convertedContent);
                }
            }
        } else {
            std::cout << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return 0.0;
}

int main() {
    // URLs for each item
    std::string rawFishUrl = "https://skyblock.finance/items/RAW_FISH";
    std::string salmonUrl = "https://skyblock.finance/items/RAW_FISH%3A1";
    std::string goldUrl = "https://skyblock.finance/items/GOLD_INGOT";
    std::string prismarineUrl = "https://skyblock.finance/items/PRISMARINE_CRYSTALS";
    std::string inkUrl = "https://skyblock.finance/items/INK_SACK";
    std::string whaleBaitUrl = "https://skyblock.finance/items/WHALE_BAIT";

    // Fetch and store prices
    double rawFishPrice = fetchData(rawFishUrl);
    double salmonPrice = fetchData(salmonUrl);
    double goldPrice = fetchData(goldUrl);
    double prismarinePrice = fetchData(prismarineUrl);
    double inkPrice = fetchData(inkUrl);
    double whaleBaitPrice = fetchData(whaleBaitUrl);

    // Output the prices
    std::cout << "Raw Fish Price: " << rawFishPrice << " coins" << std::endl;
    std::cout << "Salmon Price: " << salmonPrice << " coins" << std::endl;
    std::cout << "Gold Price: " << goldPrice << " coins" << std::endl;
    std::cout << "Prismarine Price: " << prismarinePrice << " coins" << std::endl;
    std::cout << "Ink Sack Price: " << inkPrice << " coins" << std::endl;
    std::cout << "Whale Bait Price: " << whaleBaitPrice << " coins" << std::endl;

    // Ask the user for the amount of money they have
    long long userMoney;
    std::cout << "Enter the amount of money you have: ";
    std::cin >> userMoney;
    std::cout << "You have: " << userMoney << " coins" << std::endl;

    // Calculate how many Whale Bait items the user can craft
    int whaleBaitCount = static_cast<int>(std::floor(userMoney / whaleBaitPrice));
    std::cout << "You can craft " << whaleBaitCount << " Whale Bait." << std::endl;

    return 0;
}
