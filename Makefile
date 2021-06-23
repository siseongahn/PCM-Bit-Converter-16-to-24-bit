all:
	gcc main.c pcm_bit_convert.c -o wavbitconver
clean:
	rm wavbitconver
