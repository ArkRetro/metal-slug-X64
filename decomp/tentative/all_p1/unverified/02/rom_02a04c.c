/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02a04c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2a04c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4e76();
  candidate_19dfc();
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x2a078;
  uVar1 = 0;
  candidate_2a472();
  candidate_427a();
  if (((*(byte *)((int)unaff_A6 + 0x86) & 0x80) != 0) &&
     ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
    *unaff_A6 = candidate_2a0b2;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
