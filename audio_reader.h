#ifndef AUDIO_READER_H
#define AUDIO_READER_H

#include <vector>
#include <string>

std::vector<float> readAudioFile(const std::string& filename);

#endif