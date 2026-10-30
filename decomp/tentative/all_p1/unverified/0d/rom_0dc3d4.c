/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dc3d4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dc3d4(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    candidate_40bc();
    candidate_1acf2();
    candidate_4256();
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_dc436;
    }
  }
  candidate_dc79c();
  return;
}
