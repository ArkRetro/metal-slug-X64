/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cc6c6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cc6c6(void)

{
  short sVar1;
  short sVar2;
  undefined4 *unaff_A6;

  candidate_40bc();
  *unaff_A6 = 0xcc6dc;
  candidate_1abc8();
  sVar2 = (ushort)*(byte *)((int)unaff_A6 + 0x6e) << 1;
  sVar1 = *(short *)(sVar2 + 0x29fc);
  *(undefined2 *)(unaff_A6 + 10) = *(undefined2 *)(sVar2 + 0x2a7c);
  *(short *)(unaff_A6 + 0xb) = sVar1 + 0x100;
  if (*(short *)(unaff_A6 + 0xf) < 0x141) {
    candidate_427a();
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -0x88;
  if (*(char *)((int)unaff_A6 + 0x6e) == -0x40) {
    *(undefined1 *)(unaff_A6[3] + 0x61) = 1;
  }
  if (*(byte *)((int)unaff_A6 + 0x6e) < 0x81) {
    *unaff_A6 = candidate_cc762;
  }
  if (*(short *)(unaff_A6 + 0xf) < -0x1f) {
    *unaff_A6 = candidate_cc762;
  }
  return;
}
