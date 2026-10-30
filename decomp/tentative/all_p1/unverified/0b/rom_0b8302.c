/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b8302.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b8302(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_1abc8();
  candidate_427a();
  bVar1 = false;
  if ((*(ushort *)(unaff_A6 + 0x1c) != 0) &&
     (bVar1 = *(ushort *)(unaff_A6 + 0x1c) < *(ushort *)(unaff_A5 + 0x699e), bVar1)) {
    *unaff_A6 = candidate_41ac;
    bVar1 = false;
  }
  candidate_c8bec();
  if (bVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
