/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x038702.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_38702(void)

{
  code *pcVar1;
  int iVar2;
  byte *pbVar3;
  short sVar4;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar5;
  undefined8 uVar6;

  candidate_1aca2();
  *(undefined4 *)((int)unaff_A6 + 0x72) = *(undefined4 *)(unaff_A6[3] + 0x72);
  *(undefined1 *)((int)unaff_A6 + 0x97) = *(undefined1 *)(unaff_A6[3] + 0x97);
  candidate_374ba();
  candidate_380fc();
  *(undefined1 *)(unaff_A6 + 0x26) = 0xc;
  candidate_377da();
  candidate_37854();
  *unaff_A6 = 0x3874a;
  if ((*(char *)((int)unaff_A6 + 0x97) == '\0') &&
     ((short)(*(short *)(unaff_A6 + 0xb) - *(short *)(*(int *)((int)unaff_A6 + 0x72) + 0x2c)) < 0x29
     )) {
    candidate_38228();
    *(undefined2 *)((int)unaff_A6 + 0x86) = 0x1e;
    *unaff_A6 = 0x387ea;
    sVar4 = *(short *)((int)unaff_A6 + 0x86) + -1;
    *(short *)((int)unaff_A6 + 0x86) = sVar4;
    if (-1 < sVar4) {
      candidate_1abc8();
      candidate_427a();
      return;
    }
    unaff_A6[0xc] = 0;
    unaff_A6[0xd] = 0x50000;
    *unaff_A6 = 0x38812;
    candidate_1ac38();
    candidate_37512();
    candidate_427a();
    return;
  }
  candidate_384e0();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) == 0) {
    if ((*(byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x5f) & 0x80) == 0) {
      candidate_2eb4();
      uVar6 = candidate_2e7c();
      *(undefined8 *)(unaff_A6 + 0xc) = uVar6;
    }
    candidate_1ac38();
    candidate_37512();
    candidate_427a();
    return;
  }
  pbVar3 = (byte *)(*(int *)((int)unaff_A6 + 0x72) + 0x5f);
  *pbVar3 = *pbVar3 | 0x80;
  candidate_9f88();
  candidate_9ea8();
  candidate_4256();
  *unaff_A6 = 0x37ab4;
  uVar5 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar5) {
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
