/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04a668.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4a668(void)

{
  short sVar1;
  undefined2 uVar3;
  int iVar2;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  *(undefined2 *)((int)unaff_A6 + 0x8a) = *(undefined2 *)(unaff_A6 + 10);
  if ((*(byte *)(unaff_A6 + 0x28) & 8) == 0) {
    if (*(char *)(unaff_A6 + 0x29) == '\0') {
      uVar3 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar3;
      uVar3 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar3;
    }
    else {
      uVar3 = candidate_260c8();
      *(undefined2 *)(unaff_A6 + 0x22) = uVar3;
      uVar3 = candidate_260c8();
      *(undefined2 *)((int)unaff_A6 + 0x82) = uVar3;
    }
  }
  else if (*(char *)(unaff_A6 + 0x29) == '\0') {
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x22) = uVar3;
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x82) = uVar3;
  }
  else {
    uVar3 = candidate_260c8();
    *(undefined2 *)(unaff_A6 + 0x22) = uVar3;
    uVar3 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x82) = uVar3;
  }
  iVar2 = *(int *)((int)unaff_A6 + 0x62);
  if (*(short *)(unaff_A6 + 10) < *(short *)((int)unaff_A6 + 0x8a)) {
    iVar2 = -iVar2;
  }
  unaff_A6[0xc] = iVar2;
  *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) & 0xfd;
  *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 4;
  *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 0x10;
  candidate_4ae6a();
  *unaff_A6 = 0x4a74a;
  uVar4 = false;
  if (((*(byte *)(unaff_A6 + 0x18) & 4) == 0) && ((*(byte *)(unaff_A6 + 0x18) & 0x10) == 0)) {
    uVar4 = false;
    if (*(short *)((int)unaff_A6 + 0x82) == 0) {
      if ((*(byte *)(unaff_A6 + 0x18) & 2) == 0) {
        if ((*(byte *)(unaff_A6 + 0x18) & 0x40) != 0) {
          *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 8;
          *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 0x20;
          *(byte *)(unaff_A6 + 0x18) = *(byte *)(unaff_A6 + 0x18) | 2;
        }
      }
      else if (((*(byte *)(unaff_A6 + 0x18) & 8) == 0) && ((*(byte *)(unaff_A6 + 0x18) & 0x20) == 0)
              ) {
        *(undefined1 *)((int)unaff_A6 + 0x9d) = 4;
        *(undefined1 *)((int)unaff_A6 + 0xa1) = 1;
        *(undefined1 *)((int)unaff_A6 + 0x9e) = 0xff;
        *unaff_A6 = candidate_4a7cc;
        uVar4 = false;
      }
    }
    else {
      sVar1 = *(short *)((int)unaff_A6 + 0x82);
      uVar4 = sVar1 == 0;
      *(short *)((int)unaff_A6 + 0x82) = sVar1 + -1;
    }
  }
  candidate_4d6b6();
  candidate_4e008();
  candidate_1ac38();
  candidate_427a();
  candidate_4e04e();
  candidate_4c24();
  if ((bool)uVar4) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_4a9d4;
  }
  return;
}
