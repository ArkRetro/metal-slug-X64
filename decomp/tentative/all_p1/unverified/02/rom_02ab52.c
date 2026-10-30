/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02ab52.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2ab52(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  candidate_146a8();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_2ab94;
    in_CF = 0;
  }
  candidate_427a();
  candidate_2be6a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_2ab94;
  }
  *(short *)((int)unaff_A6 + 0x86) = *(short *)((int)unaff_A6 + 0x86) + -1;
  if (*(short *)((int)unaff_A6 + 0x86) < 1) {
    *unaff_A6 = candidate_2ab94;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_2ae46;
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2ae46;
  }
  bVar1 = false;
  candidate_1945c();
  if (bVar1) {
    *unaff_A6 = candidate_2b138;
  }
  return;
}
