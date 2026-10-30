/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03c77e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3c77e(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_3c6ea();
  candidate_427a();
  if ((bool)in_CF) {
    if (*(char *)(unaff_A6 + 0x1c) == '\0') {
      *unaff_A6 = 0x3c656;
    }
    else {
      *unaff_A6 = candidate_3c7e6;
    }
  }
  *(byte *)((int)unaff_A6 + 0x15) =
       *(byte *)(unaff_A6[3] + 0x15) & 1 | *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_12022();
  return;
}
