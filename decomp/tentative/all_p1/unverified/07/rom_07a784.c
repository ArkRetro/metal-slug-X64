/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07a784.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7a784(void)

{
  int iVar1;
  short sVar2;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x7f8;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x7f8;
  candidate_1ad6e();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = *(undefined2 *)(*(int *)(unaff_A6[3] + 0xc) + 0x4e);
  *(undefined2 *)((int)unaff_A6 + 0x92) = 0;
  *(undefined2 *)(unaff_A6 + 0x25) = 5;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
  *unaff_A6 = 0x7a7c4;
  candidate_1abc8();
  iVar1 = *(int *)(unaff_A6[3] + 0xc);
  if ((*(byte *)(iVar1 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = 0x7aae8;
  }
  sVar2 = (short)(((uint)*(ushort *)(iVar1 + 0x4e) * 10) / (uint)*(ushort *)((int)unaff_A6 + 0x4e))
          * 2;
  if (*(short *)(sVar2 + 0xe4494) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x86) = *(undefined2 *)(sVar2 + 0xe4494);
    *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) + 1;
    if (*(short *)((int)unaff_A6 + 0x86) <= *(short *)((int)unaff_A6 + 0x92)) {
      *(undefined2 *)((int)unaff_A6 + 0x92) = 0;
      candidate_40bc();
      candidate_1acf2();
    }
    *(short *)(unaff_A6 + 0x25) = *(short *)(unaff_A6 + 0x25) + 1;
    if (*(short *)((int)unaff_A6 + 0x86) <= *(short *)(unaff_A6 + 0x25)) {
      *(undefined2 *)(unaff_A6 + 0x25) = 0;
      candidate_40bc();
      candidate_1acf2();
    }
  }
  return;
}
