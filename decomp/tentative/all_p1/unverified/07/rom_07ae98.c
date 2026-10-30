/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07ae98.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7ae98(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_143dc();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = 0x7aae8;
    in_CF = 0;
  }
  candidate_7b9a4();
  if ((bool)in_CF) {
    *unaff_A6 = 0x7aae8;
  }
  return;
}
