/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x01959c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_1959c(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar4;

  *(undefined2 *)((int)unaff_A6 + 0x26) = 0xf;
  *unaff_A6 = 0x195a8;
  sVar3 = *(short *)((int)unaff_A6 + 0x26);
  *(short *)((int)unaff_A6 + 0x26) = sVar3 + -1;
  if (sVar3 != 0) {
    return;
  }
  candidate_3d292();
  *unaff_A6 = 0x195d0;
  bVar4 = false;
  candidate_3d2b0();
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
