#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <operation> <operand1> <operand2>\n", argv[0]);
        return 1;
    }

    printf("exec_math_bs pid:%d\n", getpid());

      int fd = open("math.c", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    printf("fd for math.c file:%d\n", fd);

      char program[1000];
    snprintf(program, sizeof(program),
        "#include <stdio.h>\n"
        "#include <stdlib.h>\n"
        "#include <unistd.h>\n"
        "#include <string.h>\n"
        "int main(int argc, char *argv[]) {\n"
        "    printf(\"math pid: %%d\\n\", getpid());\n"
        "    int a = atoi(argv[1]);\n"
        "    int b = atoi(argv[2]);\n"
        "    int result;\n"
        "    if (strcmp(\"%s\", \"+\") == 0) {\n"
        "        result = a + b;\n"
        "    } else if (strcmp(\"%s\", \"-\") == 0) {\n"
        "        result = a - b;\n"
        "    } else if (strcmp(\"%s\", \"*\") == 0) {\n"
        "        result = a * b;\n"
        "    } else if (strcmp(\"%s\", \"/\") == 0) {\n"
        "        result = a / b;\n"
        "    }\n"
        "    printf(\"%%d %s %%d = %%d\\n\", a, b, result);\n"
        "    return 0;\n"
        "}\n",
        argv[1], argv[1], argv[1], argv[1], argv[1]);

    write(fd, program, strlen(program));
    close(fd);

      pid_t pid = fork();
    
    if (pid == 0) {
        execlp("gcc", "gcc", "math.c", "-o", "math", NULL);
        perror("execlp gcc");
        exit(1);
    } else {
         int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            fd = open("math", O_RDONLY);
            if (fd == -1) {
                perror("open math");
                return 1;
            }
            
            char *newargv[] = {"math", argv[2], argv[3], NULL};
            char *newenviron[] = {NULL};
            
            fexecve(fd, newargv, newenviron);
            perror("fexecve"); 
            close(fd);
        }
    }

    return 0;
}
