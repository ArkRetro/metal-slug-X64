/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0975c6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_975c6(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  bool bVar2;
  undefined1 uVar3;

  unaff_A6[0xc] = 0;
  if (-1 < (int)unaff_A6[0xd]) {
    unaff_A6[0xd] = 0;
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)(unaff_A6 + 0x21) = 0;
  candidate_966f0();
  *unaff_A6 = 0x975ea;
  bVar2 = false;
  candidate_aa760();
  if (!bVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x92) = 1;
    candidate_966f0();
    *unaff_A6 = 0x9760e;
  }
  uVar1 = unaff_A6[0xd];
  uVar3 = 0x4fff < uVar1;
  unaff_A6[0xd] = uVar1 - 0x5000;
  candidate_1ac38();
  candidate_427a();
  candidate_1945c();
  if ((bool)uVar3) {
    *unaff_A6 = candidate_9756a;
  }
  return;
}
