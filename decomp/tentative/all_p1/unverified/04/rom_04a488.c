/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a488.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a488(void)

{
  short sVar3;
  uint uVar1;
  undefined2 uVar4;
  int iVar2;
  undefined4 *unaff_A6;
  bool bVar5;

  if (0x30 < (short)(*(short *)(unaff_A6 + 0xf) + -0x30)) {
    *(short *)((int)unaff_A6 + 0x8a) = *(short *)(unaff_A6 + 10) + -0x30;
    unaff_A6[0xc] = 0x30000;
    if (*(short *)((int)unaff_A6 + 0x8a) < *(short *)(unaff_A6 + 10)) {
      unaff_A6[0xc] = -unaff_A6[0xc];
    }
    *unaff_A6 = 0x4a4c2;
    candidate_4e008();
    candidate_1ac38();
    candidate_427a();
    candidate_4e04e();
    sVar3 = *(short *)(unaff_A6 + 10) - *(short *)((int)unaff_A6 + 0x8a);
    if (sVar3 < 0) {
      sVar3 = -sVar3;
    }
    if (sVar3 < 4) {
      *unaff_A6 = candidate_4a668;
    }
    bVar5 = false;
    candidate_4c24();
    if (bVar5) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_4a9d4;
    }
    return;
  }
  *(undefined2 *)((int)unaff_A6 + 0x8a) = *(undefined2 *)(unaff_A6 + 10);
  uVar1 = candidate_29c6();
  if ((uVar1 & 1) == 0) {
    *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) | 2;
  }
  *(byte *)(unaff_A6 + 0x28) = *(byte *)(unaff_A6 + 0x28) | 4;
  if (*(char *)(unaff_A6 + 0x29) == '\0') {
    uVar4 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x22) = uVar4;
  }
  else {
    uVar4 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x22) = uVar4;
  }
  iVar2 = *(int *)((int)unaff_A6 + 0x62);
  if (*(short *)(unaff_A6 + 10) < *(short *)((int)unaff_A6 + 0x8a)) {
    iVar2 = -iVar2;
  }
  unaff_A6[0xc] = iVar2;
  *unaff_A6 = 0x4a57c;
  candidate_4d6b6();
  candidate_4e008();
  candidate_1ac38();
  candidate_427a();
  candidate_4e04e();
  if (*(short *)(unaff_A6 + 0x22) == 0) {
    if ((*(byte *)(unaff_A6 + 0x28) & 2) == 0) {
      *unaff_A6 = 0x4a94c;
    }
    else {
      *unaff_A6 = candidate_4a5de;
    }
    bVar5 = false;
  }
  else {
    sVar3 = *(short *)(unaff_A6 + 0x22);
    bVar5 = sVar3 == 0;
    *(short *)(unaff_A6 + 0x22) = sVar3 + -1;
  }
  candidate_4c24();
  if (bVar5) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4a9d4;
  }
  return;
}
