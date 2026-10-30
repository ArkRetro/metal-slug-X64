/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c388.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_7c388(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  ushort uVar4;
  byte bVar5;
  int unaff_A5;
  undefined4 *unaff_A6;

  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x7c394;
  func_0x0007cce0();
  if (*(short *)(unaff_A6 + 0xf) < 0x21) {
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
  if (0x110 < *(short *)(unaff_A6 + 0xf)) {
    candidate_1abc8();
    return;
  }
  candidate_9ea8();
  candidate_4520();
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  uVar3 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x21) = uVar3;
  uVar4 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x21) = (uVar4 & 0x1f) + *(short *)(unaff_A6 + 0x21);
  bVar5 = candidate_29c6();
  *(byte *)((int)unaff_A6 + 0x92) = (bVar5 & 1) + 2;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x800;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x800;
  *(undefined2 *)((int)unaff_A6 + 0x8f) = 2;
  unaff_A6[0xc] = 0x1800;
  unaff_A6[0xd] = 0x12000;
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0x58;
  if (*(char *)(unaff_A6[3] + 0x60) == -1) {
    unaff_A6[0xc] = 0x3800;
    unaff_A6[0xd] = 0x18000;
    *(undefined2 *)((int)unaff_A6 + 0x82) = 0x30;
  }
  *(undefined1 *)(unaff_A6 + 0x22) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 0;
  candidate_4256();
  *unaff_A6 = 0x7c474;
  if (*(ushort *)((int)unaff_A6 + 0x82) < 0x30) {
    candidate_120f8();
    *unaff_A6 = 0x7c48e;
  }
  if ((_DAT_0010000e & 1) == 0) {
    candidate_40bc();
    candidate_1acf2();
  }
  if ((_DAT_0010000e & 3) == 0) {
    unaff_A6[0xd] = -unaff_A6[0xd];
    func_0x000143e4();
    unaff_A6[0xd] = -unaff_A6[0xd];
  }
  else {
    func_0x000143e4();
  }
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
  if (*(short *)((int)unaff_A6 + 0x82) < 1) {
    *unaff_A6 = candidate_7c500;
  }
  func_0x0007cce0();
  return;
}
