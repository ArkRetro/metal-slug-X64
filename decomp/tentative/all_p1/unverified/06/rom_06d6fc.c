/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06d6fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6d6fc(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 6);
    *(undefined4 *)((int)unaff_A6 + 0x62) = unaff_A6[0xc];
    in_CF = 0;
  }
  candidate_6d852();
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_6d7c2;
    in_CF = 0;
  }
  candidate_1945c();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_6d7c2;
  }
  return;
}
