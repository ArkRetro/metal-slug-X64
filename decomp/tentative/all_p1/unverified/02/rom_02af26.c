/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02af26.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2af26(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_146a8();
  candidate_2be08();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_2af90;
  }
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    if (*(short *)(unaff_A6 + 0xf) <= *(short *)((int)unaff_A6 + 0x7e)) {
      *unaff_A6 = candidate_2af90;
    }
  }
  else if (*(short *)((int)unaff_A6 + 0x7e) <= *(short *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_2af90;
  }
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_2b0d4;
  }
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2ae46;
  }
  bVar2 = false;
  candidate_1945c();
  if (bVar2) {
    *unaff_A6 = candidate_2b138;
  }
  return;
}
