/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055e28.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55e28(void)

{
  ushort uVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  bVar2 = false;
  if (*(char *)(unaff_A6 + 0x93) == '\0') {
    candidate_55cde();
    if (bVar2) {
      uVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
      if ((short)uVar1 < 0) {
        uVar1 = -uVar1;
      }
      if ((uVar1 <= *(ushort *)(unaff_A6 + 0x88)) && (*(ushort *)(unaff_A6 + 0x8c) < uVar1)) {
        return;
      }
    }
  }
  return;
}
