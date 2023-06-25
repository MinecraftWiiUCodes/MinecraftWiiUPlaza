typedef unsigned char   undefined;

typedef unsigned char    byte;
typedef unsigned int    dword;
typedef unsigned int    uint;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef short    wchar_t;
typedef unsigned short    word;
typedef unsigned short    wchar16;
typedef struct menu_t menu_t, *Pmenu_t;

struct menu_t {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    int field_0x4;
    int field_0x8;
};

typedef struct Elf32_Shdr Elf32_Shdr, *PElf32_Shdr;

typedef enum Elf_SectionHeaderType_PPC {
    SHT_ANDROID_REL=1610612737,
    SHT_ANDROID_RELA=1610612738,
    SHT_CHECKSUM=1879048184,
    SHT_DYNAMIC=6,
    SHT_DYNSYM=11,
    SHT_FINI_ARRAY=15,
    SHT_GNU_ATTRIBUTES=1879048181,
    SHT_GNU_HASH=1879048182,
    SHT_GNU_LIBLIST=1879048183,
    SHT_GNU_verdef=1879048189,
    SHT_GNU_verneed=1879048190,
    SHT_GNU_versym=1879048191,
    SHT_GROUP=17,
    SHT_HASH=5,
    SHT_INIT_ARRAY=14,
    SHT_NOBITS=8,
    SHT_NOTE=7,
    SHT_NULL=0,
    SHT_PREINIT_ARRAY=16,
    SHT_PROGBITS=1,
    SHT_REL=9,
    SHT_RELA=4,
    SHT_SHLIB=10,
    SHT_STRTAB=3,
    SHT_SUNW_COMDAT=1879048187,
    SHT_SUNW_move=1879048186,
    SHT_SUNW_syminfo=1879048188,
    SHT_SYMTAB=2,
    SHT_SYMTAB_SHNDX=18
} Elf_SectionHeaderType_PPC;

struct Elf32_Shdr {
    dword sh_name;
    enum Elf_SectionHeaderType_PPC sh_type;
    dword sh_flags;
    dword sh_addr;
    dword sh_offset;
    dword sh_size;
    dword sh_link;
    dword sh_info;
    dword sh_addralign;
    dword sh_entsize;
};

typedef struct Elf32_Phdr Elf32_Phdr, *PElf32_Phdr;

typedef enum Elf_ProgramHeaderType_PPC {
    PT_DYNAMIC=2,
    PT_GNU_EH_FRAME=1685382480,
    PT_GNU_RELRO=1685382482,
    PT_GNU_STACK=1685382481,
    PT_INTERP=3,
    PT_LOAD=1,
    PT_NOTE=4,
    PT_NULL=0,
    PT_PHDR=6,
    PT_SHLIB=5,
    PT_TLS=7
} Elf_ProgramHeaderType_PPC;

struct Elf32_Phdr {
    enum Elf_ProgramHeaderType_PPC p_type;
    dword p_offset;
    dword p_vaddr;
    dword p_paddr;
    dword p_filesz;
    dword p_memsz;
    dword p_flags;
    dword p_align;
};

typedef struct Elf32_Ehdr Elf32_Ehdr, *PElf32_Ehdr;

struct Elf32_Ehdr {
    byte e_ident_magic_num;
    char e_ident_magic_str[3];
    byte e_ident_class;
    byte e_ident_data;
    byte e_ident_version;
    byte e_ident_osabi;
    byte e_ident_abiversion;
    byte e_ident_pad[7];
    word e_type;
    word e_machine;
    dword e_version;
    dword e_entry;
    dword e_phoff;
    dword e_shoff;
    dword e_flags;
    word e_ehsize;
    word e_phentsize;
    word e_phnum;
    word e_shentsize;
    word e_shnum;
    word e_shstrndx;
};




undefined4 entry(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  undefined4 unaff_r30;
  undefined4 unaff_r31;
  longlong lVar3;
  
  initialize_os_specific_functions();
  FUN_008080c8();
  FUN_00808844();
  FUN_00806f44();
  FUN_008090a0();
  if ((DAT_0081e40c != 0) &&
     (((lVar3 = (*OSGetTitleID)(), lVar3 == 0x500101004a100 ||
       (lVar3 = (*OSGetTitleID)(), lVar3 == 0x500101004a000)) ||
      (lVar3 = (*OSGetTitleID)(), lVar3 == 0x500101004a200)))) {
    return 0;
  }
  DAT_0081e40c = DAT_0081e40c + 1;
  FUN_00806e48();
  FUN_00806a34(4,0x6000);
  FUN_008065ac();
  print_to_log_2("\n\n\n==================\nGameHackingHelper by NexoCube\n");
  print_to_log_2("We\'re started ...\n\n");
  print_to_log_2("Creating our background thread..\n",param_2);
  iVar1 = FUN_00806b70(0x1000,0x10);
  uVar2 = FUN_00806b70(0x1000,0x10);
  iVar1 = (*DAT_0081c980)(uVar2,FUN_00805c60,0,0,iVar1 + 0x1000,0x1000,0,0x1c,unaff_r30,unaff_r31);
  if (iVar1 == 0) {
    print_to_log_2("Couldn\'t create our thread, exiting ...\n");
    FUN_00802538("Couldn\'t create our thread, exiting ..");
    (*OSSleepTicks)(0,0x768dbb4);
    return 0;
  }
  lVar3 = (*OSGetTitleID)();
  if (((lVar3 != 0x500101004a100) && (lVar3 = (*OSGetTitleID)(), lVar3 != 0x500101004a000)) &&
     (lVar3 = (*OSGetTitleID)(), lVar3 != 0x500101004a200)) {
    (*DAT_0081c9b4)(uVar2,"GameHackingThread");
    (*DAT_0081c984)(uVar2);
    return 0xfffffffd;
  }
  print_to_log("We\'re in the Homebrew Channel, abort.");
  (*DAT_0081c8c8)(0x50000,0x101d7500);
  return 0xfffffffd;
}



void FUN_0080214c(int address,int value)

{
  undefined4 physical_address;
  
  DAT_0081b7a8._0_1_ = (undefined)value;
  physical_address = (*OSEffectiveToPhysical)();
  write_memory(physical_address,&DAT_0081b7a8,1);
                    // WARNING: Could not recover jumptable at 0x008021b4. Too many branches
                    // WARNING: Treating indirect jump as call
  (*DCFlushRange)(address,1);
  return;
}



void patch_memory(int address,int value)

{
  undefined4 physical_address;
  
  DAT_0081b7a8 = value;
  physical_address = (*OSEffectiveToPhysical)();
  write_memory(physical_address,&DAT_0081b7a8,4);
                    // WARNING: Could not recover jumptable at 0x00802220. Too many branches
                    // WARNING: Treating indirect jump as call
  (*DCFlushRange)(address,4);
  return;
}



void FUN_00802224(double param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  DAT_0081b7a8 = (float)param_1;
  uVar1 = (*OSEffectiveToPhysical)();
  write_memory(uVar1,&DAT_0081b7a8,4);
                    // WARNING: Could not recover jumptable at 0x00802294. Too many branches
                    // WARNING: Treating indirect jump as call
  (*DCFlushRange)(param_2,4);
  return;
}



// WARNING: Unknown calling convention yet parameter storage is locked

menu_t * create_menu_root(void)

{
  menu_t *allocated_menu_root;
  
  allocated_menu_root = (menu_t *)(*OSAllocFromSystem)(8,4);
  *(undefined4 *)allocated_menu_root = 0;
  allocated_menu_root->field_0x4 = 0;
  return allocated_menu_root;
}



void create_menu(wchar_t *text)

{
  wchar_t **allocated_memory;
  
  allocated_memory = (wchar_t **)(*OSAllocFromSystem)(0xc,4);
  allocated_memory[1] = (wchar_t *)0x0;
  allocated_memory[2] = (wchar_t *)0x0;
  *allocated_memory = text;
  return;
}



void add_menu_to_menu_root(int *parent_menu,int added_menu)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  
  iVar3 = *parent_menu;
  if (iVar3 == 0) {
    piVar1 = (int *)(*OSAllocFromSystem)(0x28,4);
    iVar3 = *parent_menu;
    parent_menu[1] = (int)piVar1;
  }
  else {
    piVar1 = (int *)parent_menu[1];
  }
  piVar2 = piVar1;
  if ((((((*piVar1 != 0) && (piVar2 = piVar1 + 1, piVar1[1] != 0)) &&
        (piVar2 = piVar1 + 2, piVar1[2] != 0)) &&
       ((piVar2 = piVar1 + 3, piVar1[3] != 0 && (piVar2 = piVar1 + 4, piVar1[4] != 0)))) &&
      ((piVar2 = piVar1 + 5, piVar1[5] != 0 &&
       ((piVar2 = piVar1 + 6, piVar1[6] != 0 && (piVar2 = piVar1 + 7, piVar1[7] != 0)))))) &&
     ((piVar2 = piVar1 + 8, piVar1[8] != 0 && (piVar2 = piVar1 + 9, piVar1[9] != 0)))) {
    *parent_menu = iVar3 + 1;
    return;
  }
  *piVar2 = added_menu;
  *parent_menu = iVar3 + 1;
  return;
}



void add_menu_option(menu_t *parent_menu,wchar_t *text,void *function,int parameter)

{
  int iVar1;
  wchar_t **ppwVar2;
  wchar_t **ppwVar3;
  wchar_t **ppwVar4;
  code *pcVar5;
  int iVar6;
  
  pcVar5 = OSAllocFromSystem;
  if (parent_menu->field_0x4 == 0) {
    iVar1 = (*OSAllocFromSystem)(0x50,4);
    pcVar5 = OSAllocFromSystem;
    parent_menu->field_0x8 = iVar1;
  }
  ppwVar2 = (wchar_t **)(*pcVar5)(0xc,4);
  iVar1 = parent_menu->field_0x8;
  *ppwVar2 = text;
  ppwVar2[1] = (wchar_t *)function;
  iVar6 = 10;
  ppwVar2[2] = (wchar_t *)parameter;
  ppwVar3 = (wchar_t **)(iVar1 + -4);
  while ((ppwVar4 = ppwVar3 + 1, *ppwVar4 != (wchar_t *)0x0 &&
         (ppwVar4 = ppwVar3 + 2, *ppwVar4 != (wchar_t *)0x0))) {
    iVar6 = iVar6 + -1;
    ppwVar3 = ppwVar4;
    if (iVar6 == 0) {
LAB_00802508:
      parent_menu->field_0x4 = parent_menu->field_0x4 + 1;
      return;
    }
  }
  *ppwVar4 = (wchar_t *)ppwVar2;
  goto LAB_00802508;
}



void FUN_00802538(undefined4 param_1)

{
  (*OSScreenInit)();
  (*OSScreenEnableEx)(1,1);
  (*OSScreenClearBufferEx)(1,0);
  (*OSScreenSetBufferEx)(1,0xf4000000);
  (*OSScreenPutFontEx)(1,0,0,param_1);
                    // WARNING: Could not recover jumptable at 0x008025e4. Too many branches
                    // WARNING: Treating indirect jump as call
  (*OSScreenFlipBuffersEx)(1);
  return;
}



void draw_debug_text(void)

{
  uint uVar1;
  
  if (should_draw_debug_text != 0) {
    (*ConsoleUIController::ShowUIDebugConsole)(0x109f95e0,0);
    uVar1 = countLeadingZeros(should_draw_debug_text);
    should_draw_debug_text = uVar1 >> 5;
    return;
  }
  (*ConsoleUIController::ShowUIDebugConsole)(0x109f95e0,1);
  uVar1 = countLeadingZeros(should_draw_debug_text);
  should_draw_debug_text = uVar1 >> 5;
  return;
}



void fly(void)

{
  uint uVar1;
  
  if (DAT_0081e47c != 0) {
    patch_memory(0x271aa74,0x7fe3fb78);
    uVar1 = countLeadingZeros(DAT_0081e47c);
    DAT_0081e47c = uVar1 >> 5;
    return;
  }
  patch_memory(0x271aa74,0x38600001);
  uVar1 = countLeadingZeros(DAT_0081e47c);
  DAT_0081e47c = uVar1 >> 5;
  return;
}



void god_mode(void)

{
  uint uVar1;
  
  if (god_mode_enabled != 0) {
    patch_memory(0x2727824,-0x3df0770);
    uVar1 = countLeadingZeros(god_mode_enabled);
    god_mode_enabled = uVar1 >> 5;
    return;
  }
  patch_memory(0x2727824,0x60000000);
  uVar1 = countLeadingZeros(god_mode_enabled);
  god_mode_enabled = uVar1 >> 5;
  return;
}



void god_mode_for_all_entities(void)

{
  uint uVar1;
  
  if (DAT_0081e48c != 0) {
    patch_memory(0x25795c0,0x60000000);
    uVar1 = countLeadingZeros(DAT_0081e48c);
    DAT_0081e48c = uVar1 >> 5;
    return;
  }
  patch_memory(0x25795c0,-0x3df0770);
  uVar1 = countLeadingZeros(DAT_0081e48c);
  DAT_0081e48c = uVar1 >> 5;
  return;
}



void unlimited_blocks(void)

{
  uint uVar1;
  
  if (DAT_0081e458 != 0) {
    patch_memory(0x2173790,0x38800001);
    uVar1 = countLeadingZeros(DAT_0081e458);
    DAT_0081e458 = uVar1 >> 5;
    return;
  }
  patch_memory(0x2173790,0x38800000);
  uVar1 = countLeadingZeros(DAT_0081e458);
  DAT_0081e458 = uVar1 >> 5;
  return;
}



void fall_damage(void)

{
  uint uVar1;
  
  if (DAT_0081e478 != 0) {
    patch_memory(0x233164c,-0x6bde0040);
    uVar1 = countLeadingZeros(DAT_0081e478);
    DAT_0081e478 = uVar1 >> 5;
    return;
  }
  patch_memory(0x233164c,0x4e800020);
  uVar1 = countLeadingZeros(DAT_0081e478);
  DAT_0081e478 = uVar1 >> 5;
  return;
}



void speed_hack(void)

{
  uint uVar1;
  
  if (DAT_0081e4b0 != 0) {
    patch_memory(0x1066aae8,0x3f68f5c3);
    patch_memory(0x1066879c,0x3ca3d70a);
    patch_memory(0x1066acc8,0x3e26ad89);
    uVar1 = countLeadingZeros(DAT_0081e4b0);
    DAT_0081e4b0 = uVar1 >> 5;
    return;
  }
  patch_memory(0x1066aae8,0x3f76f5c3);
  patch_memory(0x1066879c,0x3df5c28f);
  patch_memory(0x1066acc8,0x3eb9bd1f);
  uVar1 = countLeadingZeros(DAT_0081e4b0);
  DAT_0081e4b0 = uVar1 >> 5;
  return;
}



void multi_jump(void)

{
  uint uVar1;
  
  if (DAT_0081e49c != 0) {
    patch_memory(0x232f3a0,0x38800000);
    uVar1 = countLeadingZeros(DAT_0081e49c);
    DAT_0081e49c = uVar1 >> 5;
    return;
  }
  patch_memory(0x232f3a0,0x38800001);
  uVar1 = countLeadingZeros(DAT_0081e49c);
  DAT_0081e49c = uVar1 >> 5;
  return;
}



void free_crafting(void)

{
  uint uVar1;
  
  if (DAT_0081e45c != 0) {
    patch_memory(0x2f70970,0x38600000);
    uVar1 = countLeadingZeros(DAT_0081e45c);
    DAT_0081e45c = uVar1 >> 5;
    return;
  }
  patch_memory(0x2f70970,0x38600001);
  uVar1 = countLeadingZeros(DAT_0081e45c);
  DAT_0081e45c = uVar1 >> 5;
  return;
}



void item_debug_text(void)

{
  uint uVar1;
  
  if (DAT_0081e464 != 0) {
    patch_memory(0x2f5c874,0x38600000);
    uVar1 = countLeadingZeros(DAT_0081e464);
    DAT_0081e464 = uVar1 >> 5;
    return;
  }
  patch_memory(0x2f5c874,0x38600001);
  uVar1 = countLeadingZeros(DAT_0081e464);
  DAT_0081e464 = uVar1 >> 5;
  return;
}



void reach(void)

{
  uint uVar1;
  
  if (DAT_0081e4ac != 0) {
    patch_memory(0x108f5620,0x40900000);
    patch_memory(0x108e0bdc,0x40a00000);
    patch_memory(0x108c9c20,0x40080000);
    uVar1 = countLeadingZeros(DAT_0081e4ac);
    DAT_0081e4ac = uVar1 >> 5;
    return;
  }
  patch_memory(0x108f5620,0x42480000);
  patch_memory(0x108e0bdc,0x42480000);
  patch_memory(0x108c9c20,0x42480000);
  uVar1 = countLeadingZeros(DAT_0081e4ac);
  DAT_0081e4ac = uVar1 >> 5;
  return;
}



void xray(void)

{
  uint uVar1;
  
  if (DAT_0081e4c4 != 0) {
    patch_memory(0x1095caac,0x3f800000);
    uVar1 = countLeadingZeros(DAT_0081e4c4);
    DAT_0081e4c4 = uVar1 >> 5;
    return;
  }
  patch_memory(0x1095caac,0x3f900000);
  uVar1 = countLeadingZeros(DAT_0081e4c4);
  DAT_0081e4c4 = uVar1 >> 5;
  return;
}



void kill_aura(void)

{
  uint uVar1;
  
  if (DAT_0081e454 != 0) {
    patch_memory(0x105dd948,0x3f000000);
    uVar1 = countLeadingZeros(DAT_0081e454);
    DAT_0081e454 = uVar1 >> 5;
    return;
  }
  patch_memory(0x105dd948,0x3ff00000);
  uVar1 = countLeadingZeros(DAT_0081e454);
  DAT_0081e454 = uVar1 >> 5;
  return;
}



void water_fly(void)

{
  uint uVar1;
  
  if (DAT_0081e4c0 != 0) {
    patch_memory(0x2331ba8,-0x779cfe14);
    uVar1 = countLeadingZeros(DAT_0081e4c0);
    DAT_0081e4c0 = uVar1 >> 5;
    return;
  }
  patch_memory(0x2331ba8,0x38600001);
  uVar1 = countLeadingZeros(DAT_0081e4c0);
  DAT_0081e4c0 = uVar1 >> 5;
  return;
}



void fov(void)

{
  uint uVar1;
  
  if (DAT_0081e484 != 0) {
    patch_memory(0x1088eda8,0x3f000000);
    uVar1 = countLeadingZeros(DAT_0081e484);
    DAT_0081e484 = uVar1 >> 5;
    return;
  }
  patch_memory(0x1088eda8,0x3f800000);
  uVar1 = countLeadingZeros(DAT_0081e484);
  DAT_0081e484 = uVar1 >> 5;
  return;
}



void instant_mining(void)

{
  uint uVar1;
  
  if (DAT_0081e4a8 != 0) {
    patch_memory(0x106381c8,0x3f800000);
    uVar1 = countLeadingZeros(DAT_0081e4a8);
    DAT_0081e4a8 = uVar1 >> 5;
    return;
  }
  patch_memory(0x106381c8,0x447a0000);
  uVar1 = countLeadingZeros(DAT_0081e4a8);
  DAT_0081e4a8 = uVar1 >> 5;
  return;
}



void draw_hitboxes(void)

{
  uint uVar1;
  
  if (DAT_0081e494 != 0) {
    patch_memory(0x30fa018,0x418200bc);
    uVar1 = countLeadingZeros(DAT_0081e494);
    DAT_0081e494 = uVar1 >> 5;
    return;
  }
  patch_memory(0x30fa018,0x60000000);
  uVar1 = countLeadingZeros(DAT_0081e494);
  DAT_0081e494 = uVar1 >> 5;
  return;
}



void auto_sprint(void)

{
  uint uVar1;
  
  if (DAT_0081e4b4 != 0) {
    patch_memory(0x31ed0a4,0x40820018);
    patch_memory(0x31ed0c4,0x41820050);
    uVar1 = countLeadingZeros(DAT_0081e4b4);
    DAT_0081e4b4 = uVar1 >> 5;
    return;
  }
  patch_memory(0x31ed0a4,0x48000018);
  patch_memory(0x31ed0c4,0x60000000);
  uVar1 = countLeadingZeros(DAT_0081e4b4);
  DAT_0081e4b4 = uVar1 >> 5;
  return;
}



void all_permissions(void)

{
  uint uVar1;
  
  if (DAT_0081e4a4 != 0) {
    patch_memory(0x2c57cc4,0x28040001);
    patch_memory(0x2c57cc8,0x4180005c);
    uVar1 = countLeadingZeros(DAT_0081e4a4);
    DAT_0081e4a4 = uVar1 >> 5;
    return;
  }
  patch_memory(0x2c57cc4,0x38600001);
  patch_memory(0x2c57cc8,0x4e800020);
  uVar1 = countLeadingZeros(DAT_0081e4a4);
  DAT_0081e4a4 = uVar1 >> 5;
  return;
}



void super_jump(void)

{
  uint uVar1;
  
  if (DAT_0081e4bc != 0) {
    patch_memory(0x1066aad4,0x3ed70a3d);
    uVar1 = countLeadingZeros(DAT_0081e4bc);
    DAT_0081e4bc = uVar1 >> 5;
    return;
  }
  patch_memory(0x1066aad4,0x3f400000);
  uVar1 = countLeadingZeros(DAT_0081e4bc);
  DAT_0081e4bc = uVar1 >> 5;
  return;
}



void infinite_air(void)

{
  uint uVar1;
  
  if (DAT_0081e450 != 0) {
    patch_memory(0x23410b4,0x7c9f2378);
    uVar1 = countLeadingZeros(DAT_0081e450);
    DAT_0081e450 = uVar1 >> 5;
    return;
  }
  patch_memory(0x23410b4,0x3be0012c);
  uVar1 = countLeadingZeros(DAT_0081e450);
  DAT_0081e450 = uVar1 >> 5;
  return;
}



void creative(void)

{
  uint uVar1;
  
  if (DAT_0081e4a0 != 0) {
    patch_memory(0x2c5cc84,-0x779cfed8);
    patch_memory(0x3143908,0x7fe3fb78);
    uVar1 = countLeadingZeros(DAT_0081e4a0);
    DAT_0081e4a0 = uVar1 >> 5;
    return;
  }
  patch_memory(0x2c5cc84,0x38600001);
  patch_memory(0x3143908,0x38600001);
  uVar1 = countLeadingZeros(DAT_0081e4a0);
  DAT_0081e4a0 = uVar1 >> 5;
  return;
}



void deadmanu5_ears(void)

