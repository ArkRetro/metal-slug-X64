/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d502.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d502(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  unaff_A6[0xc] = 0;
  unaff_A6[0xd] = 0;
  unaff_A6[0x14] = 0x5e918;
  candidate_4256();
  *unaff_A6 = 0x5d528;
  uVar2 = 0;
  candidate_5e234();
  candidate_427a();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_5d732;
  }
  else {
    candidate_5e244();
    if ((bool)uVar2) {
      *unaff_A6 = candidate_5d27a;
    }
  }
  candidate_5e6d2();
  sVar1 = *(short *)(unaff_A6 + 0x1f) + -1;
  *(short *)(unaff_A6 + 0x1f) = sVar1;
  if (sVar1 < 0) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  else {
    *unaff_A6 = candidate_5db92;
  }
  bVar3 = false;
  candidate_1945c();
  if (bVar3) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
