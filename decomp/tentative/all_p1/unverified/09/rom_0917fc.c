/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0917fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_917fc(void)

{
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  int unaff_A5;
  int unaff_A6;

  if ((((*(char *)(unaff_A6 + 0x92) == '\x01') || (*(short *)(FUN_000042f4 + unaff_A5 + 2) != 5)) &&
      (*(short *)(unaff_A6 + 0x80) = *(short *)(unaff_A6 + 0x80) + -1,
      *(short *)(unaff_A6 + 0x80) < 1)) &&
     ((*(undefined2 *)(unaff_A6 + 0x80) = 0, 0x1f < *(short *)(unaff_A6 + 0x3c) &&
      (*(short *)(unaff_A6 + 0x3c) < 0x121)))) {
    candidate_1af9a();
    uVar2 = *(short *)(extraout_A0 + 0x40) - *(short *)(unaff_A6 + 0x40);
    if ((short)uVar2 < 0) {
      uVar2 = -uVar2;
    }
    if (0x80 < (short)uVar2) {
      uVar2 = 0x80;
    }
    uVar2 = (ushort)*(byte *)(*(int *)((short)((*(byte *)(unaff_A6 + 0x93) & 3) << 2) + 0x91e5c) +
                             (int)(short)(uVar2 >> 5));
    sVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    if ((SBORROW2(sVar1,-uVar2) == (short)(sVar1 + uVar2) < 0) && (sVar1 <= (short)uVar2)) {
      return;
    }
  }
  return;
}
