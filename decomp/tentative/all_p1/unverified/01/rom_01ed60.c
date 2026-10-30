/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01ed60.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1ed60(void)

{
  undefined4 uVar1;
  int unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  bVar2 = false;
  if (*(char *)(unaff_A6 + 0x84) == '\0') {
    candidate_1ee82();
    if (!bVar2) {
      *(undefined1 *)(unaff_A6 + 0x85) = 0;
      if ((*(byte *)(unaff_A6 + 0x15) & 1) == 0) {
        uVar1 = 0x100000;
      }
      else {
        uVar1 = 0xfff00000;
      }
      *(undefined4 *)(unaff_A6 + 0x30) = uVar1;
      *(undefined4 *)(unaff_A6 + 0x34) = 0;
      while( true ) {
        uVar3 = false;
        candidate_14906();
        if ((bool)uVar3) break;
        candidate_1ac38();
        candidate_427a();
        candidate_1ee82();
        if ((bool)uVar3) break;
        if (*(short *)(unaff_A6 + 0x3c) < 0x11) {
          return;
        }
        if (0x12f < *(short *)(unaff_A6 + 0x3c)) {
          return;
        }
      }
    }
  }
  else {
    candidate_49e8();
  }
  *(undefined1 *)(unaff_A6 + 0x85) = 1;
  return;
}
