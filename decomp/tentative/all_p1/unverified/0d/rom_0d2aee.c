/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d2aee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d2aee(void)

{
  short sVar1;
  char cVar2;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x8e) =
       *(undefined4 *)((short)(*(short *)((int)unaff_A6 + 0x8e) << 2) + 0xe88d8);
  *(undefined1 *)((int)unaff_A6 + 0x73) = 6;
  *unaff_A6 = 0xd2b10;
  if ((*(byte *)((int)unaff_A6 + 0x6e) & 1) == 0) {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
    cVar2 = *(char *)((int)unaff_A6 + 0x73) + -1;
    *(char *)((int)unaff_A6 + 0x73) = cVar2;
    if (cVar2 == '\0') {
      *unaff_A6 = candidate_d280a;
    }
    bVar4 = false;
    candidate_40bc();
    iVar3 = extraout_A0;
    if (bVar4) {
      candidate_1b220();
      iVar3 = extraout_A0_00;
    }
    sVar1 = (*(short **)((int)unaff_A6 + 0x8e))[2];
    *(short *)(iVar3 + 0x28) = **(short **)((int)unaff_A6 + 0x8e) + *(short *)(iVar3 + 0x28);
    *(short *)(iVar3 + 0x2c) = sVar1 + *(short *)(iVar3 + 0x2c);
    bVar4 = false;
    candidate_40bc();
    iVar3 = extraout_A0_01;
    if (bVar4) {
      candidate_1b220();
      iVar3 = extraout_A0_02;
    }
    sVar1 = *(short *)(*(int *)((int)unaff_A6 + 0x8e) + 4);
    *(short *)(iVar3 + 0x28) =
         *(short *)(*(int *)((int)unaff_A6 + 0x8e) + 2) + *(short *)(iVar3 + 0x28);
    *(short *)(iVar3 + 0x2c) = sVar1 + *(short *)(iVar3 + 0x2c);
    *(int *)((int)unaff_A6 + 0x8e) = *(int *)((int)unaff_A6 + 0x8e) + 6;
  }
  *(char *)((int)unaff_A6 + 0x6e) = *(char *)((int)unaff_A6 + 0x6e) + '\x01';
  return;
}
