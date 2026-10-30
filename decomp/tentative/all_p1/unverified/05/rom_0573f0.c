/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0573f0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_573f0(void)

{
  undefined2 uVar1;
  short sVar2;
  code *extraout_A1;
  code *pcVar3;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x72) = 0xf;
  *unaff_A6 = 0x573fc;
  sVar2 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar2;
  if (sVar2 != 0) {
    return;
  }
  candidate_572ee();
  *(undefined2 *)((int)unaff_A6 + 0x72) = 8;
  *unaff_A6 = 0x57416;
  sVar2 = *(short *)((int)unaff_A6 + 0x72) + -1;
  *(short *)((int)unaff_A6 + 0x72) = sVar2;
  if (sVar2 == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x72) = 0x1e;
    *unaff_A6 = 0x5742c;
    sVar2 = *(short *)((int)unaff_A6 + 0x72) + -1;
    *(short *)((int)unaff_A6 + 0x72) = sVar2;
    if (sVar2 == 0) {
      *unaff_A6 = candidate_57490;
      candidate_c8c7e();
    }
    else {
      candidate_29f2();
    }
    uVar1 = candidate_5777e();
    *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar1;
    pcVar3 = candidate_3dd96;
    sVar2 = 3;
    do {
      (*pcVar3)();
      sVar2 = sVar2 + -1;
      pcVar3 = extraout_A1;
    } while (sVar2 != 0);
    return;
  }
  return;
}