{
  uint uVar1;
  
  if (DAT_0081e460 != 0) {
    patch_memory(0x30a5a18,0x2c030000);
    uVar1 = countLeadingZeros(DAT_0081e460);
    DAT_0081e460 = uVar1 >> 5;
    return;
  }
  patch_memory(0x30a5a18,0x2c030001);
  uVar1 = countLeadingZeros(DAT_0081e460);
  DAT_0081e460 = uVar1 >> 5;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void infinite_food(void)

{
  int iVar1;
  int address;
  uint uVar2;
  
  iVar1 = infinite_food_enabled;
  if (infinite_food_enabled != 0) {
    patch_memory(0x271ba88,0x7c0802a6);
    uVar2 = countLeadingZeros(infinite_food_enabled);
    infinite_food_enabled = uVar2 >> 5;
    return;
  }
  patch_memory(0x271ba88,0x4e800020);
  DAT_0081e4d8 = 2;
  DAT_0081e3d8 = 0x9c;
  address = 0x608;
  DAT_0081e3dc = iVar1;
  if ((_DAT_10a0a624 + 0xf000009bU < 0x30ffffff) &&
     (*(int *)(_DAT_10a0a624 + 0x9c) + 0xefffffffU < 0x30ffffff)) {
    address = *(int *)(_DAT_10a0a624 + 0x9c) + 0x608;
  }
  patch_memory(address,0x14);
  uVar2 = countLeadingZeros(infinite_food_enabled);
  infinite_food_enabled = uVar2 >> 5;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void fake_death(void)

{
  int iVar1;
  uint uVar2;
  
  if (DAT_0081e470 != 0) {
    DAT_0081e3d8 = 0x9c;
    DAT_0081e4d8 = 2;
    iVar1 = 0x3b0;
    DAT_0081e3dc = 0;
    if ((_DAT_10a0a624 + 0xf000009bU < 0x30ffffff) &&
       (*(int *)(_DAT_10a0a624 + 0x9c) + 0xefffffffU < 0x30ffffff)) {
      iVar1 = *(int *)(_DAT_10a0a624 + 0x9c) + 0x3b0;
    }
    FUN_0080214c(iVar1,0);
    uVar2 = countLeadingZeros(DAT_0081e470);
    DAT_0081e470 = uVar2 >> 5;
    return;
  }
  DAT_0081e3d8 = 0x9c;
  DAT_0081e4d8 = 2;
  DAT_0081e3dc = DAT_0081e470;
  iVar1 = 0x3b0;
  if ((_DAT_10a0a624 + 0xf000009bU < 0x30ffffff) &&
     (*(int *)(_DAT_10a0a624 + 0x9c) + 0xefffffffU < 0x30ffffff)) {
    iVar1 = *(int *)(_DAT_10a0a624 + 0x9c) + 0x3b0;
  }
  FUN_0080214c(iVar1,1);
  uVar2 = countLeadingZeros(DAT_0081e470);
  DAT_0081e470 = uVar2 >> 5;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void fake_sleep(void)

{
  int iVar1;
  uint uVar2;
  
  if (DAT_0081e474 != 0) {
    DAT_0081e3d8 = 0x9c;
    DAT_0081e4d8 = 2;
    iVar1 = 0x6c0;
    DAT_0081e3dc = 0;
    if ((_DAT_10a0a624 + 0xf000009bU < 0x30ffffff) &&
       (*(int *)(_DAT_10a0a624 + 0x9c) + 0xefffffffU < 0x30ffffff)) {
      iVar1 = *(int *)(_DAT_10a0a624 + 0x9c) + 0x6c0;
    }
    FUN_0080214c(iVar1,0);
    uVar2 = countLeadingZeros(DAT_0081e474);
    DAT_0081e474 = uVar2 >> 5;
    return;
  }
  DAT_0081e3d8 = 0x9c;
  DAT_0081e4d8 = 2;
  DAT_0081e3dc = DAT_0081e474;
  iVar1 = 0x6c0;
  if ((_DAT_10a0a624 + 0xf000009bU < 0x30ffffff) &&
     (*(int *)(_DAT_10a0a624 + 0x9c) + 0xefffffffU < 0x30ffffff)) {
    iVar1 = *(int *)(_DAT_10a0a624 + 0x9c) + 0x6c0;
  }
  FUN_0080214c(iVar1,1);
  uVar2 = countLeadingZeros(DAT_0081e474);
  DAT_0081e474 = uVar2 >> 5;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void hide_arm(void)

{
  int iVar1;
  uint uVar2;
  
  if (DAT_0081e490 != 0) {
    DAT_0081e3d8 = 0x9c;
    DAT_0081e4d8 = 2;
    iVar1 = 0x900;
    DAT_0081e3dc = 0;
    if ((_DAT_10a0a624 + 0xf000009bU < 0x30ffffff) &&
       (*(int *)(_DAT_10a0a624 + 0x9c) + 0xefffffffU < 0x30ffffff)) {
      iVar1 = *(int *)(_DAT_10a0a624 + 0x9c) + 0x900;
    }
    FUN_0080214c(iVar1,0);
    uVar2 = countLeadingZeros(DAT_0081e490);
    DAT_0081e490 = uVar2 >> 5;
    return;
  }
  DAT_0081e3d8 = 0x9c;
  DAT_0081e4d8 = 2;
  DAT_0081e3dc = DAT_0081e490;
  iVar1 = 0x900;
  if ((_DAT_10a0a624 + 0xf000009bU < 0x30ffffff) &&
     (*(int *)(_DAT_10a0a624 + 0x9c) + 0xefffffffU < 0x30ffffff)) {
    iVar1 = *(int *)(_DAT_10a0a624 + 0x9c) + 0x900;
  }
  FUN_0080214c(iVar1,1);
  uVar2 = countLeadingZeros(DAT_0081e490);
  DAT_0081e490 = uVar2 >> 5;
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

int FUN_00803b78(undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  if ((0x30fffffe < _DAT_109f845c + param_3 + 0xefffffffU) ||
     (param_4 = *(int *)(_DAT_109f845c + param_3) + param_4, 0x30fffffe < param_4 + 0xefffffffU)) {
    param_4 = 0;
  }
  return param_4;
}



void set_hud_text(short *param_1)

{
  int iVar1;
  short *psVar2;
  short sVar3;
  int iVar4;
  
  if (DAT_0081e500 == 0) {
    DAT_0081e500 = (*OSAllocFromSystem)(0x1000,4);
    DAT_0081e4cc = (*OSAllocFromSystem)(0x100,4);
    draw_menu_instructions(DAT_0081e500 + DAT_0081e504 * 2,param_1);
    sVar3 = *param_1;
  }
  else {
    draw_menu_instructions(DAT_0081e500 + DAT_0081e504 * 2,param_1);
    sVar3 = *param_1;
  }
  if (sVar3 == 0) {
    iVar4 = 0;
LAB_00803c78:
    draw_menu_instructions(DAT_0081e500 + (iVar4 + DAT_0081e504) * 2,&DAT_008188d0);
    sVar3 = *param_1;
  }
  else {
    iVar1 = 0;
    iVar4 = 0;
    psVar2 = param_1;
    do {
      while( true ) {
        iVar4 = iVar4 + 1;
        if (sVar3 == 0x2e) break;
        psVar2 = psVar2 + 1;
        sVar3 = psVar2[iVar1];
        if (sVar3 == 0) goto LAB_00803c78;
      }
      iVar1 = iVar1 + 1;
      sVar3 = psVar2[iVar1];
    } while (sVar3 != 0);
    draw_menu_instructions(DAT_0081e500 + (iVar4 + DAT_0081e504) * 2,&DAT_008188d0);
    sVar3 = *param_1;
  }
  if (sVar3 == 0) {
    DAT_0081e504 = DAT_0081e504 + 1;
    return;
  }
  iVar4 = 0;
  iVar1 = 0;
  do {
    while( true ) {
      iVar1 = iVar1 + 1;
      if (sVar3 == 0x2e) break;
      param_1 = param_1 + 1;
      sVar3 = param_1[iVar4];
      if (sVar3 == 0) {
        DAT_0081e504 = DAT_0081e504 + 1 + iVar1;
        return;
      }
    }
    iVar4 = iVar4 + 1;
    sVar3 = param_1[iVar4];
  } while (sVar3 != 0);
  DAT_0081e504 = DAT_0081e504 + 1 + iVar1;
  return;
}



void FUN_00803e0c(short *param_1)

{
  int iVar1;
  short sVar2;
  int iVar3;
  
  if (DAT_0081e500 == 0) {
    DAT_0081e500 = (*OSAllocFromSystem)(0x1000,4);
    DAT_0081e4cc = (*OSAllocFromSystem)(0x100,4);
    draw_menu_instructions(DAT_0081e500 + DAT_0081e504 * 2,param_1);
    sVar2 = *param_1;
  }
  else {
    draw_menu_instructions(DAT_0081e500 + DAT_0081e504 * 2,param_1);
    sVar2 = *param_1;
  }
  if (sVar2 != 0) {
    iVar1 = 0;
    iVar3 = 0;
    do {
      while( true ) {
        iVar3 = iVar3 + 1;
        if (sVar2 == 0x2e) break;
        param_1 = param_1 + 1;
        sVar2 = param_1[iVar1];
        if (sVar2 == 0) {
          DAT_0081e504 = DAT_0081e504 + iVar3;
          return;
        }
      }
      iVar1 = iVar1 + 1;
      sVar2 = param_1[iVar1];
    } while (sVar2 != 0);
    DAT_0081e504 = DAT_0081e504 + iVar3;
    return;
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_00803f6c(void)

{
  int iVar1;
  int iVar2;
  undefined auStack2128 [1020];
  undefined auStack1108 [1028];
  undefined4 local_50;
  uint uStack76;
  
  print_to_log("entered RenderMenu\n");
  if (_DAT_109c4564 == 0) {
    patch_memory(0x2e0c9a4,0x38800000);
    DAT_0081e424 = '\0';
  }
  else {
    if (is_trial_version == '\0') {
      print_to_log("in game");
      if (DAT_0081e424 == '\0') {
        (*OSSleepTicks)(0,0x5ed7c9);
        DAT_0081e424 = '\x01';
      }
      if (DAT_0081e498 == '\0') {
                    // UIComponent_PressStartToPlay::tick
        patch_memory(0x2e0c9a4,0x38800000);
      }
      else {
        print_to_log("rendering menu...\n");
        patch_memory(DAT_0081e414,0);
        patch_memory(0x2e0c9a4,0x38800001);
        FUN_00802224(0,DAT_0081e430 + 0x28);
        FUN_00802224((double)*(float *)(DAT_0081e42c + 0x28),DAT_0081e43c + 0x28);
        local_50 = 0x43300000;
        uStack76 = (DAT_0081e448 + 2) * 0x18 ^ 0x80000000;
        FUN_00802224((double)((float)((double)CONCAT44(0x43300000,uStack76) - 4503601774854144.0) +
                              *(float *)(DAT_0081e42c + 0x2c) + 2.0),DAT_0081e43c + 0x2c);
        FUN_00802224((double)(*(float *)(DAT_0081e42c + 0x18) - 0.05),DAT_0081e43c + 0x18);
        FUN_00802224(0x3fe0000000000000,DAT_0081e43c + 0x24);
        FUN_00802224((double)(*(float *)(DAT_0081e41c + 0x28) - 5.0),DAT_0081e42c + 0x28);
        FUN_00802224((double)DAT_0081c868,DAT_0081e42c + 0x5c);
        FUN_00802224((double)DAT_0081c86c,DAT_0081e42c + 0x60);
        FUN_00802224((double)DAT_0081c870,DAT_0081e42c + 100);
        FUN_00802224((double)DAT_0081c874,DAT_0081e42c + 0x68);
        FUN_00802224((double)DAT_0081c87c,DAT_0081e43c + 0x5c);
        FUN_00802224((double)DAT_0081c880,DAT_0081e43c + 0x60);
        FUN_00802224((double)DAT_0081c884,DAT_0081e43c + 100);
        FUN_00802224((double)DAT_0081c888,DAT_0081e43c + 0x68);
        iVar2 = DAT_0081e4b8;
        if (DAT_0081e4b8 == 0) {
          DAT_0081e4e4 = *(int *)menu_root;
          FUN_0080c398(DAT_0081e500,0,DAT_0081e504);
          DAT_0081e504 = iVar2;
          set_hud_text(L"Trinity V1.3.2");
          set_hud_text(L"");
          if (0 < DAT_0081e4e4) {
            do {
              iVar1 = iVar2 * 4;
              iVar2 = iVar2 + 1;
              set_hud_text(**(undefined4 **)(menu_root->field_0x4 + iVar1));
            } while (iVar2 < DAT_0081e4e4);
          }
        }
        else {
          DAT_0081e4e4 = *(int *)(*(int *)(menu_root->field_0x4 + DAT_0081e4b8 * 4 + -4) + 4);
          FUN_0080c398(DAT_0081e500,0,DAT_0081e504);
          DAT_0081e504 = 0;
          set_hud_text(L"Trinity V1.3.2");
          set_hud_text(L"");
          if (0 < DAT_0081e4e4) {
            iVar2 = 0;
            do {
              FUN_00803e0c(**(undefined4 **)
                             (*(int *)(*(int *)(menu_root->field_0x4 + DAT_0081e4b8 * 4 + -4) + 8) +
                             iVar2 * 4));
              switch(**(undefined4 **)
                       (*(int *)(*(int *)(*(int *)(menu_root->field_0x4 + DAT_0081e4b8 * 4 + -4) + 8
                                         ) + iVar2 * 4) + 8)) {
              case 0:
                set_hud_text(L" [OFF]");
                break;
              case 1:
                set_hud_text(L" [ON]");
                break;
              case 2:
                set_hud_text(L"");
                break;
              case 3:
                FUN_00803e0c(&DAT_00818a64);
                FUN_00803e0c(PTR_u_Default_0081c878);
                set_hud_text(&DAT_00818a6c);
                break;
              case 4:
                FUN_00803e0c(&DAT_00818a64);
                iVar1 = (*DAT_0081e3bc)(0,DAT_0081e4f4);
                FUN_00803e0c(*(int *)(iVar1 + 4) + 0x60);
                set_hud_text(&DAT_00818a6c);
                break;
              case 5:
                (*DAT_0081e3c4)(auStack1108,0x3fc,&DAT_00818a70,DAT_0081e4f8);
                FUN_00803e0c(&DAT_00818a64);
                FUN_00803e0c(auStack1108);
                set_hud_text(&DAT_00818a6c);
                break;
              case 6:
                (*DAT_0081e3c4)(auStack2128,0x3fc,&DAT_00818a70,DAT_0081e4fc);
                FUN_00803e0c(&DAT_00818a64);
                FUN_00803e0c(auStack2128);
                set_hud_text(&DAT_00818a6c);
              }
              iVar2 = iVar2 + 1;
            } while (iVar2 < DAT_0081e4e4);
          }
        }
        draw_menu_instructions(0x3f990000,DAT_0081e500);
      }
    }
    else {
      patch_memory(DAT_0081e414,0);
      print_to_log("je suis trial\n");
                    // UIComponent_PressStartToPlay::tick
      patch_memory(0x2e0c9a4,0x38800001);
      patch_memory(DAT_0081e43c,0);
      FUN_00802224(0x3ff0000000000000,DAT_0081e42c + 0x68);
      FUN_00802224(0,DAT_0081e430 + 0x28);
      draw_menu_instructions(0x3f990000,&DAT_00818928);
    }
  }
  print_to_log("exiting rendermenu\n");
  if ((_DAT_10a0a624 + 0xf000009bU < 0x30ffffff) &&
     (*(int *)(_DAT_10a0a624 + 0x9c) + 0xefffffffU < 0x30ffffff)) {
    DAT_0081e3d8 = 0x9c;
    DAT_0081e3dc = 0;
    DAT_0081e4d8 = 2;
    return *(undefined4 *)(*(int *)(_DAT_10a0a624 + 0x9c) + 0x608);
  }
  DAT_0081e3d8 = 0x9c;
  DAT_0081e3dc = 0;
  DAT_0081e4d8 = 2;
  return _DAT_00000608;
}



void FUN_00804654(undefined4 param_1,undefined4 param_2)

{
  undefined4 uVar1;
  
  (*UIComponent_PressStartToPlay::HideAchievementToast)(DAT_0081e434);
  uVar1 = (*GetString)(0x1ad380ab);
  draw_menu_instructions(uVar1,param_1);
  uVar1 = (*GetString)(0x4523129e);
  draw_menu_instructions(uVar1,param_2);
                    // WARNING: Could not recover jumptable at 0x008046fc. Too many branches
                    // WARNING: Treating indirect jump as call
  (*ConsoleUIController::SetAchievementUnlocked)(0x109f95e0,0,0x60);
  return;
}



void FUN_00804700(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  code *UNRECOVERED_JUMPTABLE;
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  iVar1 = (*DAT_0081e3bc)(0,param_1);
  iVar3 = *(int *)(iVar1 + 4);
  iVar1 = (*DAT_0081e3bc)(0,param_1);
  print_to_log_2("PLAYER : %s\nUID : %X %X %X %X %X %X\n\n",*(int *)(iVar1 + 4) + 0x4a,
                 *(undefined4 *)(iVar3 + 0x1c),*(undefined4 *)(iVar3 + 0x2c),
                 *(undefined4 *)(iVar3 + 0x3c),*(undefined4 *)(iVar3 + 0x4c),
                 *(undefined4 *)(iVar3 + 0x5c),*(undefined4 *)(iVar3 + 0x6c));
  patch_memory(0x3f100000,0x3f200000);
  patch_memory(0x3f200000,0x3f200000);
  FUN_0080c25c(0x3f200784,iVar3 + 0x1c,0x60);
  uVar2 = FUN_00803b78(&DAT_109f845c,2,0,0);
  print_to_log_2("ClientPacketListener : %X\n",uVar2);
  iVar1 = (*DAT_0081e3c8)(0x3e000000,0x3f100000,param_2,param_3,1,0x3e000050);
  if (iVar1 == 0) {
    print_to_log("packet prepare avec succes !!!!!!!!!!!!!\n");
  }
  else {
    print_to_log("probleme lors de preparepacket ! :(\n");
  }
  UNRECOVERED_JUMPTABLE = ClientPacketListener::send;
  uVar2 = FUN_00803b78(&DAT_109f845c,2,0,0);
                    // WARNING: Could not recover jumptable at 0x00804874. Too many branches
                    // WARNING: Treating indirect jump as call
  (*UNRECOVERED_JUMPTABLE)(uVar2,0x3e000000);
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00804c54(void)

{
  int *piVar1;
  code *pcVar2;
  int iVar3_2;
  int memory_compare_result;
  int player_count;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int local_78;
  int *local_5c;
  int *local_58;
  int iVar3_;
  
  while ((_DAT_109c4564 == 0 || (DAT_10a09370 == '\x01'))) {
    (*OSSleepTicks)(0,0xed1b76);
  }
  local_78 = 0;
  (*OSSleepTicks)(0,0x3b46dda);
  local_58 = &DAT_0081e4f8;
  local_5c = &DAT_0081e4fc;
  do {
    if (_DAT_102efa64 == 0) {
LAB_00805288:
      if (((_DAT_10a8a7c8 & DAT_0081e51c) != 0) && ((_DAT_10a8a7c8 & DAT_0081e520) != 0)) {
        if (DAT_0081e498 == 0) goto LAB_008052c0;
LAB_0080523c:
        DAT_0081e448 = 0;
                    // .text:031ECF30                 bctrl
        patch_memory(0x31ecf30,0x4e800421);
        DAT_0081e498 = 0;
        goto LAB_00805258;
      }
    }
    else {
      if (DAT_0081e3a4 != '\0') {
        DAT_0081e3a4 = '\0';
                    // UIComponent_PressStartToPlay::tick
        patch_memory(0x2e0c9a4,0x38800000);
      }
      if (((_DAT_102efa64 & DAT_0081e4dc) == 0) || ((_DAT_102efa64 & DAT_0081e4e0) == 0))
      goto LAB_00805288;
      if (DAT_0081e498 != 0) goto LAB_0080523c;
LAB_008052c0:
      DAT_0081e448 = 0;
                    // Follow__19MultiPlayerGameMode
      patch_memory(0x31f4240,0x3b800000);
      DAT_0081e498 = 1;
      FUN_0080214c((int)&DAT_10a09370,2);
      patch_memory(0x31ecf30,0x48803f6f);
LAB_00805258:
      (*OSSleepTicks)(0,0xed1b76);
    }
    if (DAT_0081e498 != 0) {
      if (((_DAT_102efa64 & DAT_0081e50c) != 0) || ((_DAT_10a8a7c8 & DAT_0081e52c) != 0)) {
        memory_compare_result = DAT_0081e4e4;
        if ((DAT_0081e448 < DAT_0081e4e4) && (0 < DAT_0081e448)) {
          memory_compare_result = DAT_0081e448;
        }
        DAT_0081e448 = memory_compare_result + -1;
        print_to_log_2(&DAT_00818bb4);
        (*OSSleepTicks)(0,0x8e43ad);
      }
      if (((_DAT_102efa64 & DAT_0081e444) != 0) || ((_DAT_10a8a7c8 & DAT_0081e514) != 0)) {
        memory_compare_result = 0;
        if ((-1 < DAT_0081e448) && (DAT_0081e448 < DAT_0081e4e4 + -1)) {
          memory_compare_result = DAT_0081e448 + 1;
        }
        DAT_0081e448 = memory_compare_result;
        print_to_log_2(&DAT_00818bb4);
        (*OSSleepTicks)(0,0x8e43ad);
      }
      if (((_DAT_102efa64 & DAT_0081e4f0) != 0) || ((_DAT_10a8a7c8 & DAT_0081e528) != 0)) {
        if (is_trial_version == '\0') {
          if (DAT_0081e4b8 == 0) {
            memory_compare_result = DAT_0081e448 + 1;
            DAT_0081e44c = DAT_0081e448;
            DAT_0081e448 = DAT_0081e4b8;
            DAT_0081e4b8 = memory_compare_result;
          }
          else {
            (**(code **)(*(int *)(*(int *)(*(int *)(menu_root->field_0x4 + DAT_0081e4b8 * 4 + -4) +
                                          8) + DAT_0081e448 * 4) + 4))();
            if (DAT_0081e434 != 0) {
              puVar3 = *(undefined4 **)
                        (*(int *)(*(int *)(menu_root->field_0x4 + DAT_0081e4b8 * 4 + -4) + 8) +
                        DAT_0081e448 * 4);
              if (*(int *)puVar3[2] == 1) {
                FUN_00804654(*puVar3,&DAT_00818bb8);
              }
              else {
                if (*(int *)puVar3[2] == 0) {
                  FUN_00804654(*puVar3,&DAT_00818bc0);
                }
              }
            }
          }
        }
        (*OSSleepTicks)(0,0x8e43ad);
      }
      if (((_DAT_102efa64 & DAT_0081e440) != 0) || ((_DAT_10a8a7c8 & DAT_0081e510) != 0)) {
        if (DAT_0081e4b8 == 0) {
          DAT_0081e498 = DAT_0081e498 ^ 1;
                    // Follow__19MultiPlayerGameMode
          patch_memory(0x31f4240,0x7c9c2378);
                    // UIComponent_PressStartToPlay::tick
          patch_memory(0x2e0c9a4,0x38800000);
          (*OSSleepTicks)(0,0xed1b76);
          FUN_0080214c((int)&DAT_10a09370,0);
          patch_memory(0x31ecf30,0x4e800421);
        }
        else {
          DAT_0081e448 = DAT_0081e44c;
        }
        DAT_0081e4f4 = 0;
        DAT_0081e4b8 = 0;
        (*OSSleepTicks)(0,0x8e43ad);
      }
      if (((_DAT_102efa64 & DAT_0081e4ec) != 0) || ((_DAT_10a8a7c8 & DAT_0081e524) != 0)) {
        if (DAT_0081e4b8 == 0) goto LAB_008051d8;
        memory_compare_result =
             memcmp_maybe(**(undefined4 **)
                            (*(int *)(*(int *)(menu_root->field_0x4 + DAT_0081e4b8 * 4 + -4) + 8) +
                            DAT_0081e448 * 4),L"Menu Color");
        if ((memory_compare_result == 0) && (local_78 < 3)) {
          local_78 = local_78 + 1;
          puVar4 = (undefined4 *)(&PTR_s__333_333_333_fff_0081c7e0)[local_78];
          DAT_0081c87c = *puVar4;
          DAT_0081c880 = puVar4[1];
          DAT_0081c884 = puVar4[2];
          puVar3 = (undefined4 *)(&PTR_DAT_0081c7d0)[local_78];
          DAT_0081c888 = puVar4[3];
          PTR_u__0081c88c = (undefined *)puVar4[4];
          DAT_0081c868 = *puVar3;
          DAT_0081c86c = puVar3[1];
          DAT_0081c870 = puVar3[2];
          DAT_0081c874 = puVar3[3];
          PTR_u_Default_0081c878 = (undefined *)puVar3[4];
          (*OSSleepTicks)(0,0x8e43ad);
        }
        if (DAT_0081e4b8 == 6) {
          if (DAT_0081e448 == 0) {
            player_count = (*CGameNetworkManager::GetPlayerCount)();
            if (DAT_0081e4f4 < player_count + -1) {
              DAT_0081e4f4 = DAT_0081e4f4 + 1;
              (*OSSleepTicks)(0,0x5ed7c9);
            }
          }
          else {
            if (DAT_0081e448 == 1) {
              DAT_0081e4f8 = DAT_0081e4f8 + 1;
            }
            else {
              if (DAT_0081e448 == 2) {
                DAT_0081e4fc = DAT_0081e4fc + 1;
              }
            }
          }
          (*OSSleepTicks)(0,0x25efea);
        }
      }
      if ((((_DAT_102efa64 & DAT_0081e4d0) != 0) || ((_DAT_10a8a7c8 & DAT_0081e518) != 0)) &&
         (DAT_0081e4b8 != 0)) {
        iVar3_2 = memcmp_maybe(**(undefined4 **)
                                 (*(int *)(*(int *)(menu_root->field_0x4 + DAT_0081e4b8 * 4 + -4) +
                                          8) + DAT_0081e448 * 4),L"Menu Color");
        if ((iVar3_2 == 0) && (local_78 != 0)) {
          local_78 = local_78 + -1;
          puVar4 = (undefined4 *)(&PTR_s__333_333_333_fff_0081c7e0)[local_78];
          DAT_0081c87c = *puVar4;
          DAT_0081c880 = puVar4[1];
          DAT_0081c884 = puVar4[2];
          puVar3 = (undefined4 *)(&PTR_DAT_0081c7d0)[local_78];
          DAT_0081c888 = puVar4[3];
          PTR_u__0081c88c = (undefined *)puVar4[4];
          DAT_0081c868 = *puVar3;
          DAT_0081c86c = puVar3[1];
          DAT_0081c870 = puVar3[2];
          DAT_0081c874 = puVar3[3];
          PTR_u_Default_0081c878 = (undefined *)puVar3[4];
          (*OSSleepTicks)(0,0x8e43ad);
        }
        if (DAT_0081e4b8 == 6) {
          if ((0 < DAT_0081e4f4) && (DAT_0081e448 == 0)) {
            DAT_0081e4f4 = DAT_0081e4f4 + -1;
            (*OSSleepTicks)(0,0x5ed7c9);
          }
          pcVar2 = OSSleepTicks;
          piVar1 = local_58;
          iVar3_ = DAT_0081e4f8;
          if (((DAT_0081e448 == 1) || (piVar1 = local_5c, iVar3_ = DAT_0081e4fc, DAT_0081e448 == 2))
             && (0 < iVar3_)) {
            *piVar1 = iVar3_ + -1;
          }
          (*pcVar2)(0,0x25efea);
        }
      }
    }
LAB_008051d8:
    (*OSSleepTicks)(0,0x1c73ef);
  } while( true );
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_008056a4(void)

{
  undefined4 *puVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  
  print_to_log("entered poketest\n");
  (*OSSleepTicks)(0,0xbdaf92);
  patch_memory(0x3f000010,0x3ff00000);
  patch_memory(0x3f000030,0x41f00000);
  FUN_00802224(0x3fecd4fe00000000,0x3f000050);
  if (DAT_0081e4e8 == '\0') {
                    // UIComponent_PressStartToPlay::tick
    patch_memory(0x2e0c964,0x60000000);
    patch_memory(0x2e0c980,0x60000000);
    patch_memory(0x2e0c98c,0x60000000);
    patch_memory(0x2e0c998,0x60000000);
                    // UIComponent_PressStartToPlay::handleTimerComplete
    patch_memory(0x2e0c6a0,0x60000000);
    patch_memory(0x2e0c9a4,0x38800001);
    (*ConsoleUIController::ShowPressStart)(0x109f95e0,1);
    if (is_trial_version == '\0') {
      draw_menu_instructions
                (0x3f990000,
                 L"\nL+R : OPEN THE MENU\nDPAD UP/LEFT : NAVIGATE\nZR : SELECT\nB : BACK\nDPAD LEFT/RIGHT : CHANGE VALUES\n\n(If you\'re playing in mini games and wanna pause the game while spectating, make sure you\'re spectating someone and not in free cam mode)\n\n"
                );
    }
    else {
      draw_menu_instructions(0x3f990000,&DAT_00818928);
    }
    FUN_0080214c((int)&DAT_10a09370,2);
    iVar3 = *_DAT_3f000000;
    if (iVar3 == 0) {
      do {
        if (_DAT_3f000040 != 0) {
          puVar1 = (undefined4 *)&DAT_fffffb10;
          DAT_0081e438 = -0x3350;
          DAT_0081e42c = 0x128;
          DAT_0081e41c = -0x250;
          DAT_0081e414 = -0x128;
          DAT_0081e418 = -0x378;
          goto LAB_00805954;
        }
        (*OSSleepTicks)(0,0x97bfa);
        iVar2 = *_DAT_3f000000;
      } while (iVar2 == 0);
      DAT_0081e418 = iVar2 + -0x378;
      DAT_0081e414 = iVar2 + -0x128;
      DAT_0081e41c = iVar2 + -0x250;
      DAT_0081e42c = iVar2 + 0x128;
      DAT_0081e438 = iVar2 + -0x3350;
      puVar1 = (undefined4 *)(&DAT_fffffb10 + iVar2);
      iVar3 = iVar2;
    }
    else {
      DAT_0081e418 = iVar3 + -0x378;
      DAT_0081e414 = iVar3 + -0x128;
      DAT_0081e41c = iVar3 + -0x250;
      DAT_0081e42c = iVar3 + 0x128;
      DAT_0081e438 = iVar3 + -0x3350;
      puVar1 = (undefined4 *)(&DAT_fffffb10 + iVar3);
    }
LAB_00805954:
    DAT_0081e430 = *puVar1;
    DAT_0081e420 = iVar3;
    DAT_0081e43c = DAT_0081e418;
    print_to_log("patched UI\n");
  }
  DAT_0081e42c = DAT_0081e418 + 0x4a0;
  DAT_0081e43c = DAT_0081e418;
  DAT_0081e414 = DAT_0081e418 + 0x250;
  DAT_0081e41c = DAT_0081e418 + 0x128;
  DAT_0081e420 = DAT_0081e418 + 0x378;
  DAT_0081e438 = DAT_0081e418 + -0x2fd8;
  DAT_0081e430 = *(undefined4 *)(DAT_0081e418 + -0x178);
  DAT_0081e4e8 = '\x01';
  (*OSSleepTicks)(0,0x97bfa);
  if (DAT_0081e418 != 0) {
    patch_memory(DAT_0081e414,0);
  }
  if ((DAT_0081e4e8 != '\0') && (DAT_0081e3a4 != '\0')) {
    FUN_00802224((double)*(float *)(DAT_0081e42c + 0x28),DAT_0081e43c + 0x28);
    FUN_00802224(0x3fe0000000000000,DAT_0081e43c + 0x24);
    FUN_00802224((double)DAT_0081c87c,DAT_0081e43c + 0x5c);
    FUN_00802224((double)DAT_0081c880,DAT_0081e43c + 0x60);
    FUN_00802224((double)DAT_0081c884,DAT_0081e43c + 100);
    FUN_00802224(0,DAT_0081e43c + 0x68);
    uVar4 = 0x4014000000000000;
    FUN_00802224((double)(*(float *)(DAT_0081e41c + 0x28) - 5.0),DAT_0081e42c + 0x28);
    FUN_00802224(uVar4,DAT_0081e42c + 0x18);
    FUN_00802224((double)DAT_0081c868,DAT_0081e42c + 0x5c);
    FUN_00802224((double)DAT_0081c86c,DAT_0081e42c + 0x60);
    FUN_00802224((double)DAT_0081c870,DAT_0081e42c + 100);
    FUN_00802224((double)DAT_0081c874,DAT_0081e42c + 0x68);
    if (is_trial_version == '\0') {
      draw_menu_instructions
                (0x3f990000,
                 L"\nL+R : OPEN THE MENU\nDPAD UP/LEFT : NAVIGATE\nZR : SELECT\nB : BACK\nDPAD LEFT/RIGHT : CHANGE VALUES\n\n(If you\'re playing in mini games and wanna pause the game while spectating, make sure you\'re spectating someone and not in free cam mode)\n\n"
                );
    }
    else {
      draw_menu_instructions(0x3f990000,&DAT_00818928);
    }
    print_to_log("Patched UI 2\n");
    return;
  }
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

undefined4 FUN_00805c60(void)

{
  int server_status;
  int license_check_result;
  int unknown1;
  undefined4 uVar1;
  int unknown2;
  menu_t *host_mods_menu;
  wchar_t *non_host_mods_menu;
  wchar_t *useless_mods_menu;
  wchar_t *miscellaneous_menu;
  wchar_t *mini_games_mods_menu;
  wchar_t *give_item_menu;
  wchar_t *menu_settings_menu;
  int iVar2;
  longlong title_id;
  
  title_id = (*OSGetTitleID)();
  if (((title_id != 0x50000101d7500) && (title_id = (*OSGetTitleID)(), title_id != 0x50000101d9d00))
     && (title_id = (*OSGetTitleID)(), title_id != 0x50000101dbe00)) {
    print_to_log_2("Not Minecraft, exiting thread ..\n");
    return 0;
  }
  (*OSSleepTicks)(0,0x4a189508);
  server_status = check_server();
  if (server_status == -6) {
    (*OSFatal)(
              "Server is down or in maintenance. Add Rambo6Glaz#3952 on discord for further informations"
              );
  }
  license_check_result = check_license();
  if (license_check_result == 4) {
    is_trial_version = '\x01';
  }
  else {
    if (license_check_result == 3) {
      is_trial_version = '\0';
    }
    else {
      (*OSFatal)("Banned.");
    }
  }
  unknown1 = FUN_00806b70(0x400,0x10);
  uVar1 = FUN_00806b70(0x700,0x10);
  if (unknown1 == 0) {
    (*OSFatal)(s_comme_une_envie_de_suicide_qui_s_00818e78);
  }
  unknown2 = (*DAT_0081c980)(uVar1,&UNK_00805b60,0,0,unknown1 + 0x400,0x400,1,10);
  if (unknown2 == 0) {
    (*OSFatal)("Couldn\'t create MenuThread");
  }
  (*DAT_0081c9b4)(uVar1,"MenuThreadla");
  (*DAT_0081c984)(uVar1);
  (*OSSleepTicks)(0,0x12862542);
  if (is_trial_version == '\0') {
    menu_root = create_menu_root();
    host_mods_menu = (menu_t *)0x818ef4;
    create_menu(L"Host Mods");
    non_host_mods_menu = L"Non Host Mods";
    create_menu(L"Non Host Mods");
    useless_mods_menu = L"Useless Mods";
    create_menu(L"Useless Mods");
    miscellaneous_menu = L"Misc";
    create_menu(L"Misc");
    mini_games_mods_menu = L"Mini Games Mods";
    create_menu(L"Mini Games Mods");
    give_item_menu = L"Give Item Menu";
    create_menu(L"Give Item Menu");
    menu_settings_menu = L"Menu Settings";
    create_menu(L"Menu Settings");
    add_menu_to_menu_root(menu_root,host_mods_menu);
    add_menu_option(host_mods_menu,L"God Mode",god_mode,(int)&god_mode_enabled);
    add_menu_option(host_mods_menu,L"God Mode For All Entities",god_mode_for_all_entities,
                    (int)&DAT_0081e48c);
    add_menu_option(host_mods_menu,L"Infinite Food",infinite_food,(int)&infinite_food_enabled);
    add_menu_option(host_mods_menu,L"Unlimited Blocks",unlimited_blocks,(int)&DAT_0081e458);
    add_menu_option(host_mods_menu,L"Fall Damage",fall_damage,(int)&DAT_0081e478);
    add_menu_to_menu_root(menu_root,non_host_mods_menu);
    add_menu_option((menu_t *)non_host_mods_menu,L"Fly",fly,(int)&DAT_0081e47c);
    add_menu_option((menu_t *)non_host_mods_menu,L"Speed Hack",speed_hack,(int)&DAT_0081e4b0);
    add_menu_option((menu_t *)non_host_mods_menu,L"Multi Jump",multi_jump,(int)&DAT_0081e49c);
    add_menu_option((menu_t *)non_host_mods_menu,L"FOV",fov,(int)&DAT_0081e484);
    add_menu_option((menu_t *)non_host_mods_menu,L"Free Crafting",free_crafting,(int)&DAT_0081e45c);
    add_menu_option((menu_t *)non_host_mods_menu,L"Reach",reach,(int)&DAT_0081e4ac);
    add_menu_option((menu_t *)non_host_mods_menu,L"XRAY",xray,(int)&DAT_0081e4c4);
    add_menu_option((menu_t *)non_host_mods_menu,L"Kill Aura",kill_aura,(int)&DAT_0081e454);
    add_menu_option((menu_t *)non_host_mods_menu,L"Water Fly",water_fly,(int)&DAT_0081e4c0);
    add_menu_option((menu_t *)non_host_mods_menu,L"Instant Mining",instant_mining,(int)&DAT_0081e4a8
                   );
    add_menu_option((menu_t *)non_host_mods_menu,L"Draw Hitboxes",draw_hitboxes,(int)&DAT_0081e494);
    add_menu_option((menu_t *)non_host_mods_menu,L"ESP",&esp,(int)&DAT_0081e46c);
    add_menu_option((menu_t *)non_host_mods_menu,L"Auto Sprint",auto_sprint,(int)&DAT_0081e4b4);
    add_menu_option((menu_t *)non_host_mods_menu,L"Super Jump",super_jump,(int)&DAT_0081e4bc);
    add_menu_option((menu_t *)non_host_mods_menu,L"Infinite Air",infinite_air,(int)&DAT_0081e450);
    add_menu_to_menu_root(menu_root,useless_mods_menu);
    add_menu_option((menu_t *)useless_mods_menu,L"Fake Death",fake_death,(int)&DAT_0081e470);
    add_menu_option((menu_t *)useless_mods_menu,L"Fake Sleep",fake_sleep,(int)&DAT_0081e474);
    add_menu_to_menu_root(menu_root,miscellaneous_menu);
    add_menu_option((menu_t *)miscellaneous_menu,L"Item Debug Text",item_debug_text,
                    (int)&DAT_0081e464);
    add_menu_option((menu_t *)miscellaneous_menu,L"Draw Debug Text",draw_debug_text,
                    (int)&should_draw_debug_text);
    add_menu_option((menu_t *)miscellaneous_menu,L"Hide Arm",hide_arm,(int)&DAT_0081e490);
    add_menu_option((menu_t *)miscellaneous_menu,L"Deadmau5 ears",deadmanu5_ears,(int)&DAT_0081e460)
    ;
    add_menu_to_menu_root(menu_root,mini_games_mods_menu);
    add_menu_option((menu_t *)mini_games_mods_menu,L"All Permissions",all_permissions,
                    (int)&DAT_0081e4a4);
    add_menu_option((menu_t *)mini_games_mods_menu,L"Creative",creative,(int)&DAT_0081e4a0);
    add_menu_to_menu_root(menu_root,give_item_menu);
    add_menu_option((menu_t *)give_item_menu,L"Player : ",&DAT_00802888,(int)&DAT_0081e3ac);
    add_menu_option((menu_t *)give_item_menu,L"Item ID : ",&DAT_00802888,(int)&DAT_0081e3b4);
    add_menu_option((menu_t *)give_item_menu,L"Quantity : ",&DAT_00802888,(int)&DAT_0081e3b8);
    add_menu_option((menu_t *)give_item_menu,L"Give item",&LAB_00804888,(int)&DAT_0081e3a8);
    add_menu_option((menu_t *)give_item_menu,L"Give item to everyone",&LAB_00804a50,
                    (int)&DAT_0081e3a8);
    add_menu_to_menu_root(menu_root,menu_settings_menu);
    add_menu_option((menu_t *)menu_settings_menu,L"Menu Color",&DAT_00802888,(int)&DAT_0081e3b0);
  }
  print_to_log("hooking...\n");
                    // FJ_PressStartToPlay::SetMultiLineText
  patch_memory(0x2b81318,0x48802867);
                    // UIComponent_PressStartToPlay::tick
  patch_memory(0x2e0c92c,0x4880287b);
  DAT_0081e4c8 = 1;
  print_to_log("hooked\n");
  server_status = _DAT_3f000040;
  while ((iVar2 = _DAT_3f000040, _DAT_3f000040 == 0 && (iVar2 = server_status, _DAT_3f000000 == 0)))
  {
    (*OSSleepTicks)(0,0x97bfa);
  }
  do {
    DAT_0081e428 = _DAT_3f000080;
    DAT_0081e410 = _DAT_3f000090;
    DAT_0081e434 = iVar2;
                    // UIComponent_PressStartToPlay::showPressStart+xxx lis r3, 0x3F99
    patch_memory(0x2da9508,0x3c603f99);
                    // addi r3,r3,0
    patch_memory(0x2da950c,0x38630000);
                    // DLCManager::IsDlcPackTemporarilyFree
    patch_memory(0x2ae8b30,0x38600001);
    DAT_0081e524 = 0x8000;
    DAT_0081e514 = 0x4000;
    DAT_0081e4f0 = 0x40;
    DAT_0081e510 = 0x40;
    DAT_0081e440 = 0x4000;
    DAT_0081e4ec = 0x400;
    DAT_0081e4d0 = 0x800;
    DAT_0081e50c = 0x200;
    DAT_0081e520 = 0x200;
    DAT_0081e444 = 0x100;
    DAT_0081e4dc = 0x20;
    iVar2 = 0x820000;
    DAT_0081e4e0 = 0x10;
    DAT_0081e528 = 4;
    DAT_0081e518 = 2;
    DAT_0081e52c = 1;
    DAT_0081e51c = 0x2000;
    FUN_00804c54();
  } while( true );
}



void FUN_008065ac(void)

{
  undefined4 local_18 [5];
  
  initialize_os_specific_functions();
  FUN_008090a0();
  local_18[0] = 1;
  DAT_0081c890 = (*DAT_0081cce8)(2,2,0x11);
  if (-1 < DAT_0081c890) {
    (*DAT_0081cd14)(DAT_0081c890,0xffffffff,0x20,local_18,4);
    DAT_0081c89c = 0;
    DAT_0081c8a0 = 0;
    DAT_0081c894 = 0x21135;
    DAT_0081c898 = 0xffffffff;
  }
  return;
}



void print_to_log(char *text)

{
  int iVar1;
  int iVar2;
  
  if (-1 < DAT_0081c890) {
    while (DAT_0081c8a4 != 0) {
      (*OSSleepTicks)(0,0xf2cc);
    }
    DAT_0081c8a4 = 1;
    for (iVar1 = FUN_0080c610(text); 0 < iVar1; iVar1 = iVar1 - iVar2) {
      iVar2 = iVar1;
      if (0x578 < iVar1) {
        iVar2 = 0x578;
      }
      iVar2 = (*DAT_0081cd10)(DAT_0081c890,text,iVar2,0,&DAT_0081c894,0x10);
      if (iVar2 < 0) break;
      text = text + iVar2;
    }
    DAT_0081c8a4 = 0;
  }
  return;
}



void FUN_00806718(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                 undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
                 undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16)

{
  int iVar1;
  byte in_cr1;
  undefined local_70;
  undefined local_6f;
  undefined *local_6c;
  undefined *local_68;
  int local_64;
  undefined auStack96 [4];
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  undefined8 local_10;
  undefined8 local_8;
  
  if ((bool)(in_cr1 >> 1 & 1)) {
    local_40 = param_1;
    local_38 = param_2;
    local_30 = param_3;
    local_28 = param_4;
    local_20 = param_5;
    local_18 = param_6;
    local_10 = param_7;
    local_8 = param_8;
  }
  local_64 = 0;
  local_6f = 0;
  local_6c = &stack0x00000008;
  local_68 = auStack96;
  local_70 = 1;
  local_5c = param_10;
  local_58 = param_11;
  local_54 = param_12;
  local_50 = param_13;
  local_4c = param_14;
  local_48 = param_15;
  local_44 = param_16;
  iVar1 = FUN_0080e9a0(&local_64,param_9,&local_70);
  if ((-1 < iVar1) && (local_64 != 0)) {
    (*OSFatal)();
  }
  return;
}



void print_to_log_2(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                   undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                   undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
                   undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16)

{
  int iVar1;
  byte in_cr1;
  undefined local_70;
  undefined local_6f;
  undefined *local_6c;
  undefined *local_68;
  char *local_64;
  undefined auStack96 [4];
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  undefined8 local_10;
  undefined8 local_8;
  
  if ((bool)(in_cr1 >> 1 & 1)) {
    local_40 = param_1;
    local_38 = param_2;
    local_30 = param_3;
    local_28 = param_4;
    local_20 = param_5;
    local_18 = param_6;
    local_10 = param_7;
    local_8 = param_8;
  }
  if (-1 < DAT_0081c890) {
    local_64 = (char *)0x0;
    local_6f = 0;
    local_6c = &stack0x00000008;
    local_68 = auStack96;
    local_70 = 1;
    local_5c = param_10;
    local_58 = param_11;
    local_54 = param_12;
    local_50 = param_13;
    local_4c = param_14;
    local_48 = param_15;
    local_44 = param_16;
    iVar1 = FUN_0080e9a0(&local_64,param_9,&local_70);
    if ((-1 < iVar1) && (local_64 != (char *)0x0)) {
      print_to_log(local_64);
    }
    if (local_64 != (char *)0x0) {
      FUN_00806a60();
    }
  }
  return;
}



void FUN_00806894(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  int iVar3;
  
  uVar1 = (*DAT_0081ca18)(0);
  uVar2 = (*DAT_0081ca24)(uVar1,4);
  iVar3 = (*DAT_0081ca28)(uVar1,uVar2,4);
  if (iVar3 != 0) {
    DAT_0081c8a8 = (*DAT_0081ca34)(iVar3,uVar2,0);
  }
  uVar1 = (*DAT_0081ca18)(8);
  uVar2 = (*DAT_0081ca24)(uVar1,4);
  iVar3 = (*DAT_0081ca28)(uVar1,uVar2,4);
  if (iVar3 != 0) {
    DAT_0081c8ac = (*DAT_0081ca34)(iVar3,uVar2,0);
  }
  return;
}



void FUN_00806988(void)

{
  code *pcVar1;
  undefined4 uVar2;
  
  (*DAT_0081ca3c)(DAT_0081c8a8);
  pcVar1 = DAT_0081ca2c;
  uVar2 = (*DAT_0081ca18)(0);
  (*pcVar1)(uVar2,3);
  DAT_0081c8a8 = 0xffffffff;
  (*DAT_0081ca3c)(DAT_0081c8ac);
  pcVar1 = DAT_0081ca2c;
  uVar2 = (*DAT_0081ca18)(8);
  (*pcVar1)(uVar2,3);
  DAT_0081c8ac = 0xffffffff;
  return;
}



void FUN_00806a20(void)

{
                    // WARNING: Could not recover jumptable at 0x00806a30. Too many branches
                    // WARNING: Treating indirect jump as call
  (**DAT_0081ca08)();
  return;
}



void FUN_00806a34(uint param_1,undefined4 param_2)

{
  if (param_1 < 4) {
    param_1 = 4;
  }
                    // WARNING: Could not recover jumptable at 0x00806a5c. Too many branches
                    // WARNING: Treating indirect jump as call
  (**DAT_0081ca04)(param_2,param_1);
  return;
}



void FUN_00806a60(int param_1)

{
  if (param_1 == 0) {
    return;
  }
                    // WARNING: Could not recover jumptable at 0x00806a78. Too many branches
                    // WARNING: Treating indirect jump as call
  (**DAT_0081ca0c)();
  return;
}



int FUN_00806a7c(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = FUN_00806a20(param_1 * param_2);
  if (iVar1 != 0) {
    FUN_0080c398(iVar1,0,param_1 * param_2);
  }
  return iVar1;
}



void FUN_00806b30(undefined4 param_1,undefined4 param_2)

{
  FUN_00806a20(param_2);
  return;
}



void FUN_00806b38(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  FUN_00806a7c(param_2,param_3);
  return;
}



void FUN_00806b50(undefined4 param_1,undefined4 param_2)

{
  FUN_00806a60(param_2);
  return;
}



int FUN_00806b64(undefined4 param_1,int param_2,undefined4 param_3)

{
  int iVar1;
  
  if (param_2 == 0) {
    iVar1 = FUN_00806a20(param_3);
    return iVar1;
  }
  iVar1 = FUN_00806a20(param_3);
  if (iVar1 != 0) {
    FUN_0080c25c(iVar1,param_2,param_3);
    FUN_00806a60(param_2);
  }
  return iVar1;
}



void FUN_00806b70(undefined4 param_1,undefined4 param_2)

{
  FUN_00806a34(param_2,param_1);
  return;
}



undefined8 FUN_00806da4(void)

{
  syscall();
  return 0x100000000;
}



undefined8 FUN_00806df8(void)

{
  syscall();
  return 0x100000000;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00806e48(void)

{
  FUN_00806df8(_DAT_0080150c + 0x94,&LAB_00806c14);
  FUN_00806df8(_DAT_00801510 + 0x94,&LAB_00806c14);
  FUN_00806df8(_DAT_00801514 + 0x94,&LAB_00806c14);
  FUN_00806df8(_DAT_00801518 + 0x94,&LAB_00806c14);
  FUN_00806df8(_DAT_0080151c + 0x94,&LAB_00806c14);
  return;
}



void write_memory(void)

{
                    // 0x2500
  syscall();
  return;
}



void FUN_00806ed8(void)

{
  if (DAT_0081c8e0 == 0) {
    FUN_00807194();
  }
                    // WARNING: Could not recover jumptable at 0x00806f20. Too many branches
                    // WARNING: Treating indirect jump as call
  (*DAT_0081c8dc)("sysapp.rpl",&DAT_0081c8b0);
  return;
}



void FUN_00806f44(void)

{
  int local_18 [4];
  
  local_18[0] = 0;
  FUN_00806ed8();
  resolve_function(DAT_0081c8b0,(int)"_SYSLaunchTitleByPathFromLauncher",local_18);
  DAT_0081c8b4 = local_18[0];
  resolve_function(DAT_0081c8b0,(int)"SYSRelaunchTitle",local_18);
  DAT_0081c8b8 = local_18[0];
  resolve_function(DAT_0081c8b0,(int)"SYSLaunchMenu",local_18);
  DAT_0081c8bc = local_18[0];
  resolve_function(DAT_0081c8b0,(int)"_SYSLaunchMenuWithCheckingAccount",local_18);
  DAT_0081c8c0 = local_18[0];
  resolve_function(DAT_0081c8b0,(int)"SYSCheckTitleExists",local_18);
  DAT_0081c8c4 = local_18[0];
  resolve_function(DAT_0081c8b0,(int)"SYSLaunchTitle",local_18);
  DAT_0081c8c8 = local_18[0];
  resolve_function(DAT_0081c8b0,(int)"SYSLaunchSettings",local_18);
  DAT_0081c8cc = local_18[0];
  resolve_function(DAT_0081c8b0,(int)"_SYSGetSystemApplicationTitleId",local_18);
  DAT_0081c8d0 = local_18[0];
  return;
}



void * resolve_function(undefined4 param_1,int param_2,int *param_3)

{
  void *pvVar1;
  int iVar2;
  char *pcVar3;
  char *pcVar4;
  int iVar5;
  undefined local_120 [9];
  char local_117 [8];
  char local_10f [239];
  undefined4 local_20;
  undefined4 local_1c;
  undefined2 local_18;
  undefined4 local_14;
  char local_c;
  
  pvVar1 = (void *)(*DAT_0081c8d8)(param_1,0,param_2,param_3);
  if (*param_3 == 0) {
    local_1c = 0x74696f6e;
    local_18 = 0x2000;
    local_20 = 0x46756e63;
    iVar5 = 9;
    local_14 = 0x20697320;
    local_c = '\0';
    iVar2 = 0;
    do {
      local_120[iVar2] = *(undefined *)((int)&local_20 + iVar2);
      iVar2 = iVar2 + 1;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    pcVar4 = (char *)(param_2 + -1);
    pcVar3 = local_117;
    while( true ) {
      pcVar4 = pcVar4 + 1;
      if (*pcVar4 == '\0') break;
      *pcVar3 = *pcVar4;
      pcVar3 = pcVar3 + 1;
    }
    iVar2 = 0;
    iVar5 = 8;
    do {
      pcVar3[iVar2] = *(char *)((int)&local_14 + iVar2);
      iVar2 = iVar2 + 1;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    pcVar3[8] = '\0';
    pvVar1 = (void *)(*OSFatal)();
    return pvVar1;
  }
  return pvVar1;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void FUN_00807194(void)

{
  DAT_0081c8dc = _DAT_00801500;
  DAT_0081c8d8 = _DAT_00801504;
                    // WARNING: Could not recover jumptable at 0x008071c4. Too many branches
                    // WARNING: Treating indirect jump as call
  (*_DAT_00801500)("coreinit.rpl");
  return;
}



// WARNING: Globals starting with '_' overlap smaller symbols at the same address

void initialize_os_specific_functions(void)

{
  code *local_18 [4];
  
  local_18[0] = (code *)0x0;
  FUN_00807194();
  resolve_function(DAT_0081c8e0,(int)"OSDynLoad_Release",(int *)local_18);
  OSDynLoad_Release = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetSecurityLevel",(int *)local_18);
  OSGetSecurityLevel = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSForceFullRelaunch",(int *)local_18);
  OSForceFullRelaunch = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSFatal",(int *)local_18);
  OSFatal = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetTitleID",(int *)local_18);
  OSGetTitleID = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetArgcArgv",(int *)local_18);
  OSGetArgcArgv = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSetExceptionCallback",(int *)local_18);
  OSSetExceptionCallback = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSetExceptionCallbackEx",(int *)local_18);
  OSSetExceptionCallbackEx = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSLoadContext",(int *)local_18);
  OSLoadContext = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"DCFlushRange",(int *)local_18);
  DCFlushRange = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"DCStoreRange",(int *)local_18);
  DCStoreRange = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"ICInvalidateRange",(int *)local_18);
  ICInvalidateRange = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSEffectiveToPhysical",(int *)local_18);
  OSEffectiveToPhysical = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"__OSPhysicalToEffectiveUncached",(int *)local_18);
  __OSPhysicalToEffectiveUncached = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"__OSValidateAddressSpaceRange",(int *)local_18);
  __OSValidateAddressSpaceRange = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"__os_snprintf",(int *)local_18);
  __os_snprintf = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"__gh_errno_ptr",(int *)local_18);
  __gh_errno_ptr = local_18[0];
  (*DAT_0081c8d8)(DAT_0081c8e0,0,&UNK_0081954a,&DAT_0081c924);
  resolve_function(DAT_0081c8e0,(int)"OSScreenInit",(int *)local_18);
  OSScreenInit = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSScreenShutdown",(int *)local_18);
  OSSceenShutdown = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSScreenGetBufferSizeEx",(int *)local_18);
  OSScreenGetBufferSizeEx = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSScreenSetBufferEx",(int *)local_18);
  OSScreenSetBufferEx = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSScreenClearBufferEx",(int *)local_18);
  OSScreenClearBufferEx = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSScreenFlipBuffersEx",(int *)local_18);
  OSScreenFlipBuffersEx = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSScreenPutFontEx",(int *)local_18);
  OSScreenPutFontEx = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSScreenEnableEx",(int *)local_18);
  OSScreenEnableEx = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSScreenPutPixelEx",(int *)local_18);
  OSScreenPutPixelEx = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"DisassemblePPCRange",(int *)local_18);
  DAT_0081c94c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"DisassemblePPCOpcode",(int *)local_18);
  DAT_0081c950 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetSymbolName",(int *)local_18);
  DAT_0081c954 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetSymbolNameEx",(int *)local_18);
  DAT_0081c958 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSIsDebuggerInitialized",(int *)local_18);
  DAT_0081c95c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetSharedData",(int *)local_18);
  DAT_0081c960 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSEnableInterrupts",(int *)local_18);
  DAT_0081c964 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"__OSClearAndEnableInterrupt",(int *)local_18);
  DAT_0081c968 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSIsInterruptEnabled",(int *)local_18);
  DAT_0081c96c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSIsDebuggerPresent",(int *)local_18);
  DAT_0081c970 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSRestoreInterrupts",(int *)local_18);
  DAT_0081c974 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSetDABR",(int *)local_18);
  DAT_0081c978 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSetIABR",(int *)local_18);
  DAT_0081c97c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSCreateThread",(int *)local_18);
  DAT_0081c980 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSResumeThread",(int *)local_18);
  DAT_0081c984 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSuspendThread",(int *)local_18);
  DAT_0081c988 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSIsThreadTerminated",(int *)local_18);
  DAT_0081c98c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSIsThreadSuspended",(int *)local_18);
  DAT_0081c990 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSJoinThread",(int *)local_18);
  DAT_0081c994 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSetThreadPriority",(int *)local_18);
  DAT_0081c998 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSDetachThread",(int *)local_18);
  DAT_0081c99c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetCurrentThread",(int *)local_18);
  DAT_0081c9a0 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetThreadName",(int *)local_18);
  DAT_0081c9a4 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetActiveThreadLink",(int *)local_18);
  DAT_0081c9a8 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetThreadAffinity",(int *)local_18);
  DAT_0081c9ac = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetThreadPriority",(int *)local_18);
  DAT_0081c9b0 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSetThreadName",(int *)local_18);
  DAT_0081c9b4 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetCoreId",(int *)local_18);
  DAT_0081c9b8 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSleepTicks",(int *)local_18);
  OSSleepTicks = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetTick",(int *)local_18);
  DAT_0081c9c0 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetTime",(int *)local_18);
  DAT_0081c9c4 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetSystemTime",(int *)local_18);
  DAT_0081c9c8 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSTicksToCalendarTime",(int *)local_18);
  DAT_0081c9cc = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSInitMessageQueue",(int *)local_18);
  DAT_0081c9d0 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSendMessage",(int *)local_18);
  DAT_0081c9d4 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSReceiveMessage",(int *)local_18);
  DAT_0081c9d8 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSInitMutex",(int *)local_18);
  DAT_0081c9dc = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSLockMutex",(int *)local_18);
  DAT_0081c9e0 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSUnlockMutex",(int *)local_18);
  DAT_0081c9e4 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSTryLockMutex",(int *)local_18);
  DAT_0081c9e8 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MCP_Open",(int *)local_18);
  DAT_0081c9ec = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MCP_Close",(int *)local_18);
  DAT_0081c9f0 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MCP_TitleCount",(int *)local_18);
  DAT_0081c9f4 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MCP_TitleList",(int *)local_18);
  DAT_0081c9f8 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MCP_GetOwnTitleInfo",(int *)local_18);
  DAT_0081c9fc = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MCP_GetDeviceId",(int *)local_18);
  DAT_0081ca00 = local_18[0];
  (*DAT_0081c8d8)(DAT_0081c8e0,1,"MEMAllocFromDefaultHeapEx",&DAT_0081ca04);
  (*DAT_0081c8d8)(DAT_0081c8e0,1,"MEMAllocFromDefaultHeap",&DAT_0081ca08);
  (*DAT_0081c8d8)(DAT_0081c8e0,1,"MEMFreeToDefaultHeap",&DAT_0081ca0c);
  resolve_function(DAT_0081c8e0,(int)"MEMAllocFromAllocator",(int *)local_18);
  DAT_0081ca10 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMFreeToAllocator",(int *)local_18);
  DAT_0081ca14 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMGetBaseHeapHandle",(int *)local_18);
  DAT_0081ca18 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMGetTotalFreeSizeForExpHeap",(int *)local_18);
  DAT_0081ca1c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMGetAllocatableSizeForExpHeapEx",(int *)local_18);
  DAT_0081ca20 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMGetAllocatableSizeForFrmHeapEx",(int *)local_18);
  DAT_0081ca24 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMAllocFromFrmHeapEx",(int *)local_18);
  DAT_0081ca28 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMFreeToFrmHeap",(int *)local_18);
  DAT_0081ca2c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMAllocFromExpHeapEx",(int *)local_18);
  DAT_0081ca30 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMCreateExpHeapEx",(int *)local_18);
  DAT_0081ca34 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMCreateFrmHeapEx",(int *)local_18);
  DAT_0081ca38 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMDestroyExpHeap",(int *)local_18);
  DAT_0081ca3c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMFreeToExpHeap",(int *)local_18);
  DAT_0081ca40 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSAllocFromSystem",(int *)local_18);
  OSAllocFromSystem = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSFreeToSystem",(int *)local_18);
  OSFreeToSystem = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSIsAddressValid",(int *)local_18);
  OSIsAddressValid = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"MEMFindParentHeap",(int *)local_18);
  MEMFindParentHeap = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSGetMemBound",(int *)local_18);
  DAT_0081ca54 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"DCInvalidateRange",(int *)local_18);
  DAT_0081ca58 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSDynLoad_GetModuleName",(int *)local_18);
  DAT_0081ca5c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSIsHomeButtonMenuEnabled",(int *)local_18);
  DAT_0081ca60 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSEnableHomeButtonMenu",(int *)local_18);
  DAT_0081ca64 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSetScreenCapturePermissionEx",(int *)local_18);
  DAT_0081ca68 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IMEnableDim",(int *)local_18);
  DAT_0081ca6c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IMDisableDim",(int *)local_18);
  DAT_0081ca70 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IMIsDimEnabled",(int *)local_18);
  DAT_0081ca74 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IMEnableAPD",(int *)local_18);
  DAT_0081ca78 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IMDisableAPD",(int *)local_18);
  DAT_0081ca7c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IMIsAPDEnabled",(int *)local_18);
  DAT_0081ca80 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IMIsAPDEnabledBySysSettings",(int *)local_18);
  DAT_0081ca84 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"OSSendAppSwitchRequest",(int *)local_18);
  DAT_0081ca88 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IOS_Ioctl",(int *)local_18);
  DAT_0081ca8c = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IOS_IoctlAsync",(int *)local_18);
  DAT_0081ca90 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IOS_Open",(int *)local_18);
  DAT_0081ca94 = local_18[0];
  resolve_function(DAT_0081c8e0,(int)"IOS_Close",(int *)local_18);
  DAT_0081ca98 = local_18[0];
  if (_DAT_00801404 == 0x226) {
    DAT_0081ca9c = 0x1010180;
    DAT_0081caa0 = 0x101006c;
    DAT_0081caa4 = 0x100080c;
    DAT_0081caa8 = 0xfff184e4;
    DAT_0081caac = -0x101e6180;
LAB_00807e6c:
    DAT_0081cab0 = DAT_0081caac + -0x60c4;
  }
  else {
    if ((_DAT_00801404 == 0x214) || (_DAT_00801404 == 0x21c)) {
      DAT_0081ca9c = 0x100ffa4;
      DAT_0081caa0 = 0x100fe90;
      DAT_0081caa8 = 0xfff18558;
    }
    else {
      if ((_DAT_00801404 != 500) && (_DAT_00801404 != 0x1fe)) {
        if (_DAT_00801404 == 0x19a) {
          DAT_0081caa8 = 0xfff166dc;
        }
        else {
          if (_DAT_00801404 != 400) {
            if (_DAT_00801404 == 0x136) {
              DAT_0081caa8 = 0xfff15a0c;
            }
            else {
              if (_DAT_00801404 != 300) {
                (*OSFatal)("Missing all OS specific addresses.");
                return;
              }
              DAT_0081caa8 = 0xfff15974;
            }
            DAT_0081ca9c = 0x100c4e4;
            DAT_0081caa0 = 0x100c3d4;
            DAT_0081caa4 = 0x10004d8;
            DAT_0081caac = 0xefe19340;
            DAT_0081cab0 = 0xefe1329c;
            return;
          }
          DAT_0081caa8 = 0xfff15e70;
        }
        DAT_0081caa4 = 0x10007f8;
        DAT_0081caa0 = 0x100f678;
        DAT_0081ca9c = 0x100f78c;
        DAT_0081caac = -0x101e6340;
        goto LAB_00807e6c;
      }
      DAT_0081ca9c = 0x100fbc4;
      DAT_0081caa0 = 0x100fab0;
      DAT_0081caa8 = 0xfff18534;
    }
    DAT_0081caa4 = 0x10007ec;
    DAT_0081caac = -0x101e6300;
    DAT_0081cab0 = -0x101ec3c4;
  }
  return;
}



void FUN_00808054(void)

{
  if (DAT_0081c8e0 == 0) {
    FUN_00807194();
  }
  (*DAT_0081c8dc)(&UNK_00819c00,&DAT_0081cab4);
                    // WARNING: Could not recover jumptable at 0x008080c4. Too many branches
                    // WARNING: Treating indirect jump as call
  (*DAT_0081c8dc)("vpadbase.rpl",&DAT_0081cab8);
  return;
}



void FUN_008080c8(void)

{
  int local_18 [4];
  
  local_18[0] = 0;
  FUN_00808054();
  resolve_function(DAT_0081cab4,(int)"VPADInit",local_18);
  DAT_0081cabc = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADShutdown",local_18);
  DAT_0081cac0 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADRead",local_18);
  DAT_0081cac4 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetAccParam",local_18);
  DAT_0081cac8 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetAccParam",local_18);
  DAT_0081cacc = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetBtnRepeat",local_18);
  DAT_0081cad0 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADEnableStickCrossClamp",local_18);
  DAT_0081cad4 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADDisableStickCrossClamp",local_18);
  DAT_0081cad8 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetLStickClampThreshold",local_18);
  DAT_0081cadc = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetRStickClampThreshold",local_18);
  DAT_0081cae0 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetLStickClampThreshold",local_18);
  DAT_0081cae4 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetRStickClampThreshold",local_18);
  DAT_0081cae8 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetStickOrigin",local_18);
  DAT_0081caec = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADDisableLStickZeroClamp",local_18);
  DAT_0081caf0 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADDisableRStickZeroClamp",local_18);
  DAT_0081caf4 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADEnableLStickZeroClamp",local_18);
  DAT_0081caf8 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADEnableRStickZeroClamp",local_18);
  DAT_0081cafc = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetCrossStickEmulationParamsL",local_18);
  DAT_0081cb00 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetCrossStickEmulationParamsR",local_18);
  DAT_0081cb04 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetCrossStickEmulationParamsL",local_18);
  DAT_0081cb08 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetCrossStickEmulationParamsR",local_18);
  DAT_0081cb0c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroAngle",local_18);
  DAT_0081cb10 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroDirection",local_18);
  DAT_0081cb14 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroDirectionMag",local_18);
  DAT_0081cb18 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroMagnification",local_18);
  DAT_0081cb1c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADEnableGyroZeroPlay",local_18);
  DAT_0081cb20 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADEnableGyroDirRevise",local_18);
  DAT_0081cb24 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADEnableGyroAccRevise",local_18);
  DAT_0081cb28 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADDisableGyroZeroPlay",local_18);
  DAT_0081cb2c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADDisableGyroDirRevise",local_18);
  DAT_0081cb30 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADDisableGyroAccRevise",local_18);
  DAT_0081cb34 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADIsEnableGyroZeroPlay",local_18);
  DAT_0081cb38 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADIsEnableGyroZeroDrift",local_18);
  DAT_0081cb3c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADIsEnableGyroDirRevise",local_18);
  DAT_0081cb40 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADIsEnableGyroAccRevise",local_18);
  DAT_0081cb44 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroZeroPlayParam",local_18);
  DAT_0081cb48 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroDirReviseParam",local_18);
  DAT_0081cb4c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroAccReviseParam",local_18);
  DAT_0081cb50 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroDirReviseBase",local_18);
  DAT_0081cb54 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetGyroZeroPlayParam",local_18);
  DAT_0081cb58 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetGyroDirReviseParam",local_18);
  DAT_0081cb5c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetGyroAccReviseParam",local_18);
  DAT_0081cb60 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADInitGyroZeroPlayParam",local_18);
  DAT_0081cb64 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADInitGyroDirReviseParam",local_18);
  DAT_0081cb68 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADInitGyroAccReviseParam",local_18);
  DAT_0081cb6c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADInitGyroZeroDriftMode",local_18);
  DAT_0081cb70 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetGyroZeroDriftMode",local_18);
  DAT_0081cb74 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetGyroZeroDriftMode",local_18);
  DAT_0081cb78 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADCalcTPCalibrationParam",local_18);
  DAT_0081cb7c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetTPCalibrationParam",local_18);
  DAT_0081cb80 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetTPCalibrationParam",local_18);
  DAT_0081cb84 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetTPCalibratedPoint",local_18);
  DAT_0081cb88 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetTPCalibratedPointEx",local_18);
  DAT_0081cb8c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADControlMotor",local_18);
  DAT_0081cb90 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADStopMotor",local_18);
  DAT_0081cb94 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetLcdMode",local_18);
  DAT_0081cb98 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADGetLcdMode",local_18);
  DAT_0081cb9c = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetSensorBar",local_18);
  DAT_0081cba0 = local_18[0];
  resolve_function(DAT_0081cab4,(int)"VPADSetSamplingCallback",local_18);
  DAT_0081cba4 = local_18[0];
  resolve_function(DAT_0081cab8,(int)"VPADBASEGetMotorOnRemainingCount",local_18);
  DAT_0081cba8 = local_18[0];
  resolve_function(DAT_0081cab8,(int)"VPADBASESetMotorOnRemainingCount",local_18);
  DAT_0081cbac = local_18[0];
  resolve_function(DAT_0081cab8,(int)"VPADBASESetSensorBarSetting",local_18);
  DAT_0081cbb0 = local_18[0];
  resolve_function(DAT_0081cab8,(int)"VPADBASEGetSensorBarSetting",local_18);
  DAT_0081cbb4 = local_18[0];
  return;
}



void FUN_008087d8(void)

{
  if (DAT_0081c8e0 == 0) {
    FUN_00807194();
  }
                    // WARNING: Could not recover jumptable at 0x00808820. Too many branches
                    // WARNING: Treating indirect jump as call
  (*DAT_0081c8dc)(&UNK_0081a211,&DAT_0081cbb8);
  return;
}



void FUN_00808844(void)

{
  int local_18 [4];
  
  local_18[0] = 0;
  FUN_008087d8();
  resolve_function(DAT_0081cbb8,(int)"GX2Init",local_18);
  DAT_0081cbbc = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2Shutdown",local_18);
  DAT_0081cbc0 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2Flush",local_18);
  DAT_0081cbc4 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2GetMainCoreId",local_18);
  DAT_0081cbc8 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2DrawDone",local_18);
  DAT_0081cbcc = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2ClearColor",local_18);
  DAT_0081cbd0 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetViewport",local_18);
  DAT_0081cbd4 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetScissor",local_18);
  DAT_0081cbd8 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetContextState",local_18);
  DAT_0081cbdc = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2DrawEx",local_18);
  DAT_0081cbe0 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2DrawIndexedEx",local_18);
  DAT_0081cbe4 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2ClearDepthStencilEx",local_18);
  DAT_0081cbe8 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CopyColorBufferToScanBuffer",local_18);
  DAT_0081cbec = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SwapScanBuffers",local_18);
  DAT_0081cbf0 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetTVEnable",local_18);
  DAT_0081cbf4 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetSwapInterval",local_18);
  DAT_0081cbf8 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2GetSwapInterval",local_18);
  DAT_0081cbfc = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2WaitForVsync",local_18);
  DAT_0081cc00 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CalcTVSize",local_18);
  DAT_0081cc04 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2Invalidate",local_18);
  DAT_0081cc08 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetTVBuffer",local_18);
  DAT_0081cc0c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CalcSurfaceSizeAndAlignment",local_18);
  DAT_0081cc10 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2InitDepthBufferRegs",local_18);
  DAT_0081cc14 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2InitColorBufferRegs",local_18);
  DAT_0081cc18 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CalcColorBufferAuxInfo",local_18);
  DAT_0081cc1c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CalcDepthBufferHiZInfo",local_18);
  DAT_0081cc20 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2InitDepthBufferHiZEnable",local_18);
  DAT_0081cc24 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetupContextStateEx",local_18);
  DAT_0081cc28 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetColorBuffer",local_18);
  DAT_0081cc2c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetDepthBuffer",local_18);
  DAT_0081cc30 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetAttribBuffer",local_18);
  DAT_0081cc34 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2InitTextureRegs",local_18);
  DAT_0081cc38 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2InitSampler",local_18);
  DAT_0081cc3c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CalcFetchShaderSizeEx",local_18);
  DAT_0081cc40 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2InitFetchShaderEx",local_18);
  DAT_0081cc44 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetFetchShader",local_18);
  DAT_0081cc48 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetVertexUniformReg",local_18);
  DAT_0081cc4c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetPixelUniformReg",local_18);
  DAT_0081cc50 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetPixelTexture",local_18);
  DAT_0081cc54 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetVertexTexture",local_18);
  DAT_0081cc58 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetPixelSampler",local_18);
  DAT_0081cc5c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetVertexSampler",local_18);
  DAT_0081cc60 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetPixelShader",local_18);
  DAT_0081cc64 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetVertexShader",local_18);
  DAT_0081cc68 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2InitSamplerZMFilter",local_18);
  DAT_0081cc6c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetColorControl",local_18);
  DAT_0081cc70 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetDepthOnlyControl",local_18);
  DAT_0081cc74 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetBlendControl",local_18);
  DAT_0081cc78 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CalcDRCSize",local_18);
  DAT_0081cc7c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetDRCBuffer",local_18);
  DAT_0081cc80 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetDRCScale",local_18);
  DAT_0081cc84 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetDRCEnable",local_18);
  DAT_0081cc88 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetPolygonControl",local_18);
  DAT_0081cc8c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetCullOnlyControl",local_18);
  DAT_0081cc90 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetDepthStencilControl",local_18);
  DAT_0081cc94 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetStencilMask",local_18);
  DAT_0081cc98 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetLineWidth",local_18);
  DAT_0081cc9c = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetDRCGamma",local_18);
  DAT_0081cca0 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetTVGamma",local_18);
  DAT_0081cca4 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2GetSystemTVScanMode",local_18);
  DAT_0081cca8 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2GetSystemDRCScanMode",local_18);
  DAT_0081ccac = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2RSetAllocator",local_18);
  DAT_0081ccb0 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CopySurface",local_18);
  DAT_0081ccb4 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2GetLastFrame",local_18);
  DAT_0081ccb8 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2ClearBuffersEx",local_18);
  DAT_0081ccbc = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2BeginDisplayListEx",local_18);
  DAT_0081ccc0 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2EndDisplayList",local_18);
  DAT_0081ccc4 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2CallDisplayList",local_18);
  DAT_0081ccc8 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2ExpandAAColorBuffer",local_18);
  DAT_0081cccc = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2ResolveAAColorBuffer",local_18);
  DAT_0081ccd0 = local_18[0];
  resolve_function(DAT_0081cbb8,(int)"GX2SetClearDepthStencil",local_18);
  DAT_0081ccd4 = local_18[0];
  return;
}



void FUN_00809034(void)

{
  if (DAT_0081c8e0 == 0) {
    FUN_00807194();
  }
                    // WARNING: Could not recover jumptable at 0x0080907c. Too many branches
                    // WARNING: Treating indirect jump as call
  (*DAT_0081c8dc)("nsysnet.rpl",&DAT_0081ccd8);
  return;
}



void FUN_008090a0(void)

{
  undefined4 local_28;
  code *local_24;
  code *local_20;
  code *local_1c;
  code *local_18;
  undefined4 local_14;
  code *local_10 [2];
  
  local_10[0] = (code *)0x0;
  FUN_00809034();
  (*DAT_0081c8dc)(&UNK_0081a767,&local_14);
  (*DAT_0081c8d8)(local_14,0,"ACInitialize",&local_18);
  (*DAT_0081c8d8)(local_14,0,"ACGetStartupId",&local_1c);
  (*DAT_0081c8d8)(local_14,0,"ACConnectWithConfigId",&local_20);
  (*DAT_0081c8d8)(local_14,0,"ACGetAssignedAddress",&local_24);
  resolve_function(DAT_0081ccd8,(int)"socket_lib_init",(int *)local_10);
  DAT_0081ccdc = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"socket_lib_finish",(int *)local_10);
  DAT_0081cce0 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"socketlasterr",(int *)local_10);
  DAT_0081cce4 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"socket",(int *)local_10);
  DAT_0081cce8 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"socketclose",(int *)local_10);
  DAT_0081ccec = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"shutdown",(int *)local_10);
  DAT_0081ccf0 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"connect",(int *)local_10);
  DAT_0081ccf4 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)&DAT_0081a80c,(int *)local_10);
  DAT_0081ccf8 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"listen",(int *)local_10);
  DAT_0081ccfc = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"accept",(int *)local_10);
  DAT_0081cd00 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)&DAT_0081a81f,(int *)local_10);
  DAT_0081cd04 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)&DAT_0081a824,(int *)local_10);
  DAT_0081cd08 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"recvfrom",(int *)local_10);
  DAT_0081cd0c = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"sendto",(int *)local_10);
  DAT_0081cd10 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"setsockopt",(int *)local_10);
  DAT_0081cd14 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"inet_ntoa",(int *)local_10);
  DAT_0081cd18 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"inet_aton",(int *)local_10);
  DAT_0081cd1c = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"inet_ntop",(int *)local_10);
  DAT_0081cd20 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"inet_pton",(int *)local_10);
  DAT_0081cd24 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLInit",(int *)local_10);
  DAT_0081cd28 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLFinish",(int *)local_10);
  DAT_0081cd2c = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLCreateContext",(int *)local_10);
  DAT_0081cd30 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLDestroyContext",(int *)local_10);
  DAT_0081cd34 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLAddServerPKI",(int *)local_10);
  DAT_0081cd38 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLAddServerPKIExternal",(int *)local_10);
  DAT_0081cd3c = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLWrite",(int *)local_10);
  DAT_0081cd40 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLRead",(int *)local_10);
  DAT_0081cd44 = local_10[0];
  resolve_function(DAT_0081ccd8,(int)"NSSLCreateConnection",(int *)local_10);
  DAT_0081cd48 = local_10[0];
  (*local_18)();
  (*local_1c)(&local_28);
  (*local_20)(local_28);
  (*local_24)(0x81e530);
  (*DAT_0081ccdc)();
  return;
}



