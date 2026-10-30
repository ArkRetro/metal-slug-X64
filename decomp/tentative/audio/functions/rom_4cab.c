/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x4cab; tentative_c. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_ram_4cab(void)

{
  char cVar1;
  undefined1 *puVar2;

  _DAT_ram_fdf4 = 0;
  puVar2 = (undefined1 *)0xfc00;
  cVar1 = '\x0e';
  do {
    puVar2[9] = 0;
    *puVar2 = 0;
    puVar2[1] = 0;
    puVar2[9] = puVar2[9] | 8;
    puVar2 = puVar2 + 0x23;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}
