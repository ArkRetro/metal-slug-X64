/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02a71e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2a71e(void)

{
  short sVar1;
  int extraout_A0;
  int extraout_A0_00;
  int iVar2;
  int unaff_A6;
  bool bVar3;

  sVar1 = 0x10;
  do {
    bVar3 = false;
    candidate_40bc();
    iVar2 = extraout_A0;
    if (bVar3) {
      candidate_1b220();
      iVar2 = extraout_A0_00;
    }
    *(undefined1 *)(iVar2 + 0x83) = *(undefined1 *)(unaff_A6 + 0x83);
    sVar1 = sVar1 + -1;
  } while (sVar1 != 0);
  return;
}
