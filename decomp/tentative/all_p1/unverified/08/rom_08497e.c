/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08497e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8497e(void)

{
  undefined4 uVar1;
  uint uVar2;
  ushort uVar4;
  int iVar3;
  int iVar5;
  undefined4 *unaff_A6;
  bool bVar6;

  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 0x1e) = 4;
  uVar2 = candidate_29c6();
  if ((uVar2 & 7) == 0) {
    iVar3 = unaff_A6[0xc];
    candidate_1b1dc();
    iVar5 = iVar3;
    if (((iVar3 < 0x20000) && (-0x20000 < iVar3)) && (iVar5 = 0x20000, iVar3 < 0)) {
      iVar5 = -0x20000;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar5;
    candidate_4256();
    *unaff_A6 = 0x84b12;
    candidate_1456a();
    candidate_427a();
    candidate_84b86();
    uVar2 = candidate_29c6();
    if ((uVar2 & 0xf) == 0) {
      *unaff_A6 = candidate_84a02;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_84a02;
    }
  }
  else {
    uVar4 = candidate_29c6();
    iVar3 = (short)(uVar4 & 3) * 0xc;
    uVar1 = *(undefined4 *)(iVar3 + 0xe4cd4);
    *(undefined4 *)((int)unaff_A6 + 0x6e) = *(undefined4 *)(iVar3 + 0xe4cd8);
    iVar3 = *(int *)(iVar3 + 0xe4cdc);
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      iVar3 = -iVar3;
    }
    unaff_A6[0xc] = iVar3;
    unaff_A6[0xd] = uVar1;
    *(undefined4 *)((int)unaff_A6 + 0x72) = uVar1;
    uVar4 = candidate_29c6();
    *(ushort *)((int)unaff_A6 + 0x76) = (uVar4 & 0x1f) + 0x18;
    candidate_4256();
    *unaff_A6 = 0x84a50;
    *(undefined4 *)((int)unaff_A6 + 0x72) = unaff_A6[0xd];
    func_0x000143e4();
    unaff_A6[0xd] = *(int *)((int)unaff_A6 + 0x6e) + unaff_A6[0xd];
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_84a82;
    }
    candidate_427a();
    candidate_84b86();
  }
  if (*(short *)((int)unaff_A6 + 0x76) < 1) {
    *unaff_A6 = candidate_41ac;
  }
  if ((short)*(ushort *)(unaff_A6 + 0xf) < -0x2f) {
    *unaff_A6 = candidate_41ac;
  }
  bVar6 = *(ushort *)(unaff_A6 + 0xf) < 0x8000;
  candidate_19504();
  if (bVar6) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
