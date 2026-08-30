#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// RC4 function
// RC4 uses the same operation for encryption and decryption.
vector<unsigned char> rc4(
    const vector<unsigned char>& data,
    const string& key
) {
    unsigned char S[256];

    // Initialize the S array
    for (int i = 0; i < 256; i++) {
        S[i] = static_cast<unsigned char>(i);
    }

    // Key Scheduling Algorithm (KSA)
    int j = 0;

    for (int i = 0; i < 256; i++) {
        j = (j + S[i] +
             static_cast<unsigned char>(
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

        // XOR encrypted byte with RC4 keystream
        result.push_back(byte ^ keystreamByte);
    }

    return result;
}

int main() {

    const string filename = "file.txt";

    // Must be exactly the same key used by the encryptor
    const string key = "MySecretKey123";

    // Open the encrypted file in binary mode
    ifstream inputFile(filename, ios::binary);

    if (!inputFile) {
        cerr << "Error: Could not open "
             << filename << endl;
        return 1;
    }

    // Read encrypted file into memory
    vector<unsigned char> encryptedData(
        (istreambuf_iterator<char>(inputFile)),
        istreambuf_iterator<char>()
    );

    inputFile.close();

    cout << "Encrypted file size: "
         << encryptedData.size()
         << " bytes" << endl;

    // Decrypt using RC4
    vector<unsigned char> decryptedData =
        rc4(encryptedData, key);

    // Write decrypted data back to file
    ofstream outputFile(
        filename,
        ios::binary | ios::trunc
    );

    if (!outputFile) {
        cerr << "Error: Could not write to "
             << filename << endl;
        return 1;
    }

    outputFile.write(
        reinterpret_cast<const char*>(
            decryptedData.data()
        ),
        decryptedData.size()
    );

    outputFile.close();

    cout << "File decrypted successfully." << endl;

    return 0;
}
