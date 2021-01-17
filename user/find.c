#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int checkFile(char *directory, char *file)
{
    char *ptr;
    ptr = directory + strlen(directory);
    while (ptr >= directory && *ptr != '/')
    {
        ptr--;
    }
    ptr++;
    if (strcmp(ptr, file) == 0)
    {
        return 1;
    }
    return 0;
}

void find(char *directory, char *file)
{
    char buf[64], *p;
    int fd;
    struct stat st;
    struct dirent de;

    if ((fd = open(directory, 0)) < 0)
    {
        fprintf(2, "<find> cannot open %s\n", directory);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "<find> cannot run stat %s\n", directory);
        close(fd);
        return;
    }

    if (st.type == T_FILE)
    {
        if (checkFile(directory, file))
        {
            printf("%s\n", directory);
        }
    }
    else if (st.type == T_DIR)
    {
        if (strlen(directory) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: directory is too long\n");
            return;
        }
        strcpy(buf, directory);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            //   if(de.inum == 0)
            //     continue;
            //   if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            //     continue;

            if ((de.inum == 0) || (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0))
            {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buf, file);
        }
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        exit();
    }
    find(argv[1], argv[2]);
    exit();
}