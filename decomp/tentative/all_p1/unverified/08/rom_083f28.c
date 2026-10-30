/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x083f28.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_83f28(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  short sVar4;
  int extraout_A0;
  int extraout_A0_00;
  int iVar5;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar6;

  candidate_120f8();
  bVar6 = false;
  candidate_40bc();
  iVar5 = extraout_A0;
  if (bVar6) {
    candidate_1b220();
    iVar5 = extraout_A0_00;
  }
  iVar2 = unaff_A6[0xc];
  unaff_A6[0xc] = iVar2 >> 1;
  *(int *)(iVar5 + 0x30) = -(iVar2 >> 2);
  uVar3 = candidate_29c6();
  unaff_A6[0xd] = (int)(short)((uVar3 & 0x300) + 0x400) << 8;
  *(short *)(unaff_A6 + 0xb) = *(short *)(unaff_A6 + 0xb) + 0x20;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_4256();
  *unaff_A6 = 0x83fae;
  candidate_143dc();
  candidate_427a();
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *unaff_A6 = 0x8407e;
  }
  sVar4 = candidate_49e8();
  if (sVar4 != 0) {
    *unaff_A6 = 0x8407e;
  }
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = 0x84032;
  }
  bVar6 = false;
  candidate_1945c();
  if (!bVar6) {
    return;
  }
  candidate_4126();
  iVar5 = *(int *)(unaff_A5 + -0x1c24);
  do {
    unaff_A6 = (undefined4 *)unaff_A6[2];
    pcVar1 = (code *)*unaff_A6;
    *(undefined4 *)(iVar5 + -4) = 0x424a;
    (*pcVar1)();
  } while( true );
}
