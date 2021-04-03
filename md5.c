/*   
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

uint32_t K[64];

uint32_t A = 0x67452301;
uint32_t B = 0xefcdab89;
uint32_t C = 0x98badcfe;
uint32_t D = 0x10325476;
uint32_t AA, BB, CC, DD;

short shift[64] = {
    7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
    5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
    4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
    6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21};

short word_table[48] = {
    1, 6, 11,  0,  5, 10, 15,  4,  9, 14, 3,  8, 13,  2,  7, 12,
    5, 8, 11, 14,  1,  4,  7, 10, 13,  0, 3,  6,  9, 12, 15,  2,
    0, 7, 14,  5, 12,  3, 10,  1,  8, 15, 6, 13,  4, 11,  2,  9};

struct roundInfoStruct{
    uint32_t* A;
    uint32_t* B;
    uint32_t* C;
    uint32_t* D;
}rIS[4] = {{&A, &B, &C, &D},
           {&D, &A, &B, &C},
           {&C, &D, &A, &B},
           {&B, &C, &D, &A}};

uint32_t F(uint32_t B, uint32_t C, uint32_t D)
{
    return (B & C) | (~B & D);
}

uint32_t G(uint32_t B, uint32_t C, uint32_t D)
{
    return (B & D) | (C & ~D);
}

uint32_t H(uint32_t B, uint32_t C, uint32_t D)
{
    return B ^ C ^ D;
}

uint32_t I(uint32_t B, uint32_t C, uint32_t D)
{
    return C ^ (B | ~D);
}

uint64_t leftrotate(uint64_t x, uint64_t c){
    return ((x) << (c)) | ((x) >> (32 - (c)));
}

double abs_double(double number)
{
    if(number >= 0)
    {
        return number;
    }
    else
    {
        return number * (double)-1;
    }
}

void do_round(uint8_t num, uint32_t word, short shift, int K, uint32_t (*ptr_Func)(uint32_t, uint32_t, uint32_t))
{
    *rIS[num].A = *rIS[num].B + (leftrotate((*rIS[num].A + ptr_Func(*rIS[num].B, *rIS[num].C, *rIS[num].D) + word + K), shift));
}

void md5_init() // Inits the table
{
    int i;

    for(i = 0; i < 64; i++)
    {
        K[i] = (abs_double(sin(i + 1)) * 4294967296ULL);
    }
}

char* registers_to_hex(uint32_t A, uint32_t B, uint32_t C, uint32_t D) // print with low order first
{
    char *hex_result = (char*) malloc(33);
    snprintf(hex_result,      9, "%02x%02x%02x%02x", (uint8_t)A, (uint8_t)(A >> 8), (uint8_t)(A >> 16), (uint8_t)(A >> 24));
    snprintf(hex_result + 8,  9, "%02x%02x%02x%02x", (uint8_t)B, (uint8_t)(B >> 8), (uint8_t)(B >> 16), (uint8_t)(B >> 24));
    snprintf(hex_result + 16, 9, "%02x%02x%02x%02x", (uint8_t)C, (uint8_t)(C >> 8), (uint8_t)(C >> 16), (uint8_t)(C >> 24));
    snprintf(hex_result + 24, 9, "%02x%02x%02x%02x", (uint8_t)D, (uint8_t)(D >> 8), (uint8_t)(D >> 16), (uint8_t)(D >> 24));
    return hex_result;
}

char* md5_of_file(FILE* file)
{
    if(file != NULL)
    {
        // Get the size of the file
        fseek(file, 0, SEEK_END);
        uint64_t length = ftell(file);
        rewind(file);

        int num_of_512b_blocks;
        if(length % 64 > 55)
        {
            num_of_512b_blocks = ceil((float)length / 64) + 1;
        }
        else
        {
            num_of_512b_blocks = length / 64 + 1;
        }

        uint8_t* buffer = (uint8_t*)calloc(num_of_512b_blocks * 64, 1);

        fread(buffer, length, 1, file);

        buffer[length] = 128; // padding start (10000000)

        // Append the length
        int j;
        for(j = 0; j < 8; j++)
        {
            buffer[56 + (num_of_512b_blocks - 1) * 64 + j] = (uint8_t)(length * 8 >> j * 8);
        }

        uint32_t* buffer512bit = (uint32_t*)malloc(64);

        for(j = 0; j < num_of_512b_blocks; j++) // main hashloop
        {
            memcpy(buffer512bit, buffer + j * 64, 64);

            AA = A;
            BB = B;
            CC = C;
            DD = D;

            int k;
            for(k = 0; k < 64; k++)
            {
                if(k < 16)
                {
                    do_round(k % 4, buffer512bit[k], shift[k], K[k], &F);
                }
                else if(k < 32)
                {
                    do_round(k % 4, buffer512bit[word_table[k-16]], shift[k], K[k], &G);
                }
                else if(k < 48)
                {
                    do_round(k % 4, buffer512bit[word_table[k-16]], shift[k], K[k], &H);
                }
                else
                {
                    do_round(k % 4, buffer512bit[word_table[k-16]], shift[k], K[k], &I);
                }
            }

            A = A + AA;
            B = B + BB;
            C = C + CC;
            D = D + DD;
        }
        free(buffer512bit);
        free(buffer);

        return registers_to_hex(A, B, C, D);
    }

    return NULL;
}
