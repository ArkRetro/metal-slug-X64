/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05fb26.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5fb26(void)

{
  short sVar1;
  ushort uVar2;
  short sVar3;
  int unaff_A6;

  sVar3 = *(short *)(unaff_A6 + 0x78) - *(short *)(unaff_A6 + 0x76);
  if (sVar3 != 0) {
    sVar3 = candidate_29f2();
  }
  *(short *)(unaff_A6 + 0x7a) = sVar3;
  sVar3 = *(short *)(unaff_A6 + 0x76) + sVar3;
  sVar1 = *(short *)(unaff_A6 + 0x3c);
  if (sVar3 < sVar1) {
    if (((short)(sVar1 - sVar3) < 0x10) &&
       (uVar2 = *(ushort *)(unaff_A6 + 0x7a), *(ushort *)(unaff_A6 + 0x7a) = uVar2 - 0x10,
       uVar2 < 0x10)) {
      *(undefined2 *)(unaff_A6 + 0x7a) = 0;
    }
  }
  else if (((short)(sVar3 - sVar1) < 0x10) &&
          (*(short *)(unaff_A6 + 0x7a) = *(short *)(unaff_A6 + 0x7a) + 0x10,
          *(short *)(unaff_A6 + 0x78) <
          (short)(*(short *)(unaff_A6 + 0x76) + *(short *)(unaff_A6 + 0x7a)))) {
    *(short *)(unaff_A6 + 0x7a) = *(short *)(unaff_A6 + 0x76) - *(short *)(unaff_A6 + 0x78);
  }
  return;
}
