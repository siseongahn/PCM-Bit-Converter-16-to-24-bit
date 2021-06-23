/*
    
pcm_bit_converter.h

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

#ifndef PCM_BIT_CONVERTER_H
#define PCM_BIT_CONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

char* read_raw_pcm (const char *p, size_t *bytes_read);
int convert_pcm_bit(const char* scr_path, const char* out_path, int sample_bit);

#endif // PCM_BIT_CONVERTER_H