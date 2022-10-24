

#include "headers/generate.hpp"


int main (int argc, char** argv)
{
    if (argc >  2) { printf ("enter correct pic file name while starting the program"); return 0; }
    if (argc == 1) {

        Return_code return_code = generate ();
        printf ("%d", return_code);
        return 0;
    }

    char file_name [MAX_FILE_NAME_LEN + 1] = "work/";
    strcat (file_name, argv[1]);
    strcat (file_name, ".txt");
    Return_code return_code = generate (file_name);
    printf ("%d", return_code);


    return 0;
}
