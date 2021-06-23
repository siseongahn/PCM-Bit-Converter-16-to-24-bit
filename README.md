Convert 16bit PCM wav file to 24/32bit PCM wav file


1. Build method

gcc linux / MacOS supported.

run Makefile to build

% make

will found exex binary 'wavbitconver'



2. Usage example:

% wavbitconver sine_16_48000.wav out.wav 32

32bit PCM converted out.wav file should be creared.


3. Files

main.c              : main entery point c source file
pcm_bit_convert.h   : converted c source header file
pcm_bit_convert.c   : converted c source file
Makefile            : build script Makefile
sine_16_48000.wav   : PCM 16bit mono(1 channel) sine wave 440 Hz sample file  


4. License

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
