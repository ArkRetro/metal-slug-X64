/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b8384.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_b8384(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_4256();
  *unaff_A6 = 0xb83a4;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_1affc();
  if (!(bool)uVar1) {
    *unaff_A6 = candidate_b83f0;
  }
  bVar2 = false;
  if ((*(ushort *)((int)unaff_A6 + 0x72) != 0) &&
     (bVar2 = *(ushort *)((int)unaff_A6 + 0x72) < *(ushort *)(unaff_A5 + 0x699e), bVar2)) {
    *unaff_A6 = candidate_41ac;
    bVar2 = false;
  }
  candidate_c8bec();
  if (bVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
