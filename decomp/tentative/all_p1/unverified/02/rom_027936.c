/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x027936.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_27936(void)

{
  short sVar1;
  ushort uVar2;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x8f) & 0x40) == 0) {
    if (';' < *(char *)(unaff_A6 + 0x8d)) {
      *(undefined1 *)(unaff_A6 + 0x8d) = 0x3c;
      goto LAB_000279bc;
    }
    if ((*(byte *)(unaff_A6 + 0x8f) & 0x80) == 0) {
      uVar2 = candidate_260c8();
    }
    else {
      uVar2 = candidate_260c8();
    }
    *(byte *)(unaff_A6 + 0x8f) = *(byte *)(unaff_A6 + 0x8f) | 0x20;
    uVar2 = uVar2 & 3;
    if (uVar2 == 3) {
      candidate_26230();
      *(byte *)(unaff_A6 + 0x8f) = *(byte *)(unaff_A6 + 0x8f) | 0x40;
      uVar2 = 0;
    }
  }
  else {
    sVar1 = candidate_2624c();
    if (sVar1 < 7) {
LAB_000279bc:
      if ((*(byte *)(unaff_A6 + 0x8f) & 0x80) == 0) {
        uVar2 = candidate_260c8();
      }
      else {
        uVar2 = candidate_260c8();
      }
      if (uVar2 == 2) {
        *(byte *)(unaff_A6 + 0x8f) = *(byte *)(unaff_A6 + 0x8f) & 0xdf;
      }
      return uVar2;
    }
    *(byte *)(unaff_A6 + 0x8f) = *(byte *)(unaff_A6 + 0x8f) & 0xbf;
    uVar2 = candidate_26244();
  }
  return uVar2;
}
