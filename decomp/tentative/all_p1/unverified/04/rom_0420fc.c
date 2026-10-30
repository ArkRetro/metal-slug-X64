/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0420fc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_420fc(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 extraout_A1;
  int unaff_A6;
  undefined8 uVar3;

  candidate_120f8();
  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x26) = 0x40;
  candidate_4491c();
  uVar1 = *(undefined4 *)(unaff_A6 + 0x30);
  uVar2 = *(undefined4 *)(unaff_A6 + 0x34);
  uVar3 = candidate_1ac4c();
  *(undefined8 *)(unaff_A6 + 0x28) = uVar3;
  *(undefined4 *)(unaff_A6 + 0x30) = uVar1;
  *(undefined4 *)(unaff_A6 + 0x34) = uVar2;
  *(undefined4 *)(unaff_A6 + 0x38) = extraout_A1;
  candidate_446d4();
  *(undefined1 *)(unaff_A6 + 0xa3) = 0;
  *(undefined4 *)(unaff_A6 + 0x30) = 0;
  *(undefined4 *)(unaff_A6 + 0x34) = 0;
  return;
}
