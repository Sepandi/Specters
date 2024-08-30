#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <sep/AL/al.h>
#include <sep/AL/alc.h>
#endif
#ifdef _WIN32
    #include <windows.h>
    #include <dsound.h>
    #include "Darwin.h"
#endif

// Data structure to hold sound information
typedef struct {
#ifdef __APPLE__
    ALuint buffer;
    ALuint source;
#endif
#ifdef _WIN32
    LPDIRECTSOUND directSound; 
    LPDIRECTSOUNDBUFFER buffer; 
    bool isPlaying;    
#endif
} MDMX_SOUND;

// Function prototypes
void MDMX_Init();
MDMX_SOUND MDMX_SOUND_Load(const char* filename);
MDMX_SOUND MDMX_SOUND_LoadFromBin(uint8_t *data,uint32_t sampleRate,uint32_t channels,size_t size);
void MDMX_SOUND_Play(MDMX_SOUND* sound);
void MDMX_SOUND_Stop(MDMX_SOUND* sound);
void MDMX_SOUND_CleanUp(MDMX_SOUND* sound);
bool MDMX_SOUND_IsPlaying(MDMX_SOUND* sound);

#ifdef MAD_MAX_IMP
#undef MAD_MAX_IMP

#define DR_WAV_IMPLEMENTATION
#include "DR/dr_wav.h"

#ifdef __APPLE__
    ALCdevice* device = NULL;
    ALCcontext* context = NULL;
#endif

void MDMX_Init(){
#ifdef __APPLE__
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
#endif
#ifdef _WIN32

#endif
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
    uint64_t size = wav.totalPCMFrameCount * wav.bitsPerSample / 8 * wav.channels;
    void* data = malloc(size);
    if (!data) {
        fprintf(stderr, "Failed to allocate memory for sound data\n");
        drwav_uninit(&wav);
        return newSound;
    }

    // Read sound data
    drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, data);

#ifdef __APPLE__
    

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
#endif
#ifdef _WIN32
    HRESULT hr;
    DSBUFFERDESC bufferDesc = {0};
    WAVEFORMATEX waveFormat = {0};

    // Initialize COM
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // Create DirectSound object
    hr = DirectSoundCreate(NULL, &newSound.directSound, NULL);

    // Set cooperative level
    hr = IDirectSound_SetCooperativeLevel(newSound.directSound,GetDesktopWindow(), DSSCL_PRIORITY);

    // Set up wave format
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = wav.channels;
    waveFormat.nSamplesPerSec = wav.sampleRate;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    // Create a primary buffer
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS;
    bufferDesc.dwBufferBytes = size;
    bufferDesc.lpwfxFormat = &waveFormat;

    hr = IDirectSound_CreateSoundBuffer(newSound.directSound,&bufferDesc, &newSound.buffer, NULL);
    // Set the format for the primary buffer

    // Create a secondary buffer
    // bufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS;
    // bufferDesc.dwBufferBytes = size;
    // bufferDesc.lpwfxFormat = &waveFormat;

    // hr = IDirectSound_CreateSoundBuffer(newSound.directSound,&bufferDesc, &newSound.buffer, NULL)

    newSound.isPlaying = false;

    void *ptr1, *ptr2;
    DWORD size1, size2;

    hr = IDirectSoundBuffer_Lock(newSound.buffer,0, size, &ptr1, &size1, &ptr2, &size2, 0);
    if (FAILED(hr)) {
        printf("Failed to lock the DirectSound buffer. HRESULT: 0x%08lx\n", hr);
    }

    // Copy the sound data into the buffer
    memcpy(ptr1, data, size1);
    if (ptr2) {
        memcpy(ptr2, (char*)data + size1, size2);
    }

    // Unlock the buffer
    hr = IDirectSoundBuffer_Unlock(newSound.buffer,ptr1, size1, ptr2, size2);
    if (FAILED(hr)) {
        printf("Failed to unlock the DirectSound buffer. HRESULT: 0x%08lx\n", hr);
    }
#endif
    return newSound;
}

