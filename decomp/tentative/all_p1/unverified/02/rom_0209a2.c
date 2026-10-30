/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0209a2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_209a2(void)

{
  undefined2 uVar1;
  short extraout_D1w;
  undefined4 *unaff_A6;
  bool bVar2;

  if (*(short *)((int)unaff_A6 + 0x72) == 0) {
    uVar1 = 0x709b;
  }
  else {
    uVar1 = 0x731b;
  }
  *(undefined2 *)(unaff_A6 + 0x1c) = uVar1;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  *unaff_A6 = 0x209c6;
  bVar2 = false;
  FUN_000c8cfc();
  if (bVar2) {
    candidate_3d25a();
  }
  else {
    candidate_c8d68();
    if (extraout_D1w == -1) {
      if ((*(ushort *)((int)unaff_A6 + 0x6e) & 0x10) == 0) {
        candidate_1a8aa();
      }
      candidate_3d25a();
    }
    else {
      candidate_3d25a();
    }
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 1;
  return;
}
