/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03ee00.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3ee00(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x86) = 0xfff2;
  *(undefined2 *)(unaff_A6 + 0x1d) = 8;
  sVar1 = candidate_29f2();
  *(short *)(unaff_A6 + 0x1d) = sVar1 + *(short *)(unaff_A6 + 0x1d);
  candidate_4256();
  *unaff_A6 = 0x3ee30;
  candidate_1abc8();
  candidate_1ad8c();
  candidate_3fa74();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0x160) {
    *unaff_A6 = candidate_3ed8e;
    return;
  }
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) == 0) {
    *unaff_A6 = candidate_3ed8e;
  }
  candidate_3fada();
  return;
}
