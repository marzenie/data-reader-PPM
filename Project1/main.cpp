#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <locale.h>

using namespace std;

bool isComment(string output) {

    return !output.empty() && output[0] == '#';

}
string getRGB(int pixel) {
    int R = (pixel >> 16) & 0xFF;
    int G = (pixel >> 8) & 0xFF;
    int B = pixel & 0xFF;

    return "(" + to_string(R) + ", " + to_string(G) + ", " + to_string(B) + ")";
}
int main() {

    setlocale(LC_CTYPE, "Polish");

    string name_ext, output;
    vector<int> info;
    map<string, int> data;
    /*
    Info vector:
    0 - type (Without letter P)
    1 - width
    2 - height
    3 - (PPM & PGM) color depth

        P1,P4  - PBM (0-1)
        P2,P5  - PGM (0-255)
        P3,P6  - PPM (RGB)

    */

    std::cout << "Podaj nazwê pliku ( z rozszerzeniem ): ";
    cin >> name_ext;

    fstream file_read;
    file_read.open(name_ext, ios::in);

    if (!file_read.is_open()) {
        std::cout << "Nie mo¿na otworzyæ pliku\n";
        return 1;
    }

    int i = 0;
    int color = 0;
    int limit;

    while (getline(file_read, output)) {
        
        if (isComment(output)) continue;

        if (i == 0) {
            info.push_back(stoi(output.substr(1)));
            limit = ((info[0] == 1) || (info[0] == 4)) ? 2 : 3;
            i++;
            continue;
        }

        stringstream values(output);
        string value;

        while (values >> value) {
            if (limit >= i) {
                info.push_back(stoi(value));
                i++;
                continue;
            }

            if (!((info[0] == 3) || (info[0] == 6))) {
                data[value]++;
                continue;
            }

            int val_int = stoi(value);
            if (i == limit + 1) {
                color = (val_int << 16);
            } else if (i == limit + 2) {
                color += (val_int << 8);
            } else if (i == limit + 3) {
                color += val_int;
                data[to_string(color)]++;
                i = limit + 1; //zerowanie i do startowego punktu dla kolejnej liczby
                continue;
            }

            i++;
            /*
                4 bit -  EMPTY BIT / PLANS: SPEC FUNTION like status
                3 bit -  R
                2 bit -  G
                1 bit -  B
            */

        }


    }

    int maxValue = numeric_limits<int>::min(); // Initialize with smallest possible int value
    int unique_colors = 0;
    string maxKey;
    for (const auto& pair : data) {
        unique_colors++;
        if (pair.second > maxValue) {
            maxValue = pair.second;
            maxKey = pair.first;
        }
    }
    std::cout << "Szerokoœæ: " << info[1] << "\n";
    std::cout << "Wysokoœæ: " << info[2] << "\n";
    std::cout << "Najczêœciej wystepujacy kolor to: " << (((info[0] == 3) || ((info[0] == 6))) ? getRGB(stoi(maxKey)) : maxKey) << " i wyst¹pi³ " << maxValue << " razy" << "\n";
    std::cout << "Liczba unikalnych kolorow:  " << unique_colors << "\n";
}