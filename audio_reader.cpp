#include "audio_reader.h"
#include <iostream>
#include <sndfile.h>

std::vector<float> readAudioFile(const std::string& filename) {
    SNDFILE *file;
    SF_INFO info;
    std::vector<float> audioStream;

    // Open the audio file
    file = sf_open(filename.c_str(), SFM_READ, &info);
    if (!file) {
        std::cerr << "Error opening file: " << sf_strerror(nullptr) << std::endl;
        return audioStream;
    }

    // Resize the audio stream vector to hold all samples
    audioStream.resize(info.frames * info.channels);

    // Read audio data into the stream
    sf_readf_float(file, audioStream.data(), info.frames);

    // Close the file
    sf_close(file);

    return audioStream;
}
