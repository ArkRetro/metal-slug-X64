/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x23df; tentative_c. */

void z80_candidate_23df(byte *param_1,short param_2)

{
  byte bVar1;

  bVar1 = *param_1;
  *(byte *)(param_2 + 6) = bVar1 << 7;
  *(byte *)(param_2 + 0xc) = bVar1 & 0xf0;
  *(byte *)(param_2 + 9) = param_1[1];
  *(byte *)(param_2 + 0xb) = param_1[3];
  z80_candidate_23a7();
  *(byte *)(param_2 + 10) = param_1[2];
  z80_candidate_23d1();
  *(undefined1 *)(param_2 + 3) = 0;
  *(undefined1 *)(param_2 + 4) = 0;
  *(undefined1 *)(param_2 + 5) = 0;
  return;
}
