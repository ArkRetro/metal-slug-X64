/* UNVERIFIED Z80 decompiler C, initial mapping, owner 0x2c52; tentative_c. */

void z80_candidate_2c52(short param_1)

{
  undefined1 uVar1;

  uVar1 = *(undefined1 *)CONCAT11(1,*(byte *)(param_1 + 8) | 0x80);
  *(undefined1 *)(param_1 + 9) = *(undefined1 *)CONCAT11(1,*(byte *)(param_1 + 8));
  *(undefined1 *)(param_1 + 10) = uVar1;
  return;
}
