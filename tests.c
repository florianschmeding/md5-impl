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
#include "md5.h"

char* tempfile = "tempfile.bin";

int run_test(char* to_hash, char* expected_md5) {
    FILE* file = fopen(tempfile, "wb");

    if(file == NULL)
    {
        printf("test failure, unable to open tempfile in writemode");
        return -1;
    }
    fprintf(file, "%s", to_hash);
    fclose(file);

    file = fopen(tempfile, "rb");
    if(file == NULL)
    {
        printf("test failure, unable to open tempfile in readmode");
        return -1;
    }
    char* md5 = md5_of_file(file);
    fclose(file);
    if(md5 == NULL)
    {
        fprintf(stderr, "there has been an error at the md5 calculation\n");
        return -1;
    }
    if(strcmp(expected_md5, md5) != 0) {
        printf("test failure:\n expected md5 hash '%s'\n doesn't match the calculated hash '%s'\n for input value '%s'\n",
               expected_md5, md5, to_hash);
        return -1;
    }
    printf("calculated hash '%s'\n for input value '%s'\n is [valid]\n",
               md5, to_hash);
    remove(tempfile);
    return 0;
}

int main(int argc, char* argv[])
{
    md5_init();

    run_test("",                           "d41d8cd98f00b204e9800998ecf8427e");
    run_test("a",                          "0cc175b9c0f1b6a831c399e269772661");
    run_test("abc",                        "900150983cd24fb0d6963f7d28e17f72");
    run_test("message digest",             "f96b697d7cb7938d525a2f31aaf161d0");
    run_test("abcdefghijklmnopqrstuvwxyz", "c3fcd3d76192e4007dfb496cca67e13b");
    run_test("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
             "d174ab98d277d9f5a5611c2c9f419d9f");
    run_test("12345678901234567890123456789012345678901234567890123456789012345678901234567890",
             "57edf4a22be3c955ac49da2e2107b67a");

    return 0;
}
