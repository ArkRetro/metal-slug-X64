/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x050b52.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_50b52(void)

{
  code *pcVar1;
  undefined4 uVar2;
  short sVar4;
  int iVar3;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = *(undefined2 *)(unaff_A6[3] + 0x6e);
  candidate_9ea8();
  candidate_4256();
  uVar2 = 0x10000;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    uVar2 = 0xffff0000;
  }
  unaff_A6[0xc] = uVar2;
  unaff_A6[0xd] = 0x24000;
  sVar4 = candidate_29f2();
  unaff_A6[0xd] = sVar4 * 0x10 + unaff_A6[0xd];
  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0x1e;
  *unaff_A6 = 0x50bd0;
  sVar4 = *(short *)((int)unaff_A6 + 0x7a) + -1;
  *(short *)((int)unaff_A6 + 0x7a) = sVar4;
  if (sVar4 < 0) {
    candidate_4126();
    iVar3 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar3 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  *(undefined4 *)((int)unaff_A6 + 0x76) = unaff_A6[0xd];
  candidate_143dc();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    iVar3 = *(int *)((int)unaff_A6 + 0x76);
    if (iVar3 < 0) {
      iVar3 = -iVar3;
    }
    unaff_A6[0xd] = iVar3 * 3 >> 2;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
    candidate_4256();
  }
  candidate_427a();
  return;
}
