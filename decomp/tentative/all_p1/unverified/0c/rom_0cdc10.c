/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cdc10.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cdc10(void)

{
  int iVar1;
  char *pcVar2;
  byte bVar3;
  short sVar4;
  char cVar5;
  undefined4 extraout_A0;
  undefined4 *unaff_A6;
  bool bVar6;
  undefined1 uVar7;

  candidate_1b04a();
  unaff_A6[0x22] = extraout_A0;
  bVar6 = false;
  candidate_1b08a();
  if (bVar6) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    candidate_4256();
    *unaff_A6 = candidate_cdbf8;
    uVar7 = 0;
    candidate_ce69c();
    candidate_427a();
    if ((bool)uVar7) {
      *unaff_A6 = 0xcdc22;
      uVar7 = 0;
    }
  }
  else {
    sVar4 = *(short *)(unaff_A6[0x22] + 0x28) - *(short *)(unaff_A6 + 10);
    if (sVar4 < 1) {
      sVar4 = -sVar4;
    }
    cVar5 = '\0';
    if (((0x1f < sVar4) && (cVar5 = '\x01', 0x4f < sVar4)) && (cVar5 = '\x02', 0x7f < sVar4)) {
      cVar5 = '\x03';
    }
    *(char *)(unaff_A6 + 0x24) = cVar5 + '\f';
    candidate_4256();
    *unaff_A6 = 0xcdc7c;
    uVar7 = 0;
    candidate_ce69c();
    candidate_427a();
    if ((bool)uVar7) {
      *unaff_A6 = 0xcdb94;
      iVar1 = unaff_A6[3];
      pcVar2 = (char *)(iVar1 + 0x72);
      *pcVar2 = *pcVar2 + -1;
      uVar7 = 0;
      if (*(char *)(iVar1 + 0x72) < '\x01') {
        *unaff_A6 = candidate_cdcac;
        uVar7 = 0;
      }
    }
  }
  candidate_4c24();
  if (!(bool)uVar7) {
    bVar3 = *(byte *)(unaff_A6[3] + 0xa0);
    if (bVar3 == 2) {
      *unaff_A6 = candidate_4188;
    }
    bVar6 = bVar3 < 2;
    candidate_409a();
    if (!bVar6) {
      *unaff_A6 = candidate_4188;
    }
    return;
  }
  *(undefined1 *)(unaff_A6[3] + 0xa0) = 1;
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xfff1;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xfff1;
  *(char *)(unaff_A6[3] + 0x72) = *(char *)(unaff_A6[3] + 0x72) + -1;
  candidate_406b6();
  return;
}
