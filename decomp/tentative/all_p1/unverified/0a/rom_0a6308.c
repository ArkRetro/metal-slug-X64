/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a6308.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a6308(void)

{
  code *pcVar1;
  int iVar2;
  short *psVar3;
  undefined2 uVar4;
  short sVar5;
  byte bVar6;
  undefined1 uVar7;
  ushort uVar8;
  short sVar9;
  undefined2 extraout_D0u;
  byte *pbVar10;
  byte *pbVar11;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar12;

  candidate_a6dba();
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  candidate_a64b8();
  *unaff_A6 = 0xa6326;
  if (*(char *)(unaff_A6 + 0x18) != '\0') {
    *unaff_A6 = candidate_a6260;
  }
  candidate_1abc8();
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x40) == 0) {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  }
  else {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0x5f) & 0x80) != 0) {
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
  if (*(char *)((int)unaff_A6 + 0x7e) == '\0') {
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      pbVar10 = (byte *)0x2a6351;
    }
    else {
      pbVar10 = (byte *)0x2a6355;
    }
    uVar4 = *(undefined2 *)(*(int *)((int)unaff_A6 + 0x6e) + 0x74);
    while (*pbVar10 != 0xff) {
      pbVar11 = pbVar10 + 1;
      bVar6 = *pbVar10;
      pbVar10 = pbVar11;
      if ((CONCAT22(extraout_D0u,uVar4) & 1 << (bVar6 & 0x1f)) != 0) {
        *(undefined1 *)((int)unaff_A6 + 0x7e) = 1;
      }
    }
    if (*(char *)((int)unaff_A6 + 0x7e) == '\0') {
      uVar7 = *(undefined1 *)((int)unaff_A6 + 0x15);
      *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
      candidate_427a();
      *(undefined1 *)((int)unaff_A6 + 0x15) = uVar7;
    }
  }
  if (-1 < *(char *)((int)unaff_A6 + 0x7f)) {
    candidate_49e8();
  }
  bVar12 = false;
  if ((((*(byte *)(unaff_A6[3] + 0x7c) & '\x01' << ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0)) ==
        0) && (candidate_4c24(), bVar12)) &&
     (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
    candidate_8d1d0();
    *(byte *)(unaff_A6[3] + 0x7c) =
         *(byte *)(unaff_A6[3] + 0x7c) | '\x01' << ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0);
  }
  *(byte *)(unaff_A6[3] + 0x5e) = *(byte *)((int)unaff_A6 + 0x5e) | *(byte *)(unaff_A6[3] + 0x5e);
  sVar5 = *(short *)((int)unaff_A6 + 0x4e);
  sVar9 = *(short *)((int)unaff_A6 + 0x72) - sVar5;
  psVar3 = (short *)(*(int *)((int)unaff_A6 + 0x6e) + 0x98);
  *psVar3 = sVar9 + *psVar3;
  *(short *)((int)unaff_A6 + 0x72) = sVar5;
  uVar8 = sVar9 + *(short *)(unaff_A6 + 0x1e);
  *(ushort *)(unaff_A6 + 0x1e) = uVar8;
  if (99 < uVar8) {
    *(short *)(unaff_A6 + 0x1e) = *(short *)(unaff_A6 + 0x1e) + -100;
    candidate_a6be2();
  }
  return;
}
