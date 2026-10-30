/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a0034.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a0034(void)

{
  code *pcVar1;
  short sVar3;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  candidate_1b1dc();
  sVar3 = candidate_260c8();
  iVar2 = sVar3 * 0x100;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
    iVar2 = sVar3 * -0x100;
  }
  unaff_A6[0xc] = iVar2;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0xa0090;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_a00e8;
  }
  candidate_427a();
  sVar3 = candidate_49e8();
  if (sVar3 != 0) {
    *unaff_A6 = candidate_a00e8;
  }
  bVar4 = false;
  candidate_19504();
  if (bVar4) {
    *unaff_A6 = candidate_41ac;
  }
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) == 0) {
    return;
  }
  candidate_4192();
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
