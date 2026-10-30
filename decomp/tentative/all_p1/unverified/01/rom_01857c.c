/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01857c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1857c(void)

{
  short sVar1;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  undefined1 in_CF;
  bool bVar3;

  func_0x000040ac();
  if ((bool)in_CF) {
    *(undefined2 *)(unaff_A5 + 0x4310) = *(undefined2 *)(extraout_A0 + 0x3c);
    sVar1 = *(short *)(extraout_A0 + 0x40);
    *(short *)(unaff_A5 + 0x4312) = sVar1;
    uVar2 = sVar1 < 0;
    bVar3 = false;
    func_0x000040ac();
    if ((bVar3) && (candidate_29c6(), (bool)uVar2)) {
      *(undefined2 *)(unaff_A5 + 0x4310) = *(undefined2 *)(unaff_A5 + -0x6a50);
      *(undefined2 *)(unaff_A5 + 0x4312) = *(undefined2 *)(unaff_A5 + -0x6a4c);
    }
  }
  else {
    func_0x000040ac();
    if ((bool)in_CF) {
      *(undefined2 *)(unaff_A5 + 0x4310) = *(undefined2 *)(extraout_A0_00 + 0x3c);
      *(undefined2 *)(unaff_A5 + 0x4312) = *(undefined2 *)(extraout_A0_00 + 0x40);
    }
  }
  candidate_40bc();
  *(undefined2 *)(extraout_A0_01 + 0x6e) = *(undefined2 *)(unaff_A5 + 0x4310);
  *(undefined2 *)(extraout_A0_01 + 0x70) = *(undefined2 *)(unaff_A5 + 0x4312);
  *unaff_A6 = candidate_18600;
  return;
}
