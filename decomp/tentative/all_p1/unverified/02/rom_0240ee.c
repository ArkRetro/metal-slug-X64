/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0240ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_240ee(void)

{
  undefined4 uVar1;
  short sVar2;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0;
  sVar2 = candidate_29f2();
  *(short *)(unaff_A6 + 0x25) = sVar2 + 4;
  unaff_A6[0x14] = 0;
  *unaff_A6 = 0x24110;
  candidate_1abc8();
  if (*(short *)(unaff_A6 + 0x24) == 3) {
    sVar2 = *(short *)(unaff_A6[3] + 0x9a) * 4;
    if (*(short *)(unaff_A6[3] + 0x9a) == 9) {
      sVar2 = sVar2 + -1;
    }
    uVar1 = unaff_A6[0xf];
    *(short *)(unaff_A6 + 0xf) = sVar2 + *(short *)(unaff_A6 + 0xf);
    candidate_427a();
    unaff_A6[0xf] = uVar1;
  }
  else {
    candidate_427a();
  }
  sVar2 = *(short *)(unaff_A6 + 0x25) + -1;
  *(short *)(unaff_A6 + 0x25) = sVar2;
  if (sVar2 == 0) {
    *unaff_A6 = candidate_24164;
  }
  return;
}
