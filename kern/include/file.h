/*
 * Declarations for file handle and file table management.
 * 
 * Overview of the file structure:
 * -Process
 *   - Every process has a fixed size array containing pointers to open
 *     files struct
 *   - The file descriptor(fd) would be the index of the array
 *   - The same pointer can be shared among different processes
 * 
 * Instead of having a global open file structure, a openfile struct pointer 
 * is in place. Because the pointers are kmalloced it can shared among different
 * processes. A ref_count is in place to track how many processes are referencing 
 * this pointer.
 *  
 * fork() should copy the entire open file table from parents over 
 * to child so that both parents and child share the same openfile pointer 
 * 
 */

#ifndef _FILE_H_
#define _FILE_H_

/*
 * Contains some file-related maximum length constants
 */
#include <limits.h>
#define OPEN_MAX __OPEN_MAX

/** 
 * File pointer structure 
 * 
 * newFP   - Create a pointer to a new file pointer
 */
typedef struct filePointer{
    off_t         pos;    //position of the file pointer
    unsigned int read;
    unsigned int write;
} FP;

FP *newFP(int flag);

/** 
 * Open File structure 
 * 
 * newOP   - Create a new open file pointer
*/
typedef struct openfile {
    int          ref_count; //count how many open file pointers refer to this struct
    FP           *fp;       //Pointer to a file pointer
    struct vnode *vnode;    //Pointer to a vnode
} OP;

OP *newOP(FP *fp, struct vnode *vnode);

/**
 * User-level File functions.
 * 
 * open     - Open or create a file 
 * close    - Close a file
 * 
 * read     - Read data from a file
 * write    - Write data to a file
 * 
 * lseek    - seek to a position in a file
 * dup2     - clone file handles
 */

int sys_open(char *filename, int flags, mode_t mode); //The last argument is permission 
int sys_close(int fd);

ssize_t sys_read(int fd, void *buf, size_t buflen);
ssize_t sys_write(int fd, const void *buf, size_t nbytes);

off_t sys_lseek(int fd, off_t pos, int whence); //whence is where you start
int sys_dup2(int oldfd, int newfd);


#endif /* _FILE_H_ */
