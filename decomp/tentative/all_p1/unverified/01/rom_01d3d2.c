/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01d3d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1d3d2(void)

{
  short sVar1;
  short sVar2;
  uint uVar3;
  ushort uVar5;
  char cVar6;
  int iVar4;
  undefined2 extraout_D1u;
  undefined4 *unaff_A6;
  undefined1 uVar7;

  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  uVar3 = candidate_29c6();
  if ((uVar3 & 2) == 0) {
    candidate_4256();
    candidate_9ea8();
  }
  else {
    candidate_4256();
    candidate_9ea8();
  }
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ed;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ed;
  *(undefined1 *)((int)unaff_A6 + 0x92) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x93) = 1;
  uVar5 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x8a) = (uVar5 & 0x3e) - 0x10 & 0xff;
  if (*(short *)((int)unaff_A6 + 0x62) < 1) {
    *(undefined2 *)((int)unaff_A6 + 0x62) = 0x200;
  }
  uVar5 = candidate_29c6();
  sVar2 = (*(ushort *)((int)unaff_A6 + 0x8a) & 0xff) * 2;
  sVar1 = *(short *)(sVar2 + 0x2a7c);
  unaff_A6[0xd] =
       (int)((short)(*(short *)(sVar2 + 0x29fc) * ((uVar5 & 7) + 1)) >> 2) *
       (int)*(short *)((int)unaff_A6 + 0x62);
  unaff_A6[0xc] = (int)sVar1 * (int)*(short *)((int)unaff_A6 + 0x62);
  cVar6 = candidate_29c6();
  if (cVar6 < '\x01') {
    uVar5 = candidate_29c6();
    *(short *)(unaff_A6 + 0xc) = (short)(((uVar5 & 3) + 2) * (short)((uint)unaff_A6[0xc] >> 8)) >> 1
    ;
    uVar5 = candidate_29c6();
    *(short *)(unaff_A6 + 0xd) =
         (short)(((uVar5 & 3) + 2) *
                (short)(CONCAT22(extraout_D1u,*(undefined2 *)(unaff_A6 + 0xd)) >> 8)) >> 1;
  }
  iVar4 = unaff_A6[0xd];
  if (0x100000 < iVar4) {
    iVar4 = 0x100000;
  }
  if (iVar4 < 0) {
    iVar4 = 0;
  }
  unaff_A6[0xd] = iVar4;
  iVar4 = unaff_A6[0xc];
  if (0x80000 < iVar4) {
    iVar4 = 0x80000;
  }
  if (iVar4 < -0x80000) {
    iVar4 = -0x80000;
  }
  unaff_A6[0xc] = iVar4;
  *(undefined4 *)((int)unaff_A6 + 0x7a) = unaff_A6[0xd];
  *(undefined4 *)((int)unaff_A6 + 0x7e) = 0xffffb000;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *unaff_A6 = 0x1d6e4;
  func_0x000143e4();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) == 0) {
    unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x7e) + unaff_A6[0xd];
  }
  else {
    unaff_A6[0xc] = (int)unaff_A6[0xc] >> 2;
    iVar4 = *(int *)((int)unaff_A6 + 0x7a) >> 1;
    unaff_A6[0xd] = iVar4;
    *(int *)((int)unaff_A6 + 0x7a) = iVar4;
    *(char *)((int)unaff_A6 + 0x92) = *(char *)((int)unaff_A6 + 0x92) + -1;
    if (*(char *)((int)unaff_A6 + 0x92) < '\x01') {
      *unaff_A6 = candidate_41ac;
    }
  }
  uVar7 = *(byte *)((int)unaff_A6 + 0x93) < *(byte *)((int)unaff_A6 + 0x92);
  if (*(byte *)((int)unaff_A6 + 0x93) != *(byte *)((int)unaff_A6 + 0x92)) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 8;
  }
  candidate_1b1ec();
  if (!(bool)uVar7) {
    candidate_427a();
  }
  candidate_19504();
  if ((bool)uVar7) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
