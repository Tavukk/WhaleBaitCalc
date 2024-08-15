#include <iostream>
#include <string>
#include <regex>
#include <D:\curl-8.9.1_1-win64-mingw\include\curl/curl.h>
#include <sstream>
#include <cmath> 
#include <conio.h>
#include <windows.h>


void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s) {
    size_t newLength = size * nmemb;
    try {
        s->append((char*)contents, newLength);
    } catch(std::bad_alloc &e) {
        return 0;  
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

    
    curl = curl_easy_init();
    if(curl) {
        
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        
        res = curl_easy_perform(curl);

        
        curl_easy_cleanup(curl);

        if(res == CURLE_OK) {
            
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

    std::string rawFishUrl = "https://skyblock.finance/items/RAW_FISH";
    std::string salmonUrl = "https://skyblock.finance/items/RAW_FISH%3A1";
    std::string goldUrl = "https://skyblock.finance/items/GOLD_INGOT";
    std::string prismarineUrl = "https://skyblock.finance/items/PRISMARINE_CRYSTALS";
    std::string inkUrl = "https://skyblock.finance/items/INK_SACK";
    std::string whaleBaitUrl = "https://skyblock.finance/items/WHALE_BAIT";


    double rawFishPrice = fetchData(rawFishUrl);
    double salmonPrice = fetchData(salmonUrl);
    double goldPrice = fetchData(goldUrl);
    double prismarinePrice = fetchData(prismarineUrl);
    double inkPrice = fetchData(inkUrl);
    double whaleBaitPrice = fetchData(whaleBaitUrl);


    SetColor(11); 
    std::cout << "Raw Fish Price: " << rawFishPrice << " coins" << std::endl;
    std::cout << "Salmon Price: " << salmonPrice << " coins" << std::endl;
    std::cout << "Gold Price: " << goldPrice << " coins" << std::endl;
    std::cout << "Prismarine Price: " << prismarinePrice << " coins" << std::endl;
    std::cout << "Ink Sack Price: " << inkPrice << " coins" << std::endl;
    SetColor(7);
    std::cout << "Whale Bait Price: " << whaleBaitPrice << " coins" << std::endl;
    
    SetColor(15);


    double costPerWhaleBait = (rawFishPrice * 5) + salmonPrice + (prismarinePrice * 3) + (goldPrice * 9) + inkPrice;
    long long costPerWhaleBaitLong = std::llround(costPerWhaleBait);
    SetColor(13);
    std::cout << "Cost to craft one Whale Bait: " << costPerWhaleBaitLong << " coins" << std::endl;
    SetColor(15);

    long long userMoney;
    SetColor(14);
    std::cout << "Enter the amount of money you have: ";
    std::cin >> userMoney;
    std::cout << "You have: " << userMoney << " coins" << std::endl;
    SetColor(15);


    int whaleBaitCount = static_cast<int>(std::floor(userMoney / costPerWhaleBaitLong));
    SetColor(1);
    std::cout << "You can craft " << whaleBaitCount << " Whale Bait." << std::endl;

    
    double requiredRawFish = whaleBaitCount * 5;
    double requiredSalmon = whaleBaitCount;
    double requiredPrismarineCrystals = whaleBaitCount * 3;
    double requiredGoldIngots = whaleBaitCount * 9;
    double requiredInkSacks = whaleBaitCount;

    
    double totalCostRawFish = requiredRawFish * rawFishPrice;
    double totalCostSalmon = requiredSalmon * salmonPrice;
    double totalCostPrismarineCrystals = requiredPrismarineCrystals * prismarinePrice;
    double totalCostGoldIngots = requiredGoldIngots * goldPrice;
    double totalCostInkSacks = requiredInkSacks * inkPrice;

    
    double totalCost = totalCostRawFish + totalCostSalmon + totalCostPrismarineCrystals + totalCostGoldIngots + totalCostInkSacks;

    
    long long totalCostLong = std::llround(totalCost);

    
    long long totalWhaleBaitCost = static_cast<long long>(whaleBaitCount * whaleBaitPrice);

    
    long long leftoverMoney = userMoney - totalCostLong;

    
    long long totalRevenue = whaleBaitCount * whaleBaitPrice;

    
    long long profit = leftoverMoney + totalRevenue - userMoney;

    
    SetColor(11);
    std::cout << "To craft " << whaleBaitCount << " Whale Bait items, you need:" << std::endl;
    std::cout << "  " << requiredRawFish << " Raw Fish" << std::endl;
    std::cout << "  " << requiredSalmon << " Salmon" << std::endl;
    std::cout << "  " << requiredPrismarineCrystals << " Prismarine Crystals" << std::endl;
    std::cout << "  " << requiredGoldIngots << " Gold Ingots" << std::endl;
    std::cout << "  " << requiredInkSacks << " Ink Sacks" << std::endl;
    SetColor(14);
    std::cout << "Total cost: " << totalCostLong << " coins" << std::endl;
    SetColor(12);
    std::cout << "Leftover money: " << leftoverMoney << " coins" << std::endl;
    std::cout << "Total revenue from selling Whale Bait items: " << totalRevenue << " coins" << std::endl;
    SetColor(10);
    std::cout << "Profit: " << profit << " coins" << std::endl;

    SetColor(15);

    
    std::cout << "Press any key to exit...";
    while (!_kbhit()) {
        
    }
    _getch(); 
    
    return 0;
}