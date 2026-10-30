/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07f0b6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7f0b6(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  unaff_A6[0xd] = 0;
  func_0x000143e4();
  candidate_427a();
  sVar1 = *(short *)(unaff_A6 + 0xf) - *(short *)(unaff_A6 + 0x1f);
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    sVar1 = -sVar1;
  }
  if (-1 < sVar1) {
    *unaff_A6 = candidate_7f0ec;
  }
  bVar2 = false;
  candidate_4c24();
  if ((bVar2) &&
     (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
    *unaff_A6 = candidate_7f196;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_7f26c;
  }
  candidate_7f7a0();
  return;
}
