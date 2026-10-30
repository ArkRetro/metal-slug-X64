/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04eb36.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4eb36(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  unaff_A6[0x14] = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0;
  *unaff_A6 = 0x4eb4e;
  if (*(short *)((int)unaff_A6 + 0x72) == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x72) = 4;
    bVar1 = false;
    candidate_40bc();
    if (bVar1) {
      candidate_29c6();
      candidate_29c6();
      candidate_1ad6e();
    }
  }
  else {
    *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
  }
  candidate_4fade();
  candidate_1ac38();
  candidate_1ad8c();
  candidate_427a();
  if (0xaf < *(short *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_4ebce;
  }
  candidate_4f7e8();
  return;
}
