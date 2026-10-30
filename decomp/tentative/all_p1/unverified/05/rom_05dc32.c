/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05dc32.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5dc32(void)

{
  undefined4 uVar1;
  int extraout_A0;
  int extraout_A0_00;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  bVar3 = false;
  candidate_40bc();
  iVar2 = extraout_A0;
  if (bVar3) {
    candidate_1b220();
    iVar2 = extraout_A0_00;
  }
  uVar1 = 0x8000;
  if (-1 < *(char *)((int)unaff_A6 + 0x5b)) {
    uVar1 = 0xffff8000;
  }
  *(undefined4 *)(iVar2 + 0x30) = uVar1;
  *unaff_A6 = 0x5dc70;
  candidate_406b6();
                    /* WARNING: Could not recover jumptable at 0x0005dc78. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)*unaff_A6)();
  return;
}
