/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x070eae.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

uint candidate_70eae(void)

{
  short sVar3;
  uint uVar1;
  uint uVar2;
  undefined4 uVar4;
  int extraout_A0;
  int extraout_A0_00;
  int unaff_A6;

  sVar3 = *(short *)(unaff_A6 + 0x30);
  if (sVar3 < 0) {
    sVar3 = sVar3 + 1;
  }
  if ((sVar3 != 0) && ((*(ushort *)(unaff_A6 + 0x82) & 7) == 0)) {
    candidate_411c();
    uVar4 = *(undefined4 *)(unaff_A6 + 0x34);
    *(undefined4 *)(extraout_A0 + 0x30) = *(undefined4 *)(unaff_A6 + 0x30);
    *(undefined4 *)(extraout_A0 + 0x34) = uVar4;
    candidate_1acf2();
  }
  uVar1 = candidate_29c6();
  uVar2 = uVar1 & 0xffffff7f;
  if ((uVar1 & 0x7f) == 0) {
    candidate_411c();
    uVar4 = *(undefined4 *)(unaff_A6 + 0x34);
    *(undefined4 *)(extraout_A0_00 + 0x30) = *(undefined4 *)(unaff_A6 + 0x30);
    *(undefined4 *)(extraout_A0_00 + 0x34) = uVar4;
    uVar2 = candidate_1acf2();
  }
  return uVar2;
}
