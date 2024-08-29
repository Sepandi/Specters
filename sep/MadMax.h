#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sep/AL/al.h>
#include <sep/AL/alc.h>

// Data structure to hold sound information
typedef struct {
    ALuint buffer;
    ALuint source;
} MDMX_SOUND;

extern ALCdevice* device;
extern ALCcontext* context;

// Function prototypes
void MDMX_Init();
MDMX_SOUND MDMX_SOUND_Load(const char* filename);
void MDMX_SOUND_Play(MDMX_SOUND* sound);
void MDMX_SOUND_Stop(MDMX_SOUND* sound);
void MDMX_SOUND_CleanUp(MDMX_SOUND* sound);
bool MDMX_SOUND_IsPlaying(MDMX_SOUND* sound);

#ifdef MAD_MAX_IMP
#undef MAD_MAX_IMP

#define DR_WAV_IMPLEMENTATION
#include "sep/DR/dr_wav.h"

ALCdevice* device = NULL;
ALCcontext* context = NULL;

void MDMX_Init(){
    // Initialize OpenAL device and context
    ALCdevice* device = alcOpenDevice(NULL);
    if (!device) {
        printf("Failed to open device\n");
    }

    ALCcontext* context = alcCreateContext(device, NULL);
    if (!context || !alcMakeContextCurrent(context)) {
        printf("Failed to create or set context\n");
        alcCloseDevice(device);
    }
}

MDMX_SOUND MDMX_SOUND_Load(const char* filename) {
    MDMX_SOUND newSound;
    // Load sound data (WAV file) using dr_wav
    drwav wav;
    if (!drwav_init_file(&wav, filename, NULL)) {
        fprintf(stderr, "Failed to load WAV file\n");
        return newSound;
    }

    // Allocate buffer for sound data
    ALsizei size = (ALsizei)wav.totalPCMFrameCount * wav.bitsPerSample / 8 * wav.channels;
    void* data = malloc(size);
    if (!data) {
        fprintf(stderr, "Failed to allocate memory for sound data\n");
        drwav_uninit(&wav);
        alcDestroyContext(context);
        alcCloseDevice(device);
        return newSound;
    }

    // Read sound data
    drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, data);

    // Generate OpenAL buffer and source
    alGenBuffers(1, &newSound.buffer);
    alGenSources(1, &newSound.source);

    ALenum format = (wav.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    alBufferData(newSound.buffer, format, data, size, wav.sampleRate);
    alSourcei(newSound.source, AL_BUFFER, newSound.buffer);

    // Clean up WAV data
    free(data);
    drwav_uninit(&wav);

    // Check for OpenAL errors
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        fprintf(stderr, "OpenAL Error: %d\n", error);
        MDMX_SOUND_CleanUp(&newSound);
    }
    return newSound;
}

void MDMX_SOUND_Play(MDMX_SOUND* sound) {
    if (sound && sound->source) {
        alSourcePlay(sound->source);
    }
}

void MDMX_SOUND_Stop(MDMX_SOUND* sound) {
    if (sound && sound->source) {
        alSourceStop(sound->source);
    }
}

void MDMX_SOUND_CleanUp(MDMX_SOUND* sound) {
    if (sound && sound->source) {
        alSourceStop(sound->source);
        alDeleteSources(1, &sound->source);
    }
    if (sound && sound->buffer) {
        alDeleteBuffers(1, &sound->buffer);
    }

    // Close OpenAL context and device
    ALCcontext* context = alcGetCurrentContext();
    ALCdevice* device = alcGetContextsDevice(context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool MDMX_SOUND_IsPlaying(MDMX_SOUND* sound) {
    if (!sound || !sound->source) return false;

    ALint state;
    alGetSourcei(sound->source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
}

#endif