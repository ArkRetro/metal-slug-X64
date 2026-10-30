/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x055686.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_55686(void)

{
  code *pcVar1;
  int iVar2;
  undefined2 uVar3;
  ushort uVar4;
  undefined1 uVar6;
  short sVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar7;

  *(char *)(unaff_A6 + 0x1d) = *(char *)(unaff_A6 + 0x1d) + '\x06';
  if (*(byte *)(unaff_A6 + 0x1d) < 0xc0) {
    candidate_55478();
    iVar2 = candidate_2e7c();
    if (iVar2 < 0) {
      iVar2 = -iVar2;
    }
    if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
      iVar2 = -iVar2;
    }
    unaff_A6[0xc] = iVar2;
    unaff_A6[0xd] = 0xfffff000;
    candidate_1ac38();
    candidate_427a();
    return;
  }
  candidate_4256();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x1110;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x1110;
  uVar3 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar3;
  unaff_A6[0x14] = 0x54748;
  candidate_558e8();
  candidate_55574();
  uVar6 = 0;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 1) != 0) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    uVar6 = 0x80;
  }
  *(undefined1 *)(unaff_A6 + 0x1d) = uVar6;
  *(undefined1 *)((int)unaff_A6 + 0x12) = 0xff;
  *(undefined1 *)((int)unaff_A6 + 0x13) = 0xff;
  uVar4 = candidate_29f2();
  *(ushort *)((int)unaff_A6 + 0x72) = uVar4 & 7;
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
  uVar6 = candidate_366ac();
  *(undefined1 *)((int)unaff_A6 + 0x76) = uVar6;
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  candidate_555e6();
  *unaff_A6 = 0x55772;
  if ((((*(byte *)(*(int *)(unaff_A6[3] + 0xc) + 0x5f) & 0x80) == 0) &&
      (sVar5 = *(short *)(unaff_A6 + 0x22) + -1, *(short *)(unaff_A6 + 0x22) = sVar5, -1 < sVar5))
     && ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
    candidate_55820();
    *(byte *)(unaff_A6 + 0x18) = (byte)(*(char *)(unaff_A6 + 0x1d) + 8U) >> 4;
    bVar7 = false;
    candidate_1945c();
    if (bVar7) goto candidate_4188;
    sVar5 = candidate_49e8();
    bVar7 = false;
    if (sVar5 == 0) {
      candidate_4c24();
      if (bVar7) {
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      }
      if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
        func_0x000143e4();
        candidate_427a();
        return;
      }
    }
  }
  candidate_120f8();
  bVar7 = false;
  candidate_40bc();
  if (bVar7) {
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
