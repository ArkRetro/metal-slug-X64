/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6e88.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a6e88(void)

{
  short sVar1;
  int extraout_A0;
  int unaff_A6;
  bool bVar2;
  undefined8 uVar3;

  uVar3 = candidate_1ac4c();
  for (sVar1 = 0; sVar1 < 4; sVar1 = sVar1 + 1) {
    bVar2 = false;
    candidate_40bc();
    if (bVar2) {
      *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)(unaff_A6 + 0x6e);
    }
    *(undefined8 *)(extraout_A0 + 0x28) = uVar3;
  }
  return;
}
