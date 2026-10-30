/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x075644.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_75644(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_767b0();
  candidate_21288();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_75216;
    in_CF = 0;
  }
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = 0x758e4;
    in_CF = 0;
  }
  candidate_7685c();
  if ((bool)in_CF) {
    *unaff_A6 = 0x759c0;
    in_CF = 0;
  }
  candidate_76844();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_758ba;
    in_CF = 0;
  }
  candidate_1945c();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_75216;
  }
  return;
}
