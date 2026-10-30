/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02ad2e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2ad2e(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_120f8();
  *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) & 0x7f;
  if ((*(byte *)((int)unaff_A6 + 0x91) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x2ad72;
  candidate_146a8();
  uVar1 = *(uint *)((int)unaff_A6 + 0x62);
  bVar2 = CARRY4(*(uint *)((int)unaff_A6 + 0x72),uVar1);
  *(uint *)((int)unaff_A6 + 0x62) = *(uint *)((int)unaff_A6 + 0x72) + uVar1;
  candidate_427a();
  if (bVar2) {
    *unaff_A6 = candidate_2ad94;
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
  bVar2 = false;
  candidate_1945c();
  if (bVar2) {
    *unaff_A6 = candidate_2b138;
  }
  return;
}
