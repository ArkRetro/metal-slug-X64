/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04c5ca.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4c5ca(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_4256();
  *(undefined2 *)(unaff_A6 + 0x21) = 0x10;
  *unaff_A6 = 0x4c5e6;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0xe06f;
  uVar2 = 0;
  candidate_1abc8();
  candidate_4d9de();
  if ((bool)uVar2) {
    sVar1 = *(short *)(unaff_A6 + 0x21) + -1;
    *(short *)(unaff_A6 + 0x21) = sVar1;
    if (sVar1 == 0) {
      *unaff_A6 = candidate_4c64a;
      uVar2 = 0;
    }
    else {
      uVar2 = 0;
      candidate_4256();
    }
  }
  candidate_4d74a();
  if ((bool)uVar2) {
    candidate_12192();
    *unaff_A6 = candidate_4c64a;
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  *(undefined1 *)(*(int *)(unaff_A6[3] + 0xc) + 0xa3) = *(undefined1 *)((int)unaff_A6 + 0xa3);
  return;
}
