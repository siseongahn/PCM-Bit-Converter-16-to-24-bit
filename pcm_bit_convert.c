/*
    
pcm_bit_converter.c

Convert 16bit PCM wav file to 24/32bit PCM wav file

Authors  : Simon, Siseong Ahn
Contact : siseong@gmail.com

Copyright 2021, Siseong Ahn (siseong@gmail.com)
The MIT License
Permission is hereby granted, free of charge, to any person obtaining 
a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom 
the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included 
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR 
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR 
OTHER DEALINGS IN THE SOFTWARE.
*/

/* Wav file channel define
--------------------------------------------------------------------------- 
 [Channel]    : location setup
-------------------------------------------------------------------------- 
 - streo      : [left][right]
 - 3 channel  : [left][right][center]
 - quad       : [front left][front right][rear left][reat right]
 - 4 channel  : [left][center][right][surround]
 - 6 channel  : [left center][left][center][right center][right][surround]
--------------------------------------------------------------------------- 
*/

#include "pcm_bit_convert.h"

/* wav chuck data format */
#define WAVE_FORMAT_UNKNOWN 0X0000;
#define WAVE_FORMAT_PCM 0X0001;
#define WAVE_FORMAT_MS_ADPCM 0X0002;
#define WAVE_FORMAT_IEEE_FLOAT 0X0003;
#define WAVE_FORMAT_ALAW 0X0006;
#define WAVE_FORMAT_MULAW 0X0007;
#define WAVE_FORMAT_IMA_ADPCM 0X0011;ß
#define WAVE_FORMAT_YAMAHA_ADPCM 0X0016;
#define WAVE_FORMAT_GSM 0X0031;
#define WAVE_FORMAT_ITU_ADPCM 0X0040;
#define WAVE_FORMAT_MPEG 0X0050;
#define WAVE_FORMAT_EXTENSIBLE 0XFFFE;

typedef struct { 
    unsigned char ChunkID[4];       // Contains the letters "RIFF" in ASCII form 
    unsigned int ChunkSize;         // This is the size of the rest of the chunk following this number 
    unsigned char Format[4];        // Contains the letters "WAVE" in ASCII form 
} RIFF; 


typedef struct { 
    unsigned char ChunkID[4];       // Contains the letters "fmt " in ASCII form 
    unsigned int ChunkSize;         // 16 for PCM. This is the size of the rest of the Subchunk which follows this number. 
    unsigned short AudioFormat;     // PCM = 0x01 
    unsigned short NumChannels;     // Mono = 1, Stereo = 2, etc. 
    unsigned int SampleRate;        // 8000, 44100, 48000Hz etc. 
    unsigned int AvgByteRate;       // SampleRate * NumChannels * BitsPerSample / 8 
    unsigned short BlockAlign;      // NumChannels * BitsPerSample / 8 
    unsigned short BitPerSample;    // 8 bits = 8, 16 bits = 16, 32 bits = 32 
} FMT; 
    
typedef struct { 
    char ChunkID[4];                // Contains the letters "data" in ASCII form 
    unsigned int ChunkSize;         // NumSamples * NumChannels * BitsPerSample / 8 
} DATA; 

typedef struct  { 
    RIFF Riff;
    FMT Fmt;
    DATA Data;
} WAVE_HEADER;

char* read_raw_pcm (const char *p, size_t *bytes_read)
{
	size_t lSize;
	char *pcm_buf;
	size_t result;
	FILE *fp_pcm;
	
	fp_pcm = fopen (p, "rb");
	if (fp_pcm == NULL) {
		printf("File open error\n");
		return NULL;
	}
	
	// obtain file size:
	fseek (fp_pcm , 0 , SEEK_END);
	lSize = ftell (fp_pcm);
	rewind (fp_pcm);
	
	// allocate memory to contain the whole file:
	pcm_buf = (char*) malloc (sizeof(char) * lSize);
	if (pcm_buf == NULL) {
		printf ("Memory allocation error\n");
		return NULL;
	}
	
	// copy the file into the pcm_buf:
	result = fread (pcm_buf, 1, lSize, fp_pcm);
	if (result != lSize) {
		printf ("Wave file reading error\n");
        free(pcm_buf);
		return NULL;
	}
	*bytes_read = lSize;
    return pcm_buf;	
}