undefined4 check_server(void)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  
  if (DAT_0081e550 != '\0') {
    (*DAT_0081ccec)(DAT_0081e3cc);
  }
  if (DAT_0081e548 == 0) {
    DAT_0081e548 = FUN_00806a7c(0x14,1);
  }
  (*DAT_0081c8dc)("nn_act.rpl",&DAT_0081e538);
  (*DAT_0081c8d8)(DAT_0081e538,0,"Initialize__Q2_2nn3actFv",&DAT_0081e540);
  (*DAT_0081c8d8)(DAT_0081e538,0,"GetAccountId__Q2_2nn3actFPc",&DAT_0081e53c);
  (*DAT_0081c8d8)(DAT_0081ccd8,0,"gethostbyname",&DAT_0081e534);
  (*DAT_0081e540)();
  (*DAT_0081e53c)(DAT_0081e548);
  uVar1 = (*DAT_0081c9ec)();
  (*DAT_0081ca00)(uVar1,&DAT_0081e54c);
  (*DAT_0081c9f0)(uVar1);
  (*DAT_0081ccdc)();
  DAT_0081e3cc = (*DAT_0081cce8)(2,1,6);
  puVar2 = (undefined4 *)FUN_00806a7c(0x10,1);
  (*DAT_0081cd1c)(&UNK_0081a945,puVar2 + 1);
  uVar1 = DAT_0081e3cc;
  *puVar2 = 0x2c38a;
  iVar3 = (*DAT_0081ccf4)(uVar1,puVar2,0x10);
  uVar1 = 0xfffffffa;
  if (-1 < iVar3) {
    DAT_0081e550 = '\x01';
    uVar1 = 0xfffffff3;
  }
  return uVar1;
}



void FUN_008096b8(void)

{
  (*DAT_0081ccec)(DAT_0081e3cc);
                    // WARNING: Could not recover jumptable at 0x00809704. Too many branches
                    // WARNING: Treating indirect jump as call
  (*DAT_0081ccf0)(DAT_0081e3cc,2);
  return;
}



// WARNING: Unknown calling convention yet parameter storage is locked

int check_license(void)

{
  undefined *puVar1;
  undefined in_r3;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  int iVar6;
  undefined4 in_r4;
  undefined4 uVar7;
  int iVar8;
  undefined auStack488 [416];
  undefined local_48 [20];
  undefined4 local_34 [4];
  
  puVar2 = (undefined4 *)FUN_00806a20(0x110);
  puVar3 = (undefined4 *)FUN_0080a0dc(0x40);
  puVar4 = (undefined4 *)FUN_0080a0dc(0x100);
  *puVar2 = 0x524d4236;
  puVar2[2] = 0x10001;
  local_34[0] = FUN_00809da4(&DAT_0081e54c,4,in_r4);
  FUN_0080b4fc(auStack488);
  FUN_0080b5a0(auStack488,local_34,4);
  iVar8 = 0;
  FUN_0080b30c(auStack488,local_48);
  iVar5 = FUN_00806a7c(0x30,1);
  do {
    puVar1 = local_48 + iVar8;
    iVar6 = iVar5 + iVar8;
    iVar8 = iVar8 + 2;
    FUN_0080c548(iVar6,&DAT_0081a951,*puVar1);
  } while (iVar8 != 0x14);
  FUN_0080a1d0(puVar4,in_r3);
  FUN_0080a2ac(puVar4,DAT_0081e54c);
  FUN_0080a354(puVar4,DAT_0081e548);
  FUN_0080a354(puVar4,iVar5);
  uVar7 = *puVar4;
  *(short *)(puVar2 + 1) = (short)puVar4[2] + 4;
  FUN_0080c25c(puVar2 + 3,uVar7);
  FUN_00809e1c(puVar2);
  uVar7 = FUN_00809f38(*puVar3);
  puVar3[1] = uVar7;
  iVar5 = FUN_0080a274(puVar3);
  uVar7 = FUN_0080a2e8(puVar3);
  if (DAT_0081e544 == 0) {
    DAT_0081e544 = FUN_00806a20(0x24);
  }
  FUN_0080c25c(DAT_0081e544,uVar7,0x24);
  FUN_0080a12c(puVar3);
  FUN_0080a12c(puVar4);
  return iVar5;
}



uint FUN_00809898(void)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  uint uVar4;
  undefined4 uVar5;
  
  puVar1 = (undefined4 *)FUN_00806a20(0x110);
  puVar2 = (undefined4 *)FUN_0080a0dc(0x40);
  puVar3 = (undefined4 *)FUN_0080a0dc(0x100);
  *puVar1 = 0x524d4236;
  puVar1[2] = 0x10002;
  FUN_0080a2ac(puVar3,0x13371337);
  uVar5 = *puVar3;
  *(short *)(puVar1 + 1) = (short)puVar3[2] + 4;
  FUN_0080c25c(puVar1 + 3,uVar5);
  FUN_00809e1c(puVar1);
  uVar5 = FUN_00809f38(*puVar2);
  puVar2[1] = uVar5;
  uVar4 = FUN_0080a274(puVar2);
  FUN_0080a12c(puVar2);
  FUN_0080a12c(puVar3);
  uVar4 = countLeadingZeros(uVar4 ^ 0x13371337);
  return uVar4 >> 5;
}



undefined4 FUN_00809954(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  
  puVar1 = (undefined4 *)FUN_00806a20(0x110);
  puVar2 = (undefined4 *)FUN_0080a0dc(0x40);
  puVar3 = (undefined4 *)FUN_0080a0dc(0x100);
  *puVar1 = 0x524d4236;
  puVar1[2] = 0x10005;
  FUN_0080a2ac(puVar3,param_1);
  FUN_0080a2ac(puVar3,param_2);
  uVar4 = *puVar3;
  *(short *)(puVar1 + 1) = (short)puVar3[2] + 4;
  FUN_0080c25c(puVar1 + 3,uVar4);
  FUN_00809e1c(puVar1);
  uVar4 = FUN_00809f38(*puVar2);
  puVar2[1] = uVar4;
  uVar4 = FUN_0080a274(puVar2);
  FUN_0080a12c(puVar2);
  FUN_0080a12c(puVar3);
  return uVar4;
}



undefined4 FUN_00809a10(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  
  puVar1 = (undefined4 *)FUN_00806a20(0x110);
  puVar2 = (undefined4 *)FUN_0080a0dc(0x40);
  puVar3 = (undefined4 *)FUN_0080a0dc(0x100);
  *puVar1 = 0x524d4236;
  puVar1[2] = 0x10006;
  FUN_0080a2ac(puVar3,param_1);
  FUN_0080a2ac(puVar3,param_2);
  uVar4 = *puVar3;
  *(short *)(puVar1 + 1) = (short)puVar3[2] + 4;
  FUN_0080c25c(puVar1 + 3,uVar4);
  FUN_00809e1c(puVar1);
  uVar4 = FUN_00809f38(*puVar2);
  puVar2[1] = uVar4;
  uVar4 = FUN_0080a274(puVar2);
  FUN_0080a12c(puVar2);
  FUN_0080a12c(puVar3);
  return uVar4;
}



undefined4 FUN_00809acc(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  
  puVar1 = (undefined4 *)FUN_00806a20(0x110);
  puVar2 = (undefined4 *)FUN_0080a0dc(0x40);
  puVar3 = (undefined4 *)FUN_0080a0dc(0x100);
  *puVar1 = 0x524d4236;
  puVar1[2] = 0x10007;
  FUN_0080a2ac(puVar3,param_1);
  FUN_0080a354(puVar3,param_2);
  uVar4 = *puVar3;
  *(short *)(puVar1 + 1) = (short)puVar3[2] + 4;
  FUN_0080c25c(puVar1 + 3,uVar4);
  FUN_00809e1c(puVar1);
  uVar4 = FUN_00809f38(*puVar2);
  puVar2[1] = uVar4;
  uVar4 = FUN_0080a274(puVar2);
  FUN_0080a12c(puVar2);
  FUN_0080a12c(puVar3);
  return uVar4;
}



undefined4 FUN_00809b88(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  
  puVar1 = (undefined4 *)FUN_00806a20(0x110);
  puVar2 = (undefined4 *)FUN_0080a0dc(0x40);
  puVar3 = (undefined4 *)FUN_0080a0dc(0x100);
  *puVar1 = 0x524d4236;
  puVar1[2] = 0x10009;
  FUN_0080a2ac(puVar3,param_1);
  FUN_0080a2ac(puVar3,param_2);
  FUN_0080a2ac(puVar3,param_3);
  uVar4 = *puVar3;
  *(short *)(puVar1 + 1) = (short)puVar3[2] + 4;
  FUN_0080c25c(puVar1 + 3,uVar4);
  FUN_00809e1c(puVar1);
  uVar4 = FUN_00809f38(*puVar2);
  puVar2[1] = uVar4;
  uVar4 = FUN_0080a274(puVar2);
  FUN_0080a12c(puVar2);
  FUN_0080a12c(puVar3);
  return uVar4;
}



undefined4
FUN_00809c54(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
            undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
            undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
            undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  byte in_cr1;
  undefined local_88;
  undefined local_87;
  undefined *local_84;
  undefined *local_80;
  undefined auStack120 [4];
  undefined4 local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined8 local_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  
  if ((bool)(in_cr1 >> 1 & 1)) {
    local_58 = param_1;
    local_50 = param_2;
    local_48 = param_3;
    local_40 = param_4;
    local_38 = param_5;
    local_30 = param_6;
    local_28 = param_7;
    local_20 = param_8;
  }
  local_74 = param_10;
  local_70 = param_11;
  local_6c = param_12;
  local_68 = param_13;
  local_64 = param_14;
  local_60 = param_15;
  local_5c = param_16;
  puVar1 = (undefined4 *)FUN_00806a20(0x810);
  puVar2 = (undefined4 *)FUN_0080a0dc(0x400);
  puVar3 = (undefined4 *)FUN_0080a0dc(0x800);
  *puVar1 = 0x524d4236;
  puVar1[2] = 0x1000d;
  uVar4 = FUN_00806a7c(0x400,1);
  FUN_0080a354(puVar3,DAT_0081e548);
  local_88 = 1;
  local_87 = 0;
  local_84 = &stack0x00000008;
  local_80 = auStack120;
  FUN_00810f3c(uVar4,param_9,&local_88);
  FUN_0080a354(puVar3,uVar4);
  uVar5 = *puVar3;
  *(short *)(puVar1 + 1) = (short)puVar3[2] + 4;
  FUN_0080c25c(puVar1 + 3,uVar5);
  FUN_00809e1c(puVar1);
  uVar5 = FUN_00809f38(*puVar2);
  puVar2[1] = uVar5;
  uVar5 = FUN_0080a274(puVar2);
  FUN_0080a12c(puVar2);
  FUN_0080a12c(puVar3);
  FUN_00806a60(puVar1);
  FUN_00806a60(uVar4);
  return uVar5;
}



void FUN_00809da4(undefined4 param_1,int param_2)

{
  param_2 = param_2 + 1;
  do {
    param_2 = param_2 + -1;
  } while (param_2 != 0);
  return;
}



short FUN_00809dec(int param_1,int param_2)

{
  byte *pbVar1;
  short sVar2;
  int iVar3;
  
  param_2 = param_2 + 1;
  iVar3 = 0;
  sVar2 = 0;
  while (param_2 = param_2 + -1, param_2 != 0) {
    pbVar1 = (byte *)(param_1 + iVar3);
    iVar3 = iVar3 + 1;
    sVar2 = sVar2 + (ushort)*pbVar1;
  }
  return sVar2 * 0x37ad;
}



void FUN_00809e1c(int param_1)

