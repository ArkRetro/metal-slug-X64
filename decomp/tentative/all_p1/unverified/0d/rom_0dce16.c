/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0dce16.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_dce16(void)

{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  char cVar4;
  char cVar5;
  undefined2 uVar6;
  short sVar7;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar8;
  bool bVar9;
  undefined8 uVar10;

  *(undefined1 *)((int)unaff_A6 + 0x9f) = 4;
  *unaff_A6 = 0xdce22;
  cVar4 = *(char *)((int)unaff_A6 + 0x9f) + -1;
  *(char *)((int)unaff_A6 + 0x9f) = cVar4;
  if (-1 < cVar4) {
    return;
  }
  candidate_9ea8();
  candidate_dd44c();
  *(short *)(unaff_A6 + 0xb) =
       (short)*(char *)((short)(*(byte *)((int)unaff_A6 + 0x9b) & 7) + 0xdce0e) +
       *(short *)(unaff_A6 + 0xb);
  uVar10 = candidate_2e7c();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar10;
  candidate_4256();
  uVar6 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar6;
  unaff_A6[0x14] = 0xdc936;
  *(undefined4 *)((int)unaff_A6 + 0x7e) = *(undefined4 *)(*(int *)(unaff_A6[3] + 0xc) + 0x7e);
  *(undefined1 *)((int)unaff_A6 + 0x9f) = 4;
  candidate_120f8();
  *unaff_A6 = 0xdcedc;
  cVar4 = *(char *)((int)unaff_A6 + 0x9f);
  cVar5 = cVar4 + -1;
  *(char *)((int)unaff_A6 + 0x9f) = cVar5;
  if (-1 >= cVar5) {
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0;
    *unaff_A6 = 0xdcef2;
  }
  uVar8 = -1 < cVar5 && cVar4 == '\0';
  candidate_1ac38();
  candidate_427a();
  candidate_1aeda();
  if ((!(bool)uVar8) ||
     (*(short *)(unaff_A6 + 0x10) <=
      (short)(*(short *)(*(int *)((int)unaff_A6 + 0x7e) + 0x40) + 0x10))) {
    unaff_A6[0xd] = 0;
    sVar7 = candidate_260c8();
    unaff_A6[0xc] = (int)sVar7 << 8;
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) == 0) {
      unaff_A6[0xc] = -unaff_A6[0xc];
    }
    candidate_4256();
    *unaff_A6 = candidate_dcf6a;
    return;
  }
  uVar3 = unaff_A6[0xd];
  bVar9 = 0x4fff < uVar3;
  unaff_A6[0xd] = uVar3 - 0x5000;
  candidate_4c24();
  if (bVar9) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    sVar7 = candidate_49e8();
    bVar9 = false;
    if (sVar7 == 0) {
      candidate_1945c();
      if (!bVar9) {
        return;
      }
      goto candidate_4188;
    }
  }
  candidate_120f8();
  bVar9 = false;
  candidate_40bc();
  if (bVar9) {
    candidate_1b220();
  }
candidate_4188:
  candidate_4126();
  iVar2 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar2 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
