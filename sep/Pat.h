#pragma once

#ifdef PACK_PROGRAM
#define PAT_IMP
#define PAT_WRITER
#define STB_IMAGE_IMPLEMENTATION
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#ifdef __APPLE__
    #include <dirent.h>
#endif
#include <string.h>
#include <sys/stat.h>


typedef struct {
    uint32_t *data;   // Pointer to image data
    int width;        // Width of the image
    int height;       // Height of the image
    int channels;     // Number of channels (should be 4 for RGBA)
    size_t size;      // Size of the image data in bytes
} PAT_IMAGE;

typedef struct {
    char *data;       // Pointer to text data
    size_t size;      // Size of the text data in bytes
} PAT_TEXT;

typedef struct {
    uint8_t *data;    // Pointer to audio data
    uint32_t sampleRate; // Sample rate of the audio
    uint32_t channels;   // Number of audio channels
    size_t size;         // Size of the audio data in bytes
} PAT_SOUND;

// Define a structure to hold the asset pack
typedef struct {
    PAT_IMAGE *images;    // Array of images
    char **imageKeys;     // Array of image names
    int imageCount;       // Number of images

    PAT_TEXT *texts;      // Array of texts
    char **textKeys;      // Array of text names
    int textCount;        // Number of texts

    PAT_SOUND *sounds;    // Array of sounds
    char **soundKeys;     // Array of sound names
    int soundCount;       // Number of sounds
} PAT_ASSETPACK;

//- LOADER PARTS

static char* PAT_GetBaseName(const char* filePath);

// Load PAT_ASSETPACK from a binary file
PAT_ASSETPACK* PAT_LoadAssetPack(const char* input_file);

// Get an image by base name from the PAT_ASSETPACK
PAT_IMAGE* PAT_GetImageByKey(const PAT_ASSETPACK* pack, const char* key);

// Get a text by base name from the PAT_ASSETPACK
PAT_TEXT* PAT_GetTextByKey(const PAT_ASSETPACK* pack, const char* key);

// Get a sound by base name from the PAT_ASSETPACK
PAT_SOUND* PAT_GetSoundByKey(const PAT_ASSETPACK* pack, const char* key);

// Free the PAT_ASSETPACK
void PAT_FreeAssetPack(PAT_ASSETPACK* pack);

//- WRITER PARTS

// Create a PAT_ASSETPACK from files in a directory
PAT_ASSETPACK* PAT_CreateAssetPack(const char* input_dir);

// Save PAT_ASSETPACK to a binary file
void PAT_SaveAssetPack(const char* output_file, const PAT_ASSETPACK* pack);

// Unpack PAT_ASSETPACK into a directory
void PAT_UnpackAssetPack(const char* input_file, const char* output_dir);


// Implementation
#ifdef PAT_IMP
#undef PAT_IMP



static char* PAT_GetBaseName(const char* filePath) {
    const char* base = strrchr(filePath, '/');
    base = base ? base + 1 : filePath;
    char* ext = strrchr(base, '.');
    size_t length = ext ? (size_t)(ext - base) : strlen(base);
    char* baseName = (char*)malloc(length + 1);
    if (baseName) {
        strncpy(baseName, base, length);
        baseName[length] = '\0';
    }
    return baseName;
}

