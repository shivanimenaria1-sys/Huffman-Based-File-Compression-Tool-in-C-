// ============================================================
//  Huffman Coding – File Compression & Decompression Tool
//  A beginner-friendly, single-file C++ implementation.
//  Uses only the C++ Standard Library (STL).
// ============================================================

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

// ────────────────────────────────────────────────────────────
// STEP 4 – Node structure for the Huffman Tree
// ────────────────────────────────────────────────────────────
struct Node {
    char character;    // The character stored in this node
    int  frequency;    // How often the character appears
    Node* left;        // Left child  (represents '0')
    Node* right;       // Right child (represents '1')

    // Constructor for a leaf node (holds a real character)
    Node(char ch, int freq)
        : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

    // Constructor for an internal node (merges two children)
    Node(int freq, Node* l, Node* r)
        : character('\0'), frequency(freq), left(l), right(r) {}
};

// ────────────────────────────────────────────────────────────
// Comparator so the priority_queue acts as a MIN-heap
// (lowest frequency on top).
// ────────────────────────────────────────────────────────────
struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;   // smaller freq = higher priority
    }
};

// ────────────────────────────────────────────────────────────
// STEP 3 – Build a frequency map from the input text
// ────────────────────────────────────────────────────────────
unordered_map<char, int> buildFrequencyMap(const string& text) {
    unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }
    return freqMap;
}

// ────────────────────────────────────────────────────────────
// STEP 5 – Build the Huffman Tree using a min-heap
// ────────────────────────────────────────────────────────────
Node* buildHuffmanTree(const unordered_map<char, int>& freqMap) {
    // Create a min-heap (priority queue)
    priority_queue<Node*, vector<Node*>, Compare> minHeap;

    // Push every character as a leaf node into the heap
    for (auto& pair : freqMap) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    // Edge case: if only one unique character, add a dummy node
    // so the tree still has depth and we can assign a code.
    if (minHeap.size() == 1) {
        minHeap.push(new Node('\0', 0));
    }

    // Repeatedly combine the two lowest-frequency nodes
    while (minHeap.size() > 1) {
        Node* left  = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        // Create an internal node whose frequency is the sum
        Node* merged = new Node(left->frequency + right->frequency, left, right);
        minHeap.push(merged);
    }

    // The last remaining node is the root of the Huffman Tree
    return minHeap.top();
}

// ────────────────────────────────────────────────────────────
// STEP 6 – Generate Huffman codes via DFS traversal
//   Left edge  → append "0"
//   Right edge → append "1"
// ────────────────────────────────────────────────────────────
void generateCodes(Node* root, const string& currentCode,
                   unordered_map<char, string>& huffmanCodes) {
    if (!root) return;

    // If this is a leaf node, store its code
    if (!root->left && !root->right) {
        huffmanCodes[root->character] = currentCode;
        return;
    }

    // Recurse left (add '0') and right (add '1')
    generateCodes(root->left,  currentCode + "0", huffmanCodes);
    generateCodes(root->right, currentCode + "1", huffmanCodes);
}

// ────────────────────────────────────────────────────────────
// STEP 7 – Encode the text using the Huffman codes
// ────────────────────────────────────────────────────────────
string encode(const string& text, const unordered_map<char, string>& huffmanCodes) {
    string encoded;
    for (char ch : text) {
        encoded += huffmanCodes.at(ch);
    }
    return encoded;
}

// ────────────────────────────────────────────────────────────
// STEP 9 – Decode the encoded string using the Huffman Tree
// ────────────────────────────────────────────────────────────
string decode(const string& encodedText, Node* root) {
    string decoded;
    Node* current = root;

    for (char bit : encodedText) {
        // Walk left on '0', right on '1'
        if (bit == '0') {
            current = current->left;
        } else {
            current = current->right;
        }

        // Reached a leaf → found a character
        if (!current->left && !current->right) {
            decoded += current->character;
            current = root;   // restart from the root
        }
    }

    return decoded;
}

// ────────────────────────────────────────────────────────────
// STEP 2 – Read an entire file into a string
// ────────────────────────────────────────────────────────────
string readFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "  [ERROR] Could not open file: " << filename << endl;
        return "";
    }
    // Read the whole file in one go
    stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

// ────────────────────────────────────────────────────────────
// STEP 8 / STEP 10 – Write a string to a file
// ────────────────────────────────────────────────────────────
void writeFile(const string& filename, const string& content) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "  [ERROR] Could not write to file: " << filename << endl;
        return;
    }
    file << content;
    file.close();
}

// ────────────────────────────────────────────────────────────
// Helper – Free all nodes in the tree (prevent memory leaks)
// ────────────────────────────────────────────────────────────
void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

