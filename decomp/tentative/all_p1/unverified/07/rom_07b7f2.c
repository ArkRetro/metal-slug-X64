/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07b7f2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7b7f2(void)

{
  int iVar1;
  ushort uVar2;
  short sVar3;
  short sVar4;
  char cVar5;
  undefined4 extraout_A0;
  undefined4 extraout_A0_00;
  undefined4 extraout_A0_01;
  undefined4 uVar6;
  int unaff_A6;
  bool bVar7;

  *(short *)(unaff_A6 + 0x8e) = *(short *)(unaff_A6 + 0x8e) + 1;
  bVar7 = *(ushort *)(unaff_A6 + 0x8e) < 200;
  if ((short)*(ushort *)(unaff_A6 + 0x8e) < 200) {
    candidate_1aeda();
    uVar6 = extraout_A0_00;
    if (!bVar7) {
      candidate_1b04a();
      uVar6 = extraout_A0_01;
    }
  }
  else {
    candidate_1b04a();
    uVar6 = extraout_A0;
  }
  *(undefined4 *)(unaff_A6 + 0x7a) = uVar6;
  if (*(short *)(unaff_A6 + 0x88) != 0) {
    *(short *)(unaff_A6 + 0x88) = *(short *)(unaff_A6 + 0x88) + -1;
  }
  uVar2 = candidate_1b12e();
  uVar2 = uVar2 & 0xff;
  sVar3 = 0;
  sVar4 = 6;
  while (((short)uVar2 < *(short *)(sVar3 + 0xe44aa) ||
         (*(short *)(sVar3 + 0xe44ac) <= (short)uVar2))) {
    sVar3 = sVar3 + 8;
    sVar4 = sVar4 + -1;
    if (sVar4 == -1) {
      return;
    }
  }
  cVar5 = (char)*(undefined2 *)(sVar3 + 0xe44b0);
  if (cVar5 != *(char *)(unaff_A6 + 0x97)) {
    if (cVar5 < *(char *)(unaff_A6 + 0x97)) {
      *(short *)(unaff_A6 + 0x90) = *(short *)(unaff_A6 + 0x90) + 1;
      if (4 < *(short *)(unaff_A6 + 0x90)) {
        *(undefined2 *)(unaff_A6 + 0x90) = 0;
        *(char *)(unaff_A6 + 0x97) = *(char *)(unaff_A6 + 0x97) + -1;
      }
    }
    else {
      *(short *)(unaff_A6 + 0x90) = *(short *)(unaff_A6 + 0x90) + 1;
      if (4 < *(short *)(unaff_A6 + 0x90)) {
        *(undefined2 *)(unaff_A6 + 0x90) = 0;
        *(char *)(unaff_A6 + 0x97) = *(char *)(unaff_A6 + 0x97) + '\x01';
      }
    }
  }
  iVar1 = *(int *)(unaff_A6 + 0xc);
  *(undefined2 *)(iVar1 + 0x7e) = 0;
  sVar3 = *(short *)(sVar3 + 0xe44ae) - uVar2;
  if (sVar3 < 0) {
    if (-sVar3 < 4) {
LAB_0007b974:
      *(undefined2 *)(unaff_A6 + 0x7e) = 0xffff;
      if (*(short *)(unaff_A6 + 0x88) == 0) {
        *(undefined2 *)(iVar1 + 0x7e) = 0;
      }
      return;
    }
    if (*(short *)(unaff_A6 + 0x7e) == -1) {
      if (*(short *)(iVar1 + 0x28) < 0xaac) {
        *(undefined2 *)(iVar1 + 0x7e) = 2;
      }
      else {
        *(undefined2 *)(unaff_A6 + 0x7e) = 1;
        *(undefined2 *)(iVar1 + 0x7e) = 1;
      }
    }
    else {
      *(undefined2 *)(iVar1 + 0x7e) = 1;
      if (*(short *)(unaff_A6 + 0x7e) == 2) {
        *(undefined2 *)(unaff_A6 + 0x7e) = 0xffff;
        *(undefined2 *)(iVar1 + 0x7e) = 2;
      }
    }
  }
  else {
    if (sVar3 < 4) goto LAB_0007b974;
    if (*(short *)(unaff_A6 + 0x7e) == -1) {
      if (*(short *)(iVar1 + 0x28) < 0xa3d) {
        *(undefined2 *)(iVar1 + 0x7e) = 2;
        *(undefined2 *)(unaff_A6 + 0x7e) = 2;
      }
      else {
        *(undefined2 *)(iVar1 + 0x7e) = 1;
      }
    }
    else {
      *(undefined2 *)(iVar1 + 0x7e) = 2;
      if (*(short *)(unaff_A6 + 0x7e) == 1) {
        *(undefined2 *)(unaff_A6 + 0x7e) = 0xffff;
        *(undefined2 *)(iVar1 + 0x7e) = 1;
      }
    }
  }
  return;
}
