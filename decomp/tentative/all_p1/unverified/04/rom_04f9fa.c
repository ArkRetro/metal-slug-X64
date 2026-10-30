/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04f9fa.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4f9fa(void)

{
  undefined4 uVar1;
  short sVar2;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;
  undefined8 uVar3;

  candidate_1af9a();
  if ((!(bool)in_CF) && (*(short *)(unaff_A6 + 0x72) != 0)) {
    *(short *)(unaff_A6 + 0x72) = *(short *)(unaff_A6 + 0x72) + -1;
    uVar1 = *(undefined4 *)(extraout_A0 + 0x40);
    *(short *)(extraout_A0 + 0x40) = *(short *)(extraout_A0 + 0x40) + 0x10;
    sVar2 = candidate_1b12e();
    *(undefined4 *)(extraout_A0 + 0x40) = uVar1;
    sVar2 = sVar2 - *(short *)(unaff_A6 + 0x84);
    if (sVar2 < 0) {
      if (-sVar2 < 0x80) {
        *(short *)(unaff_A6 + 0x84) = *(short *)(unaff_A6 + 0x84) + -5;
      }
      else {
        *(short *)(unaff_A6 + 0x84) = *(short *)(unaff_A6 + 0x84) + 5;
      }
    }
    else if (sVar2 != 0) {
      if (sVar2 < 0x80) {
        *(short *)(unaff_A6 + 0x84) = *(short *)(unaff_A6 + 0x84) + 5;
      }
      else {
        *(short *)(unaff_A6 + 0x84) = *(short *)(unaff_A6 + 0x84) + -5;
      }
    }
    *(ushort *)(unaff_A6 + 0x84) = *(ushort *)(unaff_A6 + 0x84) & 0xff;
  }
  candidate_260c8();
  uVar3 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0x30) = uVar3;
  return;
}
