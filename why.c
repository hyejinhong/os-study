#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
  char data[100][100];

  FILE* file;
  file = fopen("song.txt", "r");
  int i = 0;

  if(file != NULL) {

    while(!feof(file)) {
      fgets(data[i], sizeof(data[i]), file);
      i++;
      // pstr = fgets(temp, sizeof(temp), pfile);
      // song[j++] = pstr;
      // printf("%s\n", pstr);
      // song[j++] = fgets(temp, sizeof(temp), pfile);
    }
    fclose(file);
  }

  for(i=0; i<25; i++) {
    printf("%d) %s\n", i, data[i]);
  }

  return 0;
}
