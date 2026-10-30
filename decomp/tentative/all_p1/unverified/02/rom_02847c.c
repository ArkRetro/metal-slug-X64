/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02847c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2847c(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  candidate_1436a();
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_2820a;
    in_CF = 0;
  }
  candidate_28da8();
  candidate_4c24();
  if ((bool)in_CF) {
    candidate_1b206();
    if ((bool)in_CF) {
      *unaff_A6 = candidate_2849a;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_284f2;
    bVar1 = false;
    candidate_1b206();
    if (bVar1) {
      *unaff_A6 = candidate_2858c;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  bVar1 = false;
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_28660;
  }
  return;
}
