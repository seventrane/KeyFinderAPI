#include <node.h>
#include <v8.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <sndfile.h> // Include this line
#include "audio_reader.h"
#include "include/keyfinder/keyfinder.h"
#include "include/keyfinder/audiodata.h"
#include "include/keyfinder/constants.h"


#include <cstdio>

namespace fs = std::filesystem;

void deleteFile(const std::string& filePath) {
    if (!fs::exists(filePath)) {
        std::cerr << "Error: File does not exist" << std::endl;
        return;
    }

    fs::remove(filePath);

    std::cout << "File deleted successfully" << std::endl;
}

void copyFile(const std::string& sourcePath, const std::string& destinationPath) {
    if (!fs::exists(sourcePath)) {
        std::cerr << "Error: Source file does not exist" << std::endl;
        return;
    }

    std::string destinationDir = destinationPath.substr(0, destinationPath.find_last_of('/'));
    if (!fs::exists(destinationDir)) {
        if (!fs::create_directories(destinationDir)) {
            std::cerr << "Error: Failed to create destination directory" << std::endl;
            return;
        }
    }

    fs::copy_file(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
    std::cout << "File copied successfully" << std::endl;

    deleteFile(sourcePath);
}

bool fileExists(const std::string& filePath) {
    return fs::exists(filePath);
}

std::string getKeySignature(SNDFILE* file, const SF_INFO& info, const std::string& pathReal) {
    if (!fileExists(pathReal)) {
        std::cerr << "Error: Source file does not exist" << std::endl;
        return "";
    }

    std::string sourcePath = pathReal;
    std::string destinationPath = pathReal;
    size_t pos = destinationPath.find("./uploads");
    if (pos != std::string::npos) {
        destinationPath.replace(pos, strlen("./uploads"), "pianoSongs");
    }

    std::cout << "Source Path: " << sourcePath << std::endl;
    std::cout << "Destination Path: " << destinationPath << std::endl;

    copyFile(sourcePath, destinationPath);

    std::vector<float> audioStream(info.frames * info.channels);
    sf_readf_float(file, audioStream.data(), info.frames);
    sf_close(file);

    KeyFinder::AudioData a;
    a.setFrameRate(info.samplerate);
    a.setChannels(info.channels);
    a.addToSampleCount(audioStream.size());

    for (size_t i = 0; i < audioStream.size(); i++) {
        a.setSample(i, audioStream[i]);
    }

    static KeyFinder::KeyFinder k;
    KeyFinder::key_t key = k.keyOfAudio(a);

    std::string keySignature;
     switch (key) {
        case KeyFinder::A_MAJOR:
            keySignature = "A major";
            break;
        case KeyFinder::A_MINOR:
            keySignature = "A minor";
            break;
        case KeyFinder::B_FLAT_MAJOR:
            keySignature = "B♭ major";
            break;
        case KeyFinder::B_FLAT_MINOR:
            keySignature = "B♭ minor";
            break;
        case KeyFinder::B_MAJOR:
            keySignature = "B major";
            break;
        case KeyFinder::B_MINOR:
            keySignature = "B minor";
            break;
        case KeyFinder::C_MAJOR:
            keySignature = "C major";
            break;
        case KeyFinder::C_MINOR:
            keySignature = "C minor";
            break;
        case KeyFinder::D_FLAT_MAJOR:
            keySignature = "D♭ major";
            break;
        case KeyFinder::D_FLAT_MINOR:
            keySignature = "D♭ minor";
            break;
        case KeyFinder::D_MAJOR:
            keySignature = "D major";
            break;
        case KeyFinder::D_MINOR:
            keySignature = "D minor";
            break;
        case KeyFinder::E_FLAT_MAJOR:
            keySignature = "E♭ major";
            break;
        case KeyFinder::E_FLAT_MINOR:
            keySignature = "E♭ minor";
            break;
        case KeyFinder::E_MAJOR:
            keySignature = "E major";
            break;
        case KeyFinder::E_MINOR:
            keySignature = "E minor";
            break;
        case KeyFinder::F_MAJOR:
            keySignature = "F major";
            break;
        case KeyFinder::F_MINOR:
            keySignature = "F minor";
            break;
        case KeyFinder::G_FLAT_MAJOR:
            keySignature = "G♭ major";
            break;
        case KeyFinder::G_FLAT_MINOR:
            keySignature = "G♭ minor";
            break;
        case KeyFinder::G_MAJOR:
            keySignature = "G major";
            break;
        case KeyFinder::G_MINOR:
            keySignature = "G minor";
            break;
        default:
            keySignature = "Unknown";
            break;
    }

    return keySignature;
}


std::pair<SNDFILE*, SF_INFO> MySecondFunction(const char* filename) {
    if (strstr(filename, "/.")) {
        std::cerr << "Skipping hidden file: " << filename << std::endl;
        return {nullptr, SF_INFO()};
    }

    SF_INFO info;
    SNDFILE* file = sf_open(filename, SFM_READ, &info);
    if (!file) {
        std::cerr << "Error opening file: " << sf_strerror(nullptr) << std::endl;
        return {nullptr, SF_INFO()};
    }
    return {file, info};
}

std::vector<std::string> getFilePaths(const std::string& folderPath) {
    std::vector<std::string> filePaths;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (!entry.is_directory()) {
            filePaths.push_back(entry.path().string());
        }
    }

    return filePaths;
}

void MyFunction(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate* isolate = args.GetIsolate();

    if (args.Length() < 1 || !args[0]->IsString()) {
        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8(isolate, "Invalid arguments").ToLocalChecked()));
        return;
    }

    v8::Local<v8::String> folderPathArg = args[0]->ToString(isolate->GetCurrentContext()).ToLocalChecked();
    v8::String::Utf8Value folderPath(isolate, folderPathArg);
    std::string folderPathStr(*folderPath);

    std::cout << "Folder: " << folderPathStr;
    std::vector<std::string> filePaths = getFilePaths(folderPathStr);

    v8::Local<v8::Array> resultArray = v8::Array::New(isolate, filePaths.size());
    for (size_t i = 0; i < filePaths.size(); i++) {
        std::cout << "Processing file " << (i + 1) << " of " << filePaths.size() << ": " << filePaths[i] << std::endl;

        auto [file, info] = MySecondFunction(filePaths[i].c_str());
        if (!file) {
            continue;
        }

        std::string keySignature = getKeySignature(file, info, filePaths[i]);

        v8::Local<v8::Array> fileInfo = v8::Array::New(isolate, 2);
        fileInfo->Set(isolate->GetCurrentContext(), 0, v8::String::NewFromUtf8(isolate, filePaths[i].c_str()).ToLocalChecked());
        fileInfo->Set(isolate->GetCurrentContext(), 1, v8::String::NewFromUtf8(isolate, keySignature.c_str()).ToLocalChecked());
        resultArray->Set(isolate->GetCurrentContext(), i, fileInfo);
    }

    args.GetReturnValue().Set(resultArray);
}

void Initialize(v8::Local<v8::Object> exports) {
    NODE_SET_METHOD(exports, "myFunction", MyFunction);
}

NODE_MODULE(addon, Initialize)
