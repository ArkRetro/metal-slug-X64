/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2085; tentative_c. */

void z80_candidate_2085(void)

{
  byte bVar1;
  char cVar2;
  short sVar3;
  byte *pbVar4;
  undefined1 *puVar5;

  func_0x4faf(DAT_ram_f815);
  DAT_ram_f815 = 0;
  sVar3 = 0x84;
  z80_candidate_57be();
  pbVar4 = (byte *)((ushort)DAT_ram_f812 * 4 + sVar3);
  bVar1 = *pbVar4 | pbVar4[1] | pbVar4[2];
  if (bVar1 == 0) {
    return;
  }
  z80_candidate_57be(bVar1);
  uIo000c = 0;
  DAT_ram_fdeb = *(undefined1 *)(*pbVar4 + 0x4bab);
  DAT_ram_fded = 0;
  DAT_ram_fdea = 0;
  DAT_ram_fdf2 = 0x40;
  DAT_ram_fdf4 = 0;
  DAT_ram_fdef = 0;
  DAT_ram_fdf0 = 0;
  DAT_ram_fdf1 = 0;
  DAT_ram_fdee = 0;
  pbVar4 = pbVar4 + 1;
  DAT_ram_f805 = 0;
  DAT_ram_f808 = 0;
  puVar5 = (undefined1 *)0xfc00;
  cVar2 = '\x0e';
  DAT_ram_fdec = DAT_ram_fdeb;
  DAT_ram_fdf3 = DAT_ram_fdeb;
  do {
    bVar1 = pbVar4[2] | pbVar4[1] | *pbVar4;
    if (bVar1 == 0) break;
    z80_candidate_5245(bVar1);
    puVar5[9] = puVar5[9] | 2;
    puVar5[2] = pbVar4[4];
    puVar5 = puVar5 + 0x23;
    pbVar4 = pbVar4 + 5;
    cVar2 = cVar2 + -1;
  } while (cVar2 != '\0');
  for (; cVar2 != '\0'; cVar2 = cVar2 + -1) {
    puVar5[9] = 8;
    *puVar5 = 0;
    puVar5[1] = 0;
    puVar5 = puVar5 + 0x23;
  }
  return;
}
