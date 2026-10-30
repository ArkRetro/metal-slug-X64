/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05bec6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5bec6(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0x1f) + *(short *)(unaff_A6 + 0xb);
  unaff_A6[0x14] = 0x5c2f2;
  *(undefined2 *)(unaff_A6 + 0x1f) = 8;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + -8;
  *(undefined4 *)((int)unaff_A6 + 0x72) = 0xffff8000;
  candidate_4256();
  *unaff_A6 = 0x5bf00;
  candidate_5c050();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_5bf36;
  }
  candidate_5c276();
  *(short *)(unaff_A6 + 0x10) = *(short *)(unaff_A6 + 0x1f) + *(short *)(unaff_A6 + 0x10);
  candidate_427a();
  *(short *)(unaff_A6 + 0x10) = *(short *)(unaff_A6 + 0x10) - *(short *)(unaff_A6 + 0x1f);
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_5bfb8;
  }
  uVar2 = false;
  candidate_4c24();
  if ((bool)uVar2) {
    candidate_120f8();
    *unaff_A6 = candidate_5bfb8;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_4188;
  }
  return;
}
