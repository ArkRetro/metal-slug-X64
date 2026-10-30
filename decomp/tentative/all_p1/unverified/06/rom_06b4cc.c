/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06b4cc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6b4cc(void)

{
  code *pcVar1;
  ushort uVar2;
  int iVar3;
  short sVar4;
  char cVar5;
  byte bVar6;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar7;

  unaff_A6[0xd] = (-0x100000 - unaff_A6[0xd] >> 6) + unaff_A6[0xd];
  unaff_A6[0xc] = unaff_A6[0xc] - ((int)unaff_A6[0xc] >> 5);
  bVar7 = (*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0;
  if (bVar7) {
    unaff_A6[0xc] = (*(int *)((int)unaff_A6 + 0x6e) - unaff_A6[0xc] >> 4) + unaff_A6[0xc];
    *(int *)((int)unaff_A6 + 0x6e) =
         (unaff_A6[0xc] - *(int *)((int)unaff_A6 + 0x6e) >> 6) + *(int *)((int)unaff_A6 + 0x6e);
  }
  iVar3 = unaff_A6[0xd];
  func_0x000143e4();
  if (((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) && (iVar3 < -0x10000)) {
    unaff_A6[0xd] = -iVar3 >> 1;
  }
  iVar3 = unaff_A6[0xf] - unaff_A6[0x11];
  if (iVar3 < 0) {
    iVar3 = -iVar3;
  }
  uVar2 = *(ushort *)(unaff_A6 + 0x22) - (short)((uint)(iVar3 + 0x8000) >> 0x10);
  *(ushort *)(unaff_A6 + 0x22) = uVar2;
  if ((int)((uint)uVar2 << 0x10) < 0) {
    *unaff_A6 = candidate_6b620;
  }
  bVar6 = -((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0);
  if ((bVar6 & (bVar6 ^ -bVar7)) != 0) {
    candidate_411c();
    candidate_1acf2();
  }
  sVar4 = 0x200;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    iVar3 = unaff_A6[0xc];
    if (iVar3 < 0) {
      iVar3 = -iVar3;
    }
    sVar4 = (short)((uint)iVar3 >> 8) >> 1;
  }
  *(short *)((int)unaff_A6 + 0x26) =
       ((short)(sVar4 - *(short *)((int)unaff_A6 + 0x26)) >> 3) + *(short *)((int)unaff_A6 + 0x26);
  candidate_427a();
  if ((char)(*(byte *)(unaff_A6 + 0xc) ^ -((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0)) < '\0') {
    *unaff_A6 = candidate_6b620;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_6b620;
  }
  cVar5 = candidate_49e8();
  if (cVar5 != '\0') {
    *unaff_A6 = candidate_6b620;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_6b620;
  }
  bVar7 = false;
  candidate_19504();
  if (bVar7) {
    candidate_4126();
    iVar3 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar3 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
