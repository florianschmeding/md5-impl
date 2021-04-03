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
#include "md5.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("usage: md5 <filename>");
        return 1;
    }

    md5_init();

    FILE* file;

    file = fopen(argv[1], "rb");
    if(file == NULL)
    {
        printf("md5: file \"%s\" not found\n", argv[1]);
    }
    else
    {
        char* md5 = md5_of_file(file);
        if(md5 == NULL)
        {
            fprintf(stderr, "there has been an error at the md5 calculation\n");
        }
        else
        {
            printf("%s  %s\n", md5, argv[1]);
            free(md5);
        }
        fclose(file);
    }

    return 0;
}
