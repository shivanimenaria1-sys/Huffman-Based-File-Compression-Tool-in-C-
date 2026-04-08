# 🗜️ Huffman-Based File Compression Tool in C++

A beginner-friendly file compression and decompression tool built using **Huffman Coding** in C++. This project demonstrates core concepts of data structures (trees, heaps, hashmaps), file handling, and greedy algorithms — all in a single, well-commented file.

---

## 📌 Features

- **Compress** any text file using Huffman Coding
- **Decompress** the encoded file back to its original content
- Displays a **Huffman Code Table** for every character
- Shows **compression statistics** (original size, compressed size, ratio)
- Interactive **menu-driven** interface
- Clean, modular, and easy-to-understand code

---

## 🧠 How Huffman Coding Works

1. **Count Frequencies** — Scan the input and count how often each character appears.
2. **Build a Min-Heap** — Insert all characters as nodes into a priority queue (min-heap).
3. **Build the Huffman Tree** — Repeatedly extract the two lowest-frequency nodes, merge them into a new internal node, and push it back.
4. **Generate Codes** — Traverse the tree: going left appends `0`, going right appends `1`. Leaf nodes store the final binary code.
5. **Encode** — Replace each character in the input with its Huffman code.
6. **Decode** — Walk the Huffman Tree bit-by-bit to reconstruct the original text.

> Frequent characters get **shorter codes**, rare characters get **longer codes** — this is what achieves compression.

---

## 🛠️ Tech Stack

- **Language:** C++17
- **Libraries:** STL only (`iostream`, `fstream`, `queue`, `unordered_map`, `vector`, `string`)
- **No external dependencies**

---

## 📁 Project Structure

```
📦 zip project
├── main.cpp          # Complete source code (single file)
├── input.txt         # Sample input text file
├── .gitignore        # Ignores binaries and generated files
└── README.md         # This file
```

**Generated at runtime:**
```
├── compressed.txt    # Encoded binary string (after compression)
├── output.txt        # Reconstructed text (after decompression)
└── huffman.exe       # Compiled executable
```

---

## 🚀 How to Run

### Prerequisites
- A C++ compiler (g++, MinGW, or MSVC)

### Steps

```bash
# 1. Clone the repository
git clone https://github.com/shivanimenaria1-sys/Huffman-Based-File-Compression-Tool-in-C-.git
cd Huffman-Based-File-Compression-Tool-in-C-

# 2. Compile
g++ -o huffman.exe main.cpp -std=c++17

# 3. Run
./huffman.exe
```

### Menu Options

```
  +=============================================+
  |   Huffman Coding - Compress / Decompress   |
  +=============================================+

  ------------ Menu ------------
    1. Compress a file
    2. Decompress a file
    3. Exit
  ------------------------------
  Enter your choice:
```

- **Option 1** — Reads `input.txt`, compresses it, and saves to `compressed.txt`
- **Option 2** — Reads `compressed.txt`, decompresses it, and saves to `output.txt`
- **Option 3** — Exit the program

---

## 📊 Sample Output

### Huffman Code Table
```
  +-------+---------------------------+
  | Char  | Huffman Code              |
  +-------+---------------------------+
  | e     | 010                       |
  | SP    | 101                       |
  | s     | 1110                      |
  | t     | 1100                      |
  | a     | 1101                      |
  | i     | 11111                     |
  | h     | 11110                     |
  | n     | 0001                      |
  | o     | 0111                      |
  | ...   | ...                       |
  +-------+---------------------------+
```

### Compression Statistics
```
  +-------------------------------------------+
  |         Compression Statistics           |
  +-------------------------------------------+
  |  Original size   :       2904 bits     |
  |  Compressed size :       1556 bits     |
  |  Compression ratio:    46.42%          |
  +-------------------------------------------+
```

---

## 🔍 Concepts Demonstrated

| Concept | Where Used |
|---------|-----------|
| **Huffman Tree** | Binary tree built from character frequencies |
| **Priority Queue (Min-Heap)** | Selecting lowest frequency nodes |
| **Hash Map** | Storing character frequencies and Huffman codes |
| **Recursion (DFS)** | Traversing the tree to generate codes |
| **File I/O** | Reading input, writing compressed & decompressed output |
| **Greedy Algorithm** | Huffman coding is a classic greedy approach |

---

## ⚠️ Limitations

- Encoded output is stored as a **string of '0's and '1's** (not bit-packed) — this is intentional for simplicity and readability.
- Compression must happen **before** decompression in the same session (the Huffman Tree lives in memory).
- Designed for **text files** only.

---

## 📝 License

This project is open source and available for educational purposes.

---

## 👩‍💻 Author

**Shivani Menaria**

Made with ❤️ as a mini project to learn Huffman Coding and C++ data structures.
