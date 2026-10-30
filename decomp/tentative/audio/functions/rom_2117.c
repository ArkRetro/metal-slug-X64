/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2117; tentative_c. */

void thunk_FUN_ram_504c(void)

{
  char cVar1;
  undefined1 *puVar2;

  puVar2 = (undefined1 *)0xfdf6;
  cVar1 = '\b';
  do {
    puVar2[9] = 0;
    puVar2[9] = puVar2[9] | 8;
    puVar2[2] = 0xff;
    puVar2[3] = 0;
    *puVar2 = 0;
    puVar2[1] = 0;
    puVar2 = puVar2 + 0x23;
    cVar1 = cVar1 + -1;
  } while (cVar1 != '\0');
  return;
}
