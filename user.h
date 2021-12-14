struct stat;
struct rtcdate;

// system calls
int fork(void);
int exit(void) __attribute__((noreturn));
int wait(void);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);
int getch(void);
// TODO: Declare your user APIs for your system calls.
int setvga(int vgamode);
int setpixel(int x, int y, int color);
int clear(void);
int render(void* buffer, int callscount);
int present(void);

// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void printf(int, const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);

void drawline(int x0, int y0, int x1, int y1, int color);
void begingraphics();
void endgraphics();
void bufferpixel(int x, int y, int color);
void bufferline(int x0, int y0, int x1, int y1, int color);
void buffercircle(int x, int y, int radius, int color);
void bufferrect(int x0, int y0, int x1, int y1, int color);
void bufferbox(int x0, int y0, int sizex, int sizey, int color);
void bufferskew(int amount);