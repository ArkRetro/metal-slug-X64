/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x077126.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_77126(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    in_CF = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_13f1e();
    *unaff_A6 = candidate_7716e;
    in_CF = 0;
  }
  candidate_1945c();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
