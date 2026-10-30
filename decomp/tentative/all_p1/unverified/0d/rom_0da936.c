/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0da936.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_da936(void)

{
  code *pcVar1;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1abc8();
  if (*(char *)((int)unaff_A6 + 0x61) == '\x11') {
    candidate_120f8();
    candidate_1da40();
    candidate_1da40();
    candidate_1da40();
    candidate_dbbf4();
    candidate_dbbf4();
    *unaff_A6 = candidate_da998;
  }
  bVar3 = false;
  candidate_da488();
  if (bVar3) {
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
