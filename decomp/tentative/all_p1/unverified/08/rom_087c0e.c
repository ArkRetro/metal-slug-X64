/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x087c0e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_87c0e(void)

{
  short sVar1;
  ushort uVar2;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int iVar3;
  bool bVar4;

  sVar1 = candidate_29f2();
  uVar2 = sVar1 + 1;
  bVar4 = false;
  candidate_40bc();
  iVar3 = extraout_A0;
  if (bVar4) {
    candidate_1b220();
    iVar3 = extraout_A0_00;
  }
  while( true ) {
    uVar2 = uVar2 - 1;
    *(char *)(iVar3 + 0x89) = (char)uVar2;
    if ((uVar2 & 1) != 0) {
      *(undefined1 *)(iVar3 + 0x88) = 1;
    }
    if ((short)uVar2 < 1) break;
    bVar4 = false;
    candidate_40bc();
    iVar3 = extraout_A0_01;
    if (bVar4) {
      candidate_1b220();
      iVar3 = extraout_A0_02;
    }
  }
  return;
}
