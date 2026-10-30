/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055820.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55820(void)

{
  int unaff_A6;
  undefined1 uVar1;

  *(short *)(unaff_A6 + 0x72) = *(short *)(unaff_A6 + 0x72) + 1;
  if (*(short *)(unaff_A6 + 0x72) < 0x321) {
    *(short *)(unaff_A6 + 0x80) = *(short *)(unaff_A6 + 0x80) + -1;
    uVar1 = 0;
    if (*(short *)(unaff_A6 + 0x80) < 1) {
      candidate_55574();
      candidate_5588c();
      if ((bool)uVar1) {
        *(undefined1 *)(unaff_A6 + 0x76) = 0x40;
      }
      else {
        uVar1 = candidate_366ac();
        *(undefined1 *)(unaff_A6 + 0x76) = uVar1;
      }
    }
  }
  if ((*(ushort *)(unaff_A6 + 0x72) & 7) == 0) {
    uVar1 = 6;
    if ((short)((ushort)(byte)(*(char *)(unaff_A6 + 0x76) - *(char *)(unaff_A6 + 0x74)) << 8) < 0) {
      uVar1 = 0xfa;
    }
    *(undefined1 *)(unaff_A6 + 0x75) = uVar1;
  }
  *(char *)(unaff_A6 + 0x74) = *(char *)(unaff_A6 + 0x75) + *(char *)(unaff_A6 + 0x74);
  candidate_558a2();
  return;
}
