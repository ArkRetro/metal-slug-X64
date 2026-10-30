/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a5d26.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a5d26(void)

{
  code *pcVar1;
  int iVar2;
  short sVar3;
  int extraout_A0;
  int unaff_A5;
  undefined4 *unaff_A6;

  if ((*(byte *)(*(int *)((int)unaff_A6 + 0x6e) + 0xa2) & 0x40) != 0) {
    candidate_4192();
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
  sVar3 = candidate_29f2();
  if (sVar3 == 0) {
    candidate_1da40();
    *(undefined2 *)(extraout_A0 + 0x1e) = 0x1001;
    candidate_c8022();
  }
  return;
}
