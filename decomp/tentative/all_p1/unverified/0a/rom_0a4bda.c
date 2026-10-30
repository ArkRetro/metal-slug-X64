/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4bda.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a4bda(void)

{
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_a6bcc();
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)((int)unaff_A6 + 0x6e);
  }
  candidate_a6f1a();
  candidate_a5770();
  candidate_a5cfc();
  candidate_13f1e();
  candidate_c8022();
  candidate_a5634();
  *(undefined2 *)((int)unaff_A6 + 0x9e) = 0x14;
  *unaff_A6 = 0xa4c24;
  candidate_a4d90();
  sVar1 = *(short *)((int)unaff_A6 + 0x9e);
  *(short *)((int)unaff_A6 + 0x9e) = sVar1 + -1;
  if (sVar1 == 0) {
    *unaff_A6 = candidate_a4c3a;
  }
  *(int *)((int)unaff_A6 + 0x7e) = *(int *)((int)unaff_A6 + 0x86) + *(int *)((int)unaff_A6 + 0x7e);
  *(int *)((int)unaff_A6 + 0x82) = *(int *)((int)unaff_A6 + 0x8a) + *(int *)((int)unaff_A6 + 0x82);
  sVar1 = *(short *)(unaff_A6 + 0x24) - (ushort)*(byte *)((int)unaff_A6 + 0xa6);
  if (sVar1 < *(short *)((int)unaff_A6 + 0x7e)) {
    *(short *)((int)unaff_A6 + 0x7e) = sVar1;
    *(undefined1 *)((int)unaff_A6 + 0xa7) = 1;
  }
  sVar1 = (ushort)*(byte *)((int)unaff_A6 + 0xa6) + *(short *)((int)unaff_A6 + 0x8e);
  if (*(short *)((int)unaff_A6 + 0x7e) < sVar1) {
    *(short *)((int)unaff_A6 + 0x7e) = sVar1;
    *(undefined1 *)((int)unaff_A6 + 0xa7) = 2;
  }
  if ((*(short *)((int)unaff_A6 + 0x7e) < (short)(*(short *)(unaff_A6 + 0x24) + -0x98)) &&
     (*(char *)((int)unaff_A6 + 0xa7) == '\x01')) {
    *(undefined1 *)((int)unaff_A6 + 0xa7) = 0;
  }
  if (((short)(*(short *)((int)unaff_A6 + 0x8e) + 0x98) < *(short *)((int)unaff_A6 + 0x7e)) &&
     (*(char *)((int)unaff_A6 + 0xa7) == '\x02')) {
    *(undefined1 *)((int)unaff_A6 + 0xa7) = 0;
  }
  iRam0010e8ce = (*(short *)((int)unaff_A6 + 0x9a) * 0x100 + *(int *)((int)unaff_A6 + 0x7e)) -
                 iRam0010e8c6;
  iRam0010e8d2 = (*(short *)(unaff_A6 + 0x27) * 0x100 + *(int *)((int)unaff_A6 + 0x82)) -
                 iRam0010e8ca;
  candidate_a56e4();
  sVar1 = 0;
  for (uVar2 = *(ushort *)(unaff_A6 + 0x1d) ^ *(ushort *)((int)unaff_A6 + 0x76); uVar2 != 0;
      uVar2 = uVar2 >> 1) {
    if ((uVar2 & 1) != 0) {
      *(ushort *)((int)register0x0000003c + -4) = uVar2;
      *(short *)((int)register0x0000003c + -8) = sVar1;
      *(undefined4 *)((int)register0x0000003c + -0xc) = 0xa5b98;
      candidate_a5c04();
      sVar1 = *(short *)((int)register0x0000003c + -8);
      uVar2 = *(ushort *)((int)register0x0000003c + -6);
      register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
    }
    sVar1 = sVar1 + 1;
  }
  if (((*(byte *)(unaff_A6 + 0x1f) & 1) != 0) && ((*(byte *)(unaff_A6 + 0x1f) & 4) == 0)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 4;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xa5bc4;
    candidate_a6de4();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xa5bcc;
    candidate_a6de4();
  }
  if (((*(byte *)(unaff_A6 + 0x1f) & 2) != 0) && ((*(byte *)(unaff_A6 + 0x1f) & 8) == 0)) {
    *(byte *)(unaff_A6 + 0x1f) = *(byte *)(unaff_A6 + 0x1f) | 8;
    *(undefined4 *)((int)register0x0000003c + -4) = 0xa5bee;
    candidate_a6de4();
    *(undefined4 *)((int)register0x0000003c + -4) = 0xa5bf6;
    candidate_a6de4();
  }
  *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(unaff_A6 + 0x1d);
  *(undefined4 *)((int)register0x0000003c + -4) = 0xa5c02;
  candidate_1abc8();
  return;
}
