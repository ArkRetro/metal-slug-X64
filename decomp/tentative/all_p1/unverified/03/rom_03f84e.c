/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03f84e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3f84e(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x86) = 0xfffc;
  *(undefined2 *)(unaff_A6 + 0x1d) = 6;
  uVar1 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x1d) = (uVar1 & 7) + *(short *)(unaff_A6 + 0x1d);
  candidate_4256();
  *unaff_A6 = 0x3f87e;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_3fa74();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0x160) {
    *unaff_A6 = candidate_3f7dc;
    return;
  }
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) == 0) {
    *unaff_A6 = candidate_3f7dc;
  }
  candidate_3fada();
  return;
}
