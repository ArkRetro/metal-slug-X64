/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0247dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_247dc(void)

{
  code *pcVar1;
  int extraout_A0;
  int iVar2;
  int unaff_A5;
  undefined4 *unaff_A6;

  candidate_4256();
  *unaff_A6 = 0x247f2;
  candidate_1abc8();
  candidate_427a();
  iVar2 = extraout_A0;
  if ((*(char *)(unaff_A6 + 0x23) != '\0') && (iVar2 = unaff_A6[3], *(char *)(iVar2 + 0x60) == '\0')
     ) {
    *unaff_A6 = 0x24704;
  }
  if (*(char *)(iVar2 + 0x61) != '\0') {
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
