/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04ec0c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4ec0c(void)

{
  short sVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar2;

  sVar1 = 5;
  do {
    candidate_40bc();
    *(short *)(extraout_A0 + 0x88) = sVar1;
    sVar1 = sVar1 + -1;
  } while (sVar1 != -1);
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1ad6e();
  }
  candidate_49e8();
  *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) | 2;
  *(undefined1 *)((int)unaff_A6 + 0x8f) = 0xff;
  candidate_120f8();
  *unaff_A6 = 0x4ec78;
  unaff_A6[0xc] = unaff_A6[0xc] + -0x10000;
  candidate_1ac38();
  candidate_4f88e();
  return;
}
