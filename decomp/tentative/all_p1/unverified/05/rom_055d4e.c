/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055d4e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined4 candidate_55d4e(void)

{
  undefined4 uVar1;
  ushort uVar2;
  ushort uVar3;
  int extraout_A0;
  int unaff_A6;
  undefined1 in_CF;

  candidate_55cde();
  if ((bool)in_CF) {
    uVar2 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
    uVar3 = uVar2;
    if ((short)uVar2 < 0) {
      uVar3 = -uVar2;
    }
    if (uVar3 <= *(ushort *)(unaff_A6 + 0x8c)) {
      if (*(char *)(unaff_A6 + 0x94) != '\0') {
        return 2;
      }
      if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
        uVar2 = -uVar2;
      }
      uVar1 = 1;
      if (-1 < (short)uVar2) {
        uVar1 = 2;
      }
      *(undefined1 *)(unaff_A6 + 0x95) = 0xff;
      return uVar1;
    }
  }
  *(undefined1 *)(unaff_A6 + 0x94) = 0;
  return 0;
}
