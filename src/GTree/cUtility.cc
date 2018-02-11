//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <cUtility.h>
#include <vector>
#include <string>
#include <stdlib.h>

cUtility::cUtility() {
    // TODO Auto-generated constructor stub

}

cUtility::~cUtility() {
    // TODO Auto-generated destructor stub





}

std::string cUtility::getTestMsg() {

    return "This is a test message.";
}

std::vector<std::string> cUtility::split(const std::string& str,
        const char& ch) {


    std::string next;
    std::vector < std::string > result;

    // For each character in the string
    for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
        result.push_back(next);
    return result;

}

std::vector<int> cUtility::convertToInt(std::vector<std::string> vStr) {

    //std::vector<std::string> DataNumbers;
    // Fill DataNumbers
    std::vector<int> intNumbers;
    for (int i = 0; i < vStr.size(); i++) {
        int num = atoi(vStr.at(i).c_str());
        intNumbers.push_back(num);
    }

    return intNumbers;
}


/*std::vector<string> cUtility::split(const string& str, const char& ch) {

    string next;
    vector < string > result;

    // For each character in the string
    for (string::const_iterator it = str.begin(); it != str.end(); it++) {
        // If we've hit the terminal character
        if (*it == ch) {
            // If we have some characters accumulated
            if (!next.empty()) {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        } else {
            // Accumulate the next character into the sequence
            next += *it;
        }
    }
    if (!next.empty())
        result.push_back(next);
    return result;
}*/


//std::string cUtility::split() {
//}
