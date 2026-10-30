/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x3719; tentative_c. */

byte z80_candidate_3719(short param_1,short param_2)

{
  byte bVar1;
  undefined1 uVar2;

  uVar2 = *(undefined1 *)CONCAT11(1,*(byte *)(param_2 + 8) | 0x80);
  *(undefined1 *)(param_2 + 9) = *(undefined1 *)CONCAT11(1,*(byte *)(param_2 + 8));
  *(undefined1 *)(param_2 + 10) = uVar2;
  bVar1 = *(byte *)(param_2 + 0x38);
  if ((bVar1 & 0xc0) != 0) {
    *(byte *)(param_2 + 0x3b) = bVar1;
    return bVar1 & 0xc0;
  }
  bVar1 = (byte)(*(byte *)(param_1 + 0xf) << 7) >> 1 | ((*(byte *)(param_1 + 0xf) & 3) >> 1) << 7 |
          bVar1;
  *(byte *)(param_2 + 0x3b) = bVar1;
  return bVar1;
}
