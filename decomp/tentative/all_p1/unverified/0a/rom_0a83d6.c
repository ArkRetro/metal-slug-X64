/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a83d6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a83d6(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  short sVar4;
  short sVar5;
  ushort uVar6;
  char cVar7;
  ushort uVar8;
  ushort uVar9;
  uint unaff_D2;
  ushort uVar10;
  uint unaff_D3;
  short sVar11;
  uint *extraout_A1;
  undefined4 *puVar12;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar13;
  undefined1 uVar14;
  ulonglong uVar15;

  candidate_29f2();
  candidate_4256();
  candidate_9ea8();
  uVar6 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x26) = (uVar6 & 0xff) + 0x80;
  bVar13 = false;
  cVar7 = candidate_29c6();
  if (bVar13) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  if (cVar7 < '\0') {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 2;
  }
  sVar11 = 2;
  do {
    candidate_29c6();
    uVar15 = candidate_29c6();
    uVar15 = uVar15 & 0xffff001fffff003f;
    sVar4 = (short)(uVar15 >> 0x20) + -0xf;
    uVar6 = (ushort)(uVar15 >> 0x30);
    sVar5 = (short)uVar15 + -0x1f;
    uVar8 = (ushort)(uVar15 >> 0x10);
    uVar9 = (ushort)(unaff_D2 >> 0x10);
    uVar10 = (ushort)(unaff_D3 >> 0x10);
    unaff_D2 = CONCAT22(uVar9,sVar4 + *(short *)(unaff_A6[3] + 0x3c)) << 0x10 | (uint)uVar9;
    unaff_D3 = CONCAT22(uVar10,sVar5 + *(short *)(unaff_A6[3] + 0x40)) << 0x10 | (uint)uVar10;
    *extraout_A1 = unaff_D2;
    extraout_A1[1] = unaff_D3;
    extraout_A1[2] = (int)(CONCAT22(uVar6,sVar4) << 0x10 | (uint)uVar6) >> 2;
    extraout_A1[3] = (int)(CONCAT22(uVar8,sVar5) << 0x10 | (uint)uVar8) >> 2;
    sVar11 = sVar11 + -1;
  } while (sVar11 != 0);
  uVar6 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x1e) = (uVar6 & 0xff) + 1;
  *unaff_A6 = 0xa8498;
  uVar3 = *(undefined2 *)((int)unaff_A6 + 0x26);
  sVar11 = 2;
  puVar12 = (undefined4 *)((int)unaff_A6 + 0x6e);
  do {
    unaff_A6[10] = *puVar12;
    unaff_A6[0xb] = puVar12[1];
    unaff_A6[0xc] = puVar12[2];
    unaff_A6[0xd] = puVar12[3];
    uVar14 = 0;
    candidate_1ac38();
    candidate_427a();
    if ((bool)uVar14) {
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x80;
    }
    *(undefined2 *)((int)unaff_A6 + 0x26) = 0;
    *puVar12 = unaff_A6[10];
    puVar12[1] = unaff_A6[0xb];
    puVar12 = puVar12 + 4;
    sVar11 = sVar11 + -1;
  } while (sVar11 != 0);
  *(undefined2 *)((int)unaff_A6 + 0x26) = uVar3;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  return;
}