// ────────────────────────────────────────────────────────────
// Helper – Print the Huffman code table in a nice format
// ────────────────────────────────────────────────────────────
void printCodeTable(const unordered_map<char, string>& codes) {
    cout << "\n  +-------+---------------------------+" << endl;
    cout << "  | Char  | Huffman Code              |" << endl;
    cout << "  +-------+---------------------------+" << endl;
    for (auto& pair : codes) {
        string display;
        if (pair.first == '\n')      display = "\\n";
        else if (pair.first == ' ')  display = "SP";
        else if (pair.first == '\t') display = "\\t";
        else                         display = string(1, pair.first);

        cout << "  | " << setw(5) << left << display
             << " | " << setw(25) << left << pair.second << " |" << endl;
    }
    cout << "  +-------+---------------------------+" << endl;
}

// ────────────────────────────────────────────────────────────
// Compress workflow  (Steps 2-3-5-6-7-8-12)
// ────────────────────────────────────────────────────────────
// We return the Huffman tree root via the output parameter
// so it can be reused for decompression in the same session.
Node* compressFile(const string& inputFile,
                   const string& compressedFile) {
    // STEP 2 – Read input
    string text = readFile(inputFile);
    if (text.empty()) {
        cout << "  File is empty or could not be read.\n";
        return nullptr;
    }

    // STEP 3 – Build frequency map
    unordered_map<char, int> freqMap = buildFrequencyMap(text);

    // STEP 5 – Build Huffman Tree
    Node* root = buildHuffmanTree(freqMap);

    // STEP 6 – Generate codes
    unordered_map<char, string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Print the code table
    printCodeTable(huffmanCodes);

    // STEP 7 – Encode the text
    string encoded = encode(text, huffmanCodes);

    // STEP 8 – Save the encoded string to a file
    writeFile(compressedFile, encoded);

    // STEP 12 – Print statistics
    int originalSize   = text.size() * 8;        // bits (1 char = 8 bits)
    int compressedSize = encoded.size();          // bits (each '0'/'1' = 1 bit)
    double ratio = (1.0 - (double)compressedSize / originalSize) * 100.0;

    cout << "\n  +-------------------------------------------+" << endl;
    cout << "  |         Compression Statistics           |" << endl;
    cout << "  +-------------------------------------------+" << endl;
    cout << "  |  Original size   : " << setw(10) << originalSize
         << " bits     |" << endl;
    cout << "  |  Compressed size : " << setw(10) << compressedSize
         << " bits     |" << endl;
    cout << "  |  Compression ratio: " << setw(9) << fixed << setprecision(2)
         << ratio << "%      |" << endl;
    cout << "  +-------------------------------------------+" << endl;
    cout << "\n  Compressed data saved to: " << compressedFile << endl;

    return root;
}

// ────────────────────────────────────────────────────────────
// Decompress workflow  (Steps 9-10)
// ────────────────────────────────────────────────────────────
void decompressFile(const string& compressedFile,
                    const string& outputFile,
                    Node* root) {
    if (!root) {
        cout << "  [ERROR] No Huffman Tree available.\n";
        cout << "  Please compress a file first (option 1).\n";
        return;
    }

    // STEP 9 – Read encoded string and decode
    string encoded = readFile(compressedFile);
    if (encoded.empty()) {
        cout << "  Compressed file is empty or could not be read.\n";
        return;
    }

    string decoded = decode(encoded, root);

    // STEP 10 – Save decoded text to output file
    writeFile(outputFile, decoded);

    cout << "\n  Decompression complete!" << endl;
    cout << "  Decoded text saved to: " << outputFile << endl;
    cout << "  Decoded length: " << decoded.size() << " characters" << endl;
}

// ────────────────────────────────────────────────────────────
// STEP 11 – Main menu
// ────────────────────────────────────────────────────────────
int main() {
    cout << "  +=============================================+" << endl;
    cout << "  |   Huffman Coding - Compress / Decompress   |" << endl;
    cout << "  +=============================================+" << endl;

    Node* huffmanRoot = nullptr;   // shared tree between compress & decompress
    int choice = 0;

    while (true) {
        cout << "\n  ------------ Menu ------------" << endl;
        cout << "    1. Compress a file" << endl;
        cout << "    2. Decompress a file" << endl;
        cout << "    3. Exit" << endl;
        cout << "  ------------------------------" << endl;
        cout << "  Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "  Invalid input. Please enter 1, 2, or 3.\n";
            continue;
        }

        switch (choice) {
            case 1:
                // Free previous tree if any
                if (huffmanRoot) { freeTree(huffmanRoot); huffmanRoot = nullptr; }
                huffmanRoot = compressFile("input.txt", "compressed.txt");
                break;
            case 2:
                decompressFile("compressed.txt", "output.txt", huffmanRoot);
                break;
            case 3:
                cout << "\n  Goodbye!\n";
                freeTree(huffmanRoot);
                return 0;
            default:
                cout << "  Invalid choice. Try again.\n";
        }
    }

    return 0;
}
