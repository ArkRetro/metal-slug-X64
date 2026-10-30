/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a15a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a15a(void)

{
  short sVar1;
  int in_A0;
  int unaff_A6;

  sVar1 = *(short *)(in_A0 + 0x3c) - *(short *)(unaff_A6 + 0x3c);
  if (sVar1 < 0) {
    if ((*(char *)(unaff_A6 + 0x96) < '\x05') && (sVar1 == -0x20 || -sVar1 < 0x20)) {
      *(undefined2 *)(unaff_A6 + 0x80) = 0xc0;
      return;
    }
  }
  else if (('\x03' < *(char *)(unaff_A6 + 0x96)) && (sVar1 < 0x21)) {
    *(undefined2 *)(unaff_A6 + 0x80) = 0xc0;
    return;
  }
  return;
}
