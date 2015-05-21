#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __x86_64__
#define ELFHDR Elf64_Ehdr
#define ELFPHDR Elf64_Phdr
#define ELFSHDR Elf64_Shdr
#define ELFSYM Elf64_Sym
#define ELFSTBIND ELF64_ST_BIND
#define ELFSTTYPE ELF64_ST_TYPE
#define ARCHCLASS ELFCLASS64
#else //We assume this code will run in i386 system. Sorry non-intel guys!
#define ELFHDR Elf32_Ehdr
#define ELFPHDR Elf32_Phdr
#define ELFSHDR Elf32_Shdr
#define ELFSYM Elf32_Sym
#define ELFSTBIND ELF32_ST_BIND
#define ELFSTTYPE ELF32_ST_TYPE
#define ARCHCLASS ELFCLASS32
#endif

typedef struct {
  size_t size;
  int offset;
  void *sectiondata;
} section_t;

section_t *findandloadsection(int fd, ELFHDR *elfheader, ELFSHDR *elfsheader, char *shstrtab, char *name) {
  int i;
  section_t *ret;
  
  for(i = 0; i < elfheader -> e_shnum; i++) {
    if(strcmp(shstrtab + elfsheader[i].sh_name, name) == 0) {
      break;
    }
  }

  assert(i != elfheader -> e_shnum);

  ret = malloc(sizeof(section_t));
  ret -> sectiondata = malloc(elfsheader[i].sh_size);
  ret -> size = elfsheader[i].sh_size;
  ret -> offset = elfsheader[i].sh_offset;

  assert(lseek(fd, elfsheader[i].sh_offset, SEEK_SET) == elfsheader[i].sh_offset);
  assert(read(fd, ret -> sectiondata, elfsheader[i].sh_size) == elfsheader[i].sh_size);

  return ret;
}

void deletesection(section_t *section) {
  free(section -> sectiondata);
  free(section);
}

int main(int argc, char **argv) {

  int fd;
  ELFHDR elfheader;
  int i;

  fd = open("answerofeverything.o", O_RDONLY);
  //Read ELF header.
  assert(fd != -1);
  assert(read(fd, &elfheader, sizeof(ELFHDR)) == sizeof(ELFHDR));

  //Are you using same architecture class?
  assert(elfheader.e_ident[4] == ARCHCLASS);

  ELFPHDR elfpheader[elfheader.e_phnum];
  ELFSHDR elfsheader[elfheader.e_shnum];

  printf("ELF EHDR\ne_phnum = %d\ne_shnum = %d\ne_phentsize = %d\ne_shentsize = %d\ne_phoff = %d\ne_shoff = %d\n",
    elfheader.e_phnum,
    elfheader.e_shnum,
    elfheader.e_phentsize,
    elfheader.e_shentsize,
    elfheader.e_phoff,
    elfheader.e_shoff);

  for(i = 0; i < elfheader.e_phnum; i++) {
    assert(lseek(fd, elfheader.e_phoff + i * elfheader.e_phentsize, SEEK_SET)
           == elfheader.e_phoff + i * elfheader.e_phentsize);
    assert(read(fd, &elfpheader[i], elfheader.e_phentsize) == elfheader.e_phentsize);
  }

  for(i = 0; i < elfheader.e_shnum; i++) {
    assert(lseek(fd, elfheader.e_shoff + i * elfheader.e_shentsize, SEEK_SET)
           == elfheader.e_shoff + i * elfheader.e_shentsize);
    assert(read(fd, &elfsheader[i], elfheader.e_shentsize) == elfheader.e_shentsize);
  }

  assert(elfheader.e_shstrndx != SHN_UNDEF);

  assert(lseek(fd, elfsheader[elfheader.e_shstrndx].sh_offset, SEEK_SET)
         == elfsheader[elfheader.e_shstrndx].sh_offset);

  char shstrtab[elfsheader[elfheader.e_shstrndx].sh_size];

  assert(read(fd, shstrtab, elfsheader[elfheader.e_shstrndx].sh_size)
         == elfsheader[elfheader.e_shstrndx].sh_size);
  
  //Find and load .strtab
  section_t *strtab = findandloadsection(fd, &elfheader, elfsheader, shstrtab, ".strtab");
  
  //Find and load .symtab
  section_t *symtab = findandloadsection(fd, &elfheader, elfsheader, shstrtab, ".symtab");

  //Find and load .text
  section_t *text = findandloadsection(fd, &elfheader, elfsheader, shstrtab, ".text");

  ELFSYM *symtable = symtab -> sectiondata;

  
  //get first function offset
  for(i = 0; i < symtab -> size / sizeof(ELFSYM); i++) {
    if(ELFSTTYPE(symtable[i].st_info) == STT_FUNC) {
      break;
    }
  }

  //Load file into memory.
  void *code;
    
  struct stat stat_buf;
  assert(fstat(fd, &stat_buf) == 0);

  //WARN: This mmap request will fail on grsecurity PaX.
  code = mmap(0, stat_buf.st_size < 4096 ? 4096 : stat_buf.st_size, PROT_EXEC | PROT_WRITE | PROT_READ, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

  //This code will not work on AMD64.
  //AMD64 and PAE enabled machines will not execute page with NX-bit

  //code = malloc(stat_buf.st_size < 4096 ? 4096 : stat_buf.st_size);

  assert(code != 0);

  assert(lseek(fd, 0, SEEK_SET) == 0);
  assert(read(fd, code, stat_buf.st_size) == stat_buf.st_size);

  int (*answerofeverything)(void) = code + text -> offset + symtable[i].st_value;

  printf("ELF loaded at      : %p\n", code);
  printf("Function loaded at : %p\n", answerofeverything);

  printf("%d\n", answerofeverything());

  deletesection(strtab);
  deletesection(symtab);
  deletesection(text);

  return 0;
}

