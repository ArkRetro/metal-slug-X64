/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0912ee.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_912ee(void)

{
  short sVar1;
  short sVar2;
  short sVar3;
  undefined4 *unaff_A6;
  bool bVar4;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_1abc8();
  sVar3 = *(short *)(unaff_A6[3] + 0x3c);
  sVar1 = *(short *)(unaff_A6 + 0xf);
  if (unaff_A6[0xc] != 0) {
    if ((int)unaff_A6[0xc] < 0) {
      bVar4 = SBORROW2(sVar1,sVar3);
      sVar2 = sVar1 - sVar3;
    }
    else {
      bVar4 = SBORROW2(sVar3,sVar1);
      sVar2 = sVar3 - sVar1;
    }
    if (sVar1 != sVar3 && bVar4 == sVar2 < 0) {
      unaff_A6[0xc] = -unaff_A6[0xc];
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x91) & 0xf) == 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  }
  if (7 < (*(byte *)((int)unaff_A6 + 0x91) & 0xf)) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  }
  candidate_9ea8();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_4256();
  *unaff_A6 = 0x9137c;
  unaff_A6[0xd] = unaff_A6[0xd] + -0x4000;
  func_0x000143e4();
  candidate_427a();
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    *unaff_A6 = 0x9127a;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = candidate_91270;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_91270;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
