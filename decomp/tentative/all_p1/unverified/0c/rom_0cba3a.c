/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cba3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cba3a(void)

{
  undefined4 *unaff_A6;

  candidate_cb878();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(undefined1 *)((int)unaff_A6 + 0x82) = 1;
    *unaff_A6 = *(undefined4 *)((int)unaff_A6 + 0x7e);
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_cbd5c;
  }
  return;
}
