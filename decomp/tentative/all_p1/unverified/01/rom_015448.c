/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x015448.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

byte candidate_15448(void)

{
  byte bVar1;
  int unaff_A5;

  bVar1 = 0;
  if (*(char *)(unaff_A5 + -0x7f78) == '\x02') {
    if (*(char *)(unaff_A5 + 0x7d82) == '\0') {
      bVar1 = *(byte *)(unaff_A5 + 0x4367);
    }
    else if (*(char *)(unaff_A5 + 0x42be) != '\0') {
      bVar1 = 0;
      if ((*(byte *)(unaff_A5 + 0x436b) & 0x80) != 0) {
        bVar1 = 2;
      }
      if ((*(byte *)(unaff_A5 + 0x4371) & 0x80) != 0) {
        bVar1 = bVar1 | 8;
      }
      bVar1 = (byte)candidate_4364[unaff_A5 + 2] & 10 & bVar1;
    }
    if (*(char *)(unaff_A5 + 0x7db6) == '\0') {
      bVar1 = bVar1 & 0xfd;
    }
    if (*(char *)(unaff_A5 + 0x7db6) == '\x03') {
      bVar1 = bVar1 & 0xfd;
    }
    if (*(char *)(unaff_A5 + 0x7db7) == '\0') {
      bVar1 = bVar1 & 0xf7;
    }
    if (*(char *)(unaff_A5 + 0x7db7) == '\x03') {
      bVar1 = bVar1 & 0xf7;
    }
  }
  else if (*(char *)(unaff_A5 + 0x42be) != '\0') {
    bVar1 = 0;
    if ((*(byte *)(unaff_A5 + 0x436b) & 0x80) != 0) {
      bVar1 = 2;
    }
    if ((*(byte *)(unaff_A5 + 0x4371) & 0x80) != 0) {
      bVar1 = bVar1 | 8;
    }
  }
  return bVar1;
}
