/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x20b2; tentative_c. */

void z80_candidate_20b2(void)

{
  byte bVar1;
  char cVar2;
  byte *pbVar3;
  undefined1 *puVar4;

  DAT_ram_f815 = 0;
  func_0x4faf();
  uIo000c = 0;
  DAT_ram_fdeb = *(undefined1 *)(bRam248f + 0x4bab);
  DAT_ram_fded = 0;
  DAT_ram_fdea = 0;
  DAT_ram_fdf2 = 0x40;
  DAT_ram_fdf4 = 0;
  DAT_ram_fdef = 0;
  DAT_ram_fdf0 = 0;
  DAT_ram_fdf1 = 0;
  DAT_ram_fdee = 0;
  pbVar3 = (byte *)0x2490;
  DAT_ram_f805 = 0;
  DAT_ram_f808 = 0;
  puVar4 = (undefined1 *)0xfc00;
  cVar2 = '\x0e';
  DAT_ram_fdec = DAT_ram_fdeb;
  DAT_ram_fdf3 = DAT_ram_fdeb;
  do {
    bVar1 = pbVar3[2] | pbVar3[1] | *pbVar3;
    if (bVar1 == 0) break;
    z80_candidate_5245(bVar1);
    puVar4[9] = puVar4[9] | 2;
    puVar4[2] = pbVar3[4];
    puVar4 = puVar4 + 0x23;
    pbVar3 = pbVar3 + 5;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  for (; cVar2 != '\0'; cVar2 = cVar2 + -1) {
    puVar4[9] = 8;
    *puVar4 = 0;
    puVar4[1] = 0;
    puVar4 = puVar4 + 0x23;
  }
  return;
}
