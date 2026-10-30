/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c487a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_c487a(void)

{
  ushort in_D0w;
  ushort uVar1;
  int unaff_A5;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x93) == '\x03') {
    uVar1 = candidate_29c6();
    in_D0w = uVar1 & 7;
    if (in_D0w == 0) {
      if ((DAT_0010f001 & 4) != 0) {
        return in_D0w;
      }
      if (*(char *)(unaff_A5 + 0x4720 + (int)*(short *)(unaff_A6 + 0x70)) != '\0') {
        return in_D0w;
      }
    }
  }
  return in_D0w;
}
