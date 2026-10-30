/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0968c0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_968c0(void)

{
  ushort uVar1;
  int extraout_A0;
  int unaff_A6;

  if (*(char *)(unaff_A6 + 0x93) != '\x01') {
    return;
  }
  if (*(char *)(unaff_A6 + 0x91) != '\0') {
    *(char *)(unaff_A6 + 0x91) = *(char *)(unaff_A6 + 0x91) + -1;
    *(undefined4 *)(unaff_A6 + 0x34) = 0;
  }
  if (*(char *)(unaff_A6 + 0x93) == '\x01') {
    candidate_ca2ce();
    uVar1 = *(ushort *)(extraout_A0 + 2);
  }
  else {
    uVar1 = (ushort)*(byte *)(unaff_A6 + 0x85);
  }
  if (((uVar1 & 0x20) != 0) && (*(int *)(unaff_A6 + 0x34) < 1)) {
    *(undefined1 *)(unaff_A6 + 0x91) = 6;
  }
  return;
}
