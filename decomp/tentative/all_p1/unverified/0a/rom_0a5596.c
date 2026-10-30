/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5596.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5596(void)

{
  code *pcVar1;
  int iVar2;
  byte bVar3;
  short sVar4;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar5;

  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0xa2) & 0x80) != 0) {
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
  bVar3 = *(byte *)((int)unaff_A6 + 0x7e);
  *(byte *)((int)unaff_A6 + 0x7e) = bVar3 - 0x40;
  if (bVar3 < 0x40) {
    sVar4 = candidate_29f2();
    sVar4 = sVar4 + 0x28;
    candidate_29f2();
    bVar5 = false;
    candidate_40bc(sVar4);
    if (bVar5) {
      *(undefined4 *)(extraout_A0 + 0x6e) = *(undefined4 *)((int)unaff_A6 + 0x6e);
    }
    candidate_1acf2();
  }
  return;
}