// Load PAT_ASSETPACK from a binary file
PAT_ASSETPACK* PAT_LoadAssetPack(const char* input_file) {
    FILE* file = fopen(input_file, "rb");
    if (!file) {
        perror("PAT : LoadAssetPack : Failed to open file for reading");
        return NULL;
    }

    PAT_ASSETPACK* pack = (PAT_ASSETPACK*)malloc(sizeof(PAT_ASSETPACK));
    if (!pack) {
        perror("PAT : LoadAssetPack : Failed to allocate memory for PAT_ASSETPACK");
        fclose(file);
        return NULL;
    }

    fread(&pack->imageCount, sizeof(int), 1, file);

    pack->images = (PAT_IMAGE*)malloc(pack->imageCount * sizeof(PAT_IMAGE));
    pack->imageKeys = (char**)malloc(pack->imageCount * sizeof(char*));

    // Check if memory allocation was successful
    if (!pack->images || !pack->imageKeys) {
        perror("Failed to allocate memory for images or imageKeys");
        free(pack->images);
        free(pack->imageKeys);
        return NULL; // or handle error appropriately
    }

    for (int i = 0; i < pack->imageCount; ++i) {
        fread(&pack->images[i].width, sizeof(int), 1, file);
        fread(&pack->images[i].height, sizeof(int), 1, file);
        fread(&pack->images[i].channels, sizeof(int), 1, file);
        fread(&pack->images[i].size, sizeof(size_t), 1, file);
        pack->images[i].data = (uint32_t*)malloc(pack->images[i].size);
        fread(pack->images[i].data, 1, pack->images[i].size, file);

        size_t key_len;
        fread(&key_len, sizeof(size_t), 1, file);
        
        if (key_len == 0) {
            pack->imageKeys[i] = NULL; // Handle zero-length key case
        } else {
            pack->imageKeys[i] = (char*)malloc(key_len);
            fread(pack->imageKeys[i], 1, key_len, file);
        }

        // Debugging line to check if a key is null
        if (pack->imageKeys[i] == NULL) {
            fprintf(stderr, "Warning: pack->imageKeys[%d] was not properly loaded and is NULL\n", i);
        }
    }

    fread(&pack->textCount, sizeof(int), 1, file);
    pack->texts = (PAT_TEXT*)malloc(pack->textCount * sizeof(PAT_TEXT));
    pack->textKeys = (char**)malloc(pack->textCount * sizeof(char*));

    for (int i = 0; i < pack->textCount; ++i) {
        fread(&pack->texts[i].size, sizeof(size_t), 1, file);
        pack->texts[i].data = (char*)malloc(pack->texts[i].size);
        fread(pack->texts[i].data, 1, pack->texts[i].size, file);

        size_t key_len;
        fread(&key_len, sizeof(size_t), 1, file);
        pack->textKeys[i] = (char*)malloc(key_len);
        fread(pack->textKeys[i], 1, key_len, file);
    }

    fread(&pack->soundCount, sizeof(int), 1, file);
    pack->sounds = (PAT_SOUND*)malloc(pack->soundCount * sizeof(PAT_SOUND));
    pack->soundKeys = (char**)malloc(pack->soundCount * sizeof(char*));

    for (int i = 0; i < pack->soundCount; ++i) {
        fread(&pack->sounds[i].sampleRate, sizeof(uint32_t), 1, file);
        fread(&pack->sounds[i].channels, sizeof(uint32_t), 1, file);
        fread(&pack->sounds[i].size, sizeof(size_t), 1, file);
        pack->sounds[i].data = (uint8_t*)malloc(pack->sounds[i].size);
        fread(pack->sounds[i].data, 1, pack->sounds[i].size, file);

        size_t key_len;
        fread(&key_len, sizeof(size_t), 1, file);
        pack->soundKeys[i] = (char*)malloc(key_len);
        fread(pack->soundKeys[i], 1, key_len, file);
    }

    fclose(file);
    return pack;

    fclose(file);
    return pack;
}

// Get an image by base name from the PAT_ASSETPACK
PAT_IMAGE* PAT_GetImageByKey(const PAT_ASSETPACK* pack, const char* key) {
    for (int i = 0; i < pack->imageCount; ++i) {
        if (strcmp(pack->imageKeys[i], key) == 0) {
            return &pack->images[i];
        }
    }
    return NULL;
}

// Get a text by base name from the PAT_ASSETPACK
PAT_TEXT* PAT_GetTextByKey(const PAT_ASSETPACK* pack, const char* key) {
    for (int i = 0; i < pack->textCount; ++i) {
        if (strcmp(pack->textKeys[i], key) == 0) {
            return &pack->texts[i];
        }
    }
    return NULL;
}

PAT_SOUND* PAT_GetSoundByKey(const PAT_ASSETPACK* pack, const char* key) {
    for (int i = 0; i < pack->soundCount; ++i) {
        if (strcmp(pack->soundKeys[i], key) == 0) {
            return &pack->sounds[i];
        }
    }
    return NULL;
}

