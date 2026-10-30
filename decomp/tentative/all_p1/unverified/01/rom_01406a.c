/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01406a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1406a(void)

{
  short sVar1;
  int extraout_A0;
  int extraout_A0_00;
  int iVar2;
  int unaff_A5;

  *(undefined4 *)(unaff_A5 + 0x426e) = 0;
  *(undefined4 *)(unaff_A5 + 0x4272) = 0;
  *(undefined4 *)(unaff_A5 + 0x4276) = 0;
  *(undefined4 *)(candidate_427a + unaff_A5) = 0;
  *(undefined4 *)(unaff_A5 + 0x427e) = 0;
  *(undefined4 *)(unaff_A5 + 0x4282) = 0;
  *(undefined4 *)(unaff_A5 + 0x4286) = 0;
  *(undefined4 *)(unaff_A5 + 0x428a) = 0;
  *(undefined2 *)(unaff_A5 + 0x428e) = 0;
  *(undefined1 *)(unaff_A5 + 0x4290) = 0;
  *(undefined2 *)(unaff_A5 + 0x42a4) = 7;
  iVar2 = unaff_A5 + 0x422e;
  do {
    if (*(short *)(iVar2 + 0x2a) == 0) {
      candidate_1abc8();
      iVar2 = extraout_A0;
      if (*(int *)(extraout_A0 + 0x26) != 0) {
        func_0x00013f82();
        iVar2 = extraout_A0_00;
      }
    }
    else {
      *(short *)(iVar2 + 0x2a) = *(short *)(iVar2 + 0x2a) + -1;
    }
    iVar2 = iVar2 + -0x40;
    sVar1 = *(short *)(unaff_A5 + 0x42a4);
    *(short *)(unaff_A5 + 0x42a4) = sVar1 + -1;
  } while (sVar1 != 0);
  return;
}
