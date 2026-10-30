/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02802c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2802c(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  func_0x000143e4();
  candidate_1b284();
  if (!(bool)in_CF) {
    *unaff_A6 = candidate_28052;
  }
  candidate_427a();
  candidate_4c24();
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
