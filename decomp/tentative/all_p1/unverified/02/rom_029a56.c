/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029a56.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29a56(void)

{
  int unaff_A6;
  char in_NF;
  undefined1 in_ZF;
  char in_VF;

  candidate_29a3a();
  if ((bool)in_ZF || in_VF != in_NF) {
    if ((*(byte *)(unaff_A6 + 0x84) & 0x80) == 0) {
      return;
    }
  }
  else if ((*(byte *)(unaff_A6 + 0x84) & 0x80) != 0) {
    return;
  }
  return;
}
