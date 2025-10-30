#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <openssl/evp.h>
#include "cxxopts.hpp"
#include <fstream>
#include <opencv2/opencv.hpp>
// AES key (32 bytes for AES-256)
const unsigned char SUPER_SECRET_KEY[33] = "0123456789abcdef0123456789abcdef";

// Encrypt function with vectors
void encrypt(const std::vector<unsigned char>& plaintext,
             std::vector<unsigned char>& ciphertext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, SUPER_SECRET_KEY, NULL);
    EVP_CIPHER_CTX_set_padding(ctx, 1); // PKCS7 padding

    int len = 0;
    ciphertext.resize(plaintext.size() + 16); // max possible size
    EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size());
    int ciphertext_len = len;
    EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len); // shrink to actual size
    EVP_CIPHER_CTX_free(ctx);
}

// Decrypt function with vectors
void decrypt(const std::vector<unsigned char>& ciphertext,
             std::vector<unsigned char>& decrypted) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, SUPER_SECRET_KEY, NULL);
    EVP_CIPHER_CTX_set_padding(ctx, 1); // PKCS7 padding

    int len = 0;
    decrypted.resize(ciphertext.size());
    EVP_DecryptUpdate(ctx, decrypted.data(), &len, ciphertext.data(), ciphertext.size());
    int decrypted_len = len;
    EVP_DecryptFinal_ex(ctx, decrypted.data() + len, &len);
    decrypted_len += len;

    decrypted.resize(decrypted_len);
    EVP_CIPHER_CTX_free(ctx);
}

// Print hex with 32 bytes (256-bit) per line
void print_ciphertext(const std::vector<unsigned char>& buffer){
    const int block_size = 16;
    for (size_t i = 0; i < buffer.size(); i++) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(buffer[i]) ;
        if ((i + 1) % block_size == 0) std::cout << std::endl;
    }
    std::cout << std::dec << std::endl;
}
// Encrypt + Decrypt wrapper
void exec_encryption_input(const std::string& text) {
    std::vector<unsigned char> plaintext(text.begin(), text.end());

    std::vector<unsigned char> ciphertext;
    encrypt(plaintext, ciphertext);

    std::cout << "Ciphertext:" << std::endl;
    print_ciphertext(ciphertext);

    std::vector<unsigned char> decrypted;
    decrypt(ciphertext, decrypted);

    std::string decrypted_text(decrypted.begin(), decrypted.end());
    std::cout << "Payload text: " << decrypted_text << std::endl;
}


void exec_encryption_text(){

    const std::string filename = "message.txt";

    //Open the file in binary and go straight to the end 
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> plaintext(size);
    if (!file.read(reinterpret_cast<char*>(plaintext.data()), size)) {
        throw std::runtime_error("Error reading file: " + filename);
    }


    std::vector<unsigned char> ciphertext; 

    encrypt(plaintext, ciphertext); 
    print_ciphertext(ciphertext); 

    std::vector<unsigned char> decrypted; 
    decrypt(ciphertext, decrypted); 
    std::string decrypted_text(decrypted.begin(), decrypted.end());
    std::cout << "Payload text: " << decrypted_text << std::endl;

}


void exec_encryption_image(){
    const std::string filename = "linux_logo.jpg";

    // Load image
    cv::Mat img = cv::imread(filename, cv::IMREAD_COLOR);
    if (img.empty()) {
        throw std::runtime_error("Cannot open image: " + filename);
    }

    std::cout << "Original image size: " << img.cols << "x" << img.rows << std::endl;

    // Flatten image data
    std::vector<unsigned char> plaintext;
    if (img.isContinuous()) {
        plaintext.assign(img.data, img.data + img.total() * img.channels());
    } else {
        for (int i = 0; i < img.rows; ++i)
            plaintext.insert(plaintext.end(), img.ptr<unsigned char>(i), img.ptr<unsigned char>(i) + img.cols * img.channels());
    }

    // Pad rows to multiple of 16 for ECB
    int row_bytes = img.cols * img.channels();
    
    std::vector<unsigned char> ciphertext;
    encrypt(plaintext, ciphertext);

    // Convert ciphertext back to image
    cv::Mat ecb_img(img.rows, row_bytes / img.channels(), img.type(), ciphertext.data());

    // Save the ECB-encrypted image
    std::string filename_ecb = "ecb_leak.jpg";
    cv::imwrite(filename_ecb, ecb_img);
    std::cout << "ECB-encrypted image saved as " << filename_ecb << std::endl;

    // Optional: decrypt back to original
    std::vector<unsigned char> decrypted;
    decrypt(ciphertext, decrypted);

    // Remove padding for reconstruction
    cv::Mat decrypted_img(img.rows, img.cols, img.type(), decrypted.data());
    std::string filename_decrypted = "decrypted.jpg";
    cv::imwrite(filename_decrypted, decrypted_img);
    std::cout << "Decrypted image saved as " << filename_decrypted << std::endl;



    cv::Mat canvas(img.rows, img.cols*2, img.type()); 
    decrypted_img.copyTo(canvas(cv::Rect(0, 0, img.cols, img.rows))); 
    ecb_img.copyTo(canvas(cv::Rect(img.cols, 0, img.cols, img.rows))); 
    cv::imshow("Plain text vs ECB encryption", canvas); 
    cv::waitKey(0); 
    cv::destroyAllWindows(); 
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("AES_ECB", "Encryption and Decryption using AES in ECB mode");
    options.add_options()
        ("i,input", "Input text to encrypt", cxxopts::value<std::string>())
        ("h,help", "HELP!")
        ("t,test", "Specify <txt> or <jpg> to run the encryption/decryption on either of the file: message.txt, linux_logo.txt", cxxopts::value<std::string>());

    try {
        auto result = options.parse(argc, argv);
        if (argc == 1) throw cxxopts::exceptions::exception("No input provided");

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (result.count("input")) {
            std::string input_string = result["input"].as<std::string>();
            std::cout << "Payload text: " << input_string << std::endl;
            exec_encryption_input(input_string);
        }

        if (result.count("test")){
            std::string input_string = result["test"].as<std::string>(); 
            
            if (input_string == "txt"){
                exec_encryption_text(); 
            }
            else if (input_string == "jpg"){
                exec_encryption_image(); 
            }
            else {
                throw cxxopts::exceptions::no_such_option("Option not correct, write \"txt\" or \"jpg\" ");
            }

        }
    } catch (const cxxopts::exceptions::no_such_option& e){
        std::cerr << "Error: " << e.what() << "\n\n";
        return 1;

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error: " << e.what() << "\n\n";
        std::cout << options.help() << std::endl;
        return 1;
    } 

    return 0;
}
