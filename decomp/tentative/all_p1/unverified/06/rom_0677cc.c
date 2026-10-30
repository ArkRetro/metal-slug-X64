/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0677cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_677cc(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar1;

  *unaff_A6 = 0x677d2;
  candidate_1abc8();
  candidate_427a();
  bVar1 = false;
  if (*(ushort *)(unaff_A6 + 0x1c) == 0) {
    candidate_c8bec();
    if (bVar1) {
      *unaff_A6 = candidate_6780e;
    }
  }
  else if (*(ushort *)(unaff_A6 + 0x1c) <= *(ushort *)(unaff_A5 + 0x699e)) {
    *unaff_A6 = candidate_6780e;
  }
  return;
}