{
  ushort uVar1;
  undefined2 uVar3;
  int iVar2;
  int iVar4;
  undefined auStack304 [272];
  
  if (DAT_0081e550 == '\0') {
    return;
  }
  if (0 < DAT_0081e3cc) {
    if (DAT_0081e544 != 0) {
      FUN_0080ba98(auStack304,DAT_0081e544,0x20);
      FUN_0080bc1c(auStack304,param_1 + 0xc,*(undefined2 *)(param_1 + 4));
      FUN_0080be9c(auStack304);
    }
    iVar4 = param_1 + 8;
    uVar3 = FUN_00809dec(iVar4,*(undefined2 *)(param_1 + 4));
    *(undefined2 *)(param_1 + 6) = uVar3;
    FUN_0080ba98(auStack304,&DAT_0081cd4c,0x20);
    FUN_0080bc1c(auStack304,iVar4,*(ushort *)(param_1 + 4) + 4,iVar4);
    FUN_0080be9c(auStack304);
    uVar1 = *(ushort *)(param_1 + 4);
    iVar4 = 5;
    do {
      iVar2 = (*DAT_0081cd04)(DAT_0081e3cc,param_1,uVar1 + 0xc,0);
      if (-1 < iVar2) {
        return;
      }
      (*OSSleepTicks)(0,0xed1b76);
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  return;
}



int FUN_00809f38(undefined4 param_1)

{
  int iVar1;
  int iVar2;
  undefined auStack584 [264];
  undefined auStack320 [264];
  undefined4 local_38;
  undefined4 local_34;
  
  local_38 = 0x539;
  local_34 = 0;
  if (DAT_0081e550 == '\0') {
    iVar1 = 0;
  }
  else {
    iVar1 = 0;
    if (0 < DAT_0081e3cc) {
      iVar2 = 5;
      do {
        iVar1 = (*DAT_0081cd08)(DAT_0081e3cc,&local_38,8,0);
        if (-1 < iVar1) {
          iVar1 = (*DAT_0081cd08)(DAT_0081e3cc,param_1,local_38,0);
          break;
        }
        (*OSSleepTicks)(0,0xed1b76);
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
      FUN_0080ba98(auStack320,&DAT_0081cd4c,0x20);
      FUN_0080bc1c(auStack320,param_1,local_38,param_1);
      FUN_0080be9c(auStack320);
      if (DAT_0081e544 != 0) {
        FUN_0080ba98(auStack584,DAT_0081e544,0x20);
        FUN_0080bc1c(auStack584,param_1,local_38,param_1);
        FUN_0080be9c(auStack584);
      }
    }
  }
  return iVar1;
}



undefined4 * FUN_0080a080(undefined4 param_1,undefined4 param_2)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = (undefined4 *)FUN_00806a7c(0x10,1);
  uVar2 = FUN_00806a20(param_2);
  *puVar1 = uVar2;
  puVar1[3] = 0;
  puVar1[1] = param_2;
  puVar1[2] = 0;
  FUN_0080c25c(uVar2,param_1,param_2);
  return puVar1;
}



undefined4 * FUN_0080a0dc(undefined4 param_1)

{
  undefined4 *puVar1;
  undefined4 uVar2;
  
  puVar1 = (undefined4 *)FUN_00806a7c(0x10,1);
  uVar2 = FUN_00806a7c(param_1,1);
  *puVar1 = uVar2;
  puVar1[3] = 0;
  puVar1[1] = param_1;
  puVar1[2] = 0;
  return puVar1;
}



void FUN_0080a12c(int *param_1)

{
  if (param_1 != (int *)0x0) {
    if (*param_1 != 0) {
      FUN_00806a60();
    }
    FUN_00806a60(param_1);
    return;
  }
  return;
}



undefined4 FUN_0080a1d0(int *param_1,undefined param_2)

{
  if (param_1[2] < param_1[1]) {
    *(undefined *)(*param_1 + param_1[2]) = param_2;
    param_1[2] = param_1[2] + 1;
  }
  else {
    param_1[3] = -1;
  }
  return 0;
}



undefined2 FUN_0080a208(int *param_1)

{
  undefined2 uVar1;
  int iVar2;
  
  iVar2 = param_1[2] + 2;
  if (iVar2 <= param_1[1]) {
    uVar1 = *(undefined2 *)(*param_1 + param_1[2]);
    param_1[2] = iVar2;
    return uVar1;
  }
  param_1[3] = -1;
  return 0;
}



undefined4 FUN_0080a240(int *param_1,undefined2 param_2)

{
  int iVar1;
  
  iVar1 = param_1[2] + 2;
  if (param_1[1] < iVar1) {
    param_1[3] = -1;
  }
  else {
    *(undefined2 *)(*param_1 + param_1[2]) = param_2;
    param_1[2] = iVar1;
  }
  return 0;
}



undefined4 FUN_0080a274(int *param_1)

{
  undefined4 uVar1;
  int iVar2;
  
  iVar2 = param_1[2] + 4;
  if (iVar2 <= param_1[1]) {
    uVar1 = *(undefined4 *)(*param_1 + param_1[2]);
    param_1[2] = iVar2;
    return uVar1;
  }
  param_1[3] = -1;
  return 0;
}



undefined4 FUN_0080a2ac(int *param_1,undefined4 param_2)

{
  if (param_1[1] < param_1[2] + 4) {
    param_1[3] = -1;
  }
  else {
    *(undefined4 *)(*param_1 + param_1[2]) = param_2;
    param_1[2] = param_1[2] + 4;
  }
  return 0;
}



undefined4 FUN_0080a2e8(int *param_1)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  
  iVar1 = FUN_0080a208();
  uVar2 = FUN_00806a7c(iVar1,1);
  iVar3 = iVar1 + param_1[2];
  if (param_1[1] < iVar3) {
    uVar2 = 0;
    param_1[3] = -1;
  }
  else {
    FUN_0080c25c(uVar2,*param_1 + param_1[2],iVar1);
    param_1[2] = iVar3;
  }
  return uVar2;
}



undefined4 FUN_0080a354(int *param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  iVar1 = FUN_0080c610(param_2);
  uVar3 = iVar1 + 1;
  FUN_0080a240(param_1,uVar3 & 0xffff);
  if (param_1[1] < (int)(uVar3 + param_1[2])) {
    uVar2 = 0xffffffff;
    param_1[3] = -1;
  }
  else {
    FUN_0080c25c(*param_1 + param_1[2],param_2,uVar3);
    param_1[2] = param_1[2] + uVar3;
    uVar2 = 0;
  }
  return uVar2;
}



undefined4 FUN_0080a3d4(int param_1,undefined4 param_2)

{
  uint *puVar1;
  bool bVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint *puVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  int iVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint local_170;
  int local_16c;
  uint local_168;
  uint local_164;
  uint local_160;
  uint local_15c;
  uint local_158;
  uint local_154;
  int local_150;
  int local_14c;
  int local_148;
  int local_144;
  int local_140;
  uint local_13c;
  uint local_138;
  uint local_134;
  int local_130;
  int local_12c;
  int local_128;
  int local_124;
  int local_120;
  int local_11c;
  int local_118;
  int local_114;
  int local_110;
  int local_10c;
  int local_108;
  int local_104;
  int local_100;
  int local_fc;
  int local_f8;
  int local_f4;
  int local_f0;
  int local_ec;
  int local_e8;
  int local_e4;
  int local_e0;
  int local_dc;
  int local_d8;
  int local_d4;
  int local_d0;
  int local_cc;
  int local_c8;
  int local_c4;
  int local_c0;
  int local_bc;
  int local_b8;
  int local_b4;
  int local_b0;
  int local_ac;
  int local_a8;
  int local_a4;
  int local_a0;
  int local_9c;
  int local_98;
  int local_94;
  int local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  int local_7c;
  int local_78;
  uint local_74;
  int local_70;
  uint local_6c;
  int local_68;
  int local_64;
  int local_60;
  int local_5c;
  int local_58;
  uint local_54;
  int local_50;
  int local_4c;
  int local_48;
  int local_44;
  uint local_40;
  int local_3c;
  int local_38;
  
  iVar15 = 0xb;
  FUN_0080c25c(&local_170,param_2,0x40);
  uVar3 = *(uint *)(param_1 + 8);
  uVar5 = *(uint *)(param_1 + 0xc);
  uVar4 = *(uint *)(param_1 + 0x10);
  uVar6 = *(uint *)(param_1 + 0x14);
  iVar10 = *(int *)(param_1 + 0x18);
  uVar11 = local_13c;
  puVar8 = &local_170;
  uVar14 = local_164;
  uVar12 = local_168;
  uVar13 = local_160;
  uVar7 = local_138;
  uVar9 = local_134;
  uVar17 = local_15c;
  uVar18 = local_158;
  uVar19 = local_154;
  while( true ) {
    puVar1 = puVar8 + 8;
    uVar7 = uVar7 ^ uVar14;
    uVar14 = puVar8[9];
    uVar9 = uVar9 ^ uVar13;
    uVar13 = puVar8[10];
    uVar11 = uVar11 ^ uVar12 ^ *puVar1 ^ *puVar8;
    bVar2 = iVar15 == 1;
    uVar7 = uVar7 ^ uVar14 ^ puVar8[1];
    uVar11 = uVar11 << 1 | uVar11 >> 0x1f;
    uVar7 = uVar7 << 1 | uVar7 >> 0x1f;
    uVar12 = uVar11 ^ uVar17;
    uVar9 = uVar9 ^ uVar13 ^ puVar8[2];
    uVar16 = uVar7 ^ uVar18;
    uVar9 = uVar9 << 1 | uVar9 >> 0x1f;
    puVar8[0x10] = uVar11;
    puVar8[0x11] = uVar7;
    uVar11 = uVar9 ^ uVar19;
    puVar8[0x12] = uVar9;
    iVar15 = iVar15 + -1;
    if (bVar2) break;
    uVar18 = puVar8[0xc];
    uVar17 = puVar8[0xb];
    uVar19 = puVar8[0xd];
    uVar7 = uVar16 ^ uVar18 ^ puVar8[4];
    uVar9 = uVar11 ^ uVar19 ^ puVar8[5];
    uVar11 = uVar12 ^ uVar17 ^ puVar8[3];
    uVar9 = uVar9 << 1 | uVar9 >> 0x1f;
    uVar11 = uVar11 << 1 | uVar11 >> 0x1f;
    uVar7 = uVar7 << 1 | uVar7 >> 0x1f;
    puVar8[0x13] = uVar11;
    puVar8[0x14] = uVar7;
    puVar8[0x15] = uVar9;
    puVar8 = puVar8 + 6;
    uVar12 = *puVar1;
  }
  uVar9 = uVar5 << 0x1e | uVar5 >> 2;
  uVar11 = uVar3 << 0x1e | uVar3 >> 2;
  uVar12 = ((uVar4 ^ uVar6) & uVar5 ^ uVar6) +
           local_170 + 0x5a827999 + iVar10 + (uVar3 << 5 | uVar3 >> 0x1b);
  uVar7 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_16c + 0x5a827999 + uVar6 + ((uVar4 ^ uVar9) & uVar3 ^ uVar4);
  uVar14 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_168 + 0x5a827999 + uVar4 + ((uVar11 ^ uVar9) & uVar12 ^ uVar9);
  uVar12 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_164 + 0x5a827999 + uVar9 + ((uVar7 ^ uVar11) & uVar13 ^ uVar11);
  uVar9 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_160 + 0x5a827999 + uVar11 + ((uVar14 ^ uVar7) & uVar17 ^ uVar7);
  uVar11 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_15c + 0x5a827999 + uVar7 + ((uVar12 ^ uVar14) & uVar13 ^ uVar14);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_158 + 0x5a827999 + uVar14 + ((uVar9 ^ uVar12) & uVar17 ^ uVar12);
  uVar14 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_154 + 0x5a827999 + uVar12 + ((uVar11 ^ uVar9) & uVar13 ^ uVar9);
  uVar12 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_150 + 0x5a827999 + uVar9 + ((uVar7 ^ uVar11) & uVar17 ^ uVar11);
  uVar9 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_14c + 0x5a827999 + uVar11 + ((uVar14 ^ uVar7) & uVar13 ^ uVar7);
  uVar11 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar18 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_148 + 0x5a827999 + uVar7 + ((uVar12 ^ uVar14) & uVar17 ^ uVar14);
  uVar7 = uVar18 * 0x40000000 | uVar18 >> 2;
  uVar17 = (uVar18 * 0x20 | uVar18 >> 0x1b) +
           local_144 + 0x5a827999 + uVar14 + ((uVar9 ^ uVar12) & uVar13 ^ uVar12);
  uVar14 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_140 + 0x5a827999 + uVar12 + ((uVar11 ^ uVar9) & uVar18 ^ uVar9);
  uVar12 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_13c + 0x5a827999 + uVar9 + ((uVar7 ^ uVar11) & uVar17 ^ uVar11);
  uVar9 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_138 + 0x5a827999 + uVar11 + ((uVar14 ^ uVar7) & uVar13 ^ uVar7);
  uVar11 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           ((uVar12 ^ uVar14) & uVar17 ^ uVar14) + local_134 + 0x5a827999 + uVar7;
  uVar7 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           ((uVar9 ^ uVar12) & uVar13 ^ uVar12) + local_130 + 0x5a827999 + uVar14;
  uVar14 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           ((uVar9 ^ uVar11) & uVar17 ^ uVar9) + local_12c + 0x5a827999 + uVar12;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           ((uVar11 ^ uVar7) & uVar13 ^ uVar11) + local_128 + 0x5a827999 + uVar9;
  uVar9 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           ((uVar7 ^ uVar14) & uVar12 ^ uVar7) + local_124 + 0x5a827999 + uVar11;
  uVar11 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_120 + 0x6ed9eba1 + uVar7 + (uVar14 ^ uVar9 ^ uVar13);
  uVar7 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_11c + 0x6ed9eba1 + uVar14 + (uVar9 ^ uVar11 ^ uVar12);
  uVar14 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_118 + 0x6ed9eba1 + uVar9 + (uVar7 ^ uVar11 ^ uVar13);
  uVar9 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar12 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_114 + 0x6ed9eba1 + uVar11 + (uVar14 ^ uVar7 ^ uVar17);
  uVar11 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_110 + 0x6ed9eba1 + uVar7 + (uVar9 ^ uVar14 ^ uVar13);
  uVar14 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_10c + 0x6ed9eba1 + uVar14 + (uVar12 ^ uVar9 ^ uVar17);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar14 * 0x20 | uVar14 >> 0x1b) +
           local_108 + 0x6ed9eba1 + uVar9 + (uVar11 ^ uVar12 ^ uVar13);
  uVar9 = uVar14 * 0x40000000 | uVar14 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_104 + 0x6ed9eba1 + uVar12 + (uVar7 ^ uVar11 ^ uVar14);
  uVar14 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_100 + 0x6ed9eba1 + uVar11 + (uVar9 ^ uVar7 ^ uVar13);
  uVar11 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_fc + 0x6ed9eba1 + uVar7 + (uVar14 ^ uVar9 ^ uVar12);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar12 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_f8 + 0x6ed9eba1 + uVar9 + (uVar11 ^ uVar14 ^ uVar13);
  uVar9 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_f4 + 0x6ed9eba1 + uVar14 + (uVar7 ^ uVar11 ^ uVar17);
  uVar14 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_f0 + 0x6ed9eba1 + uVar11 + (uVar9 ^ uVar7 ^ uVar12);
  uVar11 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar12 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_ec + 0x6ed9eba1 + uVar7 + (uVar14 ^ uVar9 ^ uVar13);
  uVar7 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_e8 + 0x6ed9eba1 + uVar9 + (uVar11 ^ uVar14 ^ uVar17);
  uVar9 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_e4 + 0x6ed9eba1 + uVar14 + (uVar7 ^ uVar11 ^ uVar12);
  uVar14 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_e0 + 0x6ed9eba1 + uVar11 + (uVar9 ^ uVar7 ^ uVar13);
  uVar11 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_dc + 0x6ed9eba1 + uVar7 + (uVar9 ^ uVar14 ^ uVar12);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_d8 + 0x6ed9eba1 + uVar9 + (uVar14 ^ uVar11 ^ uVar13);
  uVar9 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_d4 + 0x6ed9eba1 + uVar14 + (uVar11 ^ uVar7 ^ uVar12);
  uVar14 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = ((uVar13 | uVar9) & uVar7 | uVar13 & uVar9) +
           local_d0 + -0x70e44324 + uVar11 + (uVar12 * 0x20 | uVar12 >> 0x1b);
  uVar11 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           ((uVar12 | uVar14) & uVar9 | uVar12 & uVar14) + local_cc + -0x70e44324 + uVar7;
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar12 = ((uVar11 | uVar13) & uVar14 | uVar11 & uVar13) +
           local_c8 + -0x70e44324 + uVar9 + (uVar17 * 0x20 | uVar17 >> 0x1b);
  uVar9 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           ((uVar7 | uVar17) & uVar11 | uVar7 & uVar17) + local_c4 + -0x70e44324 + uVar14;
  uVar14 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = ((uVar9 | uVar12) & uVar7 | uVar9 & uVar12) +
           local_c0 + -0x70e44324 + uVar11 + (uVar13 * 0x20 | uVar13 >> 0x1b);
  uVar11 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           ((uVar14 | uVar13) & uVar9 | uVar14 & uVar13) + local_bc + -0x70e44324 + uVar7;
  uVar7 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           ((uVar11 | uVar12) & uVar14 | uVar11 & uVar12) + local_b8 + -0x70e44324 + uVar9;
  uVar9 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar12 = ((uVar7 | uVar13) & uVar11 | uVar7 & uVar13) +
           local_b4 + -0x70e44324 + uVar14 + (uVar17 * 0x20 | uVar17 >> 0x1b);
  uVar14 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           ((uVar9 | uVar17) & uVar7 | uVar9 & uVar17) + local_b0 + -0x70e44324 + uVar11;
  uVar11 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = ((uVar14 | uVar12) & uVar9 | uVar14 & uVar12) +
           local_ac + -0x70e44324 + uVar7 + (uVar13 * 0x20 | uVar13 >> 0x1b);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           ((uVar11 | uVar13) & uVar14 | uVar11 & uVar13) + local_a8 + -0x70e44324 + uVar9;
  uVar9 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           ((uVar7 | uVar12) & uVar11 | uVar7 & uVar12) + local_a4 + -0x70e44324 + uVar14;
  uVar14 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar12 = ((uVar9 | uVar13) & uVar7 | uVar9 & uVar13) +
           local_a0 + -0x70e44324 + uVar11 + (uVar17 * 0x20 | uVar17 >> 0x1b);
  uVar11 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           ((uVar14 | uVar17) & uVar9 | uVar14 & uVar17) + local_9c + -0x70e44324 + uVar7;
  uVar7 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = ((uVar11 | uVar12) & uVar14 | uVar11 & uVar12) +
           local_98 + -0x70e44324 + uVar9 + (uVar13 * 0x20 | uVar13 >> 0x1b);
  uVar9 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           ((uVar7 | uVar13) & uVar11 | uVar7 & uVar13) + local_94 + -0x70e44324 + uVar14;
  uVar14 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           ((uVar9 | uVar12) & uVar7 | uVar9 & uVar12) + local_90 + -0x70e44324 + uVar11;
  uVar11 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar12 = ((uVar17 | uVar14) & uVar9 | uVar17 & uVar14) +
           local_8c + -0x70e44324 + uVar7 + (uVar13 * 0x20 | uVar13 >> 0x1b);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           ((uVar13 | uVar11) & uVar14 | uVar13 & uVar11) + local_88 + -0x70e44324 + uVar9;
  uVar9 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = ((uVar12 | uVar7) & uVar11 | uVar12 & uVar7) +
           local_84 + -0x70e44324 + uVar14 + (uVar13 * 0x20 | uVar13 >> 0x1b);
  uVar14 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_80 + -0x359d3e2a + uVar11 + (uVar7 ^ uVar9 ^ uVar13);
  uVar11 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_7c + -0x359d3e2a + uVar7 + (uVar9 ^ uVar14 ^ uVar12);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_78 + -0x359d3e2a + uVar9 + (uVar14 ^ uVar11 ^ uVar13);
  uVar9 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar12 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           (uVar11 ^ uVar7 ^ uVar17) + local_74 + 0xca62c1d6 + uVar14;
  uVar14 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar13 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           local_70 + -0x359d3e2a + uVar11 + (uVar9 ^ uVar7 ^ uVar13);
  uVar7 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
          (uVar12 ^ uVar9 ^ uVar17) + local_6c + 0xca62c1d6 + uVar7;
  uVar11 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar7 * 0x20 | uVar7 >> 0x1b) +
           local_68 + -0x359d3e2a + uVar9 + (uVar14 ^ uVar12 ^ uVar13);
  uVar9 = uVar7 * 0x40000000 | uVar7 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_64 + -0x359d3e2a + uVar12 + (uVar11 ^ uVar14 ^ uVar7);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_60 + -0x359d3e2a + uVar14 + (uVar9 ^ uVar11 ^ uVar13);
  uVar14 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_5c + -0x359d3e2a + uVar11 + (uVar7 ^ uVar9 ^ uVar12);
  uVar11 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_58 + -0x359d3e2a + uVar9 + (uVar14 ^ uVar7 ^ uVar13);
  uVar9 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_54 + 0xca62c1d6 + uVar7 + (uVar11 ^ uVar14 ^ uVar12);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_50 + -0x359d3e2a + uVar14 + (uVar9 ^ uVar11 ^ uVar13);
  uVar14 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar12 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_4c + -0x359d3e2a + uVar11 + (uVar7 ^ uVar9 ^ uVar12);
  uVar11 = uVar13 * 0x40000000 | uVar13 >> 2;
  uVar13 = (uVar12 * 0x20 | uVar12 >> 0x1b) +
           local_48 + -0x359d3e2a + uVar9 + (uVar14 ^ uVar7 ^ uVar13);
  uVar9 = uVar12 * 0x40000000 | uVar12 >> 2;
  uVar17 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
           local_44 + -0x359d3e2a + uVar7 + (uVar11 ^ uVar14 ^ uVar12);
  uVar7 = uVar13 * 0x40000000 | uVar13 >> 2;
  local_74 = local_40 ^ local_54 ^ local_6c ^ local_74;
  uVar12 = uVar17 * 0x40000000 | uVar17 >> 2;
  uVar14 = (uVar17 * 0x20 | uVar17 >> 0x1b) +
           (uVar9 ^ uVar11 ^ uVar13) + local_40 + 0xca62c1d6 + uVar14;
  uVar13 = (uVar14 * 0x20 | uVar14 >> 0x1b) +
           local_3c + -0x359d3e2a + uVar11 + (uVar9 ^ uVar7 ^ uVar17);
  uVar11 = uVar14 * 0x40000000 | uVar14 >> 2;
  uVar9 = (uVar13 * 0x20 | uVar13 >> 0x1b) +
          local_38 + -0x359d3e2a + uVar9 + (uVar7 ^ uVar12 ^ uVar14);
  *(uint *)(param_1 + 0x18) = iVar10 + uVar12;
  *(uint *)(param_1 + 0x14) = uVar6 + uVar11;
  *(uint *)(param_1 + 8) =
       (local_74 << 1 | local_74 >> 0x1f) + uVar3 + -0x359d3e2a + uVar7 + (uVar12 ^ uVar11 ^ uVar13)
       + (uVar9 * 0x20 | uVar9 >> 0x1b);
  *(uint *)(param_1 + 0xc) = uVar5 + uVar9;
  *(uint *)(param_1 + 0x10) = (uVar13 * 0x40000000 | uVar13 >> 2) + uVar4;
  return 0;
}



