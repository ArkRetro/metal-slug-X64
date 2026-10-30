/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08f5de.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8f5de(void)

{
  short sVar1;
  int unaff_A6;

  if (((*(byte *)(unaff_A6 + 0x93) & 4) != 0) && (*(int *)(unaff_A6 + 0x7a) != 0)) {
    sVar1 = *(short *)(*(int *)(unaff_A6 + 0x7a) + 0x3c);
    if ((*(byte *)(unaff_A6 + 0x93) & 0x80) == 0) {
      if ((short)(sVar1 + 8) < *(short *)(unaff_A6 + 0x3c)) {
        return;
      }
      *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) ^ 0x80;
    }
    else {
      if (*(short *)(unaff_A6 + 0x3c) < (short)(sVar1 + -8)) {
        return;
      }
      *(byte *)(unaff_A6 + 0x93) = *(byte *)(unaff_A6 + 0x93) ^ 0x80;
    }
    return;
  }
  return;
}
