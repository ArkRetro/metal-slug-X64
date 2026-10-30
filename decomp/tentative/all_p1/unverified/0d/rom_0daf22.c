/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0daf22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_daf22(void)

{
  code *pcVar1;
  int iVar2;
  ushort uVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar4;

  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)in_CF) {
    uVar3 = candidate_29c6();
    if ((uVar3 & 3) == 0) {
      bVar4 = false;
      candidate_40bc();
      if (bVar4) {
        candidate_1b220();
      }
    }
    *unaff_A6 = candidate_daf86;
  }
  bVar4 = false;
  candidate_da488();
  if (bVar4) {
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