undefined4 FUN_0080b30c(int *param_1,int *param_2)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  
  if (param_1 == (int *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("md != NULL","src/hashes/sha1.c",0xbd);
  }
  if (param_2 == (int *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("out != NULL","src/hashes/sha1.c",0xbe);
  }
  uVar1 = param_1[7];
  if (0x3f < uVar1) {
    return 0x10;
  }
  uVar5 = uVar1 + 1;
  iVar4 = uVar1 * 8 + param_1[1];
  iVar3 = *param_1 + (uint)CARRY4(uVar1 * 8,param_1[1]);
  param_1[1] = iVar4;
  *param_1 = iVar3;
  param_1[7] = uVar5;
  *(undefined *)((int)param_1 + uVar1 + 0x20) = 0x80;
  if (uVar5 < 0x39) {
    if (uVar5 == 0x38) goto LAB_0080b3d8;
  }
  else {
    if (uVar5 != 0x40) {
      FUN_0080c398((int)param_1 + uVar1 + 0x21,0,0x3f - uVar1);
      param_1[7] = 0x40;
    }
    FUN_0080a3d4(param_1,param_1 + 8);
    uVar5 = 0;
    iVar3 = *param_1;
    iVar4 = param_1[1];
  }
  uVar1 = uVar5 + 1;
  iVar2 = 1;
  if (0x38 >= uVar1) {
    iVar2 = 0x38 - uVar5;
  }
  FUN_0080c398((int)param_1 + uVar5 + 0x20,0,iVar2);
  if (0x38 < uVar1) {
    iVar2 = 0;
  }
  else {
    iVar2 = 0x37 - uVar5;
  }
  param_1[7] = iVar2 + uVar1;
LAB_0080b3d8:
  param_1[0x16] = iVar3;
  param_1[0x17] = iVar4;
  FUN_0080a3d4(param_1,param_1 + 8);
  *param_2 = param_1[2];
  param_2[1] = param_1[3];
  param_2[2] = param_1[4];
  param_2[3] = param_1[5];
  param_2[4] = param_1[6];
  return 0;
}



undefined4 FUN_0080b4fc(undefined4 *param_1)

{
  if (param_1 != (undefined4 *)0x0) {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0x67452301;
    param_1[3] = 0xefcdab89;
    param_1[4] = 0x98badcfe;
    param_1[5] = 0x10325476;
    param_1[6] = 0xc3d2e1f0;
    param_1[7] = 0;
    return 0;
  }
                    // WARNING: Subroutine does not return
  FUN_0080ba5c("md != NULL","src/hashes/sha1.c",0x9f);
}



int FUN_0080b5a0(uint *param_1,int param_2,uint param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  
  if (param_1 == (uint *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("md != NULL","src/hashes/sha1.c",0xb1);
  }
  if (param_2 != 0) {
    uVar5 = param_1[7];
    iVar1 = 0x10;
    if (uVar5 < 0x41) {
      uVar4 = param_1[1];
      uVar3 = *param_1;
      uVar2 = uVar3 + CARRY4(param_3 * 8,uVar4);
      if ((uVar2 < uVar3) || ((uVar3 == uVar2 && (param_3 * 8 + uVar4 < uVar4)))) {
        return 0x19;
      }
      if (param_3 != 0) {
        while( true ) {
          if ((uVar5 == 0) && (0x3f < param_3)) {
            iVar1 = FUN_0080a3d4(param_1,param_2);
            if (iVar1 != 0) {
              return iVar1;
            }
            param_3 = param_3 - 0x40;
            uVar5 = param_1[1];
            param_2 = param_2 + 0x40;
            param_1[1] = uVar5 + 0x200;
            *param_1 = *param_1 + (uint)(0xfffffdff < uVar5);
          }
          else {
            uVar2 = 0x40 - uVar5;
            if (param_3 < 0x40 - uVar5) {
              uVar2 = param_3;
            }
            param_3 = param_3 - uVar2;
            FUN_0080c25c((int)(param_1 + 8) + uVar5,param_2,uVar2);
            uVar5 = param_1[7];
            param_2 = param_2 + uVar2;
            param_1[7] = uVar2 + uVar5;
            if (uVar2 + uVar5 == 0x40) {
              iVar1 = FUN_0080a3d4(param_1,param_1 + 8);
              if (iVar1 != 0) {
                return iVar1;
              }
              uVar5 = param_1[1];
              param_1[7] = 0;
              param_1[1] = uVar5 + 0x200;
              *param_1 = *param_1 + (uint)(0xfffffdff < uVar5);
            }
          }
          if (param_3 == 0) break;
          uVar5 = param_1[7];
        }
      }
      iVar1 = 0;
    }
    return iVar1;
  }
                    // WARNING: Subroutine does not return
  FUN_0080ba5c("in != NULL","src/hashes/sha1.c",0xb1);
}



int FUN_0080ba30(int *param_1,uint param_2,int *param_3,uint param_4)

{
  int *piVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  uint uVar5;
  uint uVar6;
  
  if (param_2 != param_4) {
    if (param_4 < param_2) {
      return -1;
    }
    return 1;
  }
  uVar6 = param_2;
  if (param_2 < 4) {
LAB_0080c214:
    piVar1 = param_1;
    piVar2 = param_3;
    param_2 = uVar6;
    if (uVar6 == 0) {
      return 0;
    }
  }
  else {
    piVar1 = param_1;
    piVar2 = param_3;
    if ((((uint)param_1 | (uint)param_3) & 3) == 0) {
      uVar6 = param_2 - 4;
      uVar5 = (uVar6 >> 2) + 1;
      if ((uVar5 & 1) != 0) {
        if (*param_1 != *param_3) goto joined_r0x0080c230;
        param_1 = param_1 + 1;
        param_3 = param_3 + 1;
        param_2 = uVar6;
        if (uVar6 < 4) goto LAB_0080c214;
      }
      uVar6 = param_2;
      uVar5 = uVar5 >> 1;
      do {
        piVar1 = param_1;
        piVar2 = param_3;
        param_2 = uVar6;
        if (*param_1 != *param_3) goto joined_r0x0080c230;
        param_2 = uVar6 - 4;
        piVar1 = param_1 + 1;
        piVar2 = param_3 + 1;
        if (param_1[1] != param_3[1]) goto joined_r0x0080c230;
        param_1 = param_1 + 2;
        param_3 = param_3 + 2;
        uVar6 = uVar6 - 8;
        uVar5 = uVar5 - 1;
      } while (uVar5 != 0);
      goto LAB_0080c214;
    }
  }
joined_r0x0080c230:
  piVar3 = (int *)((int)piVar1 + -1);
  piVar4 = (int *)((int)piVar2 + -1);
  if ((param_2 & 1) != 0) {
    uVar5 = (uint)*(byte *)piVar1;
    uVar6 = (uint)*(byte *)piVar2;
    if (uVar5 != uVar6) goto LAB_0080c1e8;
    piVar3 = piVar1;
    piVar4 = piVar2;
    if (param_2 == 1) {
      return 0;
    }
  }
  param_2 = param_2 >> 1;
  while( true ) {
    uVar5 = (uint)*(byte *)((int)piVar3 + 1);
    uVar6 = (uint)*(byte *)((int)piVar4 + 1);
    if (uVar5 != uVar6) break;
    uVar5 = (uint)*(byte *)((int)piVar3 + 2);
    uVar6 = (uint)*(byte *)((int)piVar4 + 2);
    if (uVar5 != uVar6) break;
    param_2 = param_2 - 1;
    piVar3 = (int *)((int)piVar3 + 2);
    piVar4 = (int *)((int)piVar4 + 2);
    if (param_2 == 0) {
      return 0;
    }
  }
LAB_0080c1e8:
  return uVar5 - uVar6;
}



undefined4 FUN_0080ba5c(uint param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined uVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  byte *pbVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  uint uVar12;
  int iVar13;
  int iVar14;
  uint uVar15;
  byte bVar16;
  undefined4 *puVar17;
  int iVar18;
  undefined8 uVar19;
  
  FUN_0080c094(*(undefined4 *)(PTR_DAT_0081e3d4 + 0xc),
               "LTC_ARGCHK \'%s\' failure on line %d of file %s\n",param_1,param_3,param_2);
  uVar19 = FUN_0080c044();
  puVar3 = (undefined4 *)((ulonglong)uVar19 >> 0x20);
  pbVar5 = (byte *)uVar19;
  if (puVar3 == (undefined4 *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("st != NULL","src/stream/rc4/rc4_stream.c",0x15);
  }
  if (pbVar5 == (byte *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("key != NULL","src/stream/rc4/rc4_stream.c",0x16);
  }
  puVar4 = puVar3 + 2;
  iVar11 = 0;
  iVar18 = 0x20;
  if (param_1 < 5) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("keylen >= 5","src/stream/rc4/rc4_stream.c",0x17);
  }
  do {
    iVar14 = iVar11 + 3;
    iVar1 = iVar11 + 2;
    iVar6 = iVar11 + 4;
    iVar7 = iVar11 + 5;
    iVar9 = iVar11 + 6;
    iVar13 = iVar11 + 7;
    *(char *)((int)puVar4 + iVar11) = (char)iVar11;
    *(char *)((int)puVar4 + iVar11 + 1) = (char)(iVar11 + 1);
    iVar11 = iVar11 + 8;
    *(char *)((int)puVar4 + iVar1) = (char)iVar1;
    *(char *)((int)puVar4 + iVar14) = (char)iVar14;
    *(char *)((int)puVar4 + iVar6) = (char)iVar6;
    *(char *)((int)puVar4 + iVar7) = (char)iVar7;
    *(char *)((int)puVar4 + iVar9) = (char)iVar9;
    *(char *)((int)puVar4 + iVar13) = (char)iVar13;
    iVar18 = iVar18 + -1;
  } while (iVar18 != 0);
  puVar17 = puVar3 + 2;
  bVar16 = *(byte *)puVar17;
  uVar15 = (uint)bVar16;
  iVar11 = 0x100;
  uVar12 = *pbVar5 + uVar15 & 0xff;
  uVar2 = *(undefined *)((int)puVar4 + uVar12);
  uVar8 = 1;
  if (param_1 == 1) goto LAB_0080bb78;
  while( true ) {
    iVar11 = iVar11 + -1;
    *(undefined *)puVar17 = uVar2;
    *(char *)((int)puVar4 + uVar12) = (char)uVar15;
    if (iVar11 == 0) break;
    while( true ) {
      uVar10 = uVar8 + 1;
      puVar17 = (undefined4 *)((int)puVar17 + 1);
      bVar16 = *(byte *)puVar17;
      uVar15 = (uint)bVar16;
      uVar12 = (uint)pbVar5[uVar8] + uVar15 + uVar12 & 0xff;
      uVar2 = *(undefined *)((int)puVar4 + uVar12);
      uVar8 = uVar10;
      if (uVar10 != param_1) break;
LAB_0080bb78:
      iVar11 = iVar11 + -1;
      *(undefined *)puVar17 = uVar2;
      *(byte *)((int)puVar4 + uVar12) = bVar16;
      if (iVar11 == 0) goto LAB_0080bb94;
      uVar8 = 0;
    }
  }
LAB_0080bb94:
  *puVar3 = 0;
  puVar3[1] = 0;
  return 0;
}



undefined4 FUN_0080ba98(undefined4 *param_1,byte *param_2,uint param_3)

{
  int iVar1;
  undefined uVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  int iVar12;
  uint uVar13;
  byte bVar14;
  undefined4 *puVar15;
  int iVar16;
  
  if (param_1 == (undefined4 *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("st != NULL","src/stream/rc4/rc4_stream.c",0x15);
  }
  if (param_2 == (byte *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("key != NULL","src/stream/rc4/rc4_stream.c",0x16);
  }
  puVar3 = param_1 + 2;
  iVar9 = 0;
  iVar16 = 0x20;
  if (param_3 < 5) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("keylen >= 5","src/stream/rc4/rc4_stream.c",0x17);
  }
  do {
    iVar12 = iVar9 + 3;
    iVar1 = iVar9 + 2;
    iVar4 = iVar9 + 4;
    iVar5 = iVar9 + 5;
    iVar7 = iVar9 + 6;
    iVar11 = iVar9 + 7;
    *(char *)((int)puVar3 + iVar9) = (char)iVar9;
    *(char *)((int)puVar3 + iVar9 + 1) = (char)(iVar9 + 1);
    iVar9 = iVar9 + 8;
    *(char *)((int)puVar3 + iVar1) = (char)iVar1;
    *(char *)((int)puVar3 + iVar12) = (char)iVar12;
    *(char *)((int)puVar3 + iVar4) = (char)iVar4;
    *(char *)((int)puVar3 + iVar5) = (char)iVar5;
    *(char *)((int)puVar3 + iVar7) = (char)iVar7;
    *(char *)((int)puVar3 + iVar11) = (char)iVar11;
    iVar16 = iVar16 + -1;
  } while (iVar16 != 0);
  puVar15 = param_1 + 2;
  bVar14 = *(byte *)puVar15;
  uVar13 = (uint)bVar14;
  iVar9 = 0x100;
  uVar10 = *param_2 + uVar13 & 0xff;
  uVar2 = *(undefined *)((int)puVar3 + uVar10);
  uVar6 = 1;
  if (param_3 == 1) goto LAB_0080bb78;
  while( true ) {
    iVar9 = iVar9 + -1;
    *(undefined *)puVar15 = uVar2;
    *(char *)((int)puVar3 + uVar10) = (char)uVar13;
    if (iVar9 == 0) break;
    while( true ) {
      uVar8 = uVar6 + 1;
      puVar15 = (undefined4 *)((int)puVar15 + 1);
      bVar14 = *(byte *)puVar15;
      uVar13 = (uint)bVar14;
      uVar10 = (uint)param_2[uVar6] + uVar13 + uVar10 & 0xff;
      uVar2 = *(undefined *)((int)puVar3 + uVar10);
      uVar6 = uVar8;
      if (uVar8 != param_3) break;
LAB_0080bb78:
      iVar9 = iVar9 + -1;
      *(undefined *)puVar15 = uVar2;
      *(byte *)((int)puVar3 + uVar10) = bVar14;
      if (iVar9 == 0) goto LAB_0080bb94;
      uVar6 = 0;
    }
  }
LAB_0080bb94:
  *param_1 = 0;
  param_1[1] = 0;
  return 0;
}



undefined4 FUN_0080bc1c(uint *param_1,byte *param_2,uint param_3,byte *param_4)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  byte *pbVar4;
  uint uVar5;
  uint *puVar6;
  byte *pbVar7;
  
  if (param_1 == (uint *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("st != NULL","src/stream/rc4/rc4_stream.c",0x37);
  }
  if (param_2 == (byte *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("in != NULL","src/stream/rc4/rc4_stream.c",0x38);
  }
  if (param_4 == (byte *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("out != NULL","src/stream/rc4/rc4_stream.c",0x39);
  }
  puVar6 = param_1 + 2;
  uVar3 = *param_1 & 0xff;
  uVar5 = param_1[1] & 0xff;
  if (param_3 == 0) {
    *param_1 = *param_1 & 0xff;
    param_1[1] = param_1[1] & 0xff;
    return 0;
  }
  pbVar4 = param_4 + -1;
  pbVar7 = param_2 + -1;
  if ((param_3 & 1) != 0) {
    uVar3 = uVar3 + 1 & 0xff;
    pbVar4 = (byte *)((int)puVar6 + uVar3);
    bVar1 = *pbVar4;
    uVar5 = bVar1 + uVar5 & 0xff;
    *pbVar4 = *(byte *)((int)puVar6 + uVar5);
    *(byte *)((int)puVar6 + uVar5) = bVar1;
    *param_4 = *(byte *)((int)puVar6 + ((uint)bVar1 + (uint)*pbVar4 & 0xff)) ^ *param_2;
    pbVar4 = param_4;
    pbVar7 = param_2;
    if (param_3 == 1) goto LAB_0080bd08;
  }
  param_3 = param_3 >> 1;
  do {
    uVar2 = uVar3 + 1 & 0xff;
    bVar1 = *(byte *)((int)puVar6 + uVar2);
    uVar3 = uVar2 + 1 & 0xff;
    uVar5 = bVar1 + uVar5 & 0xff;
    *(undefined *)((int)puVar6 + uVar2) = *(undefined *)((int)puVar6 + uVar5);
    *(byte *)((int)puVar6 + uVar5) = bVar1;
    pbVar4[1] = *(byte *)((int)puVar6 + ((uint)bVar1 + (uint)*(byte *)((int)puVar6 + uVar2) & 0xff))
                ^ pbVar7[1];
    bVar1 = *(byte *)((int)puVar6 + uVar3);
    uVar5 = bVar1 + uVar5 & 0xff;
    *(undefined *)((int)puVar6 + uVar3) = *(undefined *)((int)puVar6 + uVar5);
    *(byte *)((int)puVar6 + uVar5) = bVar1;
    pbVar4[2] = *(byte *)((int)puVar6 + ((uint)bVar1 + (uint)*(byte *)((int)puVar6 + uVar3) & 0xff))
                ^ pbVar7[2];
    param_3 = param_3 - 1;
    pbVar4 = pbVar4 + 2;
    pbVar7 = pbVar7 + 2;
  } while (param_3 != 0);
LAB_0080bd08:
  *param_1 = uVar3;
  param_1[1] = uVar5;
  return 0;
}



undefined4 FUN_0080be9c(int param_1)

{
  undefined4 in_LR;
  
  if (param_1 != 0) {
    FUN_0080bee4(param_1,0x108);
    return in_LR;
  }
                    // WARNING: Subroutine does not return
  FUN_0080ba5c("st != NULL","src/stream/rc4/rc4_stream.c",0x60);
}



void FUN_0080bee4(undefined *param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  
  if (param_1 == (undefined *)0x0) {
                    // WARNING: Subroutine does not return
    FUN_0080ba5c("out != NULL","src/misc/zeromem.c",0x12);
  }
  if (param_2 == 0) {
    return;
  }
  uVar2 = param_2 & 7;
  if (uVar2 != 0) {
    uVar1 = param_2;
    if (uVar2 != 1) {
      if (uVar2 != 2) {
        if (uVar2 != 3) {
          if (uVar2 != 4) {
            if (uVar2 != 5) {
              if (uVar2 != 6) {
                *param_1 = 0;
                param_1 = param_1 + 1;
                uVar1 = param_2 - 1;
              }
              *param_1 = 0;
              uVar1 = uVar1 - 1;
              param_1 = param_1 + 1;
            }
            *param_1 = 0;
            uVar1 = uVar1 - 1;
            param_1 = param_1 + 1;
          }
          *param_1 = 0;
          uVar1 = uVar1 - 1;
          param_1 = param_1 + 1;
        }
        *param_1 = 0;
        uVar1 = uVar1 - 1;
        param_1 = param_1 + 1;
      }
      *param_1 = 0;
      uVar1 = uVar1 - 1;
      param_1 = param_1 + 1;
    }
    *param_1 = 0;
    param_1 = param_1 + 1;
    if (uVar1 == 1) {
      return;
    }
  }
  param_2 = param_2 >> 3;
  do {
    *param_1 = 0;
    param_1[1] = 0;
    param_1[2] = 0;
    param_1[3] = 0;
    param_1[4] = 0;
    param_1[5] = 0;
    param_1[6] = 0;
    param_1[7] = 0;
    param_2 = param_2 - 1;
    param_1 = param_1 + 8;
  } while (param_2 != 0);
  return;
}



// WARNING: Removing unreachable block (ram,0x0080c07c)
// WARNING: Removing unreachable block (ram,0x0080c08c)

void FUN_0080c044(void)

{
  FUN_0080e980(2,"Abort called.\n",0xe);
  do {
                    // WARNING: Do nothing block with infinite loop
  } while( true );
}



void FUN_0080c094(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                 undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
                 undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16)

{
  byte in_cr1;
  undefined local_68;
  undefined local_67;
  undefined *local_64;
  undefined4 ***local_60 [2];
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  undefined8 local_10;
  undefined8 local_8;
  
  if ((bool)(in_cr1 >> 1 & 1)) {
    local_40 = param_1;
    local_38 = param_2;
    local_30 = param_3;
    local_28 = param_4;
    local_20 = param_5;
    local_18 = param_6;
    local_10 = param_7;
    local_8 = param_8;
  }
  local_64 = &stack0x00000008;
  local_60[0] = local_60;
  local_68 = 2;
  local_67 = 0;
  local_58 = param_11;
  local_54 = param_12;
  local_50 = param_13;
  local_4c = param_14;
  local_48 = param_15;
  local_44 = param_16;
  FUN_0080ea20(PTR_DAT_0081e3d4,param_9,param_10,&local_68);
  return;
}



void FUN_0080c25c(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  undefined uVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  int iVar7;
  uint uVar8;
  
  uVar8 = param_3;
  if (0xf < param_3) {
    if ((((uint)param_2 | (uint)param_1) & 3) != 0) {
      iVar7 = param_3 - 1;
      goto LAB_0080c328;
    }
    iVar3 = (param_3 - 0x10 >> 4) + 1;
    puVar4 = param_1;
    puVar5 = param_2;
    iVar7 = iVar3;
    do {
      uVar6 = puVar5[1];
      uVar2 = puVar5[2];
      *puVar4 = *puVar5;
      puVar4[1] = uVar6;
      puVar4[2] = uVar2;
      puVar4[3] = puVar5[3];
      iVar7 = iVar7 + -1;
      puVar4 = puVar4 + 4;
      puVar5 = puVar5 + 4;
    } while (iVar7 != 0);
    param_2 = param_2 + iVar3 * 4;
    uVar8 = param_3 & 0xf;
    param_1 = param_1 + iVar3 * 4;
    if ((param_3 & 0xc) != 0) {
      puVar4 = param_2 + -1;
      puVar5 = param_1 + -1;
      do {
        puVar4 = puVar4 + 1;
        puVar5 = puVar5 + 1;
        *puVar5 = *puVar4;
      } while (puVar4 != (undefined4 *)((int)param_2 + (uVar8 - 4 & 0xfffffffc)));
      iVar7 = (uVar8 - 4 >> 2) + 1;
      param_1 = param_1 + iVar7;
      param_2 = param_2 + iVar7;
      iVar7 = (param_3 & 3) - 1;
      if ((param_3 & 3) == 0) {
        return;
      }
      goto LAB_0080c328;
    }
  }
  iVar7 = uVar8 - 1;
  if (uVar8 == 0) {
    return;
  }
LAB_0080c328:
  puVar4 = (undefined4 *)((int)param_1 + -1);
  puVar5 = (undefined4 *)((int)param_2 + -1);
  if (((iVar7 + 1U & 1) != 0) &&
     (*(undefined *)param_1 = *(undefined *)param_2, puVar4 = param_1, puVar5 = param_2, iVar7 == 0)
     ) {
    return;
  }
  uVar8 = iVar7 + 1U >> 1;
  do {
    uVar1 = *(undefined *)((int)puVar5 + 1);
    *(undefined *)(undefined4 *)((int)puVar4 + 2) = *(undefined *)((int)puVar5 + 2);
    *(undefined *)((int)puVar4 + 1) = uVar1;
    puVar5 = (undefined4 *)((int)puVar5 + 2);
    uVar8 = uVar8 - 1;
    puVar4 = (undefined4 *)((int)puVar4 + 2);
  } while (uVar8 != 0);
  return;
}



void FUN_0080c398(undefined4 *param_1,undefined param_2,uint param_3)

{
  uint uVar1;
  undefined4 uVar2;
  undefined4 *puVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  uint uVar7;
  
  uVar7 = param_3;
  if (((uint)param_1 & 3) != 0) {
    uVar4 = param_3 - 1;
    if (param_3 == 0) {
      return;
    }
    puVar3 = param_1;
    if ((param_3 & 1) != 0) {
      puVar3 = (undefined4 *)((int)param_1 + 1);
      *(undefined *)param_1 = param_2;
      param_1 = puVar3;
      uVar7 = uVar4;
      if (((uint)puVar3 & 3) == 0) goto LAB_0080c3f4;
      uVar4 = param_3 - 2;
      if (uVar4 == 0xffffffff) {
        return;
      }
    }
    param_3 = param_3 >> 1;
    while( true ) {
      *(undefined *)puVar3 = param_2;
      param_1 = (undefined4 *)((int)puVar3 + 1U);
      uVar7 = uVar4;
      if (((uint)(undefined4 *)((int)puVar3 + 1U) & 3) == 0) break;
      param_1 = (undefined4 *)((int)puVar3 + 2);
      *(undefined *)((int)puVar3 + 1) = param_2;
      uVar7 = uVar4 - 1;
      if (((uint)param_1 & 3) == 0) break;
      uVar4 = uVar4 - 2;
      param_3 = param_3 - 1;
      puVar3 = param_1;
      if (param_3 == 0) {
        return;
      }
    }
  }
LAB_0080c3f4:
  if (uVar7 < 4) goto LAB_0080c4a8;
  uVar2 = CONCAT22(CONCAT11(param_2,param_2),CONCAT11(param_2,param_2));
  if (0xf < uVar7) {
    uVar4 = uVar7 - 0x10 >> 4;
    uVar1 = uVar4 + 1;
    puVar3 = param_1;
    if ((uVar1 & 1) == 0) {
LAB_0080c430:
      uVar4 = uVar1 >> 1;
      do {
        *puVar3 = uVar2;
        puVar3[1] = uVar2;
        puVar3[2] = uVar2;
        puVar3[3] = uVar2;
        puVar3[4] = uVar2;
        puVar3[5] = uVar2;
        puVar3[6] = uVar2;
        puVar3[7] = uVar2;
        uVar4 = uVar4 - 1;
        puVar3 = puVar3 + 8;
      } while (uVar4 != 0);
    }
    else {
      puVar3 = param_1 + 4;
      *param_1 = uVar2;
      param_1[1] = uVar2;
      param_1[2] = uVar2;
      param_1[3] = uVar2;
      if (uVar4 != 0) goto LAB_0080c430;
    }
    uVar4 = uVar7 & 0xc;
    uVar7 = uVar7 & 0xf;
    param_1 = param_1 + uVar1 * 4;
    if (uVar4 == 0) goto LAB_0080c4a8;
  }
  puVar3 = param_1 + -1;
  iVar5 = (uVar7 - 4 >> 2) + 1;
  iVar6 = iVar5;
  do {
    puVar3 = puVar3 + 1;
    *puVar3 = uVar2;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  uVar7 = uVar7 & 3;
  param_1 = param_1 + iVar5;
LAB_0080c4a8:
  if (uVar7 == 0) {
    return;
  }
  puVar3 = (undefined4 *)((int)param_1 - 1);
  if (((uVar7 & 1) != 0) && (*(undefined *)param_1 = param_2, puVar3 = param_1, uVar7 == 1)) {
    return;
  }
  uVar7 = uVar7 >> 1;
  do {
    *(undefined *)((int)puVar3 + 1) = param_2;
    puVar3 = (undefined4 *)((int)puVar3 + 2);
    *(undefined *)puVar3 = param_2;
    uVar7 = uVar7 - 1;
  } while (uVar7 != 0);
  return;
}



void FUN_0080c548(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                 undefined *param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
                 undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16)

{
  byte in_cr1;
  undefined *local_e0 [2];
  undefined4 local_d8;
  undefined4 local_d4;
  undefined *local_d0;
  undefined4 local_cc;
  undefined local_70;
  undefined local_6f;
  undefined *local_6c;
  undefined4 ***local_68 [2];
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined8 local_48;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  undefined8 local_10;
  
  if ((bool)(in_cr1 >> 1 & 1)) {
    local_48 = param_1;
    local_40 = param_2;
    local_38 = param_3;
    local_30 = param_4;
    local_28 = param_5;
    local_20 = param_6;
    local_18 = param_7;
    local_10 = param_8;
  }
  local_6c = &stack0x00000008;
  local_68[0] = local_68;
  local_cc = 0x7fffffff;
  local_d8 = 0x7fffffff;
  local_70 = 2;
  local_6f = 0;
  local_d4 = 0x208ffff;
  local_e0[0] = param_9;
  local_d0 = param_9;
  local_60 = param_11;
  local_5c = param_12;
  local_58 = param_13;
  local_54 = param_14;
  local_50 = param_15;
  local_4c = param_16;
  FUN_0080c690(PTR_DAT_0081e3d4,local_e0,param_10,&local_70);
  *local_e0[0] = 0;
  return;
}



uint * FUN_0080c610(uint *param_1)

{
  uint *puVar1;
  char cVar2;
  uint uVar3;
  uint *puVar4;
  
  uVar3 = (uint)param_1 & 3;
  puVar4 = param_1;
  while (uVar3 != 0) {
    if (*(char *)puVar4 == '\0') goto LAB_0080c638;
    puVar4 = (uint *)((int)puVar4 + 1);
    uVar3 = (uint)puVar4 & 3;
  }
  uVar3 = *puVar4;
  while ((uVar3 + 0xfefefeff & ~uVar3 & 0x80808080) == 0) {
    puVar4 = puVar4 + 1;
    uVar3 = *puVar4;
  }
  cVar2 = *(char *)puVar4;
  puVar1 = puVar4;
  while ((puVar4 = puVar1, cVar2 != '\0' &&
         (puVar4 = (uint *)((int)puVar1 + 1), *(char *)puVar4 != '\0'))) {
    puVar1 = (uint *)((int)puVar1 + 2);
    cVar2 = *(char *)puVar1;
  }
LAB_0080c638:
  return (uint *)((int)puVar4 - (int)param_1);
}



// WARNING: Removing unreachable block (ram,0x0080caa0)
// WARNING: Removing unreachable block (ram,0x0080d604)
// WARNING: Removing unreachable block (ram,0x0080d610)
// WARNING: Removing unreachable block (ram,0x0080d628)
// WARNING: Removing unreachable block (ram,0x0080d618)
// WARNING: Removing unreachable block (ram,0x0080d648)
// WARNING: Removing unreachable block (ram,0x0080d660)
// WARNING: Removing unreachable block (ram,0x0080d678)
// WARNING: Removing unreachable block (ram,0x0080d698)
// WARNING: Removing unreachable block (ram,0x0080d6b0)
// WARNING: Removing unreachable block (ram,0x0080c9d0)
// WARNING: Removing unreachable block (ram,0x0080d3a8)
// WARNING: Removing unreachable block (ram,0x0080d3b0)
// WARNING: Removing unreachable block (ram,0x0080d3bc)
// WARNING: Removing unreachable block (ram,0x0080d3d4)
// WARNING: Removing unreachable block (ram,0x0080d3c4)
// WARNING: Removing unreachable block (ram,0x0080d3f4)
// WARNING: Removing unreachable block (ram,0x0080d40c)
// WARNING: Removing unreachable block (ram,0x0080d424)
// WARNING: Removing unreachable block (ram,0x0080e1d4)
// WARNING: Removing unreachable block (ram,0x0080e1ec)
// WARNING: Removing unreachable block (ram,0x0080d444)
// WARNING: Removing unreachable block (ram,0x0080d50c)
// WARNING: Removing unreachable block (ram,0x0080d45c)
// WARNING: Removing unreachable block (ram,0x0080d474)
// WARNING: Removing unreachable block (ram,0x0080d464)
// WARNING: Removing unreachable block (ram,0x0080d494)
// WARNING: Removing unreachable block (ram,0x0080d4ac)
// WARNING: Removing unreachable block (ram,0x0080d51c)
// WARNING: Removing unreachable block (ram,0x0080d528)
// WARNING: Removing unreachable block (ram,0x0080d540)
// WARNING: Removing unreachable block (ram,0x0080d530)
// WARNING: Removing unreachable block (ram,0x0080d560)
// WARNING: Removing unreachable block (ram,0x0080d578)
// WARNING: Removing unreachable block (ram,0x0080d594)
// WARNING: Removing unreachable block (ram,0x0080e058)
// WARNING: Removing unreachable block (ram,0x0080e070)
// WARNING: Removing unreachable block (ram,0x0080d5b4)
// WARNING: Removing unreachable block (ram,0x0080cad4)
// WARNING: Removing unreachable block (ram,0x0080c9f4)
// WARNING: Removing unreachable block (ram,0x0080d5bc)
// WARNING: Removing unreachable block (ram,0x0080d5d4)
// WARNING: Removing unreachable block (ram,0x0080ca20)
// WARNING: Removing unreachable block (ram,0x0080ca28)
// WARNING: Removing unreachable block (ram,0x0080d5e0)
// WARNING: Removing unreachable block (ram,0x0080d5f8)
// WARNING: Removing unreachable block (ram,0x0080ca54)
// WARNING: Removing unreachable block (ram,0x0080d450)
// WARNING: Removing unreachable block (ram,0x0080d8c8)
// WARNING: Removing unreachable block (ram,0x0080d8e0)
// WARNING: Removing unreachable block (ram,0x0080d4e4)
// WARNING: Removing unreachable block (ram,0x0080d29c)
// WARNING: Removing unreachable block (ram,0x0080ccbc)
// WARNING: Removing unreachable block (ram,0x0080d828)
// WARNING: Removing unreachable block (ram,0x0080d84c)
// WARNING: Removing unreachable block (ram,0x0080ccd8)
// WARNING: Removing unreachable block (ram,0x0080d914)
// WARNING: Removing unreachable block (ram,0x0080d92c)
// WARNING: Removing unreachable block (ram,0x0080ccf4)
// WARNING: Removing unreachable block (ram,0x0080d8ec)
// WARNING: Removing unreachable block (ram,0x0080d904)
// WARNING: Removing unreachable block (ram,0x0080cd1c)
// WARNING: Removing unreachable block (ram,0x0080cd20)
// WARNING: Removing unreachable block (ram,0x0080cd48)
// WARNING: Removing unreachable block (ram,0x0080cd68)
// WARNING: Removing unreachable block (ram,0x0080d858)
// WARNING: Removing unreachable block (ram,0x0080d860)
// WARNING: Removing unreachable block (ram,0x0080e940)
// WARNING: Removing unreachable block (ram,0x0080d868)
// WARNING: Removing unreachable block (ram,0x0080d884)
// WARNING: Removing unreachable block (ram,0x0080d870)
// WARNING: Removing unreachable block (ram,0x0080d8a0)
// WARNING: Removing unreachable block (ram,0x0080d8b8)
// WARNING: Removing unreachable block (ram,0x0080d874)
// WARNING: Removing unreachable block (ram,0x0080e3c8)
// WARNING: Removing unreachable block (ram,0x0080e3d0)
// WARNING: Removing unreachable block (ram,0x0080df24)
// WARNING: Removing unreachable block (ram,0x0080df3c)
// WARNING: Removing unreachable block (ram,0x0080e3ec)
// WARNING: Removing unreachable block (ram,0x0080cd78)
// WARNING: Removing unreachable block (ram,0x0080d880)
// WARNING: Removing unreachable block (ram,0x0080d2a4)
// WARNING: Removing unreachable block (ram,0x0080d2b8)
// WARNING: Removing unreachable block (ram,0x0080e0b8)
// WARNING: Removing unreachable block (ram,0x0080e0d0)
// WARNING: Removing unreachable block (ram,0x0080d2e4)
// WARNING: Removing unreachable block (ram,0x0080d2e8)
// WARNING: Removing unreachable block (ram,0x0080d2f8)
// WARNING: Removing unreachable block (ram,0x0080d300)
// WARNING: Removing unreachable block (ram,0x0080e1f8)
// WARNING: Removing unreachable block (ram,0x0080e210)
// WARNING: Removing unreachable block (ram,0x0080d32c)
// WARNING: Removing unreachable block (ram,0x0080d330)
// WARNING: Removing unreachable block (ram,0x0080d33c)
// WARNING: Removing unreachable block (ram,0x0080d348)
// WARNING: Removing unreachable block (ram,0x0080d360)
// WARNING: Removing unreachable block (ram,0x0080d350)
// WARNING: Removing unreachable block (ram,0x0080d380)
// WARNING: Removing unreachable block (ram,0x0080d398)
// WARNING: Removing unreachable block (ram,0x0080d354)
// WARNING: Removing unreachable block (ram,0x0080d6b8)
// WARNING: Removing unreachable block (ram,0x0080d6c4)
// WARNING: Removing unreachable block (ram,0x0080dc50)
// WARNING: Removing unreachable block (ram,0x0080d6dc)
// WARNING: Removing unreachable block (ram,0x0080d700)
// WARNING: Removing unreachable block (ram,0x0080e6e4)
// WARNING: Removing unreachable block (ram,0x0080e6fc)
// WARNING: Removing unreachable block (ram,0x0080e708)
// WARNING: Removing unreachable block (ram,0x0080dc5c)
// WARNING: Removing unreachable block (ram,0x0080d6d4)
// WARNING: Removing unreachable block (ram,0x0080d704)
// WARNING: Removing unreachable block (ram,0x0080dcc4)
// WARNING: Removing unreachable block (ram,0x0080d708)
// WARNING: Removing unreachable block (ram,0x0080ddbc)
// WARNING: Removing unreachable block (ram,0x0080ddc8)
// WARNING: Removing unreachable block (ram,0x0080dde0)
// WARNING: Removing unreachable block (ram,0x0080ddd0)
// WARNING: Removing unreachable block (ram,0x0080de00)
// WARNING: Removing unreachable block (ram,0x0080de18)
// WARNING: Removing unreachable block (ram,0x0080ddd4)
// WARNING: Removing unreachable block (ram,0x0080e418)
// WARNING: Removing unreachable block (ram,0x0080e5e4)
// WARNING: Removing unreachable block (ram,0x0080e5fc)
// WARNING: Removing unreachable block (ram,0x0080e438)
// WARNING: Removing unreachable block (ram,0x0080d714)
// WARNING: Removing unreachable block (ram,0x0080d724)
// WARNING: Removing unreachable block (ram,0x0080e4ac)
// WARNING: Removing unreachable block (ram,0x0080e4c4)
// WARNING: Removing unreachable block (ram,0x0080d750)
// WARNING: Removing unreachable block (ram,0x0080d754)
// WARNING: Removing unreachable block (ram,0x0080d770)
// WARNING: Removing unreachable block (ram,0x0080d774)
// WARNING: Removing unreachable block (ram,0x0080d77c)
// WARNING: Removing unreachable block (ram,0x0080e70c)
// WARNING: Removing unreachable block (ram,0x0080e724)
// WARNING: Removing unreachable block (ram,0x0080d7a8)
// WARNING: Removing unreachable block (ram,0x0080dc90)
// WARNING: Removing unreachable block (ram,0x0080dcb8)
// WARNING: Removing unreachable block (ram,0x0080d7ac)
// WARNING: Removing unreachable block (ram,0x0080d7b0)
// WARNING: Removing unreachable block (ram,0x0080d7b4)
// WARNING: Removing unreachable block (ram,0x0080d7bc)
// WARNING: Removing unreachable block (ram,0x0080d7c8)
// WARNING: Removing unreachable block (ram,0x0080d7e0)
// WARNING: Removing unreachable block (ram,0x0080d7d0)
// WARNING: Removing unreachable block (ram,0x0080d800)
// WARNING: Removing unreachable block (ram,0x0080d818)
// WARNING: Removing unreachable block (ram,0x0080d7d4)
// WARNING: Removing unreachable block (ram,0x0080e094)
// WARNING: Removing unreachable block (ram,0x0080e0b4)
// WARNING: Removing unreachable block (ram,0x0080dfbc)
// WARNING: Removing unreachable block (ram,0x0080e4d4)
// WARNING: Removing unreachable block (ram,0x0080e4ec)
// WARNING: Removing unreachable block (ram,0x0080dfe8)
// WARNING: Removing unreachable block (ram,0x0080dfec)
// WARNING: Removing unreachable block (ram,0x0080e248)
// WARNING: Removing unreachable block (ram,0x0080e274)
// WARNING: Removing unreachable block (ram,0x0080dff0)
// WARNING: Removing unreachable block (ram,0x0080e000)
// WARNING: Removing unreachable block (ram,0x0080e64c)
// WARNING: Removing unreachable block (ram,0x0080e664)
// WARNING: Removing unreachable block (ram,0x0080e67c)
// WARNING: Removing unreachable block (ram,0x0080e27c)
// WARNING: Removing unreachable block (ram,0x0080e288)
// WARNING: Removing unreachable block (ram,0x0080e2a0)
// WARNING: Removing unreachable block (ram,0x0080e290)
// WARNING: Removing unreachable block (ram,0x0080e2c0)
// WARNING: Removing unreachable block (ram,0x0080e2d8)
// WARNING: Removing unreachable block (ram,0x0080e294)
// WARNING: Removing unreachable block (ram,0x0080e768)
// WARNING: Removing unreachable block (ram,0x0080e02c)
// WARNING: Removing unreachable block (ram,0x0080e788)
// WARNING: Removing unreachable block (ram,0x0080e7a0)
// WARNING: Removing unreachable block (ram,0x0080e030)
// WARNING: Removing unreachable block (ram,0x0080e054)

int FUN_0080c690(undefined4 *param_1,int *param_2,byte *param_3)

{
  byte bVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  byte **ppbVar7;
  code *pcVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte in_cr0;
  byte in_cr1;
  byte in_cr2;
  byte in_cr3;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  byte *local_130;
  byte *local_12c;
  byte *local_128 [14];
  byte local_f0 [40];
  byte **local_c8;
  int local_c4;
  byte *local_c0;
  undefined auStack180 [20];
  int local_a0 [2];
  undefined local_96;
  undefined *local_90;
  byte *local_8c;
  undefined4 local_88;
  undefined *local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined8 local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined *local_60;
  uint local_54;
  
  local_54 = (uint)(in_cr0 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) << 4
             | (uint)(in_cr7 & 0xf);
  puVar2 = (undefined4 *)FUN_00813138();
  local_6c = *puVar2;
  local_70 = FUN_0080c610(local_6c);
  FUN_0080c398(auStack180,0,8);
  if (((*(ushort *)(param_2 + 3) & 0x80) != 0) && (param_2[4] == 0)) {
    iVar5 = FUN_00806b30(param_1,0x40);
    *param_2 = iVar5;
    param_2[4] = iVar5;
    if (iVar5 == 0) {
      *param_1 = 0xc;
      return -1;
    }
    param_2[5] = 0x40;
  }
  local_c0 = (byte *)0x0;
  local_78 = 0;
  local_90 = &DAT_0081d1b8;
  local_84 = &DAT_0081af50;
  local_60 = &DAT_0081af4c;
  local_7c = 0;
  local_68 = 0;
  local_64 = 0;
  local_8c = (byte *)0x0;
  local_c8 = &local_130;
LAB_0080c7b4:
  local_c4 = 0;
  iVar5 = *(int *)(PTR_DAT_0081e3d4 + 0x34);
  pbVar10 = param_3;
  if (iVar5 == 0) goto LAB_0080c818;
LAB_0080c7c8:
  pcVar8 = *(code **)(iVar5 + 0xe4);
  uVar3 = FUN_00813164();
  iVar5 = (*pcVar8)(param_1,local_a0,pbVar10,uVar3,auStack180);
  do {
    if (iVar5 == 0) {
      pbVar9 = pbVar10 + -(int)param_3;
      if (pbVar9 != (byte *)0x0) {
LAB_0080c850:
        local_c4 = local_c4 + 1;
        local_c0 = pbVar9 + (int)local_c0;
        ppbVar7 = local_128;
        local_130 = param_3;
        local_12c = pbVar9;
        if ((7 < local_c4) &&
           (iVar4 = FUN_00814c58(param_1,param_2,&local_c8), ppbVar7 = &local_130, iVar4 != 0))
        goto LAB_0080c898;
        local_8c = pbVar9 + (int)local_8c;
        if (iVar5 != 0) goto LAB_0080c910;
      }
LAB_0080c88c:
      if (local_c0 != (byte *)0x0) {
        FUN_00814c58(param_1,param_2,&local_c8);
      }
LAB_0080c898:
      if ((*(ushort *)(param_2 + 3) & 0x40) == 0) {
        return (int)local_8c;
      }
      return -1;
    }
    if (iVar5 < 0) {
      FUN_0080c398(auStack180,0,8);
      iVar5 = 1;
    }
    else {
      if (local_a0[0] == 0x25) {
        pbVar9 = pbVar10 + -(int)param_3;
        ppbVar7 = &local_130;
        if (pbVar9 != (byte *)0x0) goto LAB_0080c850;
LAB_0080c910:
        bVar1 = pbVar10[1];
        local_96 = 0;
        param_3 = pbVar10 + 2;
        uVar6 = bVar1 - 0x20;
        if (uVar6 < 0x59) {
                    // WARNING: Could not recover jumptable at 0x0080c958. Too many branches
                    // WARNING: Treating indirect jump as call
          iVar5 = (*(code *)(local_84 + *(int *)(local_84 + uVar6 * 4)))();
          return iVar5;
        }
        if (bVar1 == 0) goto LAB_0080c88c;
        local_80 = 0;
        local_88 = 0;
        local_96 = 0;
        pbVar10 = local_c0 + 1;
        local_c4 = local_c4 + 1;
        local_f0[0] = bVar1;
        local_c0 = pbVar10;
        *ppbVar7 = local_f0;
        ppbVar7[1] = (byte *)0x1;
        if (((7 < local_c4) &&
            (iVar5 = FUN_00814c58(param_1,param_2,&local_c8), pbVar10 = local_c0, iVar5 != 0)) ||
           ((local_8c = local_8c + 1, pbVar10 != (byte *)0x0 &&
            (iVar5 = FUN_00814c58(param_1,param_2,&local_c8), iVar5 != 0)))) goto LAB_0080c898;
        goto LAB_0080c7b4;
      }
    }
    pbVar10 = pbVar10 + iVar5;
    iVar5 = *(int *)(PTR_DAT_0081e3d4 + 0x34);
    if (iVar5 != 0) goto LAB_0080c7c8;
LAB_0080c818:
    pcVar8 = *(code **)(local_90 + 0xe4);
    uVar3 = FUN_00813164();
    iVar5 = (*pcVar8)(param_1,local_a0,pbVar10,uVar3,auStack180);
  } while( true );
}



undefined4 FUN_0080e980(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined *puVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  code *UNRECOVERED_JUMPTABLE;
  
  puVar1 = PTR_DAT_0081e3d4;
  if (param_1 == -1) {
    return 0xffffffff;
  }
  piVar2 = (int *)FUN_00817fd0(param_1);
  if (piVar2 != (int *)0x0) {
    UNRECOVERED_JUMPTABLE = *(code **)((&PTR_PTR_s_stdnull_0081e348)[*piVar2] + 0x10);
    if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
      iVar4 = piVar2[2];
      *(undefined4 *)(puVar1 + 0x440) =
           *(undefined4 *)((&PTR_PTR_s_stdnull_0081e348)[*piVar2] + 0x58);
                    // WARNING: Could not recover jumptable at 0x00817e5c. Too many branches
                    // WARNING: Treating indirect jump as call
      uVar3 = (*UNRECOVERED_JUMPTABLE)(puVar1,iVar4,param_2,param_3);
      return uVar3;
    }
    *(undefined4 *)puVar1 = 0x58;
  }
  return 0xffffffff;
}



void FUN_0080e9a0(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  undefined *local_78 [2];
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  
  local_6c = 0x288ffff;
  local_78[0] = (undefined *)0x0;
  local_70 = 0;
  local_68 = 0;
  local_64 = 0;
  iVar1 = FUN_0080c690(PTR_DAT_0081e3d4,local_78,param_2,param_3);
  if (-1 < iVar1) {
    *local_78[0] = 0;
    *param_1 = local_68;
  }
  return;
}



// WARNING: Removing unreachable block (ram,0x0080ee60)
// WARNING: Removing unreachable block (ram,0x0080f6d4)
// WARNING: Removing unreachable block (ram,0x0080f6dc)
// WARNING: Removing unreachable block (ram,0x0080fab8)
// WARNING: Removing unreachable block (ram,0x0081044c)
// WARNING: Removing unreachable block (ram,0x00810a58)
// WARNING: Removing unreachable block (ram,0x00810a70)
// WARNING: Removing unreachable block (ram,0x00810478)
// WARNING: Removing unreachable block (ram,0x0081047c)
// WARNING: Removing unreachable block (ram,0x00810798)
// WARNING: Removing unreachable block (ram,0x008107c4)
// WARNING: Removing unreachable block (ram,0x00810480)
// WARNING: Removing unreachable block (ram,0x00810490)
// WARNING: Removing unreachable block (ram,0x0081097c)
// WARNING: Removing unreachable block (ram,0x00810994)
// WARNING: Removing unreachable block (ram,0x008107c8)
// WARNING: Removing unreachable block (ram,0x00810b9c)
// WARNING: Removing unreachable block (ram,0x00810ba8)
// WARNING: Removing unreachable block (ram,0x00810bc0)
// WARNING: Removing unreachable block (ram,0x00810bb0)
// WARNING: Removing unreachable block (ram,0x00810be0)
// WARNING: Removing unreachable block (ram,0x00810bf8)
// WARNING: Removing unreachable block (ram,0x00810bb4)
// WARNING: Removing unreachable block (ram,0x00810c18)
// WARNING: Removing unreachable block (ram,0x00810c38)
// WARNING: Removing unreachable block (ram,0x00810c50)
// WARNING: Removing unreachable block (ram,0x008104bc)
// WARNING: Removing unreachable block (ram,0x008107cc)
// WARNING: Removing unreachable block (ram,0x008107f0)
// WARNING: Removing unreachable block (ram,0x0080fac4)
// WARNING: Removing unreachable block (ram,0x008100ac)
// WARNING: Removing unreachable block (ram,0x008100b8)
// WARNING: Removing unreachable block (ram,0x0080fad4)
// WARNING: Removing unreachable block (ram,0x0080fadc)
// WARNING: Removing unreachable block (ram,0x0080fb00)
// WARNING: Removing unreachable block (ram,0x0080fb04)
// WARNING: Removing unreachable block (ram,0x00810ab4)
// WARNING: Removing unreachable block (ram,0x00810acc)
// WARNING: Removing unreachable block (ram,0x00810ad8)
// WARNING: Removing unreachable block (ram,0x00810158)
// WARNING: Removing unreachable block (ram,0x0080fb08)
// WARNING: Removing unreachable block (ram,0x00810214)
// WARNING: Removing unreachable block (ram,0x00810220)
// WARNING: Removing unreachable block (ram,0x00810238)
// WARNING: Removing unreachable block (ram,0x00810228)
// WARNING: Removing unreachable block (ram,0x00810258)
// WARNING: Removing unreachable block (ram,0x00810270)
// WARNING: Removing unreachable block (ram,0x0081022c)
// WARNING: Removing unreachable block (ram,0x00810810)
// WARNING: Removing unreachable block (ram,0x008109d8)
// WARNING: Removing unreachable block (ram,0x008109f0)
// WARNING: Removing unreachable block (ram,0x00810830)
// WARNING: Removing unreachable block (ram,0x0080fb14)
// WARNING: Removing unreachable block (ram,0x0080fb2c)
// WARNING: Removing unreachable block (ram,0x00810a84)
// WARNING: Removing unreachable block (ram,0x00810a9c)
// WARNING: Removing unreachable block (ram,0x0080fb58)
// WARNING: Removing unreachable block (ram,0x0080fb5c)
// WARNING: Removing unreachable block (ram,0x0080fb78)
// WARNING: Removing unreachable block (ram,0x0080fb7c)
// WARNING: Removing unreachable block (ram,0x0080fb84)
// WARNING: Removing unreachable block (ram,0x00810adc)
// WARNING: Removing unreachable block (ram,0x00810af4)
// WARNING: Removing unreachable block (ram,0x0080fbb0)
// WARNING: Removing unreachable block (ram,0x0080fb24)
// WARNING: Removing unreachable block (ram,0x008108b4)
// WARNING: Removing unreachable block (ram,0x008108d4)
// WARNING: Removing unreachable block (ram,0x0080fbb4)
// WARNING: Removing unreachable block (ram,0x0080fbb8)
// WARNING: Removing unreachable block (ram,0x0080fbbc)
// WARNING: Removing unreachable block (ram,0x0080fbc4)
// WARNING: Removing unreachable block (ram,0x0080fbd0)
// WARNING: Removing unreachable block (ram,0x0080fbe8)
// WARNING: Removing unreachable block (ram,0x0080fbd8)
// WARNING: Removing unreachable block (ram,0x0080fc08)
// WARNING: Removing unreachable block (ram,0x0080fc20)
// WARNING: Removing unreachable block (ram,0x0080fbdc)
// WARNING: Removing unreachable block (ram,0x0080f6f0)
// WARNING: Removing unreachable block (ram,0x00810604)
// WARNING: Removing unreachable block (ram,0x0081061c)
// WARNING: Removing unreachable block (ram,0x0080f71c)
// WARNING: Removing unreachable block (ram,0x0080f720)
// WARNING: Removing unreachable block (ram,0x0080fcf4)
// WARNING: Removing unreachable block (ram,0x0080f12c)
// WARNING: Removing unreachable block (ram,0x0080f148)
// WARNING: Removing unreachable block (ram,0x008103bc)
// WARNING: Removing unreachable block (ram,0x008103d4)
// WARNING: Removing unreachable block (ram,0x0080f164)
// WARNING: Removing unreachable block (ram,0x00810394)
// WARNING: Removing unreachable block (ram,0x008103ac)
// WARNING: Removing unreachable block (ram,0x0080f18c)
// WARNING: Removing unreachable block (ram,0x0080f190)
// WARNING: Removing unreachable block (ram,0x0080fc60)
// WARNING: Removing unreachable block (ram,0x0080fc68)
// WARNING: Removing unreachable block (ram,0x00810e10)
// WARNING: Removing unreachable block (ram,0x0080fc70)
// WARNING: Removing unreachable block (ram,0x0080fc8c)
// WARNING: Removing unreachable block (ram,0x0080fc78)
// WARNING: Removing unreachable block (ram,0x0080fca8)
// WARNING: Removing unreachable block (ram,0x0080fcc0)
// WARNING: Removing unreachable block (ram,0x0080fc7c)
// WARNING: Removing unreachable block (ram,0x008100e0)
// WARNING: Removing unreachable block (ram,0x008100e8)
// WARNING: Removing unreachable block (ram,0x00810884)
// WARNING: Removing unreachable block (ram,0x0080fc88)
// WARNING: Removing unreachable block (ram,0x0080f1b8)
// WARNING: Removing unreachable block (ram,0x0080f1d8)
// WARNING: Removing unreachable block (ram,0x0080fc30)
// WARNING: Removing unreachable block (ram,0x0080fc38)
// WARNING: Removing unreachable block (ram,0x00810104)
// WARNING: Removing unreachable block (ram,0x0081011c)
// WARNING: Removing unreachable block (ram,0x00810154)
// WARNING: Removing unreachable block (ram,0x0080fc54)
// WARNING: Removing unreachable block (ram,0x0080f1e8)
// WARNING: Removing unreachable block (ram,0x0080fcfc)
// WARNING: Removing unreachable block (ram,0x0080f730)
// WARNING: Removing unreachable block (ram,0x00810170)
// WARNING: Removing unreachable block (ram,0x00810188)
// WARNING: Removing unreachable block (ram,0x0080f75c)
// WARNING: Removing unreachable block (ram,0x0080f760)
// WARNING: Removing unreachable block (ram,0x0080f76c)
// WARNING: Removing unreachable block (ram,0x0080f778)
// WARNING: Removing unreachable block (ram,0x0080f790)
// WARNING: Removing unreachable block (ram,0x0080f780)
// WARNING: Removing unreachable block (ram,0x0080f7b0)
// WARNING: Removing unreachable block (ram,0x0080f7c8)
// WARNING: Removing unreachable block (ram,0x0080f784)
// WARNING: Removing unreachable block (ram,0x008105e0)
// WARNING: Removing unreachable block (ram,0x00810600)
// WARNING: Removing unreachable block (ram,0x0080ed5c)
// WARNING: Removing unreachable block (ram,0x0080f7d8)
// WARNING: Removing unreachable block (ram,0x0080f7e0)
// WARNING: Removing unreachable block (ram,0x0080f7ec)
// WARNING: Removing unreachable block (ram,0x0080f804)
// WARNING: Removing unreachable block (ram,0x0080f7f4)
// WARNING: Removing unreachable block (ram,0x0080f824)
// WARNING: Removing unreachable block (ram,0x0080f83c)
// WARNING: Removing unreachable block (ram,0x0080f854)
// WARNING: Removing unreachable block (ram,0x008100bc)
// WARNING: Removing unreachable block (ram,0x008100d4)
// WARNING: Removing unreachable block (ram,0x0080f874)
// WARNING: Removing unreachable block (ram,0x0080f880)
// WARNING: Removing unreachable block (ram,0x0080f88c)
// WARNING: Removing unreachable block (ram,0x0080f8a4)
// WARNING: Removing unreachable block (ram,0x0080f894)
// WARNING: Removing unreachable block (ram,0x0080f8c4)
// WARNING: Removing unreachable block (ram,0x0080f8dc)
// WARNING: Removing unreachable block (ram,0x0080f8f4)
// WARNING: Removing unreachable block (ram,0x0080fcd0)
// WARNING: Removing unreachable block (ram,0x0080fce8)
// WARNING: Removing unreachable block (ram,0x0080f914)
// WARNING: Removing unreachable block (ram,0x0080ee2c)
// WARNING: Removing unreachable block (ram,0x0080fa04)
// WARNING: Removing unreachable block (ram,0x0080fa10)
// WARNING: Removing unreachable block (ram,0x0080fa28)
// WARNING: Removing unreachable block (ram,0x0080fa18)
// WARNING: Removing unreachable block (ram,0x0080fa48)
// WARNING: Removing unreachable block (ram,0x0080fa60)
// WARNING: Removing unreachable block (ram,0x0080fa78)
// WARNING: Removing unreachable block (ram,0x0080fa98)
// WARNING: Removing unreachable block (ram,0x0080fab0)
// WARNING: Removing unreachable block (ram,0x0080f0a0)
// WARNING: Removing unreachable block (ram,0x0080f91c)
// WARNING: Removing unreachable block (ram,0x0080f928)
// WARNING: Removing unreachable block (ram,0x0080f940)
// WARNING: Removing unreachable block (ram,0x0080f930)
// WARNING: Removing unreachable block (ram,0x0080f960)
// WARNING: Removing unreachable block (ram,0x0080f978)
// WARNING: Removing unreachable block (ram,0x0080f994)
// WARNING: Removing unreachable block (ram,0x008104c4)
// WARNING: Removing unreachable block (ram,0x008104dc)
// WARNING: Removing unreachable block (ram,0x0080f9b4)
// WARNING: Removing unreachable block (ram,0x0080ed80)
// WARNING: Removing unreachable block (ram,0x0080edac)
// WARNING: Removing unreachable block (ram,0x0080f9bc)
// WARNING: Removing unreachable block (ram,0x0080f9d4)
// WARNING: Removing unreachable block (ram,0x0080edb4)
// WARNING: Removing unreachable block (ram,0x0080ede0)
// WARNING: Removing unreachable block (ram,0x0080f9e0)
// WARNING: Removing unreachable block (ram,0x0080f9f8)

int FUN_0080ea20(int param_1,int param_2,byte *param_3,undefined4 param_4)

{
  byte bVar1;
  undefined4 *puVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  ushort uVar7;
  byte **ppbVar8;
  code *pcVar9;
  byte *pbVar10;
  byte *pbVar11;
  byte in_cr0;
  byte in_cr1;
  byte in_cr2;
  byte in_cr3;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  byte *local_130;
  byte *local_12c;
  byte *local_128 [14];
  byte local_f0 [40];
  byte **local_c8;
  int local_c4;
  byte *local_c0;
  undefined auStack180 [20];
  int local_a0 [2];
  undefined local_96;
  undefined *local_90;
  byte *local_8c;
  undefined4 local_88;
  undefined *local_84;
  undefined4 local_80;
  undefined4 local_7c;
  undefined8 local_78;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined *local_60;
  uint local_54;
  
  local_54 = (uint)(in_cr0 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) << 4
             | (uint)(in_cr7 & 0xf);
  puVar2 = (undefined4 *)FUN_00813138();
  local_6c = *puVar2;
  local_70 = FUN_0080c610(local_6c);
  FUN_0080c398(auStack180,0,8);
  if ((param_1 != 0) && (*(int *)(param_1 + 0x38) == 0)) {
    FUN_00812e28(param_1);
  }
  uVar7 = *(ushort *)(param_2 + 0xc);
  if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((uVar7 & 0x200) == 0)) {
    FUN_00817e90(param_2);
    uVar7 = *(ushort *)(param_2 + 0xc);
  }
  if ((uVar7 & 0x2000) == 0) {
    *(ushort *)(param_2 + 0xc) = uVar7 | 0x2000;
    *(uint *)(param_2 + 0x68) = *(uint *)(param_2 + 0x68) & 0xffffdfff;
  }
  if (((uVar7 & 8) == 0) || (*(int *)(param_2 + 0x10) == 0)) {
    iVar5 = FUN_008110fc(param_1,param_2);
    if (iVar5 != 0) {
      if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((*(ushort *)(param_2 + 0xc) & 0x200) == 0)) {
        local_8c = (byte *)0xffffffff;
        FUN_00817ee4(param_2);
      }
      else {
LAB_00810b90:
        local_8c = (byte *)0xffffffff;
      }
      return (int)local_8c;
    }
    uVar7 = *(ushort *)(param_2 + 0xc);
  }
  if (((uVar7 & 0x1a) == 10) && (-1 < *(short *)(param_2 + 0xe))) {
    if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((uVar7 & 0x200) == 0)) {
      FUN_00817ee4(param_2);
    }
    iVar5 = FUN_00810e3c(param_1,param_2,param_3,param_4);
    return iVar5;
  }
  local_c0 = (byte *)0x0;
  local_78 = 0;
  local_90 = &DAT_0081d1b8;
  local_84 = &DAT_0081b0e0;
  local_60 = &DAT_0081af4c;
  local_7c = 0;
  local_68 = 0;
  local_64 = 0;
  local_8c = (byte *)0x0;
  local_c8 = &local_130;
LAB_0080eba0:
  local_c4 = 0;
  iVar5 = *(int *)(PTR_DAT_0081e3d4 + 0x34);
  pbVar11 = param_3;
  if (iVar5 == 0) goto LAB_0080ec04;
LAB_0080ebb4:
  pcVar9 = *(code **)(iVar5 + 0xe4);
  uVar3 = FUN_00813164();
  iVar5 = (*pcVar9)(param_1,local_a0,pbVar11,uVar3,auStack180);
  do {
    if (iVar5 == 0) {
      pbVar10 = pbVar11 + -(int)param_3;
      if (pbVar10 == (byte *)0x0) {
LAB_0080ec78:
        if ((local_c0 != (byte *)0x0) &&
           (iVar5 = FUN_00814fec(param_1,param_2,&local_c8), iVar5 != 0)) goto LAB_0080f0ac;
        uVar7 = *(ushort *)(param_2 + 0xc);
        local_c4 = 0;
      }
      else {
LAB_0080ec3c:
        local_c4 = local_c4 + 1;
        local_c0 = pbVar10 + (int)local_c0;
        ppbVar8 = local_128;
        local_130 = param_3;
        local_12c = pbVar10;
        if ((local_c4 < 8) ||
           (iVar4 = FUN_00814fec(param_1,param_2,&local_c8), ppbVar8 = &local_130, iVar4 == 0)) {
          local_8c = pbVar10 + (int)local_8c;
          if (iVar5 != 0) goto LAB_0080ec9c;
          goto LAB_0080ec78;
        }
LAB_0080f0ac:
        uVar7 = *(ushort *)(param_2 + 0xc);
      }
      if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((uVar7 & 0x200) == 0)) {
        FUN_00817ee4(param_2);
        uVar7 = *(ushort *)(param_2 + 0xc);
      }
      if ((uVar7 & 0x40) == 0) {
        return (int)local_8c;
      }
      goto LAB_00810b90;
    }
    if (iVar5 < 0) {
      FUN_0080c398(auStack180,0,8);
      iVar5 = 1;
    }
    else {
      if (local_a0[0] == 0x25) {
        pbVar10 = pbVar11 + -(int)param_3;
        ppbVar8 = &local_130;
        if (pbVar10 != (byte *)0x0) goto LAB_0080ec3c;
LAB_0080ec9c:
        bVar1 = pbVar11[1];
        local_96 = 0;
        param_3 = pbVar11 + 2;
        uVar6 = bVar1 - 0x20;
        if (uVar6 < 0x59) {
                    // WARNING: Could not recover jumptable at 0x0080ece4. Too many branches
                    // WARNING: Treating indirect jump as call
          iVar5 = (*(code *)(local_84 + *(int *)(local_84 + uVar6 * 4)))();
          return iVar5;
        }
        if (bVar1 == 0) goto LAB_0080ec78;
        local_80 = 0;
        local_88 = 0;
        local_96 = 0;
        pbVar11 = local_c0 + 1;
        local_c4 = local_c4 + 1;
        local_f0[0] = bVar1;
        local_c0 = pbVar11;
        *ppbVar8 = local_f0;
        ppbVar8[1] = (byte *)0x1;
        if (((7 < local_c4) &&
            (iVar5 = FUN_00814fec(param_1,param_2,&local_c8), pbVar11 = local_c0, iVar5 != 0)) ||
           ((local_8c = local_8c + 1, pbVar11 != (byte *)0x0 &&
            (iVar5 = FUN_00814fec(param_1,param_2,&local_c8), iVar5 != 0)))) goto LAB_0080f0ac;
        goto LAB_0080eba0;
      }
    }
    pbVar11 = pbVar11 + iVar5;
    iVar5 = *(int *)(PTR_DAT_0081e3d4 + 0x34);
    if (iVar5 != 0) goto LAB_0080ebb4;
LAB_0080ec04:
    pcVar9 = *(code **)(local_90 + 0xe4);
    uVar3 = FUN_00813164();
    iVar5 = (*pcVar9)(param_1,local_a0,pbVar11,uVar3,auStack180);
  } while( true );
}



int FUN_00810e3c(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  undefined auStack1152 [1024];
  undefined *local_80 [2];
  undefined4 local_78;
  ushort local_74;
  undefined2 local_72;
  undefined *local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_5c;
  undefined auStack36 [12];
  undefined4 local_18;
  
  local_80[0] = auStack1152;
  local_18 = *(undefined4 *)(param_2 + 0x68);
  local_72 = *(undefined2 *)(param_2 + 0xe);
  local_74 = *(ushort *)(param_2 + 0xc) & 0xfffd;
  local_64 = *(undefined4 *)(param_2 + 0x1c);
  local_5c = *(undefined4 *)(param_2 + 0x24);
  local_78 = 0x400;
  local_6c = 0x400;
  local_68 = 0;
  local_70 = local_80[0];
  FUN_00817ff4(auStack36,1);
  iVar1 = FUN_0080ea20(param_1,local_80,param_3,param_4);
  if (-1 < iVar1) {
    iVar2 = FUN_00812d14(param_1,local_80);
    if (iVar2 != 0) {
      iVar1 = -1;
    }
  }
  if ((local_74 & 0x40) != 0) {
    *(ushort *)(param_2 + 0xc) = *(ushort *)(param_2 + 0xc) | 0x40;
  }
  FUN_00818010(auStack36);
  return iVar1;
}



void FUN_00810f3c(undefined *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined *local_70 [2];
  undefined4 local_68;
  undefined4 local_64;
  undefined *local_60;
  undefined4 local_5c;
  
  local_5c = 0x7fffffff;
  local_68 = 0x7fffffff;
  local_64 = 0x208ffff;
  local_70[0] = param_1;
  local_60 = param_1;
  FUN_0080c690(PTR_DAT_0081e3d4,local_70,param_2,param_3);
  *local_70[0] = 0;
  return;
}



void FUN_00810fa4(undefined4 *param_1,int param_2,undefined4 param_3,undefined4 *param_4)

{
  int iVar1;
  undefined *puVar2;
  undefined auStack24 [16];
  
  if (param_4 == (undefined4 *)0x0) {
    param_4 = param_1 + 0x45;
  }
  puVar2 = *(undefined **)(PTR_DAT_0081e3d4 + 0x34);
  if (param_2 == 0) {
    if (puVar2 == (undefined *)0x0) {
      puVar2 = &DAT_0081d1b8;
    }
    iVar1 = (**(code **)(puVar2 + 0xe0))(param_1,auStack24,0,param_4);
  }
  else {
    if (puVar2 == (undefined *)0x0) {
      puVar2 = &DAT_0081d1b8;
    }
    iVar1 = (**(code **)(puVar2 + 0xe0))(param_1,param_2,param_3,param_4);
  }
  if (iVar1 != -1) {
    return;
  }
  *param_4 = 0;
  *param_1 = 0x8a;
  return;
}



int memcmp_maybe(int param_1,int param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  
  piVar3 = (int *)(param_1 + -4);
  piVar2 = (int *)(param_2 + -4);
  do {
    piVar3 = piVar3 + 1;
    iVar1 = *piVar3;
    piVar2 = piVar2 + 1;
    if (iVar1 != *piVar2) {
      return iVar1 - *piVar2;
    }
  } while (iVar1 != 0);
  return 0;
}



void draw_menu_instructions(int *param_1,int *param_2)

{
  int iVar1;
  
  iVar1 = *param_2;
  *param_1 = 0;
  while (iVar1 != 0) {
    *param_1 = iVar1;
    param_1 = param_1 + 1;
    param_2 = param_2 + 1;
    iVar1 = *param_2;
  }
  *param_1 = 0;
  return;
}



undefined4 FUN_008110fc(undefined4 *param_1,int *param_2)

{
  bool bVar1;
  undefined4 uVar2;
  int iVar3;
  ushort uVar4;
  ushort uVar5;
  
  if ((PTR_DAT_0081e3d4 == (undefined *)0x0) || (*(int *)(PTR_DAT_0081e3d4 + 0x38) != 0)) {
    uVar4 = *(ushort *)(param_2 + 3);
  }
  else {
    FUN_00812e28(PTR_DAT_0081e3d4);
    uVar4 = *(ushort *)(param_2 + 3);
  }
  if ((uVar4 & 8) == 0) {
    if ((uVar4 & 0x10) == 0) {
      *param_1 = 9;
      *(ushort *)(param_2 + 3) = uVar4 | 0x40;
      return 0xffffffff;
    }
    if ((uVar4 & 4) == 0) {
      iVar3 = param_2[4];
    }
    else {
      if ((int *)param_2[0xc] != (int *)0x0) {
        if ((int *)param_2[0xc] != param_2 + 0x10) {
          FUN_00806b50(param_1);
          uVar4 = *(ushort *)(param_2 + 3);
        }
        param_2[0xc] = 0;
      }
      iVar3 = param_2[4];
      param_2[1] = 0;
      *param_2 = iVar3;
      uVar4 = uVar4 & 0xffdb;
    }
    *(ushort *)(param_2 + 3) = uVar4 | 8;
    uVar5 = uVar4 | 8;
  }
  else {
    iVar3 = param_2[4];
    uVar5 = uVar4;
  }
  bVar1 = iVar3 == 0;
  if ((bVar1) && ((uVar4 & 0x280) != 0x200)) {
    FUN_00813190(param_1,param_2);
    uVar4 = *(ushort *)(param_2 + 3);
    bVar1 = param_2[4] == 0;
    uVar5 = uVar4;
  }
  if ((uVar4 & 1) == 0) {
    iVar3 = 0;
    if ((uVar4 & 2) == 0) {
      iVar3 = param_2[5];
    }
    param_2[2] = iVar3;
  }
  else {
    param_2[2] = 0;
    param_2[6] = -param_2[5];
  }
  if (bVar1) {
    uVar2 = 0;
    if ((uVar4 & 0x80) != 0) {
      uVar2 = 0xffffffff;
      *(ushort *)(param_2 + 3) = uVar5 | 0x40;
    }
  }
  else {
    uVar2 = 0;
  }
  return uVar2;
}



undefined4 FUN_008129d4(int *param_1,int *param_2)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  ushort uVar6;
  uint uVar5;
  code *pcVar7;
  int iVar8;
  longlong lVar9;
  
  uVar6 = *(ushort *)(param_2 + 3);
  if ((uVar6 & 8) != 0) {
    iVar1 = param_2[4];
    if (iVar1 == 0) {
      return 0;
    }
    iVar8 = *param_2;
    iVar3 = 0;
    *param_2 = iVar1;
    iVar8 = iVar8 - iVar1;
    if ((uVar6 & 3) == 0) {
      iVar3 = param_2[5];
    }
    param_2[2] = iVar3;
    while( true ) {
      if (iVar8 < 1) {
        return 0;
      }
      iVar3 = (*(code *)param_2[9])(param_1,param_2[7],iVar1,iVar8);
      iVar8 = iVar8 - iVar3;
      if (iVar3 < 1) break;
      iVar1 = iVar1 + iVar3;
    }
    *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) | 0x40;
    return 0xffffffff;
  }
  *(ushort *)(param_2 + 3) = uVar6 | 0x800;
  if ((param_2[1] < 1) && (param_2[0xf] < 1)) {
    return 0;
  }
  pcVar7 = (code *)param_2[10];
  if (pcVar7 == (code *)0x0) {
    return 0;
  }
  iVar8 = *param_1;
  iVar1 = param_2[7];
  *param_1 = 0;
  if ((uVar6 & 0x1000) == 0) {
    lVar9 = (*pcVar7)(param_1,iVar1,0,0,1);
    if ((lVar9 == -1) && (iVar1 = *param_1, iVar1 != 0)) {
      if ((iVar1 != 0x1d) && (iVar1 != 0x16)) {
        *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) | 0x40;
        return 0xffffffff;
      }
      *param_1 = iVar8;
      return 0;
    }
    uVar6 = *(ushort *)(param_2 + 3);
    pcVar7 = (code *)param_2[10];
    iVar1 = param_2[7];
  }
  else {
    lVar9 = *(longlong *)(param_2 + 0x14);
  }
  if ((uVar6 & 4) != 0) {
    uVar5 = param_2[1];
    uVar4 = (uint)lVar9 - uVar5;
    iVar3 = (int)((ulonglong)lVar9 >> 0x20) - ((uint)((uint)lVar9 < uVar5) + ((int)uVar5 >> 0x1f));
    lVar9 = CONCAT44(iVar3,uVar4);
    if (param_2[0xc] != 0) {
      uVar5 = param_2[0xf];
      lVar9 = CONCAT44(iVar3 - ((uint)(uVar4 < uVar5) + ((int)uVar5 >> 0x1f)),uVar4 - uVar5);
    }
  }
  lVar9 = (*pcVar7)(param_1,iVar1,(int)((ulonglong)lVar9 >> 0x20),(int)lVar9,0);
  if (lVar9 == -1) {
    iVar1 = *param_1;
    uVar6 = *(ushort *)(param_2 + 3);
    if (iVar1 != 0) {
      if ((iVar1 != 0x1d) && (iVar1 != 0x16)) {
        *(ushort *)(param_2 + 3) = uVar6 | 0x40;
        return 0xffffffff;
      }
      *(ushort *)(param_2 + 3) = uVar6 & 0xf7ff;
      *param_2 = param_2[4];
      param_2[1] = 0;
      goto LAB_00812b00;
    }
    *(ushort *)(param_2 + 3) = uVar6 & 0xf7ff;
    param_2[1] = 0;
    *param_2 = param_2[4];
  }
  else {
    uVar6 = *(ushort *)(param_2 + 3);
    param_2[1] = 0;
    *(ushort *)(param_2 + 3) = uVar6 & 0xf7ff;
    *param_2 = param_2[4];
  }
  if ((uVar6 & 0x1000) != 0) {
    param_2[0x14] = (int)((ulonglong)lVar9 >> 0x20);
    param_2[0x15] = (int)lVar9;
  }
