/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08a590.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8a590(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int unaff_A6;

  *(undefined2 *)(unaff_A6 + 0x34) = 8;
  uVar1 = *(undefined4 *)(unaff_A6 + 0x28);
  uVar2 = *(undefined4 *)(unaff_A6 + 0x2c);
  uVar3 = *(undefined4 *)(unaff_A6 + 0x38);
  func_0x000143e4();
  if ((*(byte *)(unaff_A6 + 0x5f) & 2) == 0) {
    *(undefined4 *)(unaff_A6 + 0x38) = uVar3;
    *(undefined4 *)(unaff_A6 + 0x2c) = uVar2;
    *(undefined4 *)(unaff_A6 + 0x28) = uVar1;
    candidate_1abc8();
    return;
  }
  return;
}
