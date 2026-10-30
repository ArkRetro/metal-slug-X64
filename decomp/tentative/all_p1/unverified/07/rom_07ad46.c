/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ad46.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7ad46(void)

{
  short sVar1;
  ushort uVar2;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  *unaff_A6 = 0x7ad58;
  candidate_1abc8();
  sVar1 = *(short *)(unaff_A6 + 0x21) + -1;
  *(short *)(unaff_A6 + 0x21) = sVar1;
  if (sVar1 == 0) {
    uVar2 = candidate_29c6();
    *(ushort *)(unaff_A6 + 10) = ((uVar2 & 0x2f) - 0x1f) + *(short *)(unaff_A6 + 10);
    *unaff_A6 = candidate_1c706;
  }
  return;
}
