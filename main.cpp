/*
    Eluvio Take Home Coding Challenge
    @author Ming Cheng
    @date 4/9/2021
*/
#include <bits/stdc++.h>
using namespace std;


typedef unsigned char BYTE;

int FILE_NUM;
// hash maps that store local/global <file#, [indexes]> pairs
unordered_map<int, set<int>> global, local;

/*
    A memory optimized DP method that finds the longest common substring between 2 strings.
    @param 2 strings that represents bytes in two files, and index of the files. (simple.*)
    @return longest common substring of 2 strings.
*/
int longest_between_two(string a, string b, int aInd, int bInd)
{
    int maxLen = 0;
    local.clear();

    int m = a.length(), n = b.length(), curRow = 0;
    vector<vector<int>> dp(2, vector<int>(n+1, 0));

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (a[i-1] == b[j-1])
                dp[curRow][j] = 1 + dp[1-curRow][j-1];

            if (dp[curRow][j] > maxLen)
                    local.clear();

            if (dp[curRow][j] > 0 && dp[curRow][j] >= maxLen) {
                int aStart = i + 1 - dp[curRow][j], bStart = j + 1 - dp[curRow][j];
                local[aInd].insert(aStart);
                local[bInd].insert(bStart);
                maxLen = dp[curRow][j];
            }
        }
        curRow = 1 - curRow;
    }
    return maxLen;
}

/*
    Helper function that finds the length of the longest common substring among all files.
    @param vector of files, whose byte data has been converted to strings, and the number of files we have.
    @return longest common substring of all strings, which appears in at least 2 files.
*/
int longest_between_n(vector<string>& files, int n)
{
    int maxLen = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = i+1; j <= n; j++) {
            int curLen = longest_between_two(files[i-1], files[j-1], i, j);

            if (curLen > maxLen)
                    global.clear();

            if (curLen > 0 && curLen >= maxLen) {
                for (int i = 1; i <= FILE_NUM; i++) {
                    for (auto pos: local[i]) {
                        global[i].insert(pos);
                    }
                }
                maxLen = curLen;
            }
        }
    }
    return maxLen;
}

/*
    Read data from files and convert byte strands to strings. one byte is one unsigned char in the string.
    @param number of files we need to process
    @return vector of strings representing byte strands in files.
*/
vector<string> readFile(const int n)
{
    vector<string> files;
    for (int i = 1; i <= n; i++) {
        string fileName = "sample." + to_string(i);
        streampos fileSize;
        ifstream file(fileName, ios::binary);

        file.seekg(0, ios::end);
        fileSize = file.tellg();
        file.seekg(0, ios::beg);

        vector<BYTE> fileData(fileSize);
        file.read((char*) &fileData[0], fileSize);

        string file_data_str(fileData.begin(), fileData.end());
        files.push_back(file_data_str);
    }
    return files;
}

/*
    Driver function
*/
int main()
{
    FILE_NUM = 10;

    vector<string> files = readFile(FILE_NUM);
    int maxLen = longest_between_n(files, FILE_NUM);

    cout << "Max Length of Byte Strand: " << maxLen << endl;
    for (int i = 1; i <= FILE_NUM; i++) {
        if (global[i].empty()) continue;
        cout << "Start Positions of File #" << i << ": ";
        for (auto pos: global[i])
            cout << pos << " ";
        cout << endl;
    }

    return 0;
}
