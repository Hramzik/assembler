#include <stdio.h>
#include <string.h>

void  func  (void* array, void filler, size_t size) {

    memcpy (array, &filler, size);


    return;
}

int main () {

    int a [1];
    int b = 5;
    func (a, b, sizeof (int));
}