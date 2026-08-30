#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// RC4 encryption function
vector<unsigned char> rc4(
    const vector<unsigned char>& data,
    const string& key
) {
    unsigned char S[256];

    // Initialize S
    for (int i = 0; i < 256; i++) {
        S[i] = static_cast<unsigned char>(i);
    }

    // Key Scheduling Algorithm (KSA)
    int j = 0;

    for (int i = 0; i < 256; i++) {
        j = (j + S[i] + static_cast<unsigned char>(
            key[i % key.length()]
        )) % 256;

        swap(S[i], S[j]);
    }

    // Pseudo-Random Generation Algorithm (PRGA)
    vector<unsigned char> result;
    result.reserve(data.size());

    int i = 0;
    j = 0;

    for (unsigned char byte : data) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        swap(S[i], S[j]);

        int index = (S[i] + S[j]) % 256;
        unsigned char keystreamByte = S[index];

        // XOR data with RC4 keystream
        result.push_back(byte ^ keystreamByte);
    }

    return result;
}

int main() {

    const string filename = "file.txt";
    const string key = "MySecretKey123";

    // Open file in binary mode
    ifstream inputFile(filename, ios::binary);

    if (!inputFile) {
        cerr << "Error: Could not open " << filename << endl;
        return 1;
    }

    // Read file contents into memory
    vector<unsigned char> data(
        (istreambuf_iterator<char>(inputFile)),
        istreambuf_iterator<char>()
    );

    inputFile.close();

    cout << "Original file size: "
         << data.size() << " bytes" << endl;

    // Encrypt data using RC4
    vector<unsigned char> encryptedData = rc4(data, key);

    // Write encrypted data back to file
    ofstream outputFile(
        filename,
        ios::binary | ios::trunc
    );

    if (!outputFile) {
        cerr << "Error: Could not write to " << filename << endl;
        return 1;
    }

    outputFile.write(
        reinterpret_cast<const char*>(encryptedData.data()),
        encryptedData.size()
    );

    outputFile.close();

    cout << "File encrypted successfully." << endl;

    return 0;
}