// Free the PAT_ASSETPACK
void PAT_FreeAssetPack(PAT_ASSETPACK* pack) {
    if (pack) {
        for (int i = 0; i < pack->imageCount; ++i) {
            free(pack->images[i].data);
            free(pack->imageKeys[i]);
        }
        free(pack->images);
        free(pack->imageKeys);

        for (int i = 0; i < pack->textCount; ++i) {
            free(pack->texts[i].data);
            free(pack->textKeys[i]);
        }
        free(pack->texts);
        free(pack->textKeys);

        for (int i = 0; i < pack->soundCount; ++i) {
            free(pack->sounds[i].data);
            free(pack->soundKeys[i]);
        }
        free(pack->sounds);
        free(pack->soundKeys);


        free(pack);
    }
}

#ifdef PAT_WRITER
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define DR_WAV_IMPLEMENTATION
#include "DR/dr_wav.h"

// Helper function to load text data from a file
char* PAT_LoadTextFile(const char* filePath, size_t* outSize) {
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        perror("PAT : LoadTextFile : Failed to open text file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* data = (char*)malloc(size + 1);
    if (!data) {
        perror("PAT : LoadTextFile : Failed to allocate memory for text data");
        fclose(file);
        return NULL;
    }

    fread(data, 1, size, file);
    data[size] = '\0';
    fclose(file);

    if (outSize) {
        *outSize = size + 1;  // Include the null terminator
    }
    return data;
}

// Create a PAT_ASSETPACK from files in a directory
PAT_ASSETPACK* PAT_CreateAssetPack(const char* input_dir) {
    DIR *dir = opendir(input_dir);
    if (!dir) {
        perror("PAT : CREATE : Failed to open directory");
        return NULL;
    }

    struct dirent *entry;
    int num_images = 0;
    int num_texts = 0;
    int num_sounds = 0;

    // Count the number of PNG, TXT, and WAV files in the directory
    while ((entry = readdir(dir))) {
        if (strstr(entry->d_name, ".png") || strstr(entry->d_name, ".jpg")) {
            num_images++;
        } else if (strstr(entry->d_name, ".txt")) {
            num_texts++;
        } else if (strstr(entry->d_name, ".wav")) {
            num_sounds++;
        }
    }

    rewinddir(dir);

    PAT_ASSETPACK* pack = (PAT_ASSETPACK*)malloc(sizeof(PAT_ASSETPACK));
    if (!pack) {
        perror("PAT : CREATE : Failed to allocate memory for PAT_ASSETPACK");
        closedir(dir);
        return NULL;
    }

    pack->imageCount = num_images;
    pack->textCount = num_texts;
    pack->soundCount = num_sounds;

    pack->images = (PAT_IMAGE*)malloc(num_images * sizeof(PAT_IMAGE));
    pack->imageKeys = (char**)malloc(num_images * sizeof(char*));
    pack->texts = (PAT_TEXT*)malloc(num_texts * sizeof(PAT_TEXT));
    pack->textKeys = (char**)malloc(num_texts * sizeof(char*));
    pack->sounds = (PAT_SOUND*)malloc(num_sounds * sizeof(PAT_SOUND));
    pack->soundKeys = (char**)malloc(num_sounds * sizeof(char*));

    if (!pack->images || !pack->imageKeys || !pack->texts || !pack->textKeys || !pack->sounds || !pack->soundKeys) {
        perror("PAT : CREATE : Failed to allocate memory for assets");
        // Free all allocated memory before returning NULL
        free(pack->images);
        free(pack->imageKeys);
        free(pack->texts);
        free(pack->textKeys);
        free(pack->sounds);
        free(pack->soundKeys);
        free(pack);
        closedir(dir);
        return NULL;
    }

    // Initialize counts
    int i = 0, j = 0, k = 0;
    while ((entry = readdir(dir)) != NULL) {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", input_dir, entry->d_name);

        struct stat st;
        if (stat(file_path, &st) == -1 || S_ISDIR(st.st_mode)) {
            continue;
        }

        char* baseName = PAT_GetBaseName(entry->d_name);
        if (!baseName) {
            continue;
        }

        if (strstr(entry->d_name, ".png") || strstr(entry->d_name, ".jpg")) {
            int width, height, channels;
            uint8_t* image_data = stbi_load(file_path, &width, &height, &channels, 4);

            if (image_data) {
                // Allocate and assign image data
                pack->images[i].width = width;
                pack->images[i].height = height;
                pack->images[i].channels = 4;
                pack->images[i].size = width * height * 4;
                pack->images[i].data = (uint32_t*)image_data;

                // Assign the base name
                pack->imageKeys[i] = baseName;
                i++;
            } else {
                free(baseName); // Clean up base name if image loading fails
            }
        } else if (strstr(entry->d_name, ".txt")) {
            FILE* text_file = fopen(file_path, "rb");
            if (text_file) {
                fseek(text_file, 0, SEEK_END);
                long size = ftell(text_file);
                fseek(text_file, 0, SEEK_SET);

                char* text_data = (char*)malloc(size + 1);
                if (text_data) {
                    fread(text_data, 1, size, text_file);
                    text_data[size] = '\0';

                    pack->texts[j].data = text_data;
                    pack->texts[j].size = size;
                    pack->textKeys[j] = baseName;
                    j++;
                } else {
                    free(baseName); // Clean up base name if text allocation fails
                }

                fclose(text_file);
            }
        } else if (strstr(entry->d_name, ".wav")) {
            drwav wav;
            if (drwav_init_file(&wav, file_path, NULL)) {
                size_t size = wav.totalPCMFrameCount * wav.channels * sizeof(int16_t);
                uint8_t* sound_data = (uint8_t*)malloc(size);

                if (sound_data) {
                    drwav_read_pcm_frames(&wav, wav.totalPCMFrameCount, sound_data);

                    pack->sounds[k].data = sound_data;
                    pack->sounds[k].sampleRate = wav.sampleRate;
                    pack->sounds[k].channels = wav.channels;
                    pack->sounds[k].size = size;
                    pack->soundKeys[k] = baseName;
                    k++;
                } else {
                    free(baseName); // Clean up base name if sound allocation fails
                }

                drwav_uninit(&wav);
            }
        } else {
            free(baseName); // Clean up base name for unrecognized files
        }
    }

    closedir(dir);
    return pack;
}

// Save PAT_ASSETPACK to a binary file
void PAT_SaveAssetPack(const char* output_file, const PAT_ASSETPACK* pack) {
    FILE* file = fopen(output_file, "wb");
    if (!file) {
        perror("PAT : SaveAssetPack : Failed to open file for writing");
        return;
    }
    fwrite(&pack->imageCount, sizeof(int), 1, file);

    for (int i = 0; i < pack->imageCount; ++i) {
        fwrite(&pack->images[i].width, sizeof(int), 1, file);
        fwrite(&pack->images[i].height, sizeof(int), 1, file);
        fwrite(&pack->images[i].channels, sizeof(int), 1, file);
        fwrite(&pack->images[i].size, sizeof(size_t), 1, file);
        fwrite(pack->images[i].data, 1, pack->images[i].size, file);

        if (pack->imageKeys[i] == NULL) {
            fprintf(stderr, "Error: pack->imageKeys[%d] is NULL\n", i);
            fclose(file);
            return; // or handle appropriately
        }

        size_t key_len = strlen(pack->imageKeys[i]) + 1;
        fwrite(&key_len, sizeof(size_t), 1, file);
        fwrite(pack->imageKeys[i], 1, key_len, file);
    }

    fwrite(&pack->textCount, sizeof(int), 1, file);

    for (int i = 0; i < pack->textCount; ++i) {
        fwrite(&pack->texts[i].size, sizeof(size_t), 1, file);
        fwrite(pack->texts[i].data, 1, pack->texts[i].size, file);

        if (pack->textKeys[i] == NULL) {
            fprintf(stderr, "Error: pack->textKeys[%d] is NULL\n", i);
            fclose(file);
            return; // or handle appropriately
        }

        size_t key_len = strlen(pack->textKeys[i]) + 1;
        fwrite(&key_len, sizeof(size_t), 1, file);
        fwrite(pack->textKeys[i], 1, key_len, file);
    }

    fwrite(&pack->soundCount, sizeof(int), 1, file);
    for (int i = 0; i < pack->soundCount; ++i) {
        fwrite(&pack->sounds[i].sampleRate, sizeof(uint32_t), 1, file);
        fwrite(&pack->sounds[i].channels, sizeof(uint32_t), 1, file);
        fwrite(&pack->sounds[i].size, sizeof(size_t), 1, file);
        fwrite(pack->sounds[i].data, 1, pack->sounds[i].size, file);

        if (pack->soundKeys[i] == NULL) {
            fprintf(stderr, "Error: pack->soundKeys[%d] is NULL\n", i);
            fclose(file);
            return; // or handle appropriately
        }

        size_t key_len = strlen(pack->soundKeys[i]) + 1;
        fwrite(&key_len, sizeof(size_t), 1, file);
        fwrite(pack->soundKeys[i], 1, key_len, file);
    }

    fclose(file);
}


// Unpack PAT_ASSETPACK into a directory
void PAT_UnpackAssetPack(const char* input_file, const char* output_dir) {
    struct stat st = {0};
    if (stat(output_dir, &st) == -1) {
        if (mkdir(output_dir, 0700) != 0) {
            perror("PAT : UNPACK : Failed to create output directory");
            return;
        }
    }
    PAT_ASSETPACK* pack = PAT_LoadAssetPack(input_file);
    if (!pack) {
        return;
    }

    for (int i = 0; i < pack->imageCount; ++i) {
        char output_file[256];
        snprintf(output_file, sizeof(output_file), "%s/%s.png", output_dir, pack->imageKeys[i]);
        if (stbi_write_png(output_file, pack->images[i].width, pack->images[i].height, 4, pack->images[i].data, pack->images[i].width * 4) == 0) {
            printf("PAT : UNPACK : Failed to write image %s\n", output_file);
        }
    }

    for (int i = 0; i < pack->textCount; ++i) {
        char output_file[256];
        snprintf(output_file, sizeof(output_file), "%s/%s.txt", output_dir, pack->textKeys[i]);
        FILE* file = fopen(output_file, "wb");
        if (!file) {
            perror("PAT : UNPACK : Failed to open text output file");
            continue;
        }
        fwrite(pack->texts[i].data, 1, pack->texts[i].size, file);
        fclose(file);
    }
    for (int i = 0; i < pack->soundCount; ++i) {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s.wav", output_dir, pack->soundKeys[i]);
        drwav wav;
        drwav_data_format format;
        format.container = drwav_container_riff;
        format.format = DR_WAVE_FORMAT_PCM;
        format.channels = pack->sounds[i].channels;
        format.sampleRate = pack->sounds[i].sampleRate;
        format.bitsPerSample = 16;
        if (drwav_init_file_write(&wav, file_path, &format, NULL)) {
            drwav_write_pcm_frames(&wav, pack->sounds[i].size / (format.channels * sizeof(int16_t)), pack->sounds[i].data);
            drwav_uninit(&wav);
        }
    }
    PAT_FreeAssetPack(pack);
}
#endif
#endif


#ifdef PACK_PROGRAM

#include <getopt.h>

void print_usage() {
    printf("Usage:\n");
    printf("  -p, <input_dir> <output_file>   Pack images and texts from directory into a binary file\n");
    printf("  -u, <input_file> <output_dir>   Unpack images and texts from a binary file into a directory\n");
    printf("  -h,                             Show this help message\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        print_usage();
        return EXIT_FAILURE;
    }
    int option;
    while ((option = getopt(argc, argv, "p:u:h")) != -1) {
        switch (option) {
            case 'p': {
                if (optind >= argc) {
                    print_usage();
                    return EXIT_FAILURE;
                }
                const char* input_dir = optarg;
                const char* output_file = argv[optind];
                PAT_ASSETPACK* pack = PAT_CreateAssetPack(input_dir);
                if (pack) {
                    PAT_SaveAssetPack(output_file, pack);
                    PAT_FreeAssetPack(pack);
                }
                break;
            }
            case 'u': {
                if (optind >= argc) {
                    print_usage();
                    return EXIT_FAILURE;
                }
                const char* input_file = optarg;
                const char* output_dir = argv[optind];
                PAT_UnpackAssetPack(input_file, output_dir);
                break;
            }
            case 'h':
                print_usage();
                return EXIT_SUCCESS;
            default:
                print_usage();
                return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
#endif