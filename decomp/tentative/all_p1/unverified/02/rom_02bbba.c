/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x02bbba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_2bbba(void)

{
  undefined4 uVar1;
  uint uVar2;
  ushort uVar4;
  int iVar3;
  int iVar5;
  undefined4 *unaff_A6;
  bool bVar6;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1001;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1001;
  candidate_9ea8();
  *(undefined1 *)((int)unaff_A6 + 0x8f) = 4;
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
    *unaff_A6 = 0x2bd3c;
    candidate_1456a();
    candidate_427a();
    candidate_2c18e();
    uVar2 = candidate_29c6();
    if ((uVar2 & 0xf) == 0) {
      *unaff_A6 = candidate_2bc36;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_2bc36;
    }
  }
  else {
    uVar4 = candidate_29c6();
    iVar3 = (short)(uVar4 & 3) * 0xc;
    uVar1 = *(undefined4 *)(iVar3 + 0x2c58a);
    *(undefined4 *)((int)unaff_A6 + 0x6e) = *(undefined4 *)(iVar3 + 0x2c58e);
    iVar3 = *(int *)(iVar3 + 0x2c592);
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      iVar3 = -iVar3;
    }
    unaff_A6[0xc] = iVar3;
    unaff_A6[0xd] = uVar1;
    *(undefined4 *)((int)unaff_A6 + 0x76) = uVar1;
    uVar4 = candidate_29c6();
    *(ushort *)((int)unaff_A6 + 0x82) = (uVar4 & 0x1f) + 0x18;
    candidate_4256();
    *unaff_A6 = 0x2bc84;
    *(undefined4 *)((int)unaff_A6 + 0x76) = unaff_A6[0xd];
    candidate_2bdd8();
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_2bcac;
    }
    candidate_427a();
    candidate_2c18e();
  }
  if (*(short *)((int)unaff_A6 + 0x82) < 1) {
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
