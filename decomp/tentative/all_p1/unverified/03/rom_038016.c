/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x038016.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_38016(void)

{
  code *pcVar1;
  int iVar2;
  char cVar3;
  undefined2 uVar4;
  undefined4 extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  bVar5 = false;
  candidate_40bc();
  if (!bVar5) {
    candidate_4126();
    iVar2 = *(int *)(unaff_A5 + -0x1c24);
    do {
      unaff_A6 = (undefined4 *)unaff_A6[2];
      pcVar1 = (code *)*unaff_A6;
      *(undefined4 *)(iVar2 + -4) = 0x424a;
      (*pcVar1)();
    } while( true );
  }
  *(undefined4 *)((int)unaff_A6 + 0x72) = extraout_A0;
  cVar3 = candidate_29c6();
  *(bool *)((int)unaff_A6 + 0x97) = -1 < cVar3;
  candidate_380fc();
  cVar3 = candidate_29c6();
  uVar4 = 0x1ef;
  if (-1 < cVar3) {
    uVar4 = 0x61;
    unaff_A6[0xc] = -unaff_A6[0xc];
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
  }
  *(undefined2 *)(unaff_A6 + 10) = uVar4;
  candidate_29c6();
  candidate_374ba();
  *(undefined1 *)((int)unaff_A6 + 0x96) = 6;
  return;
}
