/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05da68.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5da68(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  if ('\b' < *(char *)(unaff_A6 + 0x22)) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  candidate_1b1dc();
  *(undefined1 *)((int)unaff_A6 + 0x86) = 3;
  candidate_4256();
  *unaff_A6 = 0x5da9a;
  uVar1 = 0;
  func_0x000143e4();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_5c5b4;
  }
  candidate_5e6d2();
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x86) = 2;
    *unaff_A6 = candidate_5dc32;
    return;
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if (*(short *)(unaff_A6 + 0xf) < -0x1f) {
    *unaff_A6 = candidate_5dc7a;
  }
  if (0x15f < *(short *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_5dc7a;
  }
  if (*(short *)(unaff_A6 + 0x10) < 0xd1) {
    *unaff_A6 = candidate_5dc7a;
  }
  bVar2 = false;
  candidate_1945c();
  if (bVar2) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
