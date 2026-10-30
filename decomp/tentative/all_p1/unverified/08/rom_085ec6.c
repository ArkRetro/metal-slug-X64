/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x085ec6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_85ec6(void)

{
  bool bVar1;
  undefined1 uVar2;
  short extraout_D1w;
  ushort uVar3;
  short sVar4;
  int unaff_A6;

  uVar3 = 0x80ff;
  sVar4 = *(short *)(*(int *)(unaff_A6 + 0x6e) + 0x3c);
  if (0xf0 < sVar4) {
    uVar3 = 0x8007;
  }
  if (sVar4 < 0x50) {
    uVar3 = uVar3 & 0x7f0;
  }
  if (0x110 < *(short *)(unaff_A6 + 0x3c)) {
    uVar3 = uVar3 & 0xf007;
  }
  if (*(short *)(unaff_A6 + 0x3c) < 0x30) {
    uVar3 = uVar3 & 0x7f0;
  }
  if (0x1d0 < *(short *)(unaff_A6 + 0x40)) {
    uVar3 = uVar3 & 0xc1c1;
  }
  if (*(short *)(unaff_A6 + 0x40) < 0x130) {
    uVar3 = uVar3 & 0xc1c1;
  }
  if (*(char *)(unaff_A6 + 0x87) != '\0') {
    uVar3 = uVar3 & 0xe3e3;
  }
  if (uVar3 == 0) {
    uVar3 = 0x80ff;
  }
  candidate_2eb4();
  sVar4 = 0;
  while (uVar3 != 0) {
    bVar1 = (short)uVar3 < 0;
    uVar3 = uVar3 << 1;
    if ((bVar1) && (uVar2 = candidate_85f84(), sVar4 <= extraout_D1w)) {
      *(undefined1 *)(unaff_A6 + 0x88) = uVar2;
      sVar4 = extraout_D1w;
    }
  }
  *(char *)(unaff_A6 + 0x88) = *(char *)(unaff_A6 + 0x88) << 4;
  return;
}
