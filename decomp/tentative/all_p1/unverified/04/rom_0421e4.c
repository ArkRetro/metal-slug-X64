/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0421e4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_421e4(void)

{
  short sVar1;
  int unaff_A5;
  int unaff_A6;

  if (*(ushort *)(unaff_A5 + -0x7ff2) % 0xc == 0) {
    *(char *)(unaff_A6 + 0xa2) = *(char *)(unaff_A6 + 0xa2) + '\x01';
  }
  sVar1 = *(short *)((short)(((ushort)*(byte *)(unaff_A6 + 0xa2) % 3) * 2) + 0x4222e);
  if ((*(byte *)(unaff_A6 + 0x17) != 0) && ((*(byte *)(unaff_A6 + 0x17) & 1) == 0)) {
    sVar1 = 0xda;
  }
  if (sRam00106c72 == 0) {
    return;
  }
  if (sVar1 == sRam00106c76) {
    if ((cRam00106c78 != ' ') && (cRam00106c78 = ' ', sVar1 != sRam00106c74)) {
      bRam00106c79 = bRam00106c79 | 0x80;
    }
  }
  else {
    cRam00106c78 = ' ';
    bRam00106c79 = bRam00106c79 | 0x80;
    sRam00106c76 = sVar1;
  }
  return;
}
