/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x054e0e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_54e0e(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  ushort uVar4;
  byte *pbVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;

  candidate_552e8();
  candidate_554a8();
  candidate_55526();
  *(undefined2 *)((int)unaff_A6 + 0x82) = 0;
  candidate_5530e();
  *unaff_A6 = 0x54e28;
  sVar3 = *(short *)(unaff_A6 + 0x1c);
  bVar6 = sVar3 == 0;
  sVar3 = sVar3 + -1;
  *(short *)(unaff_A6 + 0x1c) = sVar3;
  if (sVar3 < 0) {
    *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)(unaff_A6 + 0x1e);
    *unaff_A6 = 0x54e62;
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
      sVar3 = *(short *)(unaff_A6 + 0x1f);
      bVar6 = sVar3 == 0;
      sVar3 = sVar3 + -1;
      *(short *)(unaff_A6 + 0x1f) = sVar3;
      if (sVar3 < 0) {
        *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + 1;
        sVar3 = *(short *)((int)unaff_A6 + 0x7a) + -1;
        *(short *)((int)unaff_A6 + 0x7a) = sVar3;
        if (sVar3 == 0) {
          *unaff_A6 = candidate_54e0e;
        }
        *(undefined2 *)(unaff_A6 + 0x1f) = *(undefined2 *)((int)unaff_A6 + 0x76);
        bVar6 = false;
      }
      candidate_4c24();
      if (bVar6) {
        *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      }
      candidate_1abc8();
      candidate_427a();
      return;
    }
  }
  else if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    candidate_4c24();
    if (bVar6) {
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    }
    candidate_1abc8();
    candidate_427a();
    return;
  }
  iVar2 = unaff_A6[3];
  uVar4 = *(ushort *)((int)unaff_A6 + 0x6e);
  pbVar5 = (byte *)(iVar2 + 0x60);
  *pbVar5 = *pbVar5 & ~('\x01' << (uVar4 & 7));
  pbVar5 = (byte *)(iVar2 + 0x74);
  *pbVar5 = *pbVar5 & ~('\x01' << (uVar4 & 7));
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfb;
  candidate_120f8();
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
  bVar6 = false;
  candidate_40bc();
  if (bVar6) {
    candidate_1b220();
  }
  candidate_1da40();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 4;
  candidate_4256();
  *unaff_A6 = 0x54f40;
  if (*(char *)(unaff_A6[3] + 0x8f) != '\0') {
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
  candidate_1abc8();
  candidate_427a();
  return;
}