LAB_00812b00:
  piVar2 = (int *)param_2[0xc];
  *param_1 = iVar8;
  if (piVar2 == (int *)0x0) {
    return 0;
  }
  if (piVar2 != param_2 + 0x10) {
    FUN_00806b50(param_1);
  }
  param_2[0xc] = 0;
  return 0;
}



undefined4 FUN_00812d14(int param_1,int param_2)

{
  uint uVar1;
  undefined4 uVar2;
  
  if ((param_1 != 0) && (*(int *)(param_1 + 0x38) == 0)) {
    FUN_00812e28();
  }
  uVar2 = 0;
  if (*(ushort *)(param_2 + 0xc) != 0) {
    if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((*(ushort *)(param_2 + 0xc) & 0x200) == 0)) {
      FUN_00817e90(param_2);
      uVar2 = FUN_008129d4(param_1,param_2);
      uVar1 = *(uint *)(param_2 + 0x68);
    }
    else {
      uVar2 = FUN_008129d4(param_1,param_2);
      uVar1 = *(uint *)(param_2 + 0x68);
    }
    if (((uVar1 & 1) == 0) && ((*(ushort *)(param_2 + 0xc) & 0x200) == 0)) {
      FUN_00817ee4(param_2);
      return uVar2;
    }
  }
  return uVar2;
}



// WARNING: Control flow encountered bad instruction data
// WARNING: Removing unreachable block (ram,0x00000000)

undefined4 FUN_00812e28(int param_1)

{
  undefined4 *puVar1;
  
  if (DAT_0081e404 == 0) {
    FUN_00817ff4(&DAT_0081e404,1);
  }
  FUN_00818048(&DAT_0081e404);
  if (*(int *)(param_1 + 0x38) == 0) {
    puVar1 = *(undefined4 **)(param_1 + 4);
    *(undefined4 *)(param_1 + 0x2e4) = 3;
    *(int *)(param_1 + 0x2e8) = param_1 + 0x2f0;
    *(undefined **)(param_1 + 0x3c) = &DAT_00812e1c;
    *(undefined4 *)(param_1 + 0x2e0) = 0;
    puVar1[3] = 0x40000;
    puVar1[0x1a] = 0;
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
    puVar1[4] = 0;
    puVar1[5] = 0;
    puVar1[6] = 0;
    FUN_0080c398(puVar1 + 0x18,0,8);
    puVar1[8] = 0x814ab4;
    puVar1[9] = &DAT_00814b28;
    puVar1[10] = FUN_00814be0;
    puVar1[0xb] = &LAB_00814c50;
    puVar1[7] = puVar1;
    FUN_00817ff4(puVar1 + 0x17,1);
    puVar1 = *(undefined4 **)(param_1 + 8);
    puVar1[0x1a] = 0;
    puVar1[3] = 0x90001;
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
    puVar1[4] = 0;
    puVar1[5] = 0;
    puVar1[6] = 0;
    FUN_0080c398(puVar1 + 0x18,0,8);
    puVar1[8] = 0x814ab4;
    puVar1[9] = &DAT_00814b28;
    puVar1[10] = FUN_00814be0;
    puVar1[0xb] = &LAB_00814c50;
    puVar1[7] = puVar1;
    FUN_00817ff4(puVar1 + 0x17,1);
    puVar1 = *(undefined4 **)(param_1 + 0xc);
    puVar1[0x1a] = 0;
    puVar1[3] = 0x120002;
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
    puVar1[4] = 0;
    puVar1[5] = 0;
    puVar1[6] = 0;
    FUN_0080c398(puVar1 + 0x18,0,8);
    puVar1[8] = 0x814ab4;
    puVar1[9] = &DAT_00814b28;
    puVar1[10] = FUN_00814be0;
    puVar1[0xb] = &LAB_00814c50;
    puVar1[7] = puVar1;
    FUN_00817ff4(puVar1 + 0x17,1);
    *(undefined4 *)(param_1 + 0x38) = 1;
  }
  return 0;
}



uint FUN_00813084(int param_1,code *param_2)

{
  uint uVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  
  piVar2 = (int *)(param_1 + 0x2e0);
  uVar3 = 0;
  do {
    iVar4 = piVar2[1];
    iVar5 = piVar2[2];
    if (0 < iVar4) {
      do {
        if (1 < *(ushort *)(iVar5 + 0xc)) {
          if (*(short *)(iVar5 + 0xe) != -1) {
            uVar1 = (*param_2)(param_1,iVar5);
            uVar3 = uVar3 | uVar1;
          }
        }
        iVar4 = iVar4 + -1;
        iVar5 = iVar5 + 0x70;
      } while (iVar4 != 0);
    }
    piVar2 = (int *)*piVar2;
  } while (piVar2 != (int *)0x0);
  return uVar3;
}



undefined * FUN_00813138(void)

{
  if (*(int *)(PTR_DAT_0081e3d4 + 0x34) != 0) {
    return (undefined *)(*(int *)(PTR_DAT_0081e3d4 + 0x34) + 0xf0);
  }
  return &DAT_0081d2a8;
}



undefined FUN_00813164(void)

{
  if (*(int *)(PTR_DAT_0081e3d4 + 0x34) != 0) {
    return *(undefined *)(*(int *)(PTR_DAT_0081e3d4 + 0x34) + 0x128);
  }
  return DAT_0081d2e0;
}



void FUN_00813190(int param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  ushort uVar3;
  ushort uVar4;
  uint uVar5;
  undefined auStack120 [8];
  uint local_70;
  
  uVar4 = *(ushort *)(param_2 + 3);
  if ((uVar4 & 2) != 0) {
    *param_2 = (int)param_2 + 0x43;
    param_2[4] = (int)param_2 + 0x43;
    param_2[5] = 1;
    return;
  }
  if (*(short *)((int)param_2 + 0xe) < 0) {
LAB_0081328c:
    uVar5 = 0;
    if ((uVar4 & 0x80) != 0) {
      iVar1 = 0x40;
      uVar4 = 0;
      iVar2 = FUN_00806b30(param_1,0x40);
      goto joined_r0x008132b0;
    }
    uVar4 = 0;
  }
  else {
    iVar1 = FUN_00817b70(param_1,(int)*(short *)((int)param_2 + 0xe),auStack120);
    if (iVar1 < 0) {
      uVar4 = *(ushort *)(param_2 + 3);
      goto LAB_0081328c;
    }
    uVar4 = 0x800;
    uVar5 = countLeadingZeros(local_70 & 0xf000 ^ 0x2000);
    uVar5 = uVar5 >> 5;
  }
  iVar1 = 0x400;
  iVar2 = FUN_00806b30(param_1,0x400);
joined_r0x008132b0:
  if (iVar2 == 0) {
    if ((*(ushort *)(param_2 + 3) & 0x200) == 0) {
      *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) & 0xfffc | 2;
      *param_2 = (int)param_2 + 0x43;
      param_2[4] = (int)param_2 + 0x43;
      param_2[5] = 1;
    }
  }
  else {
    uVar3 = *(ushort *)(param_2 + 3) | 0x80;
    *(undefined **)(param_1 + 0x3c) = &DAT_00812e1c;
    *param_2 = iVar2;
    param_2[4] = iVar2;
    param_2[5] = iVar1;
    *(ushort *)(param_2 + 3) = uVar3;
    if (uVar5 != 0) {
      iVar1 = FUN_00817c20(param_1,(int)*(short *)((int)param_2 + 0xe));
      if (iVar1 == 0) {
        uVar3 = *(ushort *)(param_2 + 3);
      }
      else {
        uVar3 = *(ushort *)(param_2 + 3) & 0xfffc | 1;
      }
    }
    *(ushort *)(param_2 + 3) = uVar3 | uVar4;
  }
  return;
}



int FUN_00813324(undefined4 *param_1,uint *param_2,byte *param_3,int param_4,int *param_5)

{
  bool bVar1;
  byte bVar2;
  byte *pbVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  uint uVar8;
  
  if (param_2 == (uint *)0x0) {
    if (param_3 == (byte *)0x0) {
      return 0;
    }
    if (param_4 == 0) {
      return -2;
    }
    iVar7 = *param_5;
    bVar1 = iVar7 == 0;
    if (bVar1) {
      bVar2 = *param_3;
    }
    else {
      bVar2 = *(byte *)(param_5 + 1);
    }
    uVar6 = (uint)bVar1;
    uVar4 = (uint)bVar2;
    if (uVar4 == 0) {
      *param_5 = 0;
      return 0;
    }
    if (uVar4 < 0x80) {
      *param_5 = 0;
      return 1;
    }
    if (uVar4 - 0xc0 < 0x20) {
      *(byte *)(param_5 + 1) = bVar2;
      if ((bVar1) && (*param_5 = 1, param_4 == 1)) {
        return -2;
      }
      if ((param_3[uVar6] - 0x80 < 0x40) && (0xc1 < uVar4)) {
        *param_5 = 0;
        return uVar6 + 1;
      }
      goto LAB_00813878;
    }
    if (0xf < uVar4 - 0xe0) {
      if (4 < uVar4 - 0xf0) goto LAB_00813878;
      *(byte *)(param_5 + 1) = bVar2;
      if (bVar1) {
        *param_5 = 1;
        if (param_4 == 1) {
          return -2;
        }
LAB_00813640:
        bVar2 = param_3[uVar6];
        iVar7 = 1;
        uVar6 = uVar6 + 1;
      }
      else {
        param_4 = param_4 + (uint)(param_4 != -1);
        if (iVar7 == 1) goto LAB_00813640;
        bVar2 = *(byte *)((int)param_5 + 5);
      }
      uVar5 = (uint)bVar2;
      if (uVar4 == 0xf0) {
        if (uVar5 < 0x90) goto LAB_00813878;
      }
      else {
        if ((uVar4 == 0xf4) && (0x8f < uVar5)) goto LAB_00813878;
      }
      if (0x3f < uVar5 - 0x80) goto LAB_00813878;
      *(byte *)((int)param_5 + 5) = bVar2;
      if (iVar7 == 1) {
        *param_5 = 2;
        if (param_4 == 2) {
          return -2;
        }
LAB_008138bc:
        pbVar3 = param_3 + uVar6;
        uVar6 = uVar6 + 1;
        if (0x3f < *pbVar3 - 0x80) goto LAB_00813878;
        *(byte *)((int)param_5 + 6) = *pbVar3;
        *param_5 = 3;
        if (param_4 == 3) {
          return -2;
        }
      }
      else {
        param_4 = param_4 + (uint)(param_4 != -1);
        if (*param_5 == 2) goto LAB_008138bc;
        if (0x3f < *(byte *)((int)param_5 + 6) - 0x80) goto LAB_00813878;
      }
      if (param_3[uVar6] - 0x80 < 0x40) {
        *param_5 = 0;
        return uVar6 + 1;
      }
      goto LAB_00813878;
    }
    *(byte *)(param_5 + 1) = bVar2;
    if (bVar1) {
      *param_5 = 1;
      if (param_4 == 1) {
        return -2;
      }
LAB_00813740:
      bVar2 = param_3[uVar6];
      iVar7 = 1;
      uVar6 = uVar6 + 1;
    }
    else {
      param_4 = param_4 + (uint)(param_4 != -1);
      if (iVar7 == 1) goto LAB_00813740;
      bVar2 = *(byte *)((int)param_5 + 5);
    }
    if (((uVar4 != 0xe0) || (0x9f < bVar2)) && (bVar2 - 0x80 < 0x40)) {
      *(byte *)((int)param_5 + 5) = bVar2;
      if ((iVar7 == 1) && (*param_5 = 2, param_4 == 2)) {
        return -2;
      }
      if (param_3[uVar6] - 0x80 < 0x40) {
        *param_5 = 0;
        return uVar6 + 1;
      }
    }
LAB_00813878:
    *param_1 = 0x8a;
    return -1;
  }
  if (param_3 == (byte *)0x0) {
    return 0;
  }
  if (param_4 == 0) {
    return -2;
  }
  iVar7 = *param_5;
  bVar1 = iVar7 == 0;
  if (bVar1) {
    bVar2 = *param_3;
  }
  else {
    bVar2 = *(byte *)(param_5 + 1);
  }
  uVar4 = (uint)bVar2;
  uVar6 = (uint)bVar1;
  if (uVar4 == 0) {
    *param_2 = 0;
    *param_5 = 0;
    return 0;
  }
  if (uVar4 < 0x80) {
    *param_5 = 0;
    *param_2 = uVar4;
    return 1;
  }
  if (uVar4 - 0xc0 < 0x20) {
    *(byte *)(param_5 + 1) = bVar2;
    if ((bVar1) && (*param_5 = 1, param_4 == 1)) {
      return -2;
    }
    bVar2 = param_3[uVar6];
    if ((bVar2 - 0x80 < 0x40) && (0xc1 < uVar4)) {
      *param_5 = 0;
      *param_2 = (uVar4 & 0x1f) << 6 | bVar2 & 0x3f;
      return uVar6 + 1;
    }
    goto LAB_008137c8;
  }
  if (0xf < uVar4 - 0xe0) {
    if (4 < uVar4 - 0xf0) goto LAB_008137c8;
    *(byte *)(param_5 + 1) = bVar2;
    if (bVar1) {
      *param_5 = 1;
      if (param_4 == 1) {
        return -2;
      }
LAB_00813718:
      bVar2 = param_3[uVar6];
      iVar7 = 1;
      uVar6 = uVar6 + 1;
    }
    else {
      param_4 = param_4 + (uint)(param_4 != -1);
      if (iVar7 == 1) goto LAB_00813718;
      bVar2 = *(byte *)((int)param_5 + 5);
    }
    uVar5 = (uint)bVar2;
    if (uVar4 == 0xf0) {
      if (uVar5 < 0x90) goto LAB_008137c8;
    }
    else {
      if ((uVar4 == 0xf4) && (0x8f < uVar5)) goto LAB_008137c8;
    }
    if (0x3f < uVar5 - 0x80) goto LAB_008137c8;
    *(byte *)((int)param_5 + 5) = bVar2;
    if (iVar7 == 1) {
      *param_5 = 2;
      if (param_4 == 2) {
        return -2;
      }
LAB_0081380c:
      pbVar3 = param_3 + uVar6;
      uVar8 = (uint)*pbVar3;
      uVar6 = uVar6 + 1;
      if (0x3f < uVar8 - 0x80) goto LAB_008137c8;
      *(byte *)((int)param_5 + 6) = *pbVar3;
      *param_5 = 3;
      if (param_4 == 3) {
        return -2;
      }
    }
    else {
      param_4 = param_4 + (uint)(param_4 != -1);
      if (*param_5 == 2) goto LAB_0081380c;
      uVar8 = (uint)*(byte *)((int)param_5 + 6);
      if (0x3f < uVar8 - 0x80) goto LAB_008137c8;
    }
    if (param_3[uVar6] - 0x80 < 0x40) {
      *param_2 = (uVar4 & 7) << 0x12 | (uVar5 & 0x3f) << 0xc | (uVar8 & 0x3f) << 6 |
                 param_3[uVar6] & 0x3f;
      *param_5 = 0;
      return uVar6 + 1;
    }
    goto LAB_008137c8;
  }
  *(byte *)(param_5 + 1) = bVar2;
  if (bVar1) {
    *param_5 = 1;
    if (param_4 == 1) {
      return -2;
    }
LAB_008136f8:
    bVar2 = param_3[uVar6];
    iVar7 = 1;
    uVar6 = uVar6 + 1;
  }
  else {
    param_4 = param_4 + (uint)(param_4 != -1);
    if (iVar7 == 1) goto LAB_008136f8;
    bVar2 = *(byte *)((int)param_5 + 5);
  }
  uVar5 = (uint)bVar2;
  if (((uVar4 != 0xe0) || (0x9f < uVar5)) && (uVar5 - 0x80 < 0x40)) {
    *(byte *)((int)param_5 + 5) = bVar2;
    if ((iVar7 == 1) && (*param_5 = 2, param_4 == 2)) {
      return -2;
    }
    bVar2 = param_3[uVar6];
    if (bVar2 - 0x80 < 0x40) {
      *param_5 = 0;
      *param_2 = (uVar4 & 0xf) << 0xc | (uVar5 & 0x3f) << 6 | bVar2 & 0x3f;
      return uVar6 + 1;
    }
  }
LAB_008137c8:
  *param_1 = 0x8a;
  return -1;
}