int convert_pcm_bit(const char* scr_path, const char* out_path, int sample_bit)
{
	size_t raw_size = 0;
    int channel = 0;
    int sample_rate = 0;
    
    // read little endian 16 bit PCM
	char *pcm_buf = read_raw_pcm(scr_path, &raw_size);
    if (pcm_buf == NULL) return -1;     
    
    WAVE_HEADER scr_header;
    memcpy(&scr_header, pcm_buf, sizeof(scr_header)); 

    if (scr_header.Fmt.BitPerSample != 16) 
        return 1;

    channel = scr_header.Fmt.NumChannels;
    sample_rate = scr_header.Fmt.SampleRate;

    printf("scr_channel     : %d\n", channel);
    printf("scr_sample bit  : %d\n", scr_header.Fmt.BitPerSample);
    printf("scr_sample rate : %d\n", sample_rate);
    printf("scr_pcm size    : %lu\n", raw_size);

    FILE *f_out; 
    f_out = fopen(out_path, "wb");
    
    size_t out_size = (size_t)floor((double)raw_size * 1.5f);
    if (sample_bit == 24)
        // 16 bit to 24 bit so out size 1.5 times than src size(round up)
        out_size = (size_t)floor((double)raw_size * 1.5f);
    else if (sample_bit == 32)
        // 16 bit to 32 bit so out size 2 times than src size
        out_size = raw_size * 2;
    else 
        return 1;

    int sample_byte_size = sample_bit / 8; 
    WAVE_HEADER out_header; 
    memcpy(out_header.Riff.ChunkID, "RIFF", 4); 
    out_header.Riff.ChunkSize =  out_size + 36; 
    memcpy(out_header.Riff.Format, "WAVE", 4); 
    memcpy(out_header.Fmt.ChunkID, "fmt ", 4); 
    
    out_header.Fmt.ChunkSize = 0x10; 
    out_header.Fmt.AudioFormat = WAVE_FORMAT_PCM; 
    out_header.Fmt.NumChannels = channel; 
    out_header.Fmt.SampleRate = sample_rate; 
    out_header.Fmt.AvgByteRate = sample_rate * channel * sample_byte_size; 
    out_header.Fmt.BlockAlign = channel * sample_byte_size; 
    out_header.Fmt.BitPerSample = sample_bit; 
    memcpy(out_header.Data.ChunkID, "data", 4); 
    // Chunk Size calcuration : DURATION * SAMPLE_RATE * CHANNEL * SAMPLE_BIT / 8; 
    out_header.Data.ChunkSize = out_size; // Should be same out_size 
    size_t header_size = sizeof(out_header);

    fwrite(&out_header, header_size, 1, f_out); 

    printf("\nout_channel     : %d\n", channel);
    printf("out_sample bit  : %d\n", out_header.Fmt.BitPerSample);
    printf("out_sample rate : %d\n", sample_rate);
    printf("out_pcm size    : %lu\n", out_size);

    char y[4]; // 32bit frame size 
    size_t p = header_size; // skip header data from source file dump
    size_t frame_size = (raw_size - header_size) / 2; 
    
    for(size_t i = 0; i < frame_size; i++) { // shift 16 bit data to 32 bit, Little endian format
        
        // little endian 
        memset(y, 0x00, 4);
        memcpy(y+(sample_byte_size-2), pcm_buf+p, 2);
        // or
        //y[2] = pcm_buf[p+1];
        //y[1] = pcm_buf[p];
        //y[0] = 0x00;
        //y[0] = 0x00;

        // big endian 32bit
        //y[0] = pcm_buf[p+1];
        //y[1] = pcm_buf[p];
        //y[2] = 0x00;
        //y[3] = 0x00;
    
        fwrite(y, sample_byte_size, 1, f_out);
        p += 2; 
    } 
    fclose(f_out);
    free(pcm_buf);

    return 0;
}
