/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d8606.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d8606(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_d9276();
  candidate_427a();
  if (((bool)in_CF) && (*(char *)((int)unaff_A6 + 0x8a) == '\0')) {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 0xff;
    candidate_d922c();
    *(undefined1 *)(unaff_A6 + 0x21) = 0x83;
    candidate_d9336();
    *unaff_A6 = candidate_d8564;
  }
  candidate_d92ec();
  return;
}
