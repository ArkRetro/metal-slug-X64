/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a4c3a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a4c3a(void)

{
  byte *pbVar1;
  short sVar2;
  ushort uVar3;
  undefined4 *unaff_A6;

  candidate_120f8();
  candidate_c8022();
  candidate_a6f30();
  candidate_a5634();
  pbVar1 = (byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0xa2);
  *pbVar1 = *pbVar1 | 0x40;
  *(undefined2 *)((int)unaff_A6 + 0x9e) = 0x1e;
  *unaff_A6 = 0xa4c68;
  candidate_a4d90();
  sVar2 = *(short *)((int)unaff_A6 + 0x9e);
  *(short *)((int)unaff_A6 + 0x9e) = sVar2 + -1;
  if (sVar2 == 0) {
    *unaff_A6 = candidate_a4c7e;
  }
  *(int *)((int)unaff_A6 + 0x7e) = *(int *)((int)unaff_A6 + 0x86) + *(int *)((int)unaff_A6 + 0x7e);
  *(int *)((int)unaff_A6 + 0x82) = *(int *)((int)unaff_A6 + 0x8a) + *(int *)((int)unaff_A6 + 0x82);
  sVar2 = *(short *)(unaff_A6 + 0x24) - (ushort)*(byte *)((int)unaff_A6 + 0xa6);
  if (sVar2 < *(short *)((int)unaff_A6 + 0x7e)) {
    *(short *)((int)unaff_A6 + 0x7e) = sVar2;
    *(undefined1 *)((int)unaff_A6 + 0xa7) = 1;
  }
  sVar2 = (ushort)*(byte *)((int)unaff_A6 + 0xa6) + *(short *)((int)unaff_A6 + 0x8e);
  if (*(short *)((int)unaff_A6 + 0x7e) < sVar2) {
    *(short *)((int)unaff_A6 + 0x7e) = sVar2;
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
  sVar2 = 0;
  for (uVar3 = *(ushort *)(unaff_A6 + 0x1d) ^ *(ushort *)((int)unaff_A6 + 0x76); uVar3 != 0;
      uVar3 = uVar3 >> 1) {
    if ((uVar3 & 1) != 0) {
      *(ushort *)((int)register0x0000003c + -4) = uVar3;
      *(short *)((int)register0x0000003c + -8) = sVar2;
      *(undefined4 *)((int)register0x0000003c + -0xc) = 0xa5b98;
      candidate_a5c04();
      sVar2 = *(short *)((int)register0x0000003c + -8);
      uVar3 = *(ushort *)((int)register0x0000003c + -6);
      register0x0000003c = (BADSPACEBASE *)((int)register0x0000003c + -4);
    }
    sVar2 = sVar2 + 1;
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
