/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x084a82.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_84a82(void)

{
  int iVar1;
  char cVar2;
  uint uVar3;
  int iVar4;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  bool bVar6;

  cVar2 = *(char *)(unaff_A6 + 0x1e);
  *(char *)(unaff_A6 + 0x1e) = cVar2 + -1;
  if (cVar2 < '\x01') {
    iVar1 = unaff_A6[0xc];
    candidate_1b1dc();
    iVar4 = iVar1;
    if (((iVar1 < 0x20000) && (-0x20000 < iVar1)) && (iVar4 = 0x20000, iVar1 < 0)) {
      iVar4 = -0x20000;
    }
    *(int *)((int)unaff_A6 + 0x62) = iVar4;
    candidate_4256();
    *unaff_A6 = 0x84b12;
    candidate_1456a();
    candidate_427a();
    candidate_84b86();
    uVar3 = candidate_29c6();
    if ((uVar3 & 0xf) == 0) {
      *unaff_A6 = candidate_84a02;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
      *unaff_A6 = candidate_84a02;
    }
  }
  else {
    candidate_4256();
    *unaff_A6 = 0x84aa0;
    uVar5 = 0;
    candidate_1436a();
    candidate_427a();
    if ((bool)uVar5) {
      *unaff_A6 = candidate_84a2e;
    }
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
