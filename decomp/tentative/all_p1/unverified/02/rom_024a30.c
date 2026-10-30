/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x024a30.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_24a30(void)

{
  ushort uVar1;
  uint uVar2;
  short sVar3;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x8f) == '\x7f') {
    *(undefined1 *)(unaff_A6 + 0x8a) = 2;
  }
  else if (*(short *)(unaff_A6 + 0x70) < *(short *)(unaff_A6 + 0x4e)) {
    if (*(short *)(unaff_A6 + 0x6e) < *(short *)(unaff_A6 + 0x4e)) {
      uVar2 = 0;
      sVar3 = 0;
      do {
        if ((*(byte *)(unaff_A6 + 0x8f) & '\x01' << (uVar2 & 7)) == 0) {
          sVar3 = sVar3 + 1;
        }
        uVar1 = (short)uVar2 + 1;
        uVar2 = (uint)uVar1;
      } while ((short)uVar1 < 8);
      if (sVar3 < 4) {
        *(undefined1 *)(unaff_A6 + 0x8a) = 1;
      }
    }
    else {
      *(undefined1 *)(unaff_A6 + 0x8a) = 1;
    }
  }
  else {
    *(undefined1 *)(unaff_A6 + 0x8a) = 2;
  }
  return;
}
