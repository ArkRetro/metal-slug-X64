/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x3fe3; tentative_c. */

byte z80_candidate_3fe3(short param_1,short param_2)

{
  byte bVar1;
  short sVar2;
  undefined1 *puVar3;

  if (DAT_ram_ff13 != *(char *)(param_2 + 0x11)) {
    DAT_ram_ff13 = *(char *)(param_2 + 0x11);
  }
  sVar2 = *(short *)(param_2 + 0x12);
  bVar1 = *(byte *)(sVar2 + 5 + (ushort)(byte)(*(char *)(param_2 + 8) - *(char *)(sVar2 + 4)));
  z80_candidate_57be();
  puVar3 = (undefined1 *)(sVar2 + (ushort)bVar1 * 4 + (ushort)bVar1);
  *(undefined1 *)(param_2 + 0x15) = *puVar3;
  *(undefined1 *)(param_2 + 0x16) = puVar3[1];
  *(undefined1 *)(param_2 + 0x17) = puVar3[2];
  *(undefined1 *)(param_2 + 0x18) = puVar3[3];
  *(undefined1 *)(param_2 + 0x19) = puVar3[4];
  bVar1 = *(byte *)(param_2 + 0x15) & 3;
  if (bVar1 == 0) {
    bVar1 = *(byte *)(param_1 + 0xf);
    *(byte *)(param_2 + 0x15) = bVar1;
  }
  return bVar1;
}
