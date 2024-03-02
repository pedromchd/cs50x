#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
typedef char *string;

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover input.raw\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    int filecount = -1;
    string filename = malloc(8 * sizeof(char));

    BYTE buffer[BLOCK_SIZE];
    FILE *output;
    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, input) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (filecount != -1)
            {
                fclose(output);
            }

            sprintf(filename, "%03i.jpg", ++filecount);
            output = fopen(filename, "w");
        }
        if (filecount != -1)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, output);
        }
    }

    free(filename);

    fclose(input);
    fclose(output);
}
