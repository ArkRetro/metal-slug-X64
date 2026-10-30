/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x063fdc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_63fdc(void)

{
  code *pcVar1;
  byte bVar2;
  short sVar4;
  ushort uVar5;
  uint uVar3;
  char cVar6;
  int extraout_A0;
  int iVar7;
  undefined4 extraout_A0_00;
  int extraout_A1;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar8;
  undefined8 uVar9;

  candidate_49e8();
  iVar7 = extraout_A0;
  if (*(char *)((int)unaff_A6 + 0x8b) < '\0') {
    iVar7 = extraout_A1;
  }
  if (iVar7 == 0) {
    uVar5 = candidate_29c6();
    sVar4 = (uVar5 & 0xfff) - 0x800;
  }
  else {
    sVar4 = candidate_1b14a();
    *(char *)((int)unaff_A6 + 0x86) = (char)sVar4;
  }
  *(short *)((int)unaff_A6 + 0x86) = sVar4 + *(short *)((int)unaff_A6 + 0x86);
  *(short *)(unaff_A6 + 0x22) =
       ((short)(((short)(*(short *)((int)unaff_A6 + 0x86) - *(short *)((int)unaff_A6 + 0x6e)) >> 1)
               - *(short *)(unaff_A6 + 0x22)) >> 2) + *(short *)(unaff_A6 + 0x22);
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)(unaff_A6 + 0x22) + *(short *)((int)unaff_A6 + 0x6e);
  *(byte *)((int)unaff_A6 + 0x8a) =
       ((char)*(byte *)((int)unaff_A6 + 0x6e) >> 3) +
       ((*(byte *)((int)unaff_A6 + 0x6e) >> 2 & 1) != 0) & 0x1f;
  *(short *)((int)unaff_A6 + 0x72) =
       CONCAT11((char)((ushort)*(short *)(unaff_A6 + 0x22) >> 8),
                *(undefined1 *)((int)unaff_A6 + 0x6e)) + 0x80;
  uVar3 = candidate_29c6();
  uVar3 = *(int *)((int)unaff_A6 + 0x62) + (uVar3 & 0x7fff);
  if (0xfffff < uVar3) {
    uVar3 = 0x80000;
  }
  *(uint *)((int)unaff_A6 + 0x62) = uVar3;
  uVar9 = candidate_2e7c();
  unaff_A6[0xc] = ((int)((ulonglong)uVar9 >> 0x20) - unaff_A6[0xc] >> 1) + unaff_A6[0xc];
  unaff_A6[0xd] = ((int)uVar9 - unaff_A6[0xd] >> 1) + unaff_A6[0xd];
  func_0x000143e4();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x80) != 0) {
    *unaff_A6 = candidate_64128;
  }
  cVar6 = candidate_49e8();
  if (cVar6 != '\0') {
    *unaff_A6 = candidate_64128;
  }
  bVar8 = false;
  candidate_19504();
  if (bVar8) {
    candidate_c0dfc();
    candidate_4126();
    iVar7 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar7 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  bVar2 = *(byte *)(unaff_A6 + 0x1d);
  *(byte *)(unaff_A6 + 0x1d) = bVar2 + 0x90;
  if (bVar2 < 0x70) {
    candidate_40bc();
    candidate_2e7c(extraout_A0_00);
    candidate_1acf2();
  }
  candidate_505a();
  return;
}