uint * FUN_008138ec(uint *param_1,char param_2,uint param_3)

{
  int iVar1;
  uint *puVar2;
  uint uVar3;
  uint uVar4;
  uint *puVar5;
  
  uVar4 = param_3;
  if (((uint)param_1 & 3) == 0) {
LAB_00813954:
    if (3 < uVar4) {
      iVar1 = (uVar4 - 4 >> 2) + 1;
      do {
        uVar3 = CONCAT22(CONCAT11(param_2,param_2),CONCAT11(param_2,param_2)) ^ *param_1;
        if ((uVar3 + 0xfefefeff & ~uVar3 & 0x80808080) != 0) goto LAB_008139b8;
        uVar4 = uVar4 - 4;
        param_1 = param_1 + 1;
        iVar1 = iVar1 + -1;
      } while (iVar1 != 0);
    }
    if (uVar4 != 0) {
LAB_008139b8:
      puVar5 = (uint *)((int)param_1 + uVar4);
      puVar2 = (uint *)((int)puVar5 - (int)param_1);
      if (((uint)puVar2 & 1) != 0) {
        if (*(char *)param_1 == param_2) {
          return param_1;
        }
        param_1 = (uint *)((int)param_1 + 1);
        if (puVar5 == param_1) {
          return (uint *)0x0;
        }
      }
      uVar4 = (uint)puVar2 >> 1;
      do {
        if (*(char *)param_1 == param_2) {
          return param_1;
        }
        if (*(char *)(uint *)((int)param_1 + 1) == param_2) {
          return (uint *)((int)param_1 + 1);
        }
        param_1 = (uint *)((int)param_1 + 2);
        uVar4 = uVar4 - 1;
      } while (uVar4 != 0);
    }
  }
  else {
    uVar3 = param_3 - 1;
    if (param_3 == 0) {
      return (uint *)0x0;
    }
    if ((param_3 & 1) != 0) {
      if (*(char *)param_1 == param_2) {
        return param_1;
      }
      param_1 = (uint *)((int)param_1 + 1);
      uVar4 = uVar3;
      if (((uint)param_1 & 3) == 0) goto LAB_00813954;
      uVar3 = param_3 - 2;
      if (uVar3 == 0xffffffff) {
        return (uint *)0x0;
      }
    }
    param_3 = param_3 >> 1;
    puVar2 = param_1;
    do {
      if (*(char *)puVar2 == param_2) {
        return puVar2;
      }
      param_1 = (uint *)((int)puVar2 + 1);
      uVar4 = uVar3;
      if (((uint)param_1 & 3) == 0) goto LAB_00813954;
      if (*(char *)param_1 == param_2) {
        return param_1;
      }
      param_1 = (uint *)((int)puVar2 + 2);
      uVar4 = uVar3 - 1;
      if (((uint)param_1 & 3) == 0) goto LAB_00813954;
      uVar3 = uVar3 - 2;
      param_3 = param_3 - 1;
      puVar2 = param_1;
    } while (param_3 != 0);
  }
  return (uint *)0x0;
}



undefined4 * FUN_00813a44(int param_1,int param_2)

{
  undefined4 *puVar1;
  int iVar2;
  
  iVar2 = *(int *)(param_1 + 0x4c);
  if (iVar2 == 0) {
    iVar2 = FUN_00806b38(param_1,4,0x21);
    *(int *)(param_1 + 0x4c) = iVar2;
    if (iVar2 == 0) {
      return (undefined4 *)0x0;
    }
    puVar1 = *(undefined4 **)(iVar2 + param_2 * 4);
  }
  else {
    puVar1 = *(undefined4 **)(iVar2 + param_2 * 4);
  }
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)FUN_00806b38(param_1,1,((1 << param_2) + 5) * 4);
    if (puVar1 == (undefined4 *)0x0) {
      return (undefined4 *)0x0;
    }
    puVar1[1] = param_2;
    puVar1[2] = 1 << param_2;
  }
  else {
    *(undefined4 *)(iVar2 + param_2 * 4) = *puVar1;
  }
  puVar1[4] = 0;
  puVar1[3] = 0;
  return puVar1;
}



undefined4 * FUN_00813b48(int param_1,undefined4 *param_2,int param_3,uint param_4)

{
  undefined4 *puVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = param_2[4];
  iVar5 = iVar6;
  if (iVar6 < 1) {
    iVar5 = 1;
  }
  puVar4 = param_2 + 4;
  do {
    puVar4 = puVar4 + 1;
    uVar2 = (*puVar4 & 0xffff) * param_3 + param_4;
    uVar3 = (*puVar4 >> 0x10) * param_3 + (uVar2 >> 0x10);
    param_4 = uVar3 >> 0x10;
    *puVar4 = uVar3 * 0x10000 + (uVar2 & 0xffff);
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  puVar1 = param_2;
  if (param_4 != 0) {
    if (iVar6 < (int)param_2[2]) {
      param_2[iVar6 + 5] = param_4;
      param_2[4] = iVar6 + 1;
    }
    else {
      puVar1 = (undefined4 *)FUN_00813a44(param_1,param_2[1] + 1);
      if (puVar1 == (undefined4 *)0x0) {
        uVar2 = FUN_00816c10("../../../../../../newlib-3.3.0/newlib/libc/stdlib/mprec.c",0xb5,0,
                             "Balloc succeeded");
        puVar1 = (undefined4 *)0x0;
        if ((uVar2 & 0xffff0000) == 0) {
          uVar2 = uVar2 << 0x10;
          puVar1 = (undefined4 *)0x10;
        }
        if ((uVar2 & 0xff000000) == 0) {
          puVar1 = puVar1 + 2;
          uVar2 = uVar2 << 8;
        }
        if ((uVar2 & 0xf0000000) == 0) {
          puVar1 = puVar1 + 1;
          uVar2 = uVar2 << 4;
        }
        if ((uVar2 & 0xc0000000) == 0) {
          puVar1 = (undefined4 *)((int)puVar1 + 2);
          uVar2 = uVar2 << 2;
        }
        if ((int)uVar2 < 0) {
          return puVar1;
        }
        if ((uVar2 & 0x40000000) != 0) {
          return (undefined4 *)((int)puVar1 + 1);
        }
        return (undefined4 *)0x20;
      }
      FUN_0080c25c(puVar1 + 3,param_2 + 3,(param_2[4] + 2) * 4);
      iVar5 = *(int *)(param_1 + 0x4c);
      *param_2 = *(undefined4 *)(iVar5 + param_2[1] * 4);
      *(undefined4 **)(iVar5 + param_2[1] * 4) = param_2;
      puVar1[iVar6 + 5] = param_4;
      puVar1[4] = iVar6 + 1;
    }
  }
  return puVar1;
}



int FUN_00813ca0(uint param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if ((param_1 & 0xffff0000) == 0) {
    param_1 = param_1 << 0x10;
    iVar1 = 0x10;
  }
  if ((param_1 & 0xff000000) == 0) {
    iVar1 = iVar1 + 8;
    param_1 = param_1 << 8;
  }
  if ((param_1 & 0xf0000000) == 0) {
    iVar1 = iVar1 + 4;
    param_1 = param_1 << 4;
  }
  if ((param_1 & 0xc0000000) == 0) {
    iVar1 = iVar1 + 2;
    param_1 = param_1 << 2;
  }
  if ((int)param_1 < 0) {
    return iVar1;
  }
  if ((param_1 & 0x40000000) != 0) {
    return iVar1 + 1;
  }
  return 0x20;
}



int FUN_00813d04(uint *param_1)

{
  int iVar1;
  uint uVar2;
  
  uVar2 = *param_1;
  if ((uVar2 & 7) == 0) {
    iVar1 = 0;
    if ((uVar2 & 0xffff) == 0) {
      uVar2 = uVar2 >> 0x10;
      iVar1 = 0x10;
    }
    if ((uVar2 & 0xff) == 0) {
      iVar1 = iVar1 + 8;
      uVar2 = uVar2 >> 8;
    }
    if ((uVar2 & 0xf) == 0) {
      iVar1 = iVar1 + 4;
      uVar2 = uVar2 >> 4;
    }
    if ((uVar2 & 3) == 0) {
      iVar1 = iVar1 + 2;
      uVar2 = uVar2 >> 2;
    }
    if ((uVar2 & 1) == 0) {
      uVar2 = uVar2 >> 1;
      iVar1 = iVar1 + 1;
      if (uVar2 == 0) {
        return 0x20;
      }
    }
    *param_1 = uVar2;
    return iVar1;
  }
  if ((uVar2 & 1) != 0) {
    return 0;
  }
  if ((uVar2 & 2) != 0) {
    *param_1 = uVar2 >> 1;
    return 1;
  }
  *param_1 = uVar2 >> 2;
  return 2;
}



undefined4 * FUN_00813ec0(undefined4 param_1,int param_2,int param_3)

{
  uint *puVar1;
  bool bVar2;
  uint *puVar3;
  undefined4 *puVar4;
  int iVar5;
  uint *puVar6;
  undefined4 *puVar7;
  uint *puVar8;
  undefined4 *puVar9;
  undefined4 *puVar10;
  uint *puVar11;
  undefined4 uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint *puVar16;
  int iVar17;
  uint *puVar18;
  int iVar19;
  int iVar20;
  uint uVar21;
  undefined4 *puVar22;
  uint uVar23;
  undefined8 uVar24;
  
  iVar19 = *(int *)(param_3 + 0x10);
  iVar20 = *(int *)(param_2 + 0x10);
  iVar5 = iVar19;
  iVar17 = param_2;
  if (iVar19 <= iVar20) {
    iVar5 = iVar20;
    iVar17 = param_3;
    iVar20 = iVar19;
    param_3 = param_2;
  }
  uVar23 = iVar5 + iVar20;
  iVar19 = *(int *)(param_3 + 4);
  if (*(int *)(param_3 + 8) < (int)uVar23) {
    iVar19 = iVar19 + 1;
  }
  puVar4 = (undefined4 *)FUN_00813a44(param_1,iVar19);
  if (puVar4 != (undefined4 *)0x0) {
    puVar16 = puVar4 + 5;
    puVar11 = puVar16 + uVar23;
    if (puVar16 < puVar11) {
      puVar22 = puVar4 + 4;
      uVar14 = (uint)((int)puVar11 + (-0x15 - (int)puVar4)) >> 2;
      uVar13 = uVar14 + 1;
      if ((uVar13 & 1) != 0) {
        *puVar16 = 0;
        puVar22 = puVar4 + 5;
        if (uVar14 == 0) goto LAB_00813f7c;
      }
      uVar13 = uVar13 >> 1;
      do {
        puVar22[1] = 0;
        puVar22 = puVar22 + 2;
        *puVar22 = 0;
        uVar13 = uVar13 - 1;
      } while (uVar13 != 0);
    }
LAB_00813f7c:
    uVar13 = iVar17 + 0x14;
    uVar21 = uVar13 + iVar20 * 4;
    uVar14 = param_3 + 0x14 + iVar5 * 4;
    if (uVar13 < uVar21) {
      bVar2 = uVar14 < param_3 + 0x15U;
      puVar3 = (uint *)((uVar21 + (-0x15 - iVar17) & 0xfffffffc) + uVar13);
      puVar6 = (uint *)(iVar17 + 0x10);
      iVar5 = (uVar14 + (-0x15 - param_3) >> 2) + 1;
      iVar17 = 1;
      if ((bVar2) || (iVar17 = iVar5, bVar2)) {
        iVar5 = 4;
      }
      else {
        iVar5 = iVar5 * 4;
      }
      do {
        while( true ) {
          puVar6 = puVar6 + 1;
          uVar14 = *puVar6 & 0xffff;
          if (uVar14 == 0) break;
          puVar18 = (uint *)(param_3 + 0x10);
          puVar8 = puVar16 + -1;
          uVar13 = 0;
          iVar20 = iVar17;
          do {
            puVar18 = puVar18 + 1;
            puVar8 = puVar8 + 1;
            uVar21 = (*puVar18 & 0xffff) * uVar14 + (*puVar8 & 0xffff) + uVar13;
            uVar15 = (*puVar18 >> 0x10) * uVar14 + (*puVar8 >> 0x10) + (uVar21 >> 0x10);
            uVar13 = uVar15 >> 0x10;
            *puVar8 = uVar15 * 0x10000 | uVar21 & 0xffff;
            iVar20 = iVar20 + -1;
          } while (iVar20 != 0);
          *(uint *)((int)puVar16 + iVar5) = uVar13;
          uVar14 = *puVar6 >> 0x10;
          if (uVar14 == 0) goto LAB_00813fec;
LAB_00814068:
          uVar15 = *puVar16;
          puVar18 = (uint *)(param_3 + 0x10);
          uVar13 = 0;
          uVar21 = uVar15;
          puVar8 = puVar16;
          iVar20 = iVar17;
          do {
            puVar18 = puVar18 + 1;
            uVar13 = (*puVar18 & 0xffff) * uVar14 + (uVar15 >> 0x10) + uVar13;
            *puVar8 = uVar13 * 0x10000 | uVar21 & 0xffff;
            puVar8 = puVar8 + 1;
            uVar15 = *puVar8;
            uVar21 = *(ushort *)puVar18 * uVar14 + (uVar13 >> 0x10) + (uVar15 & 0xffff);
            uVar13 = uVar21 >> 0x10;
            iVar20 = iVar20 + -1;
          } while (iVar20 != 0);
          *(uint *)((int)puVar16 + iVar5) = uVar21;
          puVar16 = puVar16 + 1;
          if (puVar6 == puVar3) goto LAB_008140d8;
        }
        uVar14 = *puVar6 >> 0x10;
        if (uVar14 != 0) goto LAB_00814068;
LAB_00813fec:
        puVar16 = puVar16 + 1;
      } while (puVar6 != puVar3);
    }
LAB_008140d8:
    uVar14 = uVar23;
    if ((0 < (int)uVar23) &&
       ((uVar21 = uVar23 - 1, uVar13 = uVar23, (uVar23 & 1) == 0 ||
        ((puVar11 = puVar11 + -1, *puVar11 == 0 && (uVar14 = uVar21, uVar13 = uVar21, uVar21 != 0)))
        ))) {
      uVar23 = uVar23 >> 1;
      do {
        uVar14 = uVar13;
        if (puVar11[-1] != 0) break;
        puVar16 = puVar11 + -2;
        puVar11 = puVar11 + -2;
        uVar14 = uVar13 - 1;
        if (*puVar16 != 0) break;
        uVar13 = uVar13 - 2;
        uVar23 = uVar23 - 1;
        uVar14 = uVar13;
      } while (uVar23 != 0);
    }
    puVar4[4] = uVar14;
    return puVar4;
  }
  uVar23 = 0;
  uVar24 = FUN_00816c10("../../../../../../newlib-3.3.0/newlib/libc/stdlib/mprec.c",0x15d,0,
                        "Balloc succeeded");
  iVar5 = (int)((ulonglong)uVar24 >> 0x20);
  puVar4 = (undefined4 *)uVar24;
  if ((uVar23 & 3) != 0) {
    puVar4 = (undefined4 *)
             FUN_00813b48(iVar5,puVar4,*(undefined4 *)(&DAT_0081b458 + ((uVar23 & 3) - 1) * 4),0);
  }
  uVar23 = (int)uVar23 >> 2;
  if (uVar23 == 0) {
    return puVar4;
  }
  puVar22 = *(undefined4 **)(iVar5 + 0x48);
  if (puVar22 != (undefined4 *)0x0) {
LAB_008141ec:
    puVar7 = puVar4;
    if ((uVar23 & 1) != 0) goto LAB_00814218;
    do {
      uVar23 = (int)uVar23 >> 1;
      puVar7 = puVar22;
      if (uVar23 == 0) {
        return puVar4;
      }
      while( true ) {
        puVar22 = (undefined4 *)*puVar7;
        if (puVar22 == (undefined4 *)0x0) {
          puVar22 = (undefined4 *)FUN_00813ec0(iVar5,puVar7,puVar7);
          *puVar7 = puVar22;
          *puVar22 = 0;
        }
        puVar7 = puVar4;
        if ((uVar23 & 1) == 0) break;
LAB_00814218:
        puVar4 = (undefined4 *)FUN_00813ec0(iVar5,puVar7,puVar22);
        if (puVar7 == (undefined4 *)0x0) break;
        uVar23 = (int)uVar23 >> 1;
        iVar17 = *(int *)(iVar5 + 0x4c);
        *puVar7 = *(undefined4 *)(iVar17 + puVar7[1] * 4);
        *(undefined4 **)(iVar17 + puVar7[1] * 4) = puVar7;
        puVar7 = puVar22;
        if (uVar23 == 0) {
          return puVar4;
        }
      }
    } while( true );
  }
  puVar22 = (undefined4 *)FUN_00813a44(iVar5,1);
  if (puVar22 != (undefined4 *)0x0) {
    puVar22[5] = 0x271;
    puVar22[4] = 1;
    *(undefined4 **)(iVar5 + 0x48) = puVar22;
    *puVar22 = 0;
    goto LAB_008141ec;
  }
  uVar23 = 0;
  uVar24 = FUN_00816c10("../../../../../../newlib-3.3.0/newlib/libc/stdlib/mprec.c",0x140,0,
                        "Balloc succeeded");
  iVar19 = (int)((ulonglong)uVar24 >> 0x20);
  puVar4 = (undefined4 *)uVar24;
  uVar14 = (int)uVar23 >> 5;
  iVar20 = uVar14 + puVar4[4];
  iVar17 = puVar4[1];
  for (iVar5 = puVar4[2]; iVar5 < iVar20 + 1; iVar5 = iVar5 << 1) {
    iVar17 = iVar17 + 1;
  }
  puVar22 = (undefined4 *)FUN_00813a44(iVar19,iVar17);
  if (puVar22 == (undefined4 *)0x0) {
    uVar24 = FUN_00816c10("../../../../../../newlib-3.3.0/newlib/libc/stdlib/mprec.c",0x1d9,0,
                          "Balloc succeeded");
    iVar5 = (int)((ulonglong)uVar24 >> 0x20);
    iVar17 = *(int *)((int)uVar24 + 0x10);
    puVar4 = (undefined4 *)(*(int *)(iVar5 + 0x10) - iVar17);
    if (puVar4 != (undefined4 *)0x0) {
      return puVar4;
    }
    puVar3 = (uint *)(iVar5 + 0x14);
    puVar6 = puVar3 + iVar17;
    iVar5 = (int)uVar24 + iVar17 * 4;
    puVar11 = puVar6 + -1;
    puVar16 = (uint *)(iVar5 + 0x10);
    bVar2 = *puVar11 < *puVar16;
    if (*puVar11 == *puVar16) {
      if (puVar11 <= puVar3) {
        return (undefined4 *)0x0;
      }
      if (((int)puVar6 + ~(uint)puVar3 >> 2 & 1) == 0) goto LAB_008145f8;
      uVar23 = puVar6[-2];
      bVar2 = uVar23 < *(uint *)(iVar5 + 0xc);
      if (uVar23 == *(uint *)(iVar5 + 0xc)) {
        if (puVar6 + -2 <= puVar3) {
          return (undefined4 *)0x0;
        }
        puVar18 = puVar6 + -3;
        puVar8 = (uint *)(iVar5 + 8);
        puVar11 = puVar6 + -4;
        puVar16 = (uint *)(iVar5 + 4);
        bVar2 = puVar6[-3] < *(uint *)(iVar5 + 8);
        if (puVar6[-3] == *(uint *)(iVar5 + 8)) {
          do {
            bVar2 = puVar18[-1] < puVar8[-1];
            if (puVar18[-1] != puVar8[-1]) break;
            if (puVar11 <= puVar3) {
              return (undefined4 *)0x0;
            }
LAB_008145f8:
            puVar6 = puVar11 + -1;
            puVar18 = puVar11 + -1;
            puVar1 = puVar16 + -1;
            puVar8 = puVar16 + -1;
            puVar11 = puVar11 + -2;
            puVar16 = puVar16 + -2;
            bVar2 = *puVar6 < *puVar1;
          } while (*puVar6 == *puVar1);
        }
      }
    }
    if (!bVar2) {
      return (undefined4 *)0x1;
    }
    return (undefined4 *)0xffffffff;
  }
  puVar7 = puVar22 + 5;
  if (0 < (int)uVar14) {
    puVar9 = puVar22 + 4;
    if ((uVar14 & 1) == 0) {
LAB_008143b8:
      uVar13 = uVar14 >> 1;
      do {
        puVar9[1] = 0;
        puVar9 = puVar9 + 2;
        *puVar9 = 0;
        uVar13 = uVar13 - 1;
      } while (uVar13 != 0);
    }
    else {
      *puVar7 = 0;
      puVar9 = puVar22 + 5;
      if (uVar14 != 1) goto LAB_008143b8;
    }
    puVar7 = puVar7 + uVar14;
  }
  uVar23 = uVar23 & 0x1f;
  puVar9 = (undefined4 *)((int)puVar4 + 0x15);
  puVar10 = puVar4 + puVar4[4] + 5;
  if (uVar23 == 0) {
    uVar23 = 1;
    if (puVar9 <= puVar10) {
      uVar23 = ((uint)((-0x15 - (int)puVar4) + (int)puVar10) >> 2) + 1;
    }
    puVar9 = puVar4 + 4;
    puVar10 = puVar7 + -1;
    if ((uVar23 & 1) != 0) {
      puVar9 = puVar4 + 5;
      *puVar7 = *puVar9;
      puVar10 = puVar7;
      if (uVar23 == 1) goto LAB_008144d4;
    }
    uVar23 = uVar23 >> 1;
    do {
      puVar10[1] = puVar9[1];
      puVar10[2] = puVar9[2];
      uVar23 = uVar23 - 1;
      puVar10 = puVar10 + 2;
      puVar9 = puVar9 + 2;
    } while (uVar23 != 0);
  }
  else {
    iVar5 = ((uint)((int)puVar10 + (-0x15 - (int)puVar4)) >> 2) + 1;
    iVar17 = 1;
    if (puVar10 >= puVar9) {
      iVar17 = iVar5;
    }
    puVar11 = puVar4 + 4;
    puVar16 = puVar7 + -1;
    uVar14 = 0;
    do {
      puVar11 = puVar11 + 1;
      puVar16 = puVar16 + 1;
      *puVar16 = *puVar11 << uVar23 | uVar14;
      uVar14 = *puVar11 >> 0x20 - uVar23;
      iVar17 = iVar17 + -1;
    } while (iVar17 != 0);
    if (puVar10 < puVar9) {
      iVar5 = 4;
    }
    else {
      iVar5 = iVar5 * 4;
    }
    *(uint *)((int)puVar7 + iVar5) = uVar14;
    if (uVar14 != 0) {
      iVar20 = iVar20 + 1;
    }
  }
LAB_008144d4:
  iVar5 = puVar4[1];
  iVar17 = *(int *)(iVar19 + 0x4c);
  uVar12 = *(undefined4 *)(iVar17 + iVar5 * 4);
  puVar22[4] = iVar20;
  *puVar4 = uVar12;
  *(undefined4 **)(iVar17 + iVar5 * 4) = puVar4;
  return puVar22;
}



// WARNING: Could not reconcile some variable overlaps

uint FUN_00814630(undefined4 param_1,int param_2,int param_3)

{
  bool bVar1;
  uint uVar2;
  int iVar3;
  uint *puVar4;
  int *piVar5;
  uint *puVar6;
  int iVar7;
  uint uVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  uint *puVar12;
  uint *puVar13;
  byte in_xer_so;
  byte bVar14;
  byte in_cr1;
  byte in_cr2;
  byte in_cr3;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  int iVar15;
  undefined8 extraout_f1;
  undefined8 uVar16;
  uint local_58;
  uint local_54 [3];
  undefined8 local_48;
  uint local_3c;
  int local_2c;
  int local_28;
  int local_24;
  undefined4 local_1c;
  
  iVar7 = *(int *)(param_3 + 0x10);
  local_2c = *(int *)(param_2 + 0x10) - iVar7;
  local_28 = param_2;
  if (local_2c == 0) {
    puVar6 = (uint *)(param_2 + 0x14);
    puVar4 = puVar6 + iVar7;
    iVar7 = param_3 + iVar7 * 4;
    puVar12 = puVar4 + -1;
    puVar13 = (uint *)(iVar7 + 0x10);
    bVar1 = *puVar12 < *puVar13;
    if (*puVar12 == *puVar13) {
      if (puVar6 < puVar12) {
        if (((int)puVar4 + ~(uint)puVar6 >> 2 & 1) != 0) {
          puVar12 = puVar4 + -2;
          puVar13 = (uint *)(iVar7 + 0xc);
          bVar1 = *puVar12 < *puVar13;
          if (*puVar12 == *puVar13) goto LAB_008146d4;
          goto LAB_00814728;
        }
        do {
          bVar1 = puVar12[-1] < puVar13[-1];
          if (puVar12[-1] != puVar13[-1]) goto LAB_00814728;
          puVar4 = puVar12 + -2;
          puVar13 = puVar13 + -2;
          puVar12 = puVar12 + -2;
          bVar1 = *puVar4 < *puVar13;
          if (*puVar4 != *puVar13) goto LAB_00814728;
LAB_008146d4:
        } while (puVar6 < puVar12);
      }
      uVar2 = FUN_00813a44(param_1,0);
      bVar14 = ((int)uVar2 < 0) << 3 | (0 < (int)uVar2) << 2 | (uVar2 == 0) << 1 | in_xer_so & 1;
      if (uVar2 != 0) {
        *(undefined4 *)(uVar2 + 0x10) = 1;
        *(undefined4 *)(uVar2 + 0x14) = 0;
        return uVar2;
      }
      FUN_00816c10("../../../../../../newlib-3.3.0/newlib/libc/stdlib/mprec.c",0x232,0,
                   "Balloc succeeded");
      goto LAB_008148ec;
    }
LAB_00814728:
    if (bVar1) goto LAB_00814890;
  }
  else {
    if (local_2c < 0) {
LAB_00814890:
      local_2c = 1;
      local_28 = param_3;
      param_3 = param_2;
    }
    else {
      local_2c = 0;
    }
  }
  uVar2 = FUN_00813a44(param_1,*(undefined4 *)(local_28 + 4));
  bVar14 = ((int)uVar2 < 0) << 3 | (0 < (int)uVar2) << 2 | (uVar2 == 0) << 1 | in_xer_so & 1;
  if (uVar2 != 0) {
    iVar3 = *(int *)(local_28 + 0x10);
    uVar11 = param_3 + 0x14 + *(int *)(param_3 + 0x10) * 4;
    bVar1 = uVar11 < param_3 + 0x15U;
    uVar11 = (-0x15 - param_3) + uVar11;
    *(int *)(uVar2 + 0xc) = local_2c;
    iVar7 = (uVar11 >> 2) + 1;
    uVar10 = local_28 + 0x14 + iVar3 * 4;
    iVar15 = 1;
    if (!bVar1) {
      iVar15 = iVar7;
    }
    puVar12 = (uint *)(local_28 + 0x10);
    puVar13 = (uint *)(param_3 + 0x10);
    puVar4 = (uint *)(uVar2 + 0x10);
    iVar9 = 0;
    do {
      puVar12 = puVar12 + 1;
      puVar13 = puVar13 + 1;
      uVar8 = ((*puVar12 & 0xffff) + iVar9) - (*puVar13 & 0xffff);
      iVar9 = ((*puVar12 >> 0x10) - (*puVar13 >> 0x10)) + ((int)uVar8 >> 0x10);
      uVar8 = iVar9 * 0x10000 | uVar8 & 0xffff;
      iVar9 = iVar9 >> 0x10;
      puVar4 = puVar4 + 1;
      *puVar4 = uVar8;
      iVar15 = iVar15 + -1;
    } while (iVar15 != 0);
    if (bVar1) {
      uVar11 = 0;
    }
    else {
      uVar11 = uVar11 & 0xfffffffc;
    }
    puVar12 = (uint *)(uVar2 + 0x14 + uVar11);
    if (bVar1) {
      iVar7 = 4;
    }
    else {
      iVar7 = iVar7 * 4;
    }
    uVar11 = local_28 + 0x14 + iVar7;
    iVar7 = uVar2 + 0x14 + iVar7;
    if (uVar11 < uVar10) {
      puVar12 = (uint *)(uVar11 - 4);
      uVar11 = (uVar10 - 1) - uVar11;
      puVar13 = (uint *)(iVar7 + -4);
      iVar15 = (uVar11 >> 2) + 1;
      do {
        puVar12 = puVar12 + 1;
        uVar10 = (*puVar12 & 0xffff) + iVar9;
        iVar9 = (*puVar12 >> 0x10) + ((int)uVar10 >> 0x10);
        uVar8 = iVar9 * 0x10000 | uVar10 & 0xffff;
        iVar9 = iVar9 >> 0x10;
        puVar13 = puVar13 + 1;
        *puVar13 = uVar8;
        iVar15 = iVar15 + -1;
      } while (iVar15 != 0);
      puVar12 = (uint *)(iVar7 + (uVar11 & 0xfffffffc));
    }
    while (uVar8 == 0) {
      puVar12 = puVar12 + -1;
      iVar3 = iVar3 + -1;
      uVar8 = *puVar12;
    }
    *(int *)(uVar2 + 0x10) = iVar3;
    return uVar2;
  }
LAB_008148ec:
  piVar5 = (int *)0x0;
  local_1c = 0x814908;
  uVar16 = FUN_00816c10("../../../../../../newlib-3.3.0/newlib/libc/stdlib/mprec.c",0x240,0,
                        "Balloc succeeded");
  local_3c = (uint)(bVar14 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) << 4
             | (uint)(in_cr7 & 0xf);
  local_48._0_4_ = (uint)((ulonglong)extraout_f1 >> 0x20);
  local_48._4_4_ = (uint)extraout_f1;
  local_48 = extraout_f1;
  local_24 = param_3;
  uVar2 = FUN_00813a44((int)((ulonglong)uVar16 >> 0x20),1);
  if (uVar2 == 0) {
    uVar16 = FUN_00816c10("../../../../../../newlib-3.3.0/newlib/libc/stdlib/mprec.c",0x30a,0,
                          "Balloc succeeded");
    iVar7 = (int)uVar16;
    uVar2 = FUN_00817d08((int)((ulonglong)uVar16 >> 0x20),(int)*(short *)(iVar7 + 0xe));
    if (-1 < (int)uVar2) {
      uVar11 = *(uint *)(iVar7 + 0x54);
      *(uint *)(iVar7 + 0x54) = uVar11 + uVar2;
      *(uint *)(iVar7 + 0x50) =
           *(int *)(iVar7 + 0x50) + ((int)uVar2 >> 0x1f) + (uint)CARRY4(uVar11,uVar2);
      return uVar2;
    }
    *(ushort *)(iVar7 + 0xc) = *(ushort *)(iVar7 + 0xc) & 0xefff;
    return uVar2;
  }
  uVar11 = local_48._0_4_ >> 0x14 & 0x7ff;
  local_58 = local_48._0_4_ & 0xfffff;
  if (uVar11 != 0) {
    local_58 = local_58 | 0x100000;
  }
  if (local_48._4_4_ == 0) {
    iVar7 = 1;
    iVar3 = FUN_00813d04(&local_58);
    iVar3 = iVar3 + 0x20;
    *(undefined4 *)(uVar2 + 0x10) = 1;
    *(uint *)(uVar2 + 0x14) = local_58;
  }
  else {
    local_54[0] = local_48._4_4_;
    iVar3 = FUN_00813d04(local_54);
    if (iVar3 == 0) {
      *(uint *)(uVar2 + 0x14) = local_54[0];
    }
    else {
      uVar10 = local_58 << 0x20 - iVar3;
      local_58 = local_58 >> iVar3;
      *(uint *)(uVar2 + 0x14) = uVar10 | local_54[0];
    }
    *(uint *)(uVar2 + 0x18) = local_58;
    iVar7 = 2 - (uint)(local_58 == 0);
    *(int *)(uVar2 + 0x10) = iVar7;
  }
  if (uVar11 == 0) {
    *(int *)uVar16 = iVar3 + -0x432;
    iVar3 = FUN_00813ca0(*(undefined4 *)(uVar2 + iVar7 * 4 + 0x10));
    *piVar5 = iVar7 * 0x20 - iVar3;
    return uVar2;
  }
  *(int *)uVar16 = (uVar11 - 0x433) + iVar3;
  *piVar5 = 0x35 - iVar3;
  return uVar2;
}



void FUN_00814be0(undefined4 param_1,int param_2)

{
  longlong lVar1;
  
  lVar1 = FUN_00817c28(param_1,(int)*(short *)(param_2 + 0xe));
  if (lVar1 != -1) {
    *(int *)(param_2 + 0x50) = (int)((ulonglong)lVar1 >> 0x20);
    *(int *)(param_2 + 0x54) = (int)lVar1;
    *(ushort *)(param_2 + 0xc) = *(ushort *)(param_2 + 0xc) | 0x1000;
    return;
  }
  *(ushort *)(param_2 + 0xc) = *(ushort *)(param_2 + 0xc) & 0xefff;
  return;
}



undefined4 FUN_00814c58(undefined4 *param_1,int *param_2,undefined4 *param_3)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 *puVar6;
  uint uVar7;
  uint uVar8;
  
  if (param_3[2] != 0) {
    puVar6 = (undefined4 *)*param_3;
    iVar1 = *param_2;
    uVar7 = param_2[2];
    do {
      while( true ) {
        do {
          uVar8 = puVar6[1];
          uVar5 = *puVar6;
          puVar6 = puVar6 + 2;
        } while (uVar8 == 0);
        if (uVar7 <= uVar8) break;
        FUN_00817714(iVar1,uVar5,uVar8);
        iVar4 = param_3[2];
        uVar7 = param_2[2] - uVar8;
        iVar1 = *param_2 + uVar8;
        param_2[2] = uVar7;
        *param_2 = iVar1;
        param_3[2] = iVar4 - uVar8;
        if (iVar4 - uVar8 == 0) goto LAB_00814e14;
      }
      uVar3 = uVar7;
      if ((*(ushort *)(param_2 + 3) & 0x480) != 0) {
        uVar7 = param_2[5] * 3;
        iVar4 = iVar1 - param_2[4];
        uVar7 = ((int)uVar7 >> 1) + (uint)((int)uVar7 < 0 && (uVar7 & 1) != 0);
        uVar3 = uVar8 + 1 + iVar4;
        if (uVar7 < uVar3) {
          uVar7 = uVar3;
        }
        if ((*(ushort *)(param_2 + 3) & 0x400) == 0) {
          iVar2 = FUN_00806b64(param_1);
          if (iVar2 == 0) {
            FUN_00806b50(param_1,param_2[4]);
            goto LAB_00814e48;
          }
        }
        else {
          iVar2 = FUN_00806b30(param_1,uVar7);
          if (iVar2 == 0) {
LAB_00814e48:
            *param_1 = 0xc;
            *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) | 0x40;
            param_3[2] = 0;
            param_3[1] = 0;
            return 0xffffffff;
          }
          FUN_0080c25c(iVar2,param_2[4],iVar4);
          *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) & 0xfb7f | 0x80;
        }
        iVar1 = iVar2 + iVar4;
        *param_2 = iVar1;
        param_2[2] = uVar7 - iVar4;
        param_2[5] = uVar7;
        param_2[4] = iVar2;
        uVar3 = uVar8;
      }
      FUN_00817714(iVar1,uVar5,uVar3);
      iVar4 = param_3[2];
      uVar7 = param_2[2] - uVar3;
      iVar1 = *param_2 + uVar3;
      param_2[2] = uVar7;
      *param_2 = iVar1;
      param_3[2] = iVar4 - uVar8;
    } while (iVar4 - uVar8 != 0);
  }
LAB_00814e14:
  param_3[1] = 0;
  return 0;
}



undefined4 FUN_00814ea4(undefined4 param_1,int param_2,int **param_3)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  int *piVar5;
  uint uVar6;
  undefined4 *puVar7;
  uint uVar8;
  
  if ((*(uint *)(param_2 + 0x68) & 0x2000) == 0) {
    uVar3 = FUN_00817164();
    param_3[2] = (int *)0x0;
    param_3[1] = (int *)0x0;
    return uVar3;
  }
  piVar4 = param_3[2];
  piVar5 = *param_3;
  do {
    if (piVar4 == (int *)0x0) {
      uVar3 = 0;
LAB_00814f60:
      param_3[2] = (int *)0x0;
      param_3[1] = (int *)0x0;
      return uVar3;
    }
    uVar6 = piVar5[1];
    uVar1 = uVar6 >> 2;
    if (uVar1 != 0) {
      puVar7 = (undefined4 *)(*piVar5 + -4);
      uVar8 = 0;
      do {
        puVar7 = puVar7 + 1;
        uVar8 = uVar8 + 1;
        iVar2 = FUN_00817084(param_1,*puVar7,param_2);
        if (iVar2 == -1) {
          uVar3 = 0xffffffff;
          goto LAB_00814f60;
        }
      } while (uVar1 != uVar8);
      piVar4 = param_3[2];
    }
    piVar5 = piVar5 + 2;
    piVar4 = (int *)((int)piVar4 - (uVar6 & 0xfffffffc));
    param_3[2] = piVar4;
  } while( true );
}



undefined4 FUN_00814fec(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  
  if (*(int *)(param_3 + 8) != 0) {
    uVar1 = FUN_00814ea4();
    return uVar1;
  }
  *(undefined4 *)(param_3 + 4) = 0;
  return 0;
}



