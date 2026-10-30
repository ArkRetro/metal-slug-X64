/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08661c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8661c(void)

{
  char cVar1;
  ushort unaff_D7w;
  int unaff_A6;

  if ((*(byte *)(unaff_A6 + 0x15) & 0x40) == 0) {
    cVar1 = '\x02';
  }
  else {
    if ((short)unaff_D7w < 0x1001) {
      cVar1 = '\x02';
    }
    else if (unaff_D7w < 0x2b01) {
      cVar1 = '\0';
    }
    else if (unaff_D7w < 0x3b01) {
      cVar1 = '\x01';
    }
    else if (unaff_D7w < 0x4501) {
      cVar1 = '\x02';
    }
    else if (unaff_D7w < 0x5501) {
      cVar1 = '\x03';
    }
    else if (unaff_D7w < 0x7001) {
      cVar1 = '\x04';
    }
    else {
      cVar1 = '\x02';
    }
    if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
      cVar1 = -(cVar1 + -4);
    }
  }
  *(char *)(unaff_A6 + 0x72) = cVar1;
  return;
}
