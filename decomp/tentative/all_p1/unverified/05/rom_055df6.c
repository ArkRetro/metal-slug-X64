/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055df6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55df6(void)

{
  ushort uVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;

  bVar2 = false;
  if (*(char *)(unaff_A6 + 0x95) != '\0') {
    candidate_55cde();
    if (bVar2) {
      uVar1 = *(short *)(extraout_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
      if ((short)uVar1 < 0) {
        uVar1 = -uVar1;
      }
      if (*(ushort *)(unaff_A6 + 0x8e) <= uVar1) {
        return;
      }
    }
  }
  return;
}