// WARNING: Removing unreachable block (ram,0x00815bc8)
// WARNING: Removing unreachable block (ram,0x00815bd0)
// WARNING: Removing unreachable block (ram,0x00816704)
// WARNING: Removing unreachable block (ram,0x00815bd8)
// WARNING: Removing unreachable block (ram,0x00815bf8)
// WARNING: Removing unreachable block (ram,0x00815be0)
// WARNING: Removing unreachable block (ram,0x00815c18)
// WARNING: Removing unreachable block (ram,0x00815c20)
// WARNING: Removing unreachable block (ram,0x00815c38)
// WARNING: Removing unreachable block (ram,0x00815d98)
// WARNING: Removing unreachable block (ram,0x00815bec)
// WARNING: Removing unreachable block (ram,0x00815c58)
// WARNING: Removing unreachable block (ram,0x00815c60)
// WARNING: Removing unreachable block (ram,0x008160c0)
// WARNING: Removing unreachable block (ram,0x00815c7c)
// WARNING: Removing unreachable block (ram,0x008165e8)
// WARNING: Removing unreachable block (ram,0x00815c84)
// WARNING: Removing unreachable block (ram,0x00815c9c)
// WARNING: Removing unreachable block (ram,0x00815cb8)
// WARNING: Removing unreachable block (ram,0x0081657c)
// WARNING: Removing unreachable block (ram,0x00815cc0)
// WARNING: Removing unreachable block (ram,0x00815ce0)
// WARNING: Removing unreachable block (ram,0x00815cc8)
// WARNING: Removing unreachable block (ram,0x00815d00)
// WARNING: Removing unreachable block (ram,0x00815d08)
// WARNING: Removing unreachable block (ram,0x00815d20)
// WARNING: Removing unreachable block (ram,0x00815d6c)
// WARNING: Removing unreachable block (ram,0x00815cd4)
// WARNING: Removing unreachable block (ram,0x00815d3c)
// WARNING: Removing unreachable block (ram,0x00815d40)
// WARNING: Removing unreachable block (ram,0x00815d5c)
// WARNING: Removing unreachable block (ram,0x00816004)
// WARNING: Removing unreachable block (ram,0x0081600c)
// WARNING: Removing unreachable block (ram,0x00816024)
// WARNING: Removing unreachable block (ram,0x00816428)
// WARNING: Removing unreachable block (ram,0x00815328)
// WARNING: Removing unreachable block (ram,0x008156d0)
// WARNING: Removing unreachable block (ram,0x00815e0c)
// WARNING: Removing unreachable block (ram,0x00815e14)
// WARNING: Removing unreachable block (ram,0x00815e48)
// WARNING: Removing unreachable block (ram,0x00815e2c)
// WARNING: Removing unreachable block (ram,0x00815e64)
// WARNING: Removing unreachable block (ram,0x00815ee4)
// WARNING: Removing unreachable block (ram,0x00815e38)
// WARNING: Removing unreachable block (ram,0x00815e6c)
// WARNING: Removing unreachable block (ram,0x008156c4)
// WARNING: Removing unreachable block (ram,0x00815e84)
// WARNING: Removing unreachable block (ram,0x00815ea4)
// WARNING: Removing unreachable block (ram,0x00815e44)
// WARNING: Removing unreachable block (ram,0x008166d8)
// WARNING: Removing unreachable block (ram,0x00815eb8)
// WARNING: Removing unreachable block (ram,0x00816074)
// WARNING: Removing unreachable block (ram,0x0081607c)
// WARNING: Removing unreachable block (ram,0x00816094)
// WARNING: Removing unreachable block (ram,0x00816278)
// WARNING: Removing unreachable block (ram,0x00816614)
// WARNING: Removing unreachable block (ram,0x00816284)
// WARNING: Removing unreachable block (ram,0x00815de0)
// WARNING: Removing unreachable block (ram,0x00816288)
// WARNING: Removing unreachable block (ram,0x00815ed4)
// WARNING: Removing unreachable block (ram,0x00815448)
// WARNING: Removing unreachable block (ram,0x00815f00)
// WARNING: Removing unreachable block (ram,0x00815f08)
// WARNING: Removing unreachable block (ram,0x00815f0c)
// WARNING: Removing unreachable block (ram,0x008166fc)
// WARNING: Removing unreachable block (ram,0x00815f18)
// WARNING: Removing unreachable block (ram,0x00815f38)
// WARNING: Removing unreachable block (ram,0x00815f58)
// WARNING: Removing unreachable block (ram,0x00815fd8)
// WARNING: Removing unreachable block (ram,0x00815f60)
// WARNING: Removing unreachable block (ram,0x00815f78)
// WARNING: Removing unreachable block (ram,0x00815f20)
// WARNING: Removing unreachable block (ram,0x00815f2c)
// WARNING: Removing unreachable block (ram,0x00815f94)
// WARNING: Removing unreachable block (ram,0x00815fb0)
// WARNING: Removing unreachable block (ram,0x00815fb8)
// WARNING: Removing unreachable block (ram,0x00815fd0)
// WARNING: Removing unreachable block (ram,0x0081535c)
// WARNING: Removing unreachable block (ram,0x00815380)
// WARNING: Removing unreachable block (ram,0x00815da8)
// WARNING: Removing unreachable block (ram,0x0081605c)
// WARNING: Removing unreachable block (ram,0x008161c0)
// WARNING: Removing unreachable block (ram,0x00816060)
// WARNING: Removing unreachable block (ram,0x00815db0)
// WARNING: Removing unreachable block (ram,0x00815dc8)
// WARNING: Removing unreachable block (ram,0x0081547c)
// WARNING: Removing unreachable block (ram,0x00815394)
// WARNING: Removing unreachable block (ram,0x00815df8)
// WARNING: Removing unreachable block (ram,0x008153b8)
// WARNING: Removing unreachable block (ram,0x00816048)
// WARNING: Removing unreachable block (ram,0x008153c0)
// WARNING: Removing unreachable block (ram,0x008153d8)

int FUN_00815008(int param_1,int param_2,byte *param_3,undefined4 param_4)

{
  byte bVar1;
  undefined *puVar2;
  byte *pbVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  ushort uVar8;
  uint uVar7;
  byte **ppbVar9;
  code *pcVar10;
  byte *pbVar11;
  byte *pbVar12;
  byte in_cr0;
  byte in_cr1;
  byte in_cr2;
  byte in_cr3;
  byte unaff_cr4;
  byte in_cr5;
  byte in_cr6;
  byte in_cr7;
  byte *local_f8;
  byte *local_f4;
  byte *local_f0 [14];
  byte local_b8 [40];
  byte **local_90;
  byte *local_8c;
  byte *local_88;
  undefined auStack124 [12];
  int local_70;
  undefined local_6a;
  byte *local_68;
  undefined *local_64;
  undefined4 local_60;
  char *local_58;
  uint local_4c;
  
  local_4c = (uint)(in_cr0 & 0xf) << 0x1c | (uint)(in_cr1 & 0xf) << 0x18 |
             (uint)(in_cr2 & 0xf) << 0x14 | (uint)(in_cr3 & 0xf) << 0x10 |
             (uint)(unaff_cr4 & 0xf) << 0xc | (uint)(in_cr5 & 0xf) << 8 | (uint)(in_cr6 & 0xf) << 4
             | (uint)(in_cr7 & 0xf);
  FUN_0080c398(auStack124,0,8);
  if ((param_1 != 0) && (*(int *)(param_1 + 0x38) == 0)) {
    FUN_00812e28(param_1);
  }
  uVar8 = *(ushort *)(param_2 + 0xc);
  if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((uVar8 & 0x200) == 0)) {
    FUN_00817e90(param_2);
    uVar8 = *(ushort *)(param_2 + 0xc);
  }
  if ((uVar8 & 0x2000) == 0) {
    *(ushort *)(param_2 + 0xc) = uVar8 | 0x2000;
    *(uint *)(param_2 + 0x68) = *(uint *)(param_2 + 0x68) & 0xffffdfff;
  }
  if (((uVar8 & 8) == 0) || (*(int *)(param_2 + 0x10) == 0)) {
    iVar6 = FUN_008110fc(param_1,param_2);
    if (iVar6 != 0) {
      if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((*(ushort *)(param_2 + 0xc) & 0x200) == 0)) {
        local_68 = (byte *)0xffffffff;
        FUN_00817ee4(param_2);
      }
      else {
LAB_00816720:
        local_68 = (byte *)0xffffffff;
      }
      return (int)local_68;
    }
    uVar8 = *(ushort *)(param_2 + 0xc);
  }
  if (((uVar8 & 0x1a) == 10) && (-1 < *(short *)(param_2 + 0xe))) {
    if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((uVar8 & 0x200) == 0)) {
      FUN_00817ee4(param_2);
    }
    iVar6 = FUN_008167d0(param_1,param_2,param_3,param_4);
    return iVar6;
  }
  local_88 = (byte *)0x0;
  local_64 = &DAT_0081b464;
  local_58 = "0123456789abcdef";
  local_60 = 0;
  local_68 = (byte *)0x0;
  local_90 = &local_f8;
LAB_00815154:
  local_8c = (byte *)0x0;
  iVar6 = *(int *)(PTR_DAT_0081e3d4 + 0x34);
  pbVar12 = param_3;
  if (iVar6 == 0) goto LAB_008151b8;
LAB_00815168:
  pcVar10 = *(code **)(iVar6 + 0xe4);
  uVar4 = FUN_00813164();
  iVar6 = (*pcVar10)(param_1,&local_70,pbVar12,uVar4,auStack124);
  do {
    ppbVar9 = &local_f8;
    if (iVar6 == 0) {
      pbVar11 = pbVar12 + -(int)param_3;
      if (pbVar11 == (byte *)0x0) {
LAB_0081524c:
        if ((local_88 != (byte *)0x0) &&
           (iVar6 = FUN_00814ea4(param_1,param_2,&local_90), iVar6 != 0)) goto LAB_008156dc;
        uVar8 = *(ushort *)(param_2 + 0xc);
        local_8c = (byte *)0x0;
      }
      else {
LAB_008151f0:
        local_8c = local_8c + 1;
        local_88 = pbVar11 + (int)local_88;
        local_f8 = param_3;
        local_f4 = pbVar11;
        if ((int)local_8c < 8) {
          ppbVar9 = local_f0;
          pbVar3 = local_8c;
LAB_0081523c:
          local_8c = pbVar3;
          local_68 = pbVar11 + (int)local_68;
          if (iVar6 != 0) goto LAB_00815270;
          goto LAB_0081524c;
        }
        pbVar3 = local_88;
        if ((local_88 == (byte *)0x0) ||
           (iVar5 = FUN_00814ea4(param_1,param_2,&local_90), pbVar3 = local_8c, iVar5 == 0))
        goto LAB_0081523c;
LAB_008156dc:
        uVar8 = *(ushort *)(param_2 + 0xc);
      }
      if (((*(uint *)(param_2 + 0x68) & 1) == 0) && ((uVar8 & 0x200) == 0)) {
        FUN_00817ee4(param_2);
        uVar8 = *(ushort *)(param_2 + 0xc);
      }
      if ((uVar8 & 0x40) == 0) {
        return (int)local_68;
      }
      goto LAB_00816720;
    }
    if (iVar6 < 0) {
      FUN_0080c398(auStack124,0,8);
      iVar6 = 1;
    }
    else {
      if (local_70 == 0x25) {
        pbVar11 = pbVar12 + -(int)param_3;
        if (pbVar11 != (byte *)0x0) goto LAB_008151f0;
LAB_00815270:
        bVar1 = pbVar12[1];
        local_6a = 0;
        param_3 = pbVar12 + 2;
        uVar7 = bVar1 - 0x20;
        if (uVar7 < 0x59) {
                    // WARNING: Could not recover jumptable at 0x008152b8. Too many branches
                    // WARNING: Treating indirect jump as call
          iVar6 = (*(code *)(local_64 + *(int *)(local_64 + uVar7 * 4)))();
          return iVar6;
        }
        if (bVar1 == 0) goto LAB_0081524c;
        local_8c = local_8c + 1;
        local_6a = 0;
        pbVar12 = local_88 + 1;
        local_b8[0] = bVar1;
        local_88 = pbVar12;
        *ppbVar9 = local_b8;
        ppbVar9[1] = (byte *)0x1;
        if (7 < (int)local_8c) {
          if (pbVar12 == (byte *)0x0) {
            local_68 = local_68 + 1;
            goto LAB_00815154;
          }
          iVar6 = FUN_00814ea4(param_1,param_2,&local_90);
          pbVar12 = local_88;
          if (iVar6 != 0) goto LAB_008156dc;
        }
        local_68 = local_68 + 1;
        if ((pbVar12 != (byte *)0x0) &&
           (iVar6 = FUN_00814ea4(param_1,param_2,&local_90), iVar6 != 0)) goto LAB_008156dc;
        goto LAB_00815154;
      }
    }
    pbVar12 = pbVar12 + iVar6;
    iVar6 = *(int *)(PTR_DAT_0081e3d4 + 0x34);
    if (iVar6 != 0) goto LAB_00815168;
LAB_008151b8:
    puVar2 = PTR_FUN_0081d29c;
    uVar4 = FUN_00813164();
    iVar6 = (*(code *)puVar2)(param_1,&local_70,pbVar12,uVar4,auStack124);
  } while( true );
}



int FUN_008167d0(undefined4 param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  undefined auStack1152 [1024];
  undefined *local_80 [2];
  undefined4 local_78;
  ushort local_74;
  undefined2 local_72;
  undefined *local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_5c;
  undefined auStack36 [12];
  undefined4 local_18;
  
  local_80[0] = auStack1152;
  local_18 = *(undefined4 *)(param_2 + 0x68);
  local_72 = *(undefined2 *)(param_2 + 0xe);
  local_74 = *(ushort *)(param_2 + 0xc) & 0xfffd;
  local_64 = *(undefined4 *)(param_2 + 0x1c);
  local_5c = *(undefined4 *)(param_2 + 0x24);
  local_78 = 0x400;
  local_6c = 0x400;
  local_68 = 0;
  local_70 = local_80[0];
  FUN_00817ff4(auStack36,1);
  iVar1 = FUN_00815008(param_1,local_80,param_3,param_4);
  if (-1 < iVar1) {
    iVar2 = FUN_00812d14(param_1,local_80);
    if (iVar2 != 0) {
      iVar1 = -1;
    }
  }
  if ((local_74 & 0x40) != 0) {
    *(ushort *)(param_2 + 0xc) = *(ushort *)(param_2 + 0xc) | 0x40;
  }
  FUN_00818010(auStack36);
  return iVar1;
}



undefined4 FUN_00816b08(undefined4 *param_1,byte *param_2,uint param_3)

{
  byte bVar1;
  
  if (param_2 == (byte *)0x0) {
    return 0;
  }
  bVar1 = (byte)param_3;
  if (param_3 < 0x80) {
    *param_2 = bVar1;
    return 1;
  }
  if (param_3 - 0x80 < 0x780) {
    *param_2 = (byte)(param_3 >> 6) & 0x3f | 0xc0;
    param_2[1] = bVar1 & 0x3f | 0x80;
    return 2;
  }
  if (0xf7ff < param_3 - 0x800) {
    if (param_3 - 0x10000 < 0x100000) {
      *param_2 = (byte)(param_3 >> 0x12) & 0xf | 0xf0;
      param_2[1] = (byte)(param_3 >> 0xc) & 0x3f | 0x80;
      param_2[2] = (byte)(param_3 >> 6) & 0x3f | 0x80;
      param_2[3] = bVar1 & 0x3f | 0x80;
      return 4;
    }
    *param_1 = 0x8a;
    return 0xffffffff;
  }
  *param_2 = (byte)(param_3 >> 0xc) & 0x1f | 0xe0;
  param_2[1] = (byte)(param_3 >> 6) & 0x3f | 0x80;
  param_2[2] = bVar1 & 0x3f | 0x80;
  return 3;
}



void FUN_00816c10(void)

{
  undefined4 uVar1;
  
  uVar1 = *(undefined4 *)(PTR_DAT_0081e3d4 + 0xc);
  do {
    FUN_00816e64(uVar1,"assertion \"%s\" failed: file \"%s\", line %d%s%s\n");
    uVar1 = FUN_0080c044();
  } while( true );
}



void FUN_00816e64(undefined8 param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4,
                 undefined8 param_5,undefined8 param_6,undefined8 param_7,undefined8 param_8,
                 undefined4 param_9,undefined4 param_10,undefined4 param_11,undefined4 param_12,
                 undefined4 param_13,undefined4 param_14,undefined4 param_15,undefined4 param_16)

{
  byte in_cr1;
  undefined local_68;
  undefined local_67;
  undefined *local_64;
  undefined4 ***local_60 [2];
  undefined4 local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  undefined8 local_10;
  undefined8 local_8;
  
  if ((bool)(in_cr1 >> 1 & 1)) {
    local_40 = param_1;
    local_38 = param_2;
    local_30 = param_3;
    local_28 = param_4;
    local_20 = param_5;
    local_18 = param_6;
    local_10 = param_7;
    local_8 = param_8;
  }
  local_64 = &stack0x00000008;
  local_60[0] = local_60;
  local_68 = 2;
  local_67 = 0;
  local_58 = param_11;
  local_54 = param_12;
  local_50 = param_13;
  local_4c = param_14;
  local_48 = param_15;
  local_44 = param_16;
  FUN_00815008(PTR_DAT_0081e3d4,param_9,param_10,&local_68);
  return;
}



uint FUN_00816ef8(undefined4 param_1,uint param_2,int *param_3)

{
  int iVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte local_28;
  byte abStack39 [15];
  
  iVar1 = FUN_00813164();
  if ((iVar1 == 1) && (param_2 - 1 < 0xff)) {
    uVar4 = param_2 & 0xff;
    local_28 = (byte)uVar4;
    pbVar5 = (byte *)0x1;
  }
  else {
    pbVar5 = (byte *)FUN_00810fa4(param_1,&local_28,param_2,param_3 + 0x18);
    if (pbVar5 == (byte *)0xffffffff) {
      *(ushort *)(param_3 + 3) = *(ushort *)(param_3 + 3) | 0x40;
      return 0xffffffff;
    }
    if (pbVar5 == (byte *)0x0) {
      return param_2;
    }
    uVar4 = (uint)local_28;
  }
  pbVar6 = &local_28;
  iVar1 = 1 - (int)pbVar6;
  while( true ) {
    iVar3 = param_3[2] + -1;
    param_3[2] = iVar3;
    if ((iVar3 < 0) && ((iVar3 < param_3[6] || (uVar4 == 10)))) {
      iVar3 = FUN_008178c0(param_1,uVar4,param_3);
      if (iVar3 == -1) {
        return 0xffffffff;
      }
    }
    else {
      puVar2 = (undefined *)*param_3;
      *param_3 = (int)(puVar2 + 1);
      *puVar2 = (char)uVar4;
    }
    if (pbVar5 <= pbVar6 + iVar1) break;
    pbVar6 = pbVar6 + 1;
    uVar4 = (uint)*pbVar6;
  }
  return param_2;
}



undefined4 FUN_00817084(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  ushort uVar2;
  
  uVar2 = *(ushort *)(param_3 + 0xc);
  if ((*(uint *)(param_3 + 0x68) & 1) == 0) {
    if ((uVar2 & 0x200) == 0) {
      FUN_00817e90(param_3);
      uVar2 = *(ushort *)(param_3 + 0xc);
    }
  }
  if ((uVar2 & 0x2000) == 0) {
    *(ushort *)(param_3 + 0xc) = uVar2 | 0x2000;
    *(uint *)(param_3 + 0x68) = *(uint *)(param_3 + 0x68) | 0x2000;
  }
  uVar1 = FUN_00816ef8(param_1,param_2,param_3);
  if ((*(uint *)(param_3 + 0x68) & 1) == 0) {
    if ((*(ushort *)(param_3 + 0xc) & 0x200) == 0) {
      FUN_00817ee4(param_3);
      return uVar1;
    }
  }
  return uVar1;
}



undefined4 FUN_00817164(undefined4 *param_1,uint *param_2,int **param_3)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  code *pcVar4;
  int *piVar5;
  ushort uVar6;
  int iVar7;
  uint uVar8;
  int *piVar9;
  uint uVar10;
  uint uVar11;
  
  if (param_3[2] == (int *)0x0) {
    return 0;
  }
  uVar6 = *(ushort *)(param_2 + 3);
  if (((uVar6 & 8) == 0) || (param_2[4] == 0)) {
    iVar7 = FUN_008110fc(param_1,param_2);
    if (iVar7 != 0) {
      return 0xffffffff;
    }
    uVar6 = *(ushort *)(param_2 + 3);
    piVar9 = *param_3;
  }
  else {
    piVar9 = *param_3;
  }
  if ((uVar6 & 2) == 0) {
    if ((uVar6 & 1) != 0) {
LAB_008174fc:
      do {
        uVar3 = piVar9[1];
        piVar5 = piVar9 + 2;
        iVar7 = *piVar9;
        piVar9 = piVar5;
      } while (uVar3 == 0);
      do {
        iVar1 = FUN_008138ec(iVar7,10,uVar3);
        if (iVar1 == 0) {
          uVar10 = uVar3 + 1;
        }
        else {
          uVar10 = (iVar1 + 1) - iVar7;
        }
        do {
          uVar8 = uVar3;
          if (uVar10 < uVar3) {
            uVar8 = uVar10;
          }
          uVar2 = *param_2;
          if ((param_2[4] < uVar2) && (uVar11 = param_2[5] + param_2[2], (int)uVar11 < (int)uVar8))
          {
            FUN_00817714(uVar2,iVar7,uVar11);
            *param_2 = *param_2 + uVar11;
            iVar1 = FUN_00812d14(param_1,param_2);
            if (iVar1 == 0) goto LAB_008174cc;
LAB_00817618:
            uVar6 = *(ushort *)(param_2 + 3);
            goto LAB_00817624;
          }
          if ((int)uVar8 < (int)param_2[5]) {
            FUN_00817714(uVar2,iVar7,uVar8);
            param_2[2] = param_2[2] - uVar8;
            *param_2 = *param_2 + uVar8;
            uVar11 = uVar8;
          }
          else {
            uVar11 = (*(code *)param_2[9])(param_1,param_2[7],iVar7);
            if ((int)uVar11 < 1) goto LAB_00817618;
          }
LAB_008174cc:
          uVar10 = uVar10 - uVar11;
          iVar1 = 1;
          if ((uVar10 == 0) && (iVar1 = FUN_00812d14(param_1,param_2), iVar1 != 0))
          goto LAB_00817618;
          piVar5 = param_3[2];
          iVar7 = iVar7 + uVar11;
          uVar3 = uVar3 - uVar11;
          param_3[2] = (int *)((int)piVar5 - uVar11);
          if ((int *)((int)piVar5 - uVar11) == (int *)0x0) {
            return 0;
          }
          if (uVar3 == 0) goto LAB_008174fc;
        } while (iVar1 != 0);
      } while( true );
    }
    uVar10 = param_2[2];
    uVar3 = *param_2;
    iVar7 = 0;
    uVar8 = 0;
joined_r0x008172e8:
    while (uVar8 == 0) {
      iVar7 = *piVar9;
      uVar8 = piVar9[1];
      piVar9 = piVar9 + 2;
    }
    if ((uVar6 & 0x200) == 0) {
      if ((param_2[4] < uVar3) || (uVar2 = param_2[5], uVar8 < uVar2)) {
        if (uVar8 < uVar10) {
          uVar10 = uVar8;
        }
        FUN_00817714(uVar3,iVar7,uVar10);
        uVar3 = param_2[2];
        param_2[2] = uVar3 - uVar10;
        *param_2 = *param_2 + uVar10;
        if ((uVar3 - uVar10 == 0) && (iVar1 = FUN_00812d14(param_1,param_2), iVar1 != 0))
        goto LAB_008175dc;
      }
      else {
        uVar3 = uVar8;
        if (0x7ffffffe < uVar8) {
          uVar3 = 0x7fffffff;
        }
        uVar10 = (*(code *)param_2[9])(param_1,param_2[7],iVar7,((int)uVar3 / (int)uVar2) * uVar2);
        if ((int)uVar10 < 1) {
LAB_008175dc:
          uVar6 = *(ushort *)(param_2 + 3);
          goto LAB_00817624;
        }
      }
      uVar2 = uVar8 - uVar10;
    }
    else {
      uVar11 = uVar8;
      if ((uVar10 <= uVar8) && (uVar11 = uVar10, (uVar6 & 0x480) != 0)) {
        uVar10 = param_2[5] * 3;
        iVar1 = uVar3 - param_2[4];
        uVar10 = ((int)uVar10 >> 1) + (uint)((int)uVar10 < 0 && (uVar10 & 1) != 0);
        uVar3 = uVar8 + 1 + iVar1;
        if (uVar10 < uVar3) {
          uVar10 = uVar3;
        }
        if ((uVar6 & 0x400) == 0) {
          uVar2 = FUN_00806b64(param_1);
          if (uVar2 == 0) {
            FUN_00806b50(param_1,param_2[4]);
            uVar6 = *(ushort *)(param_2 + 3);
            *param_1 = 0xc;
            uVar6 = uVar6 & 0xff7f;
            goto LAB_00817624;
          }
        }
        else {
          uVar2 = FUN_00806b30(param_1,uVar10);
          if (uVar2 == 0) {
            uVar6 = *(ushort *)(param_2 + 3);
            *param_1 = 0xc;
            goto LAB_00817624;
          }
          FUN_0080c25c(uVar2,param_2[4],iVar1);
          *(ushort *)(param_2 + 3) = *(ushort *)(param_2 + 3) & 0xfb7f | 0x80;
        }
        uVar3 = uVar2 + iVar1;
        *param_2 = uVar3;
        param_2[2] = uVar10 - iVar1;
        param_2[5] = uVar10;
        param_2[4] = uVar2;
        uVar11 = uVar8;
      }
      FUN_00817714(uVar3,iVar7,uVar11);
      uVar2 = 0;
      param_2[2] = param_2[2] - uVar11;
      *param_2 = *param_2 + uVar11;
      uVar10 = uVar8;
    }
    piVar5 = param_3[2];
    iVar7 = iVar7 + uVar10;
    param_3[2] = (int *)((int)piVar5 - uVar10);
    if ((int *)((int)piVar5 - uVar10) != (int *)0x0) {
      uVar6 = *(ushort *)(param_2 + 3);
      uVar10 = param_2[2];
      uVar3 = *param_2;
      uVar8 = uVar2;
      goto joined_r0x008172e8;
    }
  }
  else {
    pcVar4 = (code *)param_2[9];
    uVar3 = param_2[7];
    iVar7 = 0;
    uVar10 = 0;
joined_r0x00817200:
    while (uVar10 == 0) {
      iVar7 = *piVar9;
      uVar10 = piVar9[1];
      piVar9 = piVar9 + 2;
    }
    uVar8 = uVar10;
    if (0x7ffffc00 < uVar10) {
      uVar8 = 0x7ffffc00;
    }
    iVar1 = (*pcVar4)(param_1,uVar3,iVar7,uVar8);
    if (iVar1 < 1) {
      uVar6 = *(ushort *)(param_2 + 3);
LAB_00817624:
      *(ushort *)(param_2 + 3) = uVar6 | 0x40;
      return 0xffffffff;
    }
    piVar5 = param_3[2];
    iVar7 = iVar7 + iVar1;
    uVar10 = uVar10 - iVar1;
    param_3[2] = (int *)((int)piVar5 - iVar1);
    if ((int *)((int)piVar5 - iVar1) != (int *)0x0) {
      pcVar4 = (code *)param_2[9];
      uVar3 = param_2[7];
      goto joined_r0x00817200;
    }
  }
  return 0;
}



void FUN_00817714(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  int iVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  undefined *puVar5;
  uint uVar6;
  
  if ((param_2 < param_1) && (puVar4 = (undefined4 *)((int)param_2 + param_3), param_1 < puVar4)) {
    puVar5 = (undefined *)((int)param_1 + param_3);
    if (param_3 == 0) {
      return;
    }
    if ((param_3 & 1) != 0) {
      puVar4 = (undefined4 *)((int)puVar4 + -1);
      puVar5 = puVar5 + -1;
      *puVar5 = *(undefined *)puVar4;
      if (param_3 == 1) {
        return;
      }
    }
    param_3 = param_3 >> 1;
    do {
      puVar5[-1] = *(undefined *)((int)puVar4 + -1);
      puVar5[-2] = *(undefined *)((int)puVar4 + -2);
      param_3 = param_3 - 1;
      puVar4 = (undefined4 *)((int)puVar4 + -2);
      puVar5 = puVar5 + -2;
    } while (param_3 != 0);
    return;
  }
  uVar6 = param_3;
  if (0xf < param_3) {
    if ((((uint)param_2 | (uint)param_1) & 3) != 0) {
      iVar1 = param_3 - 1;
      goto LAB_00817788;
    }
    iVar2 = (param_3 - 0x10 >> 4) + 1;
    puVar4 = param_1;
    puVar3 = param_2;
    iVar1 = iVar2;
    do {
      *puVar4 = *puVar3;
      puVar4[1] = puVar3[1];
      puVar4[2] = puVar3[2];
      puVar4[3] = puVar3[3];
      iVar1 = iVar1 + -1;
      puVar4 = puVar4 + 4;
      puVar3 = puVar3 + 4;
    } while (iVar1 != 0);
    param_2 = param_2 + iVar2 * 4;
    param_1 = param_1 + iVar2 * 4;
    uVar6 = param_3 & 0xf;
    if ((param_3 & 0xc) != 0) {
      uVar6 = (param_3 & 0xf) - 4;
      puVar4 = param_2 + -1;
      puVar3 = param_1 + -1;
      do {
        puVar4 = puVar4 + 1;
        puVar3 = puVar3 + 1;
        *puVar3 = *puVar4;
      } while (puVar4 != (undefined4 *)((int)param_2 + (uVar6 & 0xfffffffc)));
      iVar1 = (uVar6 >> 2) + 1;
      param_1 = param_1 + iVar1;
      param_2 = param_2 + iVar1;
      iVar1 = (param_3 & 3) - 1;
      if ((param_3 & 3) == 0) {
        return;
      }
      goto LAB_00817788;
    }
  }
  iVar1 = uVar6 - 1;
  if (uVar6 == 0) {
    return;
  }
LAB_00817788:
  puVar4 = (undefined4 *)((int)param_1 + -1);
  puVar3 = (undefined4 *)((int)param_2 + -1);
  if (((iVar1 + 1U & 1) != 0) &&
     (*(undefined *)param_1 = *(undefined *)param_2, puVar4 = param_1, puVar3 = param_2, iVar1 == 0)
     ) {
    return;
  }
  uVar6 = iVar1 + 1U >> 1;
  do {
    *(undefined *)((int)puVar4 + 1) = *(undefined *)((int)puVar3 + 1);
    *(undefined *)((int)puVar4 + 2) = *(undefined *)((int)puVar3 + 2);
    uVar6 = uVar6 - 1;
    puVar4 = (undefined4 *)((int)puVar4 + 2);
    puVar3 = (undefined4 *)((int)puVar3 + 2);
  } while (uVar6 != 0);
  return;
}



uint FUN_008178c0(int param_1,byte param_2,byte **param_3)

{
  int iVar1;
  ushort uVar2;
  byte *pbVar3;
  byte *pbVar4;
  
  if ((param_1 != 0) && (*(int *)(param_1 + 0x38) == 0)) {
    FUN_00812e28();
  }
  uVar2 = *(ushort *)(param_3 + 3);
  param_3[2] = param_3[6];
  if (((uVar2 & 8) == 0) || (pbVar4 = param_3[4], pbVar4 == (byte *)0x0)) {
    iVar1 = FUN_008110fc(param_1,param_3);
    if (iVar1 != 0) {
      return 0xffffffff;
    }
    uVar2 = *(ushort *)(param_3 + 3);
    pbVar4 = param_3[4];
    if ((uVar2 & 0x2000) != 0) goto LAB_00817930;
LAB_008179d4:
    *(ushort *)(param_3 + 3) = uVar2 | 0x2000;
    param_3[0x1a] = (byte *)((uint)param_3[0x1a] & 0xffffdfff);
    pbVar3 = *param_3;
    pbVar4 = pbVar3 + -(int)pbVar4;
    if ((int)param_3[5] <= (int)pbVar4) goto LAB_008179fc;
LAB_00817944:
    *param_3 = pbVar3 + 1;
    param_3[2] = param_3[2] + -1;
    *pbVar3 = param_2;
    if (param_3[5] == pbVar4 + 1) goto LAB_00817a3c;
LAB_0081796c:
    if (((*(ushort *)(param_3 + 3) & 1) == 0) || (param_2 != 10)) goto LAB_00817978;
    iVar1 = FUN_00812d14(param_1,param_3);
  }
  else {
    if ((uVar2 & 0x2000) == 0) goto LAB_008179d4;
LAB_00817930:
    pbVar3 = *param_3;
    pbVar4 = pbVar3 + -(int)pbVar4;
    if ((int)pbVar4 < (int)param_3[5]) goto LAB_00817944;
LAB_008179fc:
    iVar1 = FUN_00812d14(param_1,param_3);
    if (iVar1 != 0) {
      return 0xffffffff;
    }
    pbVar4 = *param_3;
    *param_3 = pbVar4 + 1;
    param_3[2] = param_3[2] + -1;
    *pbVar4 = param_2;
    if (param_3[5] != (byte *)0x1) goto LAB_0081796c;
LAB_00817a3c:
    iVar1 = FUN_00812d14(param_1,param_3);
  }
  if (iVar1 != 0) {
    return 0xffffffff;
  }
LAB_00817978:
  return (uint)param_2;
}



undefined4 FUN_00817b70(undefined4 *param_1,int param_2,undefined4 param_3)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  code *UNRECOVERED_JUMPTABLE;
  
  if (param_2 == -1) {
    return 0xffffffff;
  }
  piVar1 = (int *)FUN_00817fd0(param_2);
  if (piVar1 != (int *)0x0) {
    UNRECOVERED_JUMPTABLE = *(code **)((&PTR_PTR_s_stdnull_0081e348)[*piVar1] + 0x1c);
    if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
      iVar3 = piVar1[2];
      param_1[0x110] = *(undefined4 *)((&PTR_PTR_s_stdnull_0081e348)[*piVar1] + 0x58);
                    // WARNING: Could not recover jumptable at 0x00817bf0. Too many branches
                    // WARNING: Treating indirect jump as call
      uVar2 = (*UNRECOVERED_JUMPTABLE)(param_1,iVar3,param_3);
      return uVar2;
    }
    *param_1 = 0x58;
  }
  return 0xffffffff;
}



undefined4 FUN_00817c20(void)

{
  return 0;
}



undefined8
FUN_00817c28(undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  int *piVar1;
  code *UNRECOVERED_JUMPTABLE;
  undefined8 uVar2;
  
  if (param_2 == -1) {
    return 0xffffffffffffffff;
  }
  piVar1 = (int *)FUN_00817fd0(param_2);
  if (piVar1 != (int *)0x0) {
    UNRECOVERED_JUMPTABLE = *(code **)((&PTR_PTR_s_stdnull_0081e348)[*piVar1] + 0x18);
    if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
      param_1[0x110] = *(undefined4 *)((&PTR_PTR_s_stdnull_0081e348)[*piVar1] + 0x58);
                    // WARNING: Could not recover jumptable at 0x00817cc8. Too many branches
                    // WARNING: Treating indirect jump as call
      uVar2 = (*UNRECOVERED_JUMPTABLE)(param_1,piVar1[2],param_3,param_4,param_5);
      return uVar2;
    }
    *param_1 = 0x58;
  }
  return 0xffffffffffffffff;
}



undefined4 FUN_00817d08(undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  code *UNRECOVERED_JUMPTABLE;
  
  if (param_2 == -1) {
    return 0xffffffff;
  }
  piVar1 = (int *)FUN_00817fd0(param_2);
  if (piVar1 != (int *)0x0) {
    UNRECOVERED_JUMPTABLE = *(code **)((&PTR_PTR_s_stdnull_0081e348)[*piVar1] + 0x14);
    if (UNRECOVERED_JUMPTABLE != (code *)0x0) {
      iVar3 = piVar1[2];
      param_1[0x110] = *(undefined4 *)((&PTR_PTR_s_stdnull_0081e348)[*piVar1] + 0x58);
                    // WARNING: Could not recover jumptable at 0x00817d98. Too many branches
                    // WARNING: Treating indirect jump as call
      uVar2 = (*UNRECOVERED_JUMPTABLE)(param_1,iVar3,param_3,param_4);
      return uVar2;
    }
    *param_1 = 0x58;
  }
  return 0xffffffff;
}



void FUN_00817e90(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 + 0x5c;
  if (*(int *)(param_1 + 0x5c) != 0) {
    FUN_00818048(iVar1);
    return;
  }
  FUN_00817ff4(iVar1,1);
  FUN_00818048(iVar1);
  return;
}



// WARNING: Control flow encountered bad instruction data
// WARNING: Removing unreachable block (ram,0x00000000)

undefined4 FUN_00817ee4(void)

{
  return 0;
}



// WARNING: Control flow encountered bad instruction data
// WARNING: Removing unreachable block (ram,0x00000000)

uint FUN_00817eec(uint param_1)

{
  undefined *puVar1;
  
  if (param_1 < 0x400) {
    if (DAT_0081e408 == 0) {
      FUN_00817ff4(&DAT_0081e408,0);
      FUN_00818048(&DAT_0081e408);
      puVar1 = (&PTR_DAT_0081d348)[param_1];
    }
    else {
      FUN_00818048(&DAT_0081e408);
      puVar1 = (&PTR_DAT_0081d348)[param_1];
    }
    if ((((puVar1 != (undefined *)0x0) && (puVar1 != &DAT_0081d330)) && (puVar1 != &DAT_0081d33c))
       && (puVar1 != &DAT_0081d324)) {
      FUN_00806a60();
    }
    (&PTR_DAT_0081d348)[param_1] = (undefined *)0x0;
    return 0;
  }
  return param_1;
}



undefined * FUN_00817fd0(uint param_1)

{
  if (param_1 < 0x400) {
    return (&PTR_DAT_0081d348)[param_1];
  }
  return (undefined *)0x0;
}



// WARNING: Control flow encountered bad instruction data
// WARNING: Removing unreachable block (ram,0x00000000)

undefined4 FUN_00817ff4(void)

{
  return 0;
}



// WARNING: Control flow encountered bad instruction data
// WARNING: Removing unreachable block (ram,0x00000000)

undefined4 FUN_00818010(void)

{
  return 0;
}



// WARNING: Control flow encountered bad instruction data
// WARNING: Removing unreachable block (ram,0x00000000)

undefined4 FUN_00818048(void)

{
  return 0;
}


