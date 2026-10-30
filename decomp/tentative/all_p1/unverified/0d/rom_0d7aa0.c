/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7aa0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7aa0(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_1abc8();
  if (*(short *)(*(int *)((int)unaff_A6 + 0x9e) + 0x3c) <
      (short)(*(short *)(unaff_A6 + 0xf) + -0x20)) {
    candidate_d90d2();
    if (*(char *)((int)unaff_A6 + 0x81) == '\x01') {
      *unaff_A6 = candidate_d7ade;
    }
  }
  else {
    *(short *)(unaff_A6 + 10) = *(short *)(unaff_A6 + 10) + 1;
    candidate_d90d2();
  }
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    if (((*(short *)(unaff_A6 + 0xf) < 0x90) ||
        (bVar1 = *(ushort *)(unaff_A6 + 0xf) < 0x120, 0x120 < (short)*(ushort *)(unaff_A6 + 0xf)))
       || (candidate_1af9a(), bVar1)) {
      *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    }
    else {
      *unaff_A6 = candidate_d7d7c;
    }
  }
  return;
}
