/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0da76c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_da76c(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 uVar3;
  bool bVar4;

  if (0x10f < *(short *)(unaff_A6 + 0xf)) {
    candidate_1abc8();
    candidate_427a();
    return;
  }
  *unaff_A6 = 0xda78a;
  uVar3 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar3) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_1da40();
    candidate_1da40();
    candidate_1da40();
    *unaff_A6 = candidate_da7e6;
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