MDMX_SOUND MDMX_SOUND_LoadFromBin(uint8_t *data,uint32_t sampleRate,uint32_t channels,size_t size) {
    MDMX_SOUND newSound;


#ifdef __APPLE__
    

    // Generate OpenAL buffer and source
    alGenBuffers(1, &newSound.buffer);
    alGenSources(1, &newSound.source);

    ALenum format = (channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    alBufferData(newSound.buffer, format, data, size, sampleRate);
    alSourcei(newSound.source, AL_BUFFER, newSound.buffer);

    // Clean up data
    free(data);

    // Check for OpenAL errors
    ALenum error = alGetError();
    if (error != AL_NO_ERROR) {
        fprintf(stderr, "OpenAL Error: %d\n", error);
        MDMX_SOUND_CleanUp(&newSound);
    }
#endif
#ifdef _WIN32
    HRESULT hr;
    DSBUFFERDESC bufferDesc = {0};
    WAVEFORMATEX waveFormat = {0};

    // Initialize COM
    hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    // Create DirectSound object
    hr = DirectSoundCreate(NULL, &newSound.directSound, NULL);

    // Set cooperative level
    hr = IDirectSound_SetCooperativeLevel(newSound.directSound,GetDesktopWindow(), DSSCL_PRIORITY);

    // Set up wave format
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nChannels = channels;
    waveFormat.nSamplesPerSec = sampleRate;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    // Create a primary buffer
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS;
    bufferDesc.dwBufferBytes = size;
    bufferDesc.lpwfxFormat = &waveFormat;

    hr = IDirectSound_CreateSoundBuffer(newSound.directSound,&bufferDesc, &newSound.buffer, NULL);
    // Set the format for the primary buffer
    // hr = primaryBuffer->SetFormat(&waveFormat);

    // primaryBuffer->Release();

    // Create a secondary buffer
    // bufferDesc.dwFlags = DSBCAPS_GLOBALFOCUS;
    // bufferDesc.dwBufferBytes = size;
    // bufferDesc.lpwfxFormat = &waveFormat;

    // hr = IDirectSound_CreateSoundBuffer(newSound.directSound,&bufferDesc, &newSound.buffer, NULL);

    newSound.isPlaying = false;

    void *ptr1, *ptr2;
    DWORD size1, size2;

    hr = IDirectSoundBuffer_Lock(newSound.buffer,0, size, &ptr1, &size1, &ptr2, &size2, 0);
    if (FAILED(hr)) {
        printf("Failed to lock the DirectSound buffer. HRESULT: 0x%08lx\n", hr);
    }

    // Copy the sound data into the buffer
    memcpy(ptr1, data, size1);
    if (ptr2) {
        memcpy(ptr2, (char*)data + size1, size2);
    }

    // Unlock the buffer
    hr = IDirectSoundBuffer_Unlock(newSound.buffer,ptr1, size1, ptr2, size2);
    if (FAILED(hr)) {
        printf("Failed to unlock the DirectSound buffer. HRESULT: 0x%08lx\n", hr);
    }
#endif
    return newSound;
}

void MDMX_SOUND_Play(MDMX_SOUND* sound) {
#ifdef __APPLE__
    if (sound && sound->source) {
        alSourcePlay(sound->source);
    }
#endif
#ifdef _WIN32
    HRESULT hr = IDirectSoundBuffer_Play(sound->buffer,0, 0, 0);
    if (FAILED(hr)) {
        printf("Failed to play the DirectSound buffer. HRESULT: 0x%08lx\n", hr);
    }
    sound->isPlaying = true;
#endif
}

void MDMX_SOUND_Stop(MDMX_SOUND* sound) {
#ifdef __APPLE__
    if (sound && sound->source) {
        alSourceStop(sound->source);
    }
#endif
#ifdef _WIN32
    if (!sound->isPlaying) return;

    IDirectSoundBuffer_Stop(sound->buffer);
    sound->isPlaying = false;
#endif
}

void MDMX_SOUND_CleanUp(MDMX_SOUND* sound) {
#ifdef __APPLE__
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
#endif
#ifdef _WIN32
    if (sound->buffer) {
        IDirectSoundBuffer_Release(sound->buffer);
        sound->buffer = NULL;
    }
    if (sound->directSound) {
        IDirectSoundBuffer_Release(sound->directSound);
        sound->directSound = NULL;
    }
    CoUninitialize();
#endif
}

bool MDMX_SOUND_IsPlaying(MDMX_SOUND* sound) {
#ifdef __APPLE__
    if (!sound || !sound->source) return false;

    ALint state;
    alGetSourcei(sound->source, AL_SOURCE_STATE, &state);
    return (state == AL_PLAYING);
#endif
#ifdef _WIN32
    DWORD status = 0;
    HRESULT hr = IDirectSoundBuffer_GetStatus(sound->buffer,&status);
    if (FAILED(hr)) {
        printf("Failed to get status from DirectSound buffer. HRESULT: 0x%08lx\n", hr);
        return false;
    }

    return (status & DSBSTATUS_PLAYING) != 0;
#endif
}

#endif