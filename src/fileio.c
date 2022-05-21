/*
 * Copyright (c) 2008-2011, Michael Kohn
 * Copyright (c) 2013, Robin Hahling
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of the author nor the names of its contributors may be
 *   used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Usefull IO functions.
 */

#include <stdio.h>

int
write_int(FILE *out, unsigned int n)
{
	unsigned char buffer[4];

	buffer[0] = (0xff & (n));
	buffer[1] = (0xff & (n >> 8));
	buffer[2] = (0xff & (n >> 16));
	buffer[3] = (0xff & (n >> 24));

	if (fwrite(buffer, 1, 4, out) != 4)
		return -1;

	return 0;
}

int
write_short(FILE *out, unsigned int n)
{
	unsigned char buffer[2];

	buffer[0] = (0xff & (n));
	buffer[1] = (0xff & (n >> 8));

	if (fwrite(buffer, 1, 2, out) != 2)
		return -1;

	return 0;
}

int
write_chars(FILE *out, const char *s)
{
	int t = 0;

	while(s[t] != 0 && t < 255)
		if (fputc(s[t++], out) == EOF)
			return -1;

	return 0;
}

int
write_chars_bin(FILE *out, const char *s, size_t count)
{
	if (fwrite(s, 1, count, out) != count)
		return -1;

	return 0;
}

