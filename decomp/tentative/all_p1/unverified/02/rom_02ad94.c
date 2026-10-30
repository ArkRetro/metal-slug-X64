/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02ad94.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2ad94(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  candidate_260c8();
  sVar1 = candidate_2be52();
  *(int *)((int)unaff_A6 + 0x76) = (int)-sVar1 << 8;
  *(byte *)((int)unaff_A6 + 0x91) = *(byte *)((int)unaff_A6 + 0x91) | 0x80;
  if ((*(byte *)((int)unaff_A6 + 0x91) & 2) == 0) {
    candidate_4256();
  }
  else {
    candidate_4256();
  }
  *unaff_A6 = 0x2ae0a;
  candidate_146a8();
  candidate_2be08();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_2ae46;
  }
  candidate_427a();
  sVar1 = candidate_49e8();
  if (sVar1 != 0) {
    *unaff_A6 = candidate_2ae46;
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
